//
// Created by 404Polaris on 2019/10/15.
//


#pragma once

#include <Sirius/Server/LoginServer/System/SystemBase.hpp>
#include <Sirius/Server/LoginServer/Component/Session.h>
#include <fmt/format.h>

namespace Sirius::LoginServer::System {

	class NetworkSystem : public SystemBase {
	public:
		void Init(App &app) override;
		void Update(double delta, App &app) override;
	public:
		static void HandleMessage(Component::Session &session, App &app);
	};
}


