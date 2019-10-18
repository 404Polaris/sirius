//
// Created by 404Polaris on 2019/10/18.
//


#pragma once

#include <sirius/core/network/tcp_server.hpp>
#include <sirius/server/login-server/component/session.h>
#include <sirius/server/login-server/game/network/message_reader.h>

namespace sirius::login_server {
	class app {
	private:
		entt::registry registry;
		std::unique_ptr<sirius::core::tcp_server<app>> server_;
	public:
		app();
		~app() = default;
	private:
		void update();
	public:
		template<typename _session_type>
		void on_connect(_session_type session);

		void start();
	};

	app::app() {
		server_ = std::make_unique<decltype(server_)::element_type>(3724);
	}

	template<typename _session_type>
	void app::on_connect(_session_type session) {
		auto entity = registry.create();
		registry.assign<component::session<game::message_reader, app>>(entity, session);
	}

	void app::start() {
		server_->start();
	}

	void app::update() {

	}
}


