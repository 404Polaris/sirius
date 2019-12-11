//
// Created by 404Polaris on 2019/10/19.
//

#pragma once

#include <Yoa/Server/LoginServer/System/SystemBase.hpp>
#include <Yoa/Server/LoginServer/Component/Session.h>
#include <Yoa/Net/MessageBuffer.hpp>

namespace Yoa::LoginServer::System {
	class LoginSystem : public SystemBase {
	public:
		static void Load(Environment &env);
	private:
		static void RegisterEvent(Environment &env);
		static void HandleAuthLogonChallenge(EventBase *evt);
	};
}


