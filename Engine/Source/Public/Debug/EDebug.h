#pragma once

// External Libs
#include <GLM/vec2.hpp>
#include <GLM/vec3.hpp>
#include <iostream>
#include <string>
#include <sstream>

// Internal Libs
#include "EngineTypes.h"

#define SHOW_LOGS true
#define SHOW_DEBUG true

enum EELogType : uint8_t {
	LT_LOG = 0U,
	LT_WARNING,
	LT_ERROR,
	LT_SUCCESS,
    LT_DEBUG
};

class EDebug {
public:
    template<typename... Args>
    static void Log(const EELogType logType, Args&&... args) {
        if (!SHOW_LOGS || logType == LT_DEBUG && !SHOW_DEBUG) return;

        std::ostringstream oss;
        (oss << ... << Format(args)); // Formats each arg and passes into stream

        switch (logType) {
            case LT_WARNING:    std::cout << "[WARNING] ";  break;
            case LT_ERROR:      std::cerr << "[ERROR] ";    break;
            case LT_SUCCESS:    std::cout << "[SUCCESS] ";  break;
            case LT_DEBUG:      std::cout << "[DEBUG] ";    break;
            default:            std::cout << "[LOG] ";      break;
        }

        auto& stream = (logType == LT_ERROR) ? std::cerr : std::cout;
        stream << oss.str() << "\n";
    }

    template<typename... Args>
    static void Log(Args&&... args) {
        Log(LT_LOG, std::forward<Args>(args)...);
    }

private:
    static std::string Format(const glm::vec2& v) {
        return "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ")";
    }
    static std::string Format(const glm::vec3& v) {
        return "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z) + ")";
    }
    template<typename T>
    static std::string Format(const T& val) {
        if constexpr (std::is_arithmetic_v<T>) {
            return std::to_string(val);
        }
        else if constexpr (std::is_convertible_v<T, std::string_view>) {
            // Handles string types
            return std::string(std::string_view(val));
        }
        else {
            // Pass as string stream
            std::ostringstream tmp;
            tmp << val;
            return tmp.str();
        }
    }
};