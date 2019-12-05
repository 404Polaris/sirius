//
// Created by 404Polaris on 2019/12/4.
//

#pragma once

#include <Yoa/Server/LoginServer/Environment.h>

namespace Yoa::LoginServer::Event {

	struct UpdateEvent {
		Environment &env_;
	};
}
