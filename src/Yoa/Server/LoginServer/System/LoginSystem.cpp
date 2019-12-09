//
// Created by 404Polaris on 2019/10/19.
//

#include <Yoa/Server/LoginServer/System/LoginSystem.h>
#include <Yoa/Server/LoginServer/Environment.h>

namespace Yoa::LoginServer::System {

	void LoginSystem::Load(Environment &env) {
		RegisterEvent(env);
	}

	void LoginSystem::RegisterEvent(Environment &env) {
		using _Event_type = NetCommandEvent<RemoteCommand::kAuthLogonChallenge>;
		env.RegisterEvent<_Event_type, &LoginSystem::HandleAuthLogonChallenge>();
	}

	void LoginSystem::HandleAuthLogonChallenge(const NetCommandEvent<RemoteCommand::kAuthLogonChallenge> &event) {
		auto &session = event.session_;
		auto &buffer = event.buffer_;

		session.status = AuthStatus::kStatusClosed;
		auto *challenge = reinterpret_cast<S_AuthLogonChallenge_C *>(buffer.Data());
		std::string account((char const *) challenge->i, challenge->i_len);
		uint16_t build = challenge->build;

		MessageBuffer pkt{3};
		pkt.Data()[0] = (std::byte) RemoteCommand::kAuthLogonChallenge;
		pkt.Data()[1] = (std::byte) 0x00;
		pkt.Data()[2] = (std::byte) 0x04;
		pkt.Write(3);

		session.session_->Write(std::move(pkt));
		fmt::print("HandleAuthLogonChallenge: {0} {1} \n", account, build);
	}
}


