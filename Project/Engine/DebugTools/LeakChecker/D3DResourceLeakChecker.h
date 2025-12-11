
#pragma once

// リークチェッカ

namespace magi {
class D3DResourceLeakChecker {
public:
	D3DResourceLeakChecker() = default;
	~D3DResourceLeakChecker();
};

} // namespace magi
