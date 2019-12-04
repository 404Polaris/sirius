//
// Created by 404Polaris on 2019/10/15.
//

#include <Yoa/Server/LoginServer/App.h>
#include <Yoa/Common/Timer.hpp>
#include <Yoa/Common/Event.hpp>
#include <iostream>

int main() {
	auto app = std::make_shared<Yoa::LoginServer::App>();
	app->Start();

	return 0;
}