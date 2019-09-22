//
// Created by 404Polaris on 2019/9/19.
//

#pragma once

#include <sirius/common/noncopyable.h>

#include <chrono>
#include <thread>
#include <type_traits>

namespace sirius::common {

	template<typename T, typename = typename std::enable_if<std::is_invocable_r<bool, T>::value>::type>
	inline void ReleaseCPUUntil(T &&checker) {
		while (!checker()) {
			const static auto sleep_time = std::chrono::nanoseconds(1);
			std::this_thread::sleep_for(sleep_time);
		}
	}
}
