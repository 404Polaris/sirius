//
// Created by 404Polaris on 2019/10/20.
//

#pragma  once

#include <sirius/server/login-server/game/network/protocol.h>

#include <entt/entt.hpp>
#include <unordered_map>

namespace sirius::login_server::component {
	struct remote_cmd_handler_map {
		using call_back_type = entt::delegate<void(entt::registry &registry, core::message_buffer &)>;
		std::unordered_map<login_server_cmd, call_back_type> delegate_map_;
	};
}
