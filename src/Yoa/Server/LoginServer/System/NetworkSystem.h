//
// Created by 404Polaris on 2019/10/15.
//


#pragma once

#include <Yoa/Server/LoginServer/System/SystemBase.hpp>
#include <Yoa/Server/LoginServer/Component/Session.h>
#include <fmt/format.h>

namespace Yoa::LoginServer::System {

	class NetworkSystem : public SystemBase {
	public:
		static void Init(App &app);
	private:
		static void Update(double delta, App &app);
		static void HandleMessage(Component::Session &session, App &app);
	};
}


