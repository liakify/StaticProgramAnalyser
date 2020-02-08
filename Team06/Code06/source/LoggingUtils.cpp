#include <cstdarg>
#include <iostream>
#include "LoggingUtils.h"

namespace SPA {
	namespace LoggingUtils {

		void Log(LoggingUtils::Level severityLevel, std::string format, std::va_list params) {
			
			char buffer[1000];
			std::vsnprintf(buffer, 1000, format.c_str(), params);
			
			switch (severityLevel) {
			case LoggingUtils::Level::DEBUG:
				std::cout << "[DEBUG] " << buffer << std::endl;
				break;
			case LoggingUtils::Level::ERROR:
				std::cout << "[ERROR] " << buffer << std::endl;
				break;
			}
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