#pragma once
#include <cstdint>
#include <iostream>
#include <string>

enum EELogType : uint8_t {
	LT_LOG = 0U,
	LT_WARNING,
	LT_ERROR,
	LT_SUCCESS
};

class EDebug {
public:
	// Log a message to the console based on log type
	static void Log(const std::string& msg, const EELogType& logType = LT_LOG) {
		std::cout << msg << std::endl;
	}
};