//
// Created by 404Polaris on 2019/10/20.
//

#pragma  once

#include <Sirius/Server/LoginServer/Game/Network/Protocol.h>

#include <entt/entt.hpp>
#include <unordered_map>

namespace Sirius::LoginServer::Component {
	struct RemoteCmdHandlerMap {
		using call_back_type = entt::delegate<void(entt::registry &registry, MessageBuffer &)>;
		std::unordered_map<LoginServerCmd, call_back_type> delegate_map_;
	};
}
