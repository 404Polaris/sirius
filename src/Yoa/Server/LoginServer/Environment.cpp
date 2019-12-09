//
// Created by 404Polaris on 2019/10/31.
//

#include <Yoa/Server/LoginServer/Environment.h>
#include <Yoa/Server/LoginServer/Event/Events.h>
#include <Yoa/Server/LoginServer/System/LoginSystem.h>
#include <Yoa/Server/LoginServer/System/NetworkSystem.h>

namespace Yoa::LoginServer {

	Environment::Environment() : running_(false) {
		server_ = std::make_unique<decltype(server_)::element_type>(3724, [this](auto &session) {
			Sync([this, session]() {
				auto entity = registry_.create();
				registry_.assign<Component::Session>(entity, AuthStatus::kStatusClosed, session);
			});
		});
	}

	void Environment::Start() {
		running_ = true;
		server_->Start();
		timer_heap_.Start();
		sync_executor_.Start();
		async_executor_.Start();

		InitSystem();
		TriggerUpdateEvent();
	}

	void Environment::InitSystem() {
		System::NetworkSystem::Load(*this);
		System::LoginSystem::Load(*this);
	}

	void Environment::TriggerUpdateEvent() {
		if (running_) {
			TriggerEvent(Event::UpdateEvent{*this});
			Sync([this]() { TriggerUpdateEvent(); });
		}
	}

	entt::registry &Environment::GetRegistry() {
		return registry_;
	}

	void Environment::Stop() {
		running_ = false;
		timer_heap_.Stop();
		sync_executor_.Stop();
		async_executor_.Stop();
	}

	Environment::~Environment() {
		Stop();
	}
}