//
// Created by 404Polaris on 2019/10/31.
//

#include <Yoa/Server/LoginServer/App.h>
#include <Yoa/Server/LoginServer/System/LoginSystem.h>
#include <Yoa/Server/LoginServer/System/NetworkSystem.h>

namespace Yoa::LoginServer {

	App::App() : running_(false) {
		server_ = std::make_unique<decltype(server_)::element_type>(3724, [this](auto &session) {
			auto entity = registry_.create();
			registry_.assign<Component::Session>(entity, AuthStatus::kStatusClosed, session);
		});
	}

	void App::Start() {
		running_ = true;
		server_->Start();

		InitSystem();
	}

	void App::InitSystem() {

	}

	entt::registry &App::GetRegistry() {
		return registry_;
	}

	void App::Stop() {
		running_ = false;
	}

	App::~App() {
		timer_heap_.Stop();
		async_executor_.Stop();
	}
}