//
// Created by 404Polaris on 2019/10/18.
//


#pragma once

#include <sirius/core/network/tcp_session.hpp>
#include <memory>

namespace sirius::login_server::component {
	template<typename _pkt_reader_type, typename _env_type>
	struct session {
		std::shared_ptr<sirius::core::tcp_session<_pkt_reader_type, _env_type>> real_session_;
	};
}


