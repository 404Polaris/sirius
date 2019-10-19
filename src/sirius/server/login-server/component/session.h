//
// Created by 404Polaris on 2019/10/18.
//


#pragma once

#include <sirius/core/network/tcp_session.hpp>
#include <sirius/core/network/message_buffer.hpp>
#include <sirius/server/login-server/game/network/message_reader.h>

#include <queue>
#include <string>
#include <memory>

namespace sirius::login_server {
	class app;
}

namespace sirius::login_server::component {

	struct session {
		std::shared_ptr<sirius::core::tcp_session<game::message_reader, sirius::login_server::app>> session_;
		std::queue<core::message_buffer> read_buffer_queue_;
		std::queue<core::message_buffer> write_buffer_queue_;
	};
}


