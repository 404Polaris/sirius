//
// Created by 404Polaris on 2019/10/15.
//

#include <Sirius/Server/LoginServer/App.h>
#include <Sirius/Common/Timer.hpp>
#include <Sirius/Common/Database/MySqlDatabase.hpp>
#include <iostream>

int main() {
	auto app = std::make_shared<Sirius::LoginServer::App>();
	app->Start();

	Sirius::Database::MySqlDatabase mysql({"world", "127.0.0.1", "3306", "root", "root", "256"});

	char ch;
	std::cin >> ch;

	return 0;
}