//
// Created by 404Polaris on 2019/9/19.
//

#pragma once

namespace sirius::common {
	class NonCopyable {
	public:
		NonCopyable() = default;
		~NonCopyable() = default;
		NonCopyable(const NonCopyable &) = delete;
		NonCopyable &operator=(const NonCopyable &) = delete;
	};
}
