//
// Created by 404Polaris on 2019/10/15.
//

#include <Yoa/Server/LoginServer/System/NetworkSystem.h>
#include <Yoa/Server/LoginServer/Component/Session.h>
#include <Yoa/Server/LoginServer/Environment.h>

namespace Yoa::LoginServer::System {
	void NetworkSystem::Init(Environment &env) {
		RegisterEvent(env);
	}

	void NetworkSystem::RegisterEvent(Environment &env) {
		env.RegisterEvent<Event::UpdateEvent, &NetworkSystem::Update>();
	}

	void NetworkSystem::Update(const Event::UpdateEvent &event) {
		auto &env = event.env_;

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

	void NetworkSystem::HandleMessage(Component::Session &session, Environment &env, MessageBuffer &buffer) {
		auto cmd = (RemoteCommand) buffer.Data()[0];

		switch (cmd) {
			case RemoteCommand::kAuthLogonChallenge:
				env.TriggerEvent(NetCommandEvent<RemoteCommand::kAuthLogonChallenge>{env, buffer, session});
			case RemoteCommand::kAuthLogonProof:break;
			case RemoteCommand::kAuthReconnectChallenge:break;
			case RemoteCommand::kAuthReconnectProof:break;
			case RemoteCommand::kRealmList:break;
		}
	}

	void NetworkSystem::HandleMessage(Component::Session &session, Environment &env) {
		while (auto opt = session.session_->Read()) {
			auto buffer = std::move(opt.value());
			HandleMessage(session, env, buffer);
		}
	}
}
