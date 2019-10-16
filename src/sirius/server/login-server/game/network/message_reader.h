//
// Created by 404Polaris on 2019/10/15.
//


#pragma once

#include <sirius/core/nocopyable.hpp>
#include <sirius/server/login-server/game/network/protocol.h>
#include <sirius/core/network/message_buffer.hpp>

#include <vector>

namespace sirius::login_server::game {

	class message_reader : public core::nocopyable {
	protected:
		core::message_buffer buffer_;
	public:
		void decode();

	};
}


