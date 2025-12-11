#pragma once

#include "Logger/Logger.h"

/// <summary>
/// 自作Assertクラス
/// </summary>
class MAGIAssert {
public:
	static void Assert(bool test, const std::string& message);

};