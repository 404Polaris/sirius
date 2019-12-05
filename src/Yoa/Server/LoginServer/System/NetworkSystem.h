//
// Created by 404Polaris on 2019/10/15.
//


#pragma once

#include <Yoa/Server/LoginServer/System/SystemBase.hpp>
#include <Yoa/Server/LoginServer/Component/Session.h>
#include <Yoa/Server/LoginServer/Event/UpdateEvent.h>
#include <fmt/format.h>

namespace Yoa::LoginServer::System {

	class NetworkSystem : public SystemBase {
	public:
		static void Init(Environment &env);
	private:
		static void Update(const Event::UpdateEvent &event);
		static void RegisterEvent(Environment &env);
		static void HandleMessage(Component::Session &session, Environment &env);
		static void HandleMessage(Component::Session &session, Environment &env, MessageBuffer &buffer);
	};
}


