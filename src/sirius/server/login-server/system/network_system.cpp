//
// Created by 404Polaris on 2019/10/15.
//

#include <sirius/server/login-server/system/network_system.h>
#include <sirius/server/login-server/component/session.h>
#include <sirius/server/login-server/app.hpp>

namespace sirius::login_server::system {
	void network_system::update(double delta, entt::registry &registry) {
		auto view = registry.view<component::session, component::login_status>();

		for (auto entity: view) {
			component::session &session = view.get<component::session>(entity);

			if (session.session_->work_fine()) {
				read_message(session);
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

	void network_system::read_message(component::session &session) {
		while (true) {
			auto &&opt = session.session_->read();
			if (opt) {
				session.read_buffer_queue_.emplace(std::move(opt.value()));
			} else {
				break;
			}
		}
	}
}
