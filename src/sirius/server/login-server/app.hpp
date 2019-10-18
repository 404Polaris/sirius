//
// Created by 404Polaris on 2019/10/18.
//


#pragma once

#include <sirius/core/network/tcp_server.hpp>
#include <sirius/server/login-server/component/session.h>
#include <sirius/server/login-server/game/network/message_reader.h>

namespace sirius::login_server {
	class app : public std::enable_shared_from_this<app> {
		using session_type = sirius::core::tcp_session<game::message_reader, app>;
	private:
		entt::registry registry;
		std::unique_ptr<sirius::core::tcp_server<session_type, app>> server_;
	public:
		app();
		~app() = default;
	private:
		void update();
	public:
		void start();
		void on_connect(const std::shared_ptr<session_type> &session);
	};

	app::app() {
		server_ = std::make_unique<decltype(server_)::element_type>(3724);
	}

	void app::on_connect(const std::shared_ptr<session_type> &session) {
		auto entity = registry.create();
		session->register_env(shared_from_this());
		registry.assign<component::session<session_type>>(entity, session);
	}

	void app::start() {
		server_->register_env(shared_from_this());
		server_->start();
	}

	void app::update() {

	}
}


