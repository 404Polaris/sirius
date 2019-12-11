//
// Created by 404Polaris on 2019/12/10.
//

#pragma once

#include <functional>

namespace Yoa {
	template<class T>
	inline void HashCombine(std::size_t &seed, const T &v) {
		std::hash<T> hasher{};
		seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2); // NOLINT(hicpp-signed-bitwise)
	}
}
