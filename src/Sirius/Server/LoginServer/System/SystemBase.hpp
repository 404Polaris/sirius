//
// Created by 404Polaris on 2019/10/19.
//

#pragma once

#include <entt/entt.hpp>

namespace Sirius::LoginServer::System {

	class SystemBase {
	public:
		virtual void Init(entt::registry &registry) {};
		virtual void Update(double delta, entt::registry &registry) {};
		virtual void LateUpdate(double delta, entt::registry &registry) {};
	};
}
