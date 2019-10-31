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
		ThreadPool thread_pool_;
		entt::registry registry_;
		std::atomic_bool running_;
		std::unique_ptr<_Tcp_server_type> server_;
		std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> update_time_;
	protected:
		std::vector<std::unique_ptr<_system_::SystemBase>> system_list_;
	public:
		App();
		~App();
	private:
		void InitSystem();
		void UpdateSystem();
		void CreateSystem();
	public:
		void Stop();
		void Start();
		void OnConnect(const std::shared_ptr<_Session_type> &session);
	public:
		entt::registry &GetRegistry();

		template<typename _Task_type>
		void Async(_Task_type &&task) {
			thread_pool_.Post(std::forward<_Task_type>(task));
		}
	};
}


