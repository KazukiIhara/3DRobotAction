#pragma once

#include "Logger/Logger.h"


namespace magi {
/// <summary>
/// 自作Assertクラス
/// </summary>
class MAGIAssert {
public:
	static void Assert(bool test, const std::string& message);

};
}
