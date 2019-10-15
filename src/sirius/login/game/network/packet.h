//
// Created by 404Polaris on 2019/10/15.
//


#pragma once

#include <sirius/core/nocopyable.hpp>
#include <sirius/core/network/message_buffer.hpp>

#include <vector>

namespace sirius::login_server::game {

	class packet : public core::nocopyable {
	public:
		inline static const auto KHeadSize = 4;
	protected:
		std::vector<std::byte> buffer_;
	public:
		void decode();
	};
}


