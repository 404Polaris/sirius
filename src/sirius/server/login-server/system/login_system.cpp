//
// Created by 404Polaris on 2019/10/19.
//

#include <sirius/server/login-server/system/login_system.h>
#include <sirius/server/login-server/component/remote_cmd_handler_map.h>

namespace sirius::login_server::system {

	void login_system::init(entt::registry &registry) {
		auto view = registry.view<component::remote_cmd_handler_map>();

		if (!view.empty()) {
			auto &delegate_map = view.raw()->delegate_map_;
			delegate_map[login_server_cmd::kAuthLogonChallenge].connect<&login_system::handle_auth_logon_challenge>();
		}
	}

	void login_system::handle_auth_logon_challenge(entt::registry &registry, core::message_buffer &msg_buffer) {
		fmt::print("handle_auth_logon_challenge \n");
	}
}


