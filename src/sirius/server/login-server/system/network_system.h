//
// Created by 404Polaris on 2019/10/15.
//


#pragma once

#include <sirius/server/login-server/system/system_base.hpp>

#include <fmt/format.h>
#include <sirius/server/login-server/component/session.h>

namespace sirius::login_server::system {

	class network_system : public system_base {
	public:
		void init(entt::registry &registry) override;
		void update(double delta, entt::registry &registry) override;
	public:
		static void handle_message(component::session &session, entt::registry &registry);
		static void write_message(component::session &session);
	};
}


