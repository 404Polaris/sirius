//
// Created by 404Polaris on 2019/10/18.
//


#pragma once

#include <entt/entt.hpp>
#include <Sirius/Common/TcpServer.hpp>
#include <Sirius/Server/LoginServer/Component/Session.h>
#include <Sirius/Server/LoginServer/System/SystemBase.hpp>
#include <Sirius/Server/LoginServer/System/LoginSystem.h>
#include <Sirius/Server/LoginServer/System/NetworkSystem.h>
#include <Sirius/Server/LoginServer/Game/Network/MessageReader.h>

#include <chrono>

namespace Sirius::LoginServer {
	namespace _system_ = Sirius::LoginServer::System;

	class App : public std::enable_shared_from_this<App> {
	public:
		using _Session_type = Sirius::TcpSession<Game::MessageReader, App>;
		using _Tcp_server_type = Sirius::TcpServer<_Session_type, App>;
	private:
		entt::registry registry_;
		std::atomic_bool running_;
		std::unique_ptr<_Tcp_server_type> server_;
		std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> update_time_;
	protected:
		std::vector<std::unique_ptr<_system_::SystemBase>> system_list_;
	public:
		App();
		~App() = default;
	private:
		void InitSystem();
		void UpdateSystem();
		void CreateSystem();
	public:
		void Start();
		void Stop();
		void OnConnect(const std::shared_ptr<_Session_type> &session);
	};

	inline App::App() : running_(false) {
		update_time_ = std::chrono::steady_clock::now();
		server_ = std::make_unique<decltype(server_)::element_type>(3724);
	}
	inline void App::Start() {
		running_ = true;
		server_->RegisterEnv(shared_from_this());
		server_->Start();

		CreateSystem();
		InitSystem();
		UpdateSystem();
	}

	inline void App::UpdateSystem() {
		while (running_) {
			auto delta = std::chrono::steady_clock::now() - update_time_;

			for (auto &s : system_list_) {
				s->Update(delta.count() * 1.0 / 10E9, registry_);
			}

			update_time_ = std::chrono::steady_clock::now();
		}
	}

	inline void App::InitSystem() {
		for (auto &s : system_list_) {
			s->Init(registry_);
		}
	}

	inline void App::CreateSystem() {
		system_list_.emplace_back(std::make_unique<_system_::NetworkSystem>());
		system_list_.emplace_back(std::make_unique<_system_::LoginSystem>());
	}

	inline void App::Stop() {
		running_ = false;
	}

	inline void App::OnConnect(const std::shared_ptr<_Session_type> &session) {
		auto entity = registry_.create();
		session->RegisterEnv(shared_from_this());
		registry_.assign<Component::Session>(entity, AuthStatus::kStatusClosed, session);
	}
}


