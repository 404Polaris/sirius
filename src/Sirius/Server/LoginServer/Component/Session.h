//
// Created by 404Polaris on 2019/10/18.
//


#pragma once

#include <Sirius/Common/TcpSession.hpp>
#include <Sirius/Common/MessageBuffer.hpp>
#include <Sirius/Server/LoginServer/Game/Network/MessageReader.h>
#include <Sirius/Server/LoginServer/Game/AuthStatus.h>

#include <queue>
#include <string>
#include <memory>

namespace Sirius::LoginServer {
	class App;
}

namespace Sirius::LoginServer::Component {

	struct Session {
		AuthStatus status;
		std::shared_ptr<Sirius::TcpSession<Game::MessageReader, Sirius::LoginServer::App>> session_;
	};
}


