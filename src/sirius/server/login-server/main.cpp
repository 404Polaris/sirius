//
// Created by 404Polaris on 2019/10/15.
//

#include <sirius/server/login-server/app.hpp>
#include <iostream>

int main() {

	auto app = std::make_shared<sirius::login_server::app>();
	app->start();

	char ch;
	std::cin >> ch;

	return 0;
}