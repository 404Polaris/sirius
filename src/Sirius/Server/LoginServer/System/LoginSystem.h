//
// Created by 404Polaris on 2019/10/19.
//

#pragma once

#include <Sirius/Server/LoginServer/System/SystemBase.hpp>
#include <Sirius/Server/LoginServer/Component/Session.h>
#include <Sirius/Common/MessageBuffer.hpp>

namespace Sirius::LoginServer::System {
	class LoginSystem : public SystemBase {
	public:
		void Init(App &app) override;
	private:
		static void HandleAuthLogonChallenge(Component::Session &session, App &app, MessageBuffer &msg_buffer);
	};
}


