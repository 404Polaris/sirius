//
// Created by 404Polaris on 2019/12/5.
//


#pragma once

#include <Yoa/Server/LoginServer/Environment.h>
#include <Yoa/Server/LoginServer/Component/Session.h>

namespace Yoa::LoginServer::Event {

	template<auto CMD>
	struct NetCommandEvent {
		Environment &env_;
		mutable MessageBuffer buffer_;
		Yoa::LoginServer::Component::Session &session_;
	public:
		NetCommandEvent(Environment &env, MessageBuffer &msg_buffer, Component::Session &session)
			: env_(env),
			  buffer_(std::move(msg_buffer)),
			  session_(session) {
		}
	};
}