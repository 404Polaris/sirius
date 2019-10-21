//
// Created by 404Polaris on 2019/10/19.
//

#include <Sirius/Server/LoginServer/System/LoginSystem.h>
#include <Sirius/Server/LoginServer/Component/RemoteCmdHandlerMap.h>

namespace Sirius::LoginServer::System {

	void LoginSystem::init(entt::registry &registry) {
		auto view = registry.view<Component::RemoteCmdHandlerMap>();

		if (!view.empty()) {
			auto &delegate_map = view.raw()->delegate_map_;
			delegate_map[LoginServerCmd::kAuthLogonChallenge].connect<&LoginSystem::HandleAuthLogonChallenge>();
		}
	}

	void LoginSystem::HandleAuthLogonChallenge(entt::registry &registry, MessageBuffer &msg_buffer) {
		fmt::print("HandleAuthLogonChallenge \n");
	}
}


