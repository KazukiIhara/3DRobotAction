#pragma once

// C++
#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <wrl.h>

// MyHeader
#include "Structs/SoundStruct.h"

namespace Magi {

	class SoundDataContainer {
	public:
		SoundDataContainer();
		~SoundDataContainer();

		// コンテナをクリア
		void ClearContainer();

		// 読み込み
		void LoadWave(const std::string& filename);

		// 通常再生
		void PlayWave(const std::string& filename);

		// 音声停止
		void StopWave(const std::string& filename);

		// ループ再生
		void PlayWaveLoop(const std::string& filename, uint32_t loopCount = XAUDIO2_LOOP_INFINITE);

		// ループ再生停止
		void StopWaveLoop(const std::string& filename);

		// 統一された停止処理
		void StopAll(const std::string& filename);

		// 停止した音声を再生中コンテナから削除
		void CleanupFinishedVoices();

		// 検索
		SoundData* FindWave(const std::string& filename);

	private:
		struct VoiceDeleter {
			void operator()(IXAudio2SourceVoice* p) const;
		};

		using VoicePtr = std::unique_ptr<IXAudio2SourceVoice, VoiceDeleter>;

	private:
		// 初期化
		void Initialize();

		// 終了
		void Finalize();

	private:
		Microsoft::WRL::ComPtr<IXAudio2> xAudio2;
		IXAudio2MasteringVoice* masterVoice = nullptr;

		// サウンドデータコンテナ
		std::map<std::string, std::unique_ptr<SoundData>> sounds_;

		// 通常再生中のVoiceを管理
		std::unordered_map<std::string, std::vector<VoicePtr>> playingVoices_;

		// ループ再生中のVoiceを管理
		std::unordered_map<std::string, VoicePtr> loopingVoices_;
	};

}