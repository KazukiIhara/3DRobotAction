#include "SoundDataContainer.h"

// C++
#include <cassert>
#include <unordered_map>

#include "Logger/Logger.h"

// 通常再生中のVoiceを管理するコンテナ
std::unordered_map<std::string, std::vector<IXAudio2SourceVoice*>> playingVoices_;
// ループ再生中のVoiceを管理するコンテナ
std::unordered_map<std::string, IXAudio2SourceVoice*> loopingVoices_;

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
	// XAudioエンジンのインスタンスを生成
	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	// マスターボイスを生成
	result = xAudio2->CreateMasteringVoice(&masterVoice);
}

void SoundDataContainer::Finalize() {
	// 再生中のすべての音声を停止し、リソースを解放
	for (auto& [filename, voices] : playingVoices_) {
		for (IXAudio2SourceVoice* voice : voices) {
			voice->Stop();
			voice->DestroyVoice();
		}
	}
	playingVoices_.clear();

	for (auto& [filename, voice] : loopingVoices_) {
		voice->Stop();
		voice->DestroyVoice();
	}
	loopingVoices_.clear();

	// XAudio2 をリセット
	xAudio2.Reset();
	ClearContainer();
}


void SoundDataContainer::ClearContainer() {
	sounds_.clear();
}

void SoundDataContainer::LoadWave(const std::string& filename) {
	// サウンドデータのディレクトリパス
	const std::string directoryPath = "Assets/Sounds";
	// 連結してフルパス作成
	const std::string fullpath = directoryPath + "/" + filename;
	// ファイル入力ストリームのインスタンス
	std::ifstream file;
	// .wavファイルをバイナリモードで開く
	file.open(fullpath, std::ios_base::binary);
	// ファイルオープン失敗を検出する
	assert(file.is_open());

	// RIFFヘッダーの読み込み
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	// ファイルがRIFFかチェック
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		assert(0);
	}
	// タイプがWAVEかチェック
	if (strncmp(riff.type, "WAVE", 4) != 0) {
		assert(0);
	}

	// Formatチャンクの読み込み
	FormatChunk format = {};
	// チャンクヘッダーの確認
	file.read((char*)&format, sizeof(ChunkHeader));
	if (strncmp(format.chunk.id, "fmt ", 4) != 0) {
		assert(0);
	}
	// チャンク本体の読み込み
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	// Dataチャンクを探すループ
	ChunkHeader data;
	while (true) {
		file.read((char*)&data, sizeof(data));
		if (strncmp(data.id, "data", 4) == 0) {
			break; // dataチャンク見つかった
		}
		// 見つからなければ、そのチャンク分スキップ
		file.seekg(data.size, std::ios_base::cur);
	}

	// Dataチャンクのデータ部(波形データの読み込み)
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	// WAVEファイルを閉じる
	file.close();

	// 音声データ
	std::unique_ptr<SoundData> soundData = std::make_unique<SoundData>();

	soundData->wfex = format.fmt;
	soundData->pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData->bufferSize = data.size;

	// コンテナに挿入　
	sounds_.insert(std::make_pair(filename, std::move(soundData)));
}

void SoundDataContainer::PlayWave(const std::string& filename) {
	SoundData* soundData = FindWave(filename);
	assert(soundData);

	HRESULT result;
	IXAudio2SourceVoice* pSourceVoice = nullptr;

	// SourceVoice生成
	result = xAudio2->CreateSourceVoice(&pSourceVoice, &soundData->wfex);
	assert(SUCCEEDED(result));

	// バッファ設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData->pBuffer;
	buf.AudioBytes = soundData->bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	// 再生開始
	result = pSourceVoice->SubmitSourceBuffer(&buf);
	assert(SUCCEEDED(result));
	result = pSourceVoice->Start();
	assert(SUCCEEDED(result));

	// 同じ名前の音声を追跡
	playingVoices_[filename].push_back(pSourceVoice);
}

void SoundDataContainer::StopWave(const std::string& filename) {
	auto it = playingVoices_.find(filename);
	if (it != playingVoices_.end()) {
		// 登録されているすべてのSourceVoiceを停止して破棄
		for (IXAudio2SourceVoice* voice : it->second) {
			voice->Stop();
			voice->DestroyVoice();
		}
		// コンテナから削除
		playingVoices_.erase(it);
	}
}

void SoundDataContainer::PlayWaveLoop(const std::string& filename, uint32_t loopCount) {
	auto it = loopingVoices_.find(filename);
	if (it != loopingVoices_.end()) {
		// すでにループ再生中なら再生しない
		return;
	}

	SoundData* soundData = FindWave(filename);
	assert(soundData);

	HRESULT result;
	IXAudio2SourceVoice* pSourceVoice = nullptr;

	// SourceVoice生成
	result = xAudio2->CreateSourceVoice(&pSourceVoice, &soundData->wfex);
	assert(SUCCEEDED(result));

	// バッファ設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData->pBuffer;
	buf.AudioBytes = soundData->bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	buf.LoopCount = loopCount;

	// 再生開始
	result = pSourceVoice->SubmitSourceBuffer(&buf);
	assert(SUCCEEDED(result));
	result = pSourceVoice->Start();
	assert(SUCCEEDED(result));

	// ループ再生用コンテナに登録
	loopingVoices_[filename] = pSourceVoice;
}

void SoundDataContainer::StopWaveLoop(const std::string& filename) {
	auto it = loopingVoices_.find(filename);
	if (it != loopingVoices_.end()) {
		it->second->Stop();
		it->second->DestroyVoice();
		loopingVoices_.erase(it);
	}
}

void SoundDataContainer::StopAll(const std::string& filename) {
	// 通常再生を停止
	StopWave(filename);
	// ループ再生を停止
	StopWaveLoop(filename);
}

void SoundDataContainer::CleanupFinishedVoices() {
	for (auto it = playingVoices_.begin(); it != playingVoices_.end(); ) {
		auto& voices = it->second;
		voices.erase(std::remove_if(voices.begin(), voices.end(), [](IXAudio2SourceVoice* voice) {
			XAUDIO2_VOICE_STATE state;
			voice->GetState(&state);
			if (state.BuffersQueued == 0) {
				voice->DestroyVoice();
				return true; // 再生が終了したVoiceを削除
			}
			return false; // まだ再生中のVoiceは残す
			}), voices.end());

		if (voices.empty()) {
			it = playingVoices_.erase(it); // 名前に対応するVoiceリストが空ならエントリごと削除
		} else {
			++it;
		}
	}
}

SoundData* SoundDataContainer::FindWave(const std::string& filename) {
	// 読み込み済み音声を検索
	if (sounds_.contains(filename)) {
		// 読み込み音声を戻り値としてreturn
		return sounds_.at(filename).get();
	}
	// ファイル名一致なし
	return nullptr;
}