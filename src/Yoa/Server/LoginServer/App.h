//
// Created by 404Polaris on 2019/10/18.
//


#pragma once

#include <entt/entt.hpp>
#include <Yoa/Common/Timer.hpp>
#include <Yoa/NetWork/TcpServer.hpp>
#include <Yoa/Common/ASyncExecutor.hpp>
#include <Yoa/Common/SyncExecutor.hpp>
#include <Yoa/Server/LoginServer/Component/Session.h>
#include <Yoa/Server/LoginServer/Game/Network/MessageReader.h>

#include <chrono>

namespace Yoa::LoginServer {
	class App : public std::enable_shared_from_this<App> {
	public:
		using _Session_type = Yoa::TcpSession<Game::MessageReader>;
		using _Tcp_server_type = Yoa::TcpServer<_Session_type>;
	private:
		TimerHeap timer_heap_;
		entt::registry registry_;
		std::atomic_bool running_;
		SyncExecutor sync_executor_;
		ASyncExecutor async_executor_;
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
			async_executor_.Post(std::forward<_Task_type>(task));
		}

		template<typename _Task_type>
		void Sync(_Task_type &&task) {
			sync_executor_.Push(std::forward<_Task_type>(task));
		}
	};
}


