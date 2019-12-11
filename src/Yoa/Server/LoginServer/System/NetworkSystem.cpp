//
// Created by 404Polaris on 2019/10/15.
//

#include <Yoa/Server/LoginServer/System/NetworkSystem.h>
#include <Yoa/Server/LoginServer/Component/Session.h>
#include <Yoa/Server/LoginServer/Environment.h>

namespace Yoa::LoginServer::System {
	void NetworkSystem::Load(Environment &env) {
		RegisterEvent(env);
	}

	void NetworkSystem::RegisterEvent(Environment &env) {
		env.RegisterEvent<Event::UpdateEvent, &NetworkSystem::Update>();
	}

	void NetworkSystem::Update(EventBase *evt) {
		auto event = reinterpret_cast<UpdateEvent *>(evt);
		auto &env = event->env_;

		auto &registry = env.GetRegistry();
		auto view = registry.view<Component::Session>();

		for (auto entity: view) {
			Component::Session &session = view.get(entity);

			if (session.session_->WorkFine()) {
				HandleMessage(session, env);
			} else {
				registry.destroy(entity);
			}
		}
	}

	void NetworkSystem::HandleMessage(Component::Session &session, Environment &env) {
		while (auto opt = session.session_->Read()) {
			auto buffer = std::move(opt.value());
			auto cmd = (RemoteCommand) buffer.Data()[0];
			env.TriggerEvent<NetCmdEvent>(env, cmd, std::move(buffer), session);
		}
	}
}
