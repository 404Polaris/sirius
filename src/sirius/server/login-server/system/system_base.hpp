//
// Created by 404Polaris on 2019/10/19.
//

#pragma once

#include <entt/entt.hpp>

namespace sirius::login_server::system {

	class system_base {
	public:
		virtual void init(entt::registry &registry) {};
		virtual void update(double delta, entt::registry &registry) {};
	};
}
