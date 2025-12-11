#pragma once

// C++
#include <stdint.h>

// Xaudio
#include <xaudio2.h>

// チャンクヘッダ
struct ChunkHeader {
	char id[4]; // チャンク毎のID
	int32_t size; // チャンクサイズ
};

// RIFFヘッダチャンク
struct RiffHeader {
	ChunkHeader chunk; // Riff
	char type[4]; // WAVE
};

// FMTチャンク
struct FormatChunk {
	ChunkHeader chunk; // fmt
	WAVEFORMATEX fmt; // 波形フォーマット
};

// 音声データ
struct SoundData {
	WAVEFORMATEX wfex;
	BYTE* pBuffer;
	unsigned int bufferSize;
};