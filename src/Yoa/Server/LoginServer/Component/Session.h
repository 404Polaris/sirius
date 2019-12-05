//
// Created by 404Polaris on 2019/10/18.
//


#pragma once

#include <Yoa/Net/TcpSession.hpp>
#include <Yoa/Net/MessageBuffer.hpp>
#include <Yoa/Server/LoginServer/Game/Net/MessageReader.h>
#include <Yoa/Server/LoginServer/Game/AuthStatus.h>

#include <queue>
#include <string>
#include <memory>

namespace Yoa::LoginServer {
	class Environment;
}

namespace Yoa::LoginServer::Component {

	struct Session {
		AuthStatus status;
		std::shared_ptr<Yoa::TcpSession<Game::MessageReader>> session_;
	};
}


