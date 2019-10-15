//
// Created by 404Polaris on 2019/10/15.
//


#pragma once
namespace sirius::core {

	class nocopyable {
	public:
		nocopyable() = default;
		~nocopyable() = default;
		nocopyable(const nocopyable &) = delete;
		nocopyable &operator=(const nocopyable &) = delete;
	};
}


