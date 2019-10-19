//
// Created by 404Polaris on 2019/10/18.
//


#pragma once

#include <entt/entt.hpp>
#include <sirius/core/network/tcp_server.hpp>
#include <sirius/server/login-server/component/session.h>
#include <sirius/server/login-server/system/system_base.hpp>
#include <sirius/server/login-server/system/network_system.h>
#include <sirius/server/login-server/component/login_status.h>
#include <sirius/server/login-server/game/network/message_reader.h>

#include <chrono>

namespace sirius::login_server {
	namespace _system_ = sirius::login_server::system;

	class app : public std::enable_shared_from_this<app> {
	public:
		using session_type = sirius::core::tcp_session<game::message_reader, app>;
		using tcp_server = sirius::core::tcp_server<session_type, app>;
	private:
		entt::registry registry_;
		std::atomic_bool running_;
		std::unique_ptr<tcp_server> server_;
		std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> update_time_;
	protected:
		std::vector<std::unique_ptr<_system_::system_base>> system_list_;
	public:
		app();
		~app() = default;
	private:
		void init_system();
		void update_system();
		void create_system();
	public:
		void start();
		void stop();
		void on_connect(const std::shared_ptr<session_type> &session);
	};

	inline app::app() : running_(false) {
		update_time_ = std::chrono::steady_clock::now();
		server_ = std::make_unique<decltype(server_)::element_type>(3724);
	}
	inline void app::start() {
		running_ = true;
		server_->register_env(shared_from_this());
		server_->start();

		create_system();
		init_system();
		update_system();
	}

	inline void app::update_system() {
		while (running_) {
			auto delta = std::chrono::steady_clock::now() - update_time_;

			for (auto &s : system_list_) {
				s->update(delta.count() * 1.0 / 10E9, registry_);
			}

			update_time_ = std::chrono::steady_clock::now();
		}
	}

	inline void app::init_system() {
		for (auto &s : system_list_) {
			s->init(registry_);
		}
	}

	inline void app::create_system() {
		system_list_.emplace_back(std::make_unique<_system_::network_system>());
	}

	inline void app::stop() {
		running_ = false;
	}

	inline void app::on_connect(const std::shared_ptr<session_type> &session) {
		auto entity = registry_.create();
		session->register_env(shared_from_this());
		registry_.assign<component::session>(entity, session);
		registry_.assign<component::login_status>(entity, component::auth_status::kStatusChallenge);
	}
}


