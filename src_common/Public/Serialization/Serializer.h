#pragma once
#include <String.hpp>
#include <Logger.hpp>
#include <Serialization/ISerializable.h>
class Serializer {
	static String Serialize(const ISerializable& s) {
		String res = s.Serialize();
		if (IsEmptyOrWhitespace(res)) {
			Logger::LogWarning("Failed to serialize!");
			return String();
		}
		return res;
	}
	template <typename TSerializable>
	static TSerializable* Deserialize(const String& str) {
		static_assert(std::is_base_of<ISerializable, TSerializable>::value, "Class must inherit from ISerializable");

		TSerializable* res = new TSerializable();
		if (!res.Deserialize(str)) {
			Logger::LogWarning("Failed to deserialize!");
			return nullptr;
		}
		return res;
	}
};