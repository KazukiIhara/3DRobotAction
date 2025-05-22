#pragma once

#include "Framework/MAGI.h"

// 実行するゲームクラス
class Game :public MAGISYSTEM {
public:
	~Game();
	// 初期化
	void Initialize()override;
};