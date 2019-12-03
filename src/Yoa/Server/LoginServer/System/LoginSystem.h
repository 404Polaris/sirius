//
// Created by 404Polaris on 2019/10/19.
//

#pragma once

#include <Yoa/Server/LoginServer/System/SystemBase.hpp>
#include <Yoa/Server/LoginServer/Component/Session.h>
#include <Yoa/Common/MessageBuffer.hpp>

namespace Yoa::LoginServer::System {
	class LoginSystem : public SystemBase {
	public:
		static void Init(App &app);
	private:
		static void HandleAuthLogonChallenge(Component::Session &session, App &app, MessageBuffer &msg_buffer);
	};
}


