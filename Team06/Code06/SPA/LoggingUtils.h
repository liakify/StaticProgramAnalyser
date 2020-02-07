#pragma once

#include <string>

namespace SPA {
	namespace LoggingUtils {
		enum Level {
			DEBUG, ERROR
		};

		void LogDebugMessage(std::string format, ...);
		void LogErrorMessage(std::string format, ...);

	}
}