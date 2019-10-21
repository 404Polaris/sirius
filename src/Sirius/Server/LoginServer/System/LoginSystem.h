//
// Created by 404Polaris on 2019/10/19.
//

#pragma once

#include <Sirius/Server/LoginServer/System/SystemBase.hpp>
#include <Sirius/Common/MessageBuffer.hpp>

namespace Sirius::LoginServer::System {
	class LoginSystem : public SystemBase {
	public:
		void init(entt::registry &registry) override;
	private:
		static void HandleAuthLogonChallenge(entt::registry &registry, MessageBuffer &msg_buffer);
	};
}


