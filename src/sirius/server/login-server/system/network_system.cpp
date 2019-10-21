//
// Created by 404Polaris on 2019/10/15.
//

#include <sirius/server/login-server/system/network_system.h>
#include <sirius/server/login-server/component/session.h>
#include <sirius/server/login-server/component/remote_cmd_handler_map.h>
#include <sirius/server/login-server/app.hpp>

namespace sirius::login_server::system {
	void network_system::init(entt::registry &registry) {
		auto entity = registry.create();
		registry.assign<component::remote_cmd_handler_map>(entity);
	}

	void network_system::update(double delta, entt::registry &registry) {
		auto view = registry.view<component::session, component::login_status>();

		for (auto entity: view) {
			component::session &session = view.get<component::session>(entity);

			if (session.session_->work_fine()) {
				handle_message(session, registry);
				write_message(session);
			}
		}
	}

	void network_system::write_message(component::session &session) {
		while (!session.write_buffer_queue_.empty()) {
			session.session_->write(std::move(session.write_buffer_queue_.front()));
			session.write_buffer_queue_.pop();
		}
	}

	void network_system::handle_message(component::session &session, entt::registry &registry) {
		auto view = registry.view<component::remote_cmd_handler_map>();

		if (view.empty())return;

		while (true) {
			auto &&opt = session.session_->read();

			if (opt) {
				auto buffer = std::move(opt.value());
				auto cmd = (login_server_cmd) buffer.data()[0];
				auto &delegate = view.raw()->delegate_map_[cmd];

				if (delegate) {
					delegate(registry, buffer);
				}
			} else {
				break;
			}
		}
	}
}
