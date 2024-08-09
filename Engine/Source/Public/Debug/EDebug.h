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

	// Log a message to the console based on log type as a uInt
	static void Log(const unsigned int& uIntMsg, const EELogType& logType = LT_LOG) {
		std::cout << std::to_string(uIntMsg) << std::endl;
	}

	// Log a message to the console based on log type as an int
	static void Log(const int& intMsg, const EELogType& logType = LT_LOG) {
		std::cout << std::to_string(intMsg) << std::endl;
	}
	
	// Log a message to the console based on log type as a float
	static void Log(const float& floatMsg, const EELogType& logType = LT_LOG) {
		std::cout << std::to_string(floatMsg) << std::endl;
	}
};