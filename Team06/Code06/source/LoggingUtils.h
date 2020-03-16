#pragma once

#include <string>

namespace SPA {
    namespace LoggingUtils {
        enum class Level {
            DEBUG, INFO, ERROR
        };

        void LogDebugMessage(std::string format, ...);
        void LogInfoMessage(std::string format, ...);
        void LogErrorMessage(std::string format, ...);

    }
}