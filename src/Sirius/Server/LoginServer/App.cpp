//
// Created by 404Polaris on 2019/10/31.
//

#include <Sirius/Server/LoginServer/App.h>
#include <Sirius/Common/Timer.hpp>

namespace Sirius::LoginServer {

	App::App() : running_(false) {
		update_time_ = std::chrono::steady_clock::now();
		server_ = std::make_unique<decltype(server_)::element_type>(3724);
	}

	void App::Start() {
		running_ = true;
		server_->RegisterEnv(shared_from_this());
		server_->Start();

		CreateSystem();
		InitSystem();
		MainLoop();
	}

	void App::MainLoop() {
		while (running_) {
			Update();
			LateUpdate();
		}
	}

	void App::InitSystem() {
		for (auto &s : system_list_) {
			s->Init(*this);
		}
	}

	void App::Update() {
		auto delta = std::chrono::steady_clock::now() - update_time_;

		for (auto &s : system_list_) {
			s->Update(delta.count() * 1.0 / 10E9, *this);
		}

		update_time_ = std::chrono::steady_clock::now();
	}

	void App::CreateSystem() {
		system_list_.emplace_back(std::make_unique<_system_::NetworkSystem>());
		system_list_.emplace_back(std::make_unique<_system_::LoginSystem>());
	}

	void App::LateUpdate() {
		auto delta = std::chrono::steady_clock::now() - late_update_time_;

		while (auto opt = action_queue_.Pop()) {
			auto action = std::move(opt.value());
			action();
		}

		for (auto &s : system_list_) {
			s->LateUpdate(delta.count() * 1.0 / 10E9, *this);
		}

		late_update_time_ = std::chrono::steady_clock::now();
	}

	entt::registry &App::GetRegistry() {
		return registry_;
	}

	void App::Stop() {
		running_ = false;
	}

	void App::OnConnect(const std::shared_ptr<_Session_type> &session) {
		auto entity = registry_.create();
		session->RegisterEnv(shared_from_this());
		registry_.assign<Component::Session>(entity, AuthStatus::kStatusClosed, session);
	}

	App::~App() {
		thread_pool_.Stop();
		system_list_.clear();
	}
}