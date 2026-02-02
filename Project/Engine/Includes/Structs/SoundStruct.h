#pragma once

// C++
#include <cstdint>
#include <vector>

// XAudio
#include <xaudio2.h>

// チャンクヘッダ
struct ChunkHeader {
	char id[4];
	int32_t size;
};

// RIFFヘッダチャンク
struct RiffHeader {
	ChunkHeader chunk;
	char type[4];
};

// FMTチャンク
struct FormatChunk {
	ChunkHeader chunk;
	WAVEFORMATEX fmt;
};

// 音声データ
struct SoundData {
	WAVEFORMATEX wfex{};
	std::vector<BYTE> ownedBuffer;
	BYTE* pBuffer = nullptr;
	uint32_t bufferSize = 0;
};