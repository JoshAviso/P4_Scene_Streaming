
#pragma once

#include <iostream>
#include "String.h"

class Logger {
public:
	enum ELogLevel { Error, Warning, Info };

public:
	static void Log(ELogLevel level, const String& message) {
		if (level < LogLevel) return;
		String prefix = String();
		switch (level) {
		case ELogLevel::Error:   prefix = "\033[31m[ERROR]:\033[0m "; break;
		case ELogLevel::Warning: prefix = "\033[33m[WARN]:\033[0m "; break;
		case ELogLevel::Info:    prefix = "\033[36m[INFO]:\033[0m "; break;
		}
		std::cout << prefix << message << std::endl;
	};
	static void Log(const String& message) { Log(ELogLevel::Info, message); };
	static void LogWarning(const String& message) { Log(ELogLevel::Warning, message); };
	static void LogError(const String& message) { Log(ELogLevel::Error, message); };

	inline static ELogLevel LogLevel = ELogLevel::Error;
};
