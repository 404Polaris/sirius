//
// Created by 404Polaris on 2019/10/18.
//


#pragma once

#include <Yoa/Common/TcpSession.hpp>
#include <Yoa/Common/MessageBuffer.hpp>
#include <Yoa/Server/LoginServer/Game/Network/MessageReader.h>
#include <Yoa/Server/LoginServer/Game/AuthStatus.h>

#include <queue>
#include <string>
#include <memory>

namespace Yoa::LoginServer {
	class App;
}

namespace Yoa::LoginServer::Component {

	struct Session {
		AuthStatus status;
		std::shared_ptr<Yoa::TcpSession<Game::MessageReader>> session_;
	};
}


