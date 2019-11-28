//
// Created by 404Polaris on 2019/10/19.
//

#pragma once

#include <entt/entt.hpp>
#include <Sirius/Common/ThreadPool.hpp>

namespace Sirius::LoginServer {
	class App;
}

namespace Sirius::LoginServer::System {

	class SystemBase {
	public:
		template<typename _Task_type>
		void AsyncRun(_Task_type &&task) {}
	public:
		virtual void Init(App &app) {};
		virtual void Update(double delta, App &app) {};
		virtual void LateUpdate(double delta, App &app) {};
	public:
		virtual ~ SystemBase() = default;
	};
}
