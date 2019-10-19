//
// Created by 404Polaris on 2019/10/18.
//


#pragma once

#include <sirius/core/network/tcp_session.hpp>
#include <sirius/core/network/message_buffer.hpp>

#include <queue>
#include <memory>

namespace sirius::login_server::component {
	template<typename _session_type>
	struct session {
		std::shared_ptr<_session_type> session_;
		std::queue<core::message_buffer> read_buffer_queue_;
		std::queue<core::message_buffer> write_buffer_queue_;
	};
}


