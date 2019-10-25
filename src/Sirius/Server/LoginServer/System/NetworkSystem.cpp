//
// Created by 404Polaris on 2019/10/15.
//

#include <Sirius/Server/LoginServer/System/NetworkSystem.h>
#include <Sirius/Server/LoginServer/Component/Session.h>
#include <Sirius/Server/LoginServer/Component/RemoteCmdHandlerMap.h>
#include <Sirius/Server/LoginServer/App.hpp>

namespace Sirius::LoginServer::System {
	void NetworkSystem::Init(entt::registry &registry) {
		auto entity = registry.create();
		registry.assign<Component::RemoteCmdHandlerMap>(entity);
	}

	void NetworkSystem::Update(double delta, entt::registry &registry) {
		auto view = registry.view<Component::Session>();

		for (auto entity: view) {
			Component::Session &session = view.get(entity);

			if (session.session_->WorkFine()) {
				HandleMessage(session, registry);
				WriteMessage(session);
			} else {
				registry.destroy(entity);
			}
		}
	}

	void NetworkSystem::WriteMessage(Component::Session &session) {
		while (!session.write_buffer_queue_.empty()) {
			session.session_->Write(std::move(session.write_buffer_queue_.front()));
			session.write_buffer_queue_.pop();
		}
	}

	void NetworkSystem::HandleMessage(Component::Session &session, entt::registry &registry) {
		auto view = registry.view<Component::RemoteCmdHandlerMap>();

		if (view.empty())return;

		while (true) {
			auto &&opt = session.session_->Read();

			if (opt) {
				auto buffer = std::move(opt.value());
				auto cmd = (LoginServerCmd) buffer.Data()[0];
				auto &delegate = view.raw()->delegate_map_[cmd];

				if (delegate) {
					delegate(session, buffer, registry);
				}
			} else {
				break;
			}
		}
	}
}
