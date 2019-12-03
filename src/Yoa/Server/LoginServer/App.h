//
// Created by 404Polaris on 2019/10/18.
//


#pragma once

#include <entt/entt.hpp>
#include <Yoa/Common/Timer.hpp>
#include <Yoa/Common/TcpServer.hpp>
#include <Yoa/Common/ThreadPool.hpp>
#include <Yoa/Common/ActionQueue.hpp>
#include <Yoa/Server/LoginServer/Component/Session.h>
#include <Yoa/Server/LoginServer/Game/Network/MessageReader.h>

#include <chrono>

namespace Yoa::LoginServer {
	class App : public std::enable_shared_from_this<App> {
	public:
		using _Session_type = Yoa::TcpSession<Game::MessageReader>;
		using _Tcp_server_type = Yoa::TcpServer<_Session_type>;
	private:
		TimerPool timer_pool_;
		ThreadPool thread_pool_;
		entt::registry registry_;
		ActionQueue action_queue_;
		std::atomic_bool running_;
		std::unique_ptr<_Tcp_server_type> server_;
	public:
		App();
		~App();
	private:
		void InitSystem();
	public:
		void Stop();
		void Start();
	public:
		entt::registry &GetRegistry();

		template<typename _Task_type>
		void Async(_Task_type &&task) {
			thread_pool_.Post(std::forward<_Task_type>(task));
		}

		template<typename _Task_type>
		void Sync(_Task_type &&task) {
			action_queue_.Push(std::forward<_Task_type>(task));
		}

		void MainLoop();
	};
}


