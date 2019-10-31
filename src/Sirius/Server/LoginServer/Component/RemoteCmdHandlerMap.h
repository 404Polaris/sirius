//
// Created by 404Polaris on 2019/10/20.
//

#pragma  once

#include <Sirius/Server/LoginServer/Game/Network/Protocol.h>
#include <Sirius/Server/LoginServer/Component/Session.h>

#include <entt/entt.hpp>
#include <unordered_map>

namespace Sirius::LoginServer::Component {
	struct RemoteCmdHandlerMap {
		using _Handler_type = entt::delegate<void(Session & , App & , MessageBuffer & )>;
		std::unordered_map<LoginServerCmd, _Handler_type> delegate_map_;
	};
}
