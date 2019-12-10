//
// Created by 404Polaris on 2019/12/4.
//

#pragma once

#include <Yoa/Server/LoginServer/Component/Session.h>

namespace Yoa::LoginServer {
	class Environment;
}

namespace Yoa::LoginServer::Event {

	struct EventBase {
	public:
		using Id_type = size_t;
	};

	template<size_t N>
	struct EventWithId : EventBase {
		constexpr static Id_type event_id_ = N;
	};

	struct UpdateEvent : public EventWithId<0> {
		Environment &env_;
	public:
		explicit UpdateEvent(Environment &env) : env_(env) {}
	};

	struct NetCmdEvent : public EventWithId<1> {
		Environment &env_;
		mutable MessageBuffer buffer_;
		Yoa::LoginServer::Component::Session &session_;
	public:
		explicit NetCmdEvent(Environment &env, MessageBuffer msg_buffer, Component::Session &session)
			: env_(env),
			  buffer_(std::move(msg_buffer)),
			  session_(session) {
		}
	};
}