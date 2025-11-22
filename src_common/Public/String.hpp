#pragma once

#include <string>
using String = std::string;

inline String ToString(int val) { return std::to_string(val); };
inline String ToString(bool val) { return val ? "True" : "False"; };

#include <cstring>

inline bool IsNullOrWhiteSpace(const String* str) {
	if (str == nullptr) return true;
	if (str->empty()) return true;
	for (char ch : *str) {
		if (!std::isspace(static_cast<unsigned char>(ch))) {
			return false;
		}
	}
	return true;
}

inline bool IsEmptyOrWhitespace(const String& str) {
	if (str.empty()) return true;
	for (char ch : str) {
		if (!std::isspace(static_cast<unsigned char>(ch))) {
			return false;
		}
	}
	return true;
}