//
// Created by 404Polaris on 2019/10/19.
//

#pragma once

#include <sirius/server/login-server/system/system_base.hpp>
#include <sirius/core/network/message_buffer.hpp>

namespace sirius::login_server::system {
	class login_system : public system_base {
	public:
		void init(entt::registry &registry) override;
	private:
		static void handle_auth_logon_challenge(entt::registry &registry, core::message_buffer &msg_buffer);
	};
}


