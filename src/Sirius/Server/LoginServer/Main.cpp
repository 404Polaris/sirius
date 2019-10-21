//
// Created by 404Polaris on 2019/10/15.
//

#include <Sirius/Server/LoginServer/App.hpp>
#include <iostream>

int main() {

	auto app = std::make_shared<Sirius::LoginServer::App>();
	app->Start();

	char ch;
	std::cin >> ch;

	return 0;
}