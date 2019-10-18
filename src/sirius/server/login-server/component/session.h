//
// Created by 404Polaris on 2019/10/18.
//


#pragma once

#include <sirius/core/network/tcp_session.hpp>
#include <memory>

namespace sirius::login_server::component {
	template<typename _real_session_type>
	struct session {
		std::shared_ptr<_real_session_type> real_session_;
	};
}


