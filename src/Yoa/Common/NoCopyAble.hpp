//
// Created by 404Polaris on 2019/10/15.
//


#pragma once
namespace Yoa {

	class NoCopyAble {
	public:
		NoCopyAble() = default;
		~NoCopyAble() = default;
		NoCopyAble(const NoCopyAble &) = delete;
		NoCopyAble &operator=(const NoCopyAble &) = delete;
	};
}


