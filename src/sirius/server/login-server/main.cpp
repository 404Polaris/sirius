//
// Created by 404Polaris on 2019/10/15.
//

#include <sirius/server/login-server/app.hpp>
#include <iostream>


int main() {

	sirius::login_server::app app;
	app.start();

	char ch;
	std::cin >> ch;

	return 0;
}