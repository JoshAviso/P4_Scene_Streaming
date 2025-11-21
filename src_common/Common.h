#pragma once

#include <cstdint>

using uint = unsigned int;
using uint8 = uint8_t;

#include <vector>
template <typename T>
using List = std::vector<T>;

#include <unordered_map>
template <typename T, typename T2>
using Dict = std::unordered_map<T, T2>;

#include <map>
template <typename T, typename T2>
using Map = std::map<T, T2>;

#include <queue>
template <typename T>
using Queue = std::queue<T>;

#include <memory>
template <typename T>
using Unique = std::unique_ptr<T>;
template <typename T>
using Shared = std::shared_ptr<T>;
template <typename T, typename... Args>
inline Unique<T> Make_Unique(Args&&... args) {
	return std::make_unique<T>(std::forward<Args>(args)...);
};
template <typename T, typename... Args>
inline Shared<T> Make_Shared(Args&&... args) {
	return std::make_unique<T>(std::forward<Args>(args)...);
};