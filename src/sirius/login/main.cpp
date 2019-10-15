//
// Created by 404Polaris on 2019/10/15.
//

#include <sirius/login/core/network/tcp_server.hpp>

#include <iostream>

int main() {

	sirius::core::tcp_server server(7000);
	server.start();

	char c;
	std::cin >> c;

	return 0;
}