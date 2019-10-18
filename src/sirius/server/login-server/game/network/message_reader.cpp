//
// Created by 404Polaris on 2019/10/15.
//


#include <sirius/server/login-server/game/network/message_reader.h>

namespace sirius::login_server::game {

	std::tuple<bool, size_t> message_reader::should_read() {
		if (buffer_.size() == 0) return {true, 1};

		auto cmd = buffer_.data()[0];
		std::tuple<bool, size_t> result{false, 0};

		switch ((login_server_cmd) cmd) {
			case login_server_cmd::kAuthLogonChallenge:result = read_check<s_auth_logon_challenge_c>();
			case login_server_cmd::kAuthLogonProof:result = read_check<s_auth_logon_proof_c>();;
			case login_server_cmd::kAuthReconnectChallenge:result = read_check<s_auth_logon_challenge_c>();;
			case login_server_cmd::kAuthReconnectProof:result = read_check<s_auth_reconnect_proof_c>();
			case login_server_cmd::kRealmList:result = read_check<s_realm_list_c>();
		}

		return result;
	}
}
