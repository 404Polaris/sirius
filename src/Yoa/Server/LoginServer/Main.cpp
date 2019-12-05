//
// Created by 404Polaris on 2019/10/15.
//

#include <Yoa/Server/LoginServer/Environment.h>
#include <Yoa/Common/Timer.hpp>
#include <iostream>

int main() {
	auto env = std::make_shared<Yoa::LoginServer::Environment>();
	env->Start();

	char c;
	std::cin >> c;

	return 0;
}