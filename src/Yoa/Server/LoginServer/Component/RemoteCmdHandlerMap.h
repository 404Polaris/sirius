//
// Created by 404Polaris on 2019/10/20.
//

#pragma  once

#include <Yoa/Server/LoginServer/Game/Network/Protocol.h>
#include <Yoa/Server/LoginServer/Component/Session.h>

#include <entt/entt.hpp>
#include <unordered_map>

namespace Yoa::LoginServer::Component {
	struct RemoteCmdHandlerMap {
		using _Handler_type = entt::delegate<void(Session & , App & , MessageBuffer & )>;
		std::unordered_map<LoginServerCmd, _Handler_type> delegate_map_;
	};
}
