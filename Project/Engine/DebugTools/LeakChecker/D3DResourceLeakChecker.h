#pragma once


namespace magi {
// リークチェッカ
class D3DResourceLeakChecker {
public:
	D3DResourceLeakChecker() = default;
	~D3DResourceLeakChecker();
};
}
