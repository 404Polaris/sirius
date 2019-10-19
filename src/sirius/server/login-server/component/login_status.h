//
// Created by 404Polaris on 2019/10/19.
//

#pragma once

#include <cstdint>

namespace sirius::login_server::component {

	enum class auth_status : uint8_t {
		kStatusChallenge = 0,
		kStatusLogonProof,
		kStatusReconnectProof,
		kStatusAuthed,
		kStatusWaitingForRealmList,
		kStatusClosed
	};

	struct login_status {
		auth_status status;
	};
}