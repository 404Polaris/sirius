//
// Created by 404Polaris on 2019/10/15.
//

#include <Sirius/Server/LoginServer/App.h>
#include <Sirius/Common/Timer.hpp>
#include <iostream>

int main() {
	auto app = std::make_shared<Sirius::LoginServer::App>();
	app->Start();

	return 0;
}