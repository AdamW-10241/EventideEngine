#pragma once

// External Libs
#include <GLM/vec3.hpp>
#include <iostream>
#include <string>

// Internal Libs
#include "EngineTypes.h"

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

	// Log a message to the console based on log type as a vec3
	static void Log(const glm::vec3& vec3Msg, const EELogType& logType = LT_LOG) {
		std::cout << std::to_string(vec3Msg.x) << ", "
				  << std::to_string(vec3Msg.y) << ", " 
			      << std::to_string(vec3Msg.z) << std::endl;
	}
};