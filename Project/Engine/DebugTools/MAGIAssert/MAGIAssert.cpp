#include "MAGIAssert.h"

#include <cstdlib> 
#include <cassert>
#include <combaseapi.h>

void MAGIAssert::Assert(bool test, const std::string& message) {
	if (test) {
		return;
	}

	Logger::Log("Assertion Failed!\n");
	Logger::Log(message);
	Logger::Finalize();
	CoUninitialize();

#if defined(DEBUG)||defined(DEVELOP)
	std::abort();
#else
	std::exit(EXIT_FAILURE);
#endif

}
