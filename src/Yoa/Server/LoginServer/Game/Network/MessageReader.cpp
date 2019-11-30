//
// Created by 404Polaris on 2019/10/15.
//


#include <Yoa/Server/LoginServer/Game/Network/MessageReader.h>

namespace Yoa::LoginServer::Game {

	std::tuple<bool, size_t> MessageReader::ShouldRead() {
		if (buffer_.Size() == 0) {
			return {true, cmd_size_};
		}

		auto cmd = buffer_.Data()[0];
		std::tuple<bool, size_t> result{false, 0};

		switch ((LoginServerCmd) cmd) {
			case LoginServerCmd::kAuthLogonChallenge: result = Check<S_AuthLogonChallenge_C>();
				break;
			case LoginServerCmd::kAuthLogonProof: result = Check<S_AuthLogonProof_C>();
				break;
			case LoginServerCmd::kAuthReconnectChallenge: result = Check<S_AuthLogonChallenge_C>();
				break;
			case LoginServerCmd::kAuthReconnectProof: result = Check<S_AuthReconnectProof_C>();
				break;
			case LoginServerCmd::kRealmList: result = Check<S_RealmList_C>();
				break;
		}

		if (std::get<bool>(result))
			buffer_.ExpandSize(std::get<size_t>(result));

		return result;
	}
}
