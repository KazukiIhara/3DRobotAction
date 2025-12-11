#pragma once

#include "MAGI.h"

// 実行するゲームクラス
class Game :public MAGI::MAGISYSTEM {
public:
	~Game();
	// 初期化
	void Initialize()override;
};