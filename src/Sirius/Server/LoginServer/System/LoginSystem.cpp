//
// Created by 404Polaris on 2019/10/19.
//

#include <Sirius/Server/LoginServer/System/LoginSystem.h>
#include <Sirius/Server/LoginServer/Component/RemoteCmdHandlerMap.h>
#include <Sirius/Server/LoginServer/App.h>

namespace Sirius::LoginServer::System {

	void LoginSystem::Init(App &app) {
		auto &registry = app.GetRegistry();
		auto view = registry.view<Component::RemoteCmdHandlerMap>();

		if (!view.empty()) {
			auto &delegate_map = view.raw()->delegate_map_;
			delegate_map[LoginServerCmd::kAuthLogonChallenge].connect<&LoginSystem::HandleAuthLogonChallenge>(*this);
		}
	}

	void LoginSystem::HandleAuthLogonChallenge(Component::Session &session, App &app, MessageBuffer &msg_buffer) {

		session.status = AuthStatus::kStatusClosed;
		auto *challenge = reinterpret_cast<S_AuthLogonChallenge_C *>(msg_buffer.Data());
		std::string account((char const *) challenge->i, challenge->i_len);
		uint16_t build = challenge->build;

		MessageBuffer pkt{3};
		pkt.Data()[0] = (std::byte) LoginServerCmd::kAuthLogonChallenge;
		pkt.Data()[1] = (std::byte) 0x00;
		pkt.Data()[2] = (std::byte) 0x04;
		pkt.Write(3);

		session.session_->Write(std::move(pkt));
		fmt::print("HandleAuthLogonChallenge: {0} {1} \n", account, build);
	}
}


