//
// Created by 404Polaris on 2019/10/19.
//

#pragma once

#include <cstdint>

namespace Sirius::LoginServer::Component {

	enum class AuthStatus : uint8_t {
		kStatusChallenge = 0,
		kStatusLogonProof,
		kStatusReconnectProof,
		kStatusAuthed,
		kStatusWaitingForRealmList,
		kStatusClosed
	};

	struct LoginStatus {
		AuthStatus status;
	};
}