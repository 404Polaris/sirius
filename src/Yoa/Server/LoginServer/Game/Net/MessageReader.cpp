//
// Created by 404Polaris on 2019/10/15.
//


#include <Yoa/Server/LoginServer/Game/Net/MessageReader.h>

namespace Yoa::LoginServer::Game {

	std::tuple<bool, size_t> MessageReader::ShouldRead() {
		if (buffer_.Size() == 0) {
			return {true, cmd_size_};
		}

		auto cmd = buffer_.Data()[0];
		std::tuple<bool, size_t> result{false, 0};

		switch ((RemoteCommand) cmd) {
			case RemoteCommand::kAuthLogonChallenge: result = Check<S_AuthLogonChallenge_C>();
				break;
			case RemoteCommand::kAuthLogonProof: result = Check<S_AuthLogonProof_C>();
				break;
			case RemoteCommand::kAuthReconnectChallenge: result = Check<S_AuthLogonChallenge_C>();
				break;
			case RemoteCommand::kAuthReconnectProof: result = Check<S_AuthReconnectProof_C>();
				break;
			case RemoteCommand::kRealmList: result = Check<S_RealmList_C>();
				break;
			case RemoteCommand::kVoid:break;
		}

		if (std::get<bool>(result))
			buffer_.ExpandSize(std::get<size_t>(result));

		return result;
	}
}
