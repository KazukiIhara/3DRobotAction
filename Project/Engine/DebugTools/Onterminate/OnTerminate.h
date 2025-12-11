#pragma once

#include <exception>
#include "Logger/Logger.h"



namespace magi {
void OnTerminate() {
    Logger::Log("std::terminate による異常終了が発生しました");
    Logger::Finalize();

    std::abort(); // returnせずに終了
}

} // namespace magi
