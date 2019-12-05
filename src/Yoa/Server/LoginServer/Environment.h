//
// Created by 404Polaris on 2019/10/18.
//


#pragma once

#include <entt/entt.hpp>
#include <Yoa/Common/Timer.hpp>
#include <Yoa/Net/TcpServer.hpp>
#include <Yoa/Common/ASyncExecutor.hpp>
#include <Yoa/Common/SyncExecutor.hpp>
#include <Yoa/Server/LoginServer/Component/Session.h>
#include <Yoa/Server/LoginServer/Game/Net/MessageReader.h>

#include <chrono>

namespace Yoa::LoginServer {
	class Environment : public std::enable_shared_from_this<Environment> {
	public:
		using Session_type = Yoa::TcpSession<Game::MessageReader>;
		using Server_type = Yoa::TcpServer<Session_type>;
	private:
		TimerHeap timer_heap_;
		entt::registry registry_;
		std::atomic_bool running_;
		SyncExecutor sync_executor_;
		ASyncExecutor async_executor_;
		entt::dispatcher event_dispatcher_{};
		std::unique_ptr<Server_type> server_;
	public:
		Environment();
		~Environment();
	public:
		void Stop();
		void Start();
	private:
		void InitSystem();
		void TriggerUpdateEvent();
	public:
		entt::registry &GetRegistry();

		template<typename _Event_t>
		void TriggerEvent(_Event_t &&evt) {
			Sync([this, evt_ = std::forward<_Event_t>(evt)]() {
				event_dispatcher_.trigger<_Event_t>(std::move(evt_));
			});
		}

		template<typename _Event_t, typename ...Args>
		void TriggerEvent(Args &&... args) {
			TriggerEvent<_Event_t>(_Event_t{std::forward<Args>(args)...});
		}

		template<typename _Event_t, auto _Event_handler_t>
		void RegisterEvent() {
			Sync([this] {
				event_dispatcher_.sink<_Event_t>().template connect<_Event_handler_t>();
			});
		}

		template<typename _Event_t, auto _Event_handler_t>
		void UnRegisterEvent() {
			Sync([this] {
				event_dispatcher_.sink<_Event_t>().template disconnect<_Event_handler_t>();
			});
		}

		template<typename _Task_t>
		void Async(_Task_t &&task) {
			async_executor_.Post(std::forward<_Task_t>(task));
		}

		template<typename _Task_t>
		void Sync(_Task_t &&task) {
			sync_executor_.Push(std::forward<_Task_t>(task));
		}
	};
}


