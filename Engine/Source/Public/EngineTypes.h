#pragma once
#include "Debug/EDebug.h"

#define PI 3.14159265358979323846

// System libs
#include <iostream>
#include <vector>
#include <cstdint>
#include <string>

// Redefine of standard string
typedef std::string EString;

// Redefine all of the unsigned ints
typedef uint8_t EUi8;
typedef uint16_t EUi16;
typedef uint32_t EUi32;
typedef uint64_t EUi64;

// Redefine the standard vector
template <typename T>
using TArray = std::vector<T>;

// Redefine the smart pointers
// Shared pointer
template <typename T>
using TShared = std::shared_ptr<T>;

// Unique pointer
template <typename T>
using TUnique = std::unique_ptr<T>;

// Weak pointer
template <typename T>
using TWeak = std::weak_ptr<T>;

// Redefine the make shared function
template <typename T, typename ... Args>
TShared<T> TMakeShared(Args&&... args) {
	return std::make_shared<T>(std::forward<Args>(args)...);
}

// Redefine the make unique function
template <typename T, typename ... Args>
TUnique<T> TMakeUnique(Args&&... args) {
	return std::make_unique<T>(std::forward<Args>(args)...);
}

// Template function to convert to EString
template <typename T>
EString toEString(T value) {
	return std::to_string(value);
}