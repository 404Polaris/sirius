//
// Created by 404Polaris on 2019/10/15.
//


#pragma once

#include <Sirius/Server/LoginServer/System/SystemBase.hpp>

#include <fmt/format.h>
#include <Sirius/Server/LoginServer/Component/Session.h>

namespace Sirius::LoginServer::System {

	class NetworkSystem : public SystemBase {
	public:
		void init(entt::registry &registry) override;
		void update(double delta, entt::registry &registry) override;
	public:
		static void HandleMessage(Component::Session &session, entt::registry &registry);
		static void WriteMessage(Component::Session &session);
	};
}


