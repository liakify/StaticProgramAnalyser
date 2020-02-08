#include <cstdarg>
#include "LoggingUtils.h"

namespace SPA {
	namespace LoggingUtils {

		void Log(LoggingUtils::Level severityLevel, std::string format, std::va_list params) {
			va_start(params, format);
			
			switch (severityLevel) {
			case LoggingUtils::Level::DEBUG:
				std::printf("[DEBUG] ");
				break;
			case LoggingUtils::Level::ERROR:
				std::printf("[ERROR] ");
			}
			
			std::vprintf(format.c_str(), params);
		}

		void LogDebugMessage(std::string format, ...) {
#ifdef DEBUG
			std::va_list params;
			va_start(params, format);
			Log(Level::DEBUG, format, params);
#endif
		}

		void LogErrorMessage(std::string format, ...) {
			std::va_list params;
			va_start(params, format);
			Log(Level::ERROR, format, params);
		}

	}

}