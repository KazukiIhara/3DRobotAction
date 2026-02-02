#include "SoundDataContainer.h"

// C++
#include <algorithm>
#include <cassert>
#include <fstream>

#include "Logger/Logger.h"

using namespace Magi;

void SoundDataContainer::VoiceDeleter::operator()(IXAudio2SourceVoice* p) const {
	if (!p) {
		return;
	}
	// 停止
	p->Stop();
	// 破棄
	p->DestroyVoice();
}

SoundDataContainer::SoundDataContainer() {
	Initialize();
	Logger::Log("SoundDataContainer Initialize\n");
}

SoundDataContainer::~SoundDataContainer() {
	Finalize();
	Logger::Log("SoundDataContainer Finalize\n");
}

void SoundDataContainer::Initialize() {
	HRESULT result;

	// XAudio2生成
	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	assert(SUCCEEDED(result));

	// マスターボイス生成
	result = xAudio2->CreateMasteringVoice(&masterVoice);
	assert(SUCCEEDED(result));
}

void SoundDataContainer::Finalize() {
	// 通常再生Voice解放
	playingVoices_.clear();

	// ループ再生Voice解放
	loopingVoices_.clear();

	// マスターボイス破棄
	if (masterVoice) {
		masterVoice->DestroyVoice();
		masterVoice = nullptr;
	}

	// XAudio2解放
	xAudio2.Reset();

	// サウンド解放
	ClearContainer();
}

void SoundDataContainer::ClearContainer() {
	sounds_.clear();
}

void SoundDataContainer::LoadWave(const std::string& filename) {
	// ロード済みなら戻る
	if (sounds_.contains(filename)) {
		return;
	}

	// ディレクトリ
	const std::string directoryPath = "Assets/Sounds";
	// フルパス
	const std::string fullpath = directoryPath + "/" + filename;

	// ファイルを開く
	std::ifstream file(fullpath, std::ios_base::binary);
	assert(file.is_open());

	// RIFF読み込み
	RiffHeader riff{};
	file.read(reinterpret_cast<char*>(&riff), sizeof(riff));
	assert(strncmp(riff.chunk.id, "RIFF", 4) == 0);
	assert(strncmp(riff.type, "WAVE", 4) == 0);

	// fmt読み込み
	FormatChunk format{};
	file.read(reinterpret_cast<char*>(&format), sizeof(ChunkHeader));
	assert(strncmp(format.chunk.id, "fmt ", 4) == 0);
	assert(format.chunk.size <= static_cast<int32_t>(sizeof(format.fmt)));
	file.read(reinterpret_cast<char*>(&format.fmt), format.chunk.size);

	// data検索
	ChunkHeader data{};
	while (true) {
		file.read(reinterpret_cast<char*>(&data), sizeof(data));
		assert(!file.fail());

		if (strncmp(data.id, "data", 4) == 0) {
			break;
		}

		// チャンクスキップ
		file.seekg(data.size, std::ios_base::cur);
	}

	// 波形読み込み
	std::vector<BYTE> buffer;
	buffer.resize(static_cast<size_t>(data.size));
	file.read(reinterpret_cast<char*>(buffer.data()), data.size);
	assert(!file.fail());

	// SoundData作成
	auto soundData = std::make_unique<SoundData>();

	// フォーマット設定
	soundData->wfex = format.fmt;

	// バッファ所有
	soundData->ownedBuffer = std::move(buffer);

	// 生ポインタ設定
	soundData->pBuffer = soundData->ownedBuffer.data();

	// サイズ設定
	soundData->bufferSize = static_cast<uint32_t>(soundData->ownedBuffer.size());

	// 挿入
	sounds_.insert(std::make_pair(filename, std::move(soundData)));
}

void SoundDataContainer::PlayWave(const std::string& filename) {
	SoundData* soundData = FindWave(filename);
	assert(soundData);

	HRESULT result;
	IXAudio2SourceVoice* rawVoice = nullptr;

	// SourceVoice生成
	result = xAudio2->CreateSourceVoice(&rawVoice, &soundData->wfex);
	assert(SUCCEEDED(result));

	// RAII管理
	VoicePtr voice(rawVoice);

	// バッファ設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData->pBuffer;
	buf.AudioBytes = soundData->bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	// バッファ投入
	result = rawVoice->SubmitSourceBuffer(&buf);
	assert(SUCCEEDED(result));

	// 再生開始
	result = rawVoice->Start();
	assert(SUCCEEDED(result));

	// 追跡
	playingVoices_[filename].push_back(std::move(voice));
}

void SoundDataContainer::StopWave(const std::string& filename) {
	auto it = playingVoices_.find(filename);
	if (it == playingVoices_.end()) {
		return;
	}

	// まとめて解放
	playingVoices_.erase(it);
}

void SoundDataContainer::PlayWaveLoop(const std::string& filename, uint32_t loopCount) {
	if (loopingVoices_.contains(filename)) {
		return;
	}

	SoundData* soundData = FindWave(filename);
	assert(soundData);

	HRESULT result;
	IXAudio2SourceVoice* rawVoice = nullptr;

	// SourceVoice生成
	result = xAudio2->CreateSourceVoice(&rawVoice, &soundData->wfex);
	assert(SUCCEEDED(result));

	// RAII管理
	VoicePtr voice(rawVoice);

	// バッファ設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData->pBuffer;
	buf.AudioBytes = soundData->bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	buf.LoopCount = loopCount;

	// バッファ投入
	result = rawVoice->SubmitSourceBuffer(&buf);
	assert(SUCCEEDED(result));

	// 再生開始
	result = rawVoice->Start();
	assert(SUCCEEDED(result));

	// 追跡
	loopingVoices_.insert(std::make_pair(filename, std::move(voice)));
}

void SoundDataContainer::StopWaveLoop(const std::string& filename) {
	auto it = loopingVoices_.find(filename);
	if (it == loopingVoices_.end()) {
		return;
	}

	// 解放
	loopingVoices_.erase(it);
}

void SoundDataContainer::StopAll(const std::string& filename) {
	StopWave(filename);
	StopWaveLoop(filename);
}

void SoundDataContainer::CleanupFinishedVoices() {
	for (auto it = playingVoices_.begin(); it != playingVoices_.end(); ) {
		auto& voices = it->second;

		// 終了Voice削除
		voices.erase(std::remove_if(voices.begin(), voices.end(),
			[](const VoicePtr& v) {
				XAUDIO2_VOICE_STATE state{};
				v->GetState(&state);
				return state.BuffersQueued == 0;
			}), voices.end());

		// 空なら削除
		if (voices.empty()) {
			it = playingVoices_.erase(it);
		} else {
			++it;
		}
	}
}

SoundData* SoundDataContainer::FindWave(const std::string& filename) {
	if (sounds_.contains(filename)) {
		return sounds_.at(filename).get();
	}
	return nullptr;
}
