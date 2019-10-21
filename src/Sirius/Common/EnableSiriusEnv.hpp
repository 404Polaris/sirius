//
// Created by 404Polaris on 2019/10/18.
//


#pragma once

#include <memory>

namespace Sirius {

	template<typename _env_type>
	class EnableSiriusEnv {
	private:
		std::weak_ptr<_env_type> env_;
	public:
		EnableSiriusEnv() = default;
		~EnableSiriusEnv() = default;
	public:
		void RegisterEnv(const std::shared_ptr<_env_type> &env);
	protected:
		std::shared_ptr<_env_type> Env();
	};

	template<typename _env_type>
	inline void EnableSiriusEnv<_env_type>::RegisterEnv(const std::shared_ptr<_env_type> &env) {
		this->env_ = env;
	}

	template<typename _env_type>
	inline std::shared_ptr<_env_type> EnableSiriusEnv<_env_type>::Env() {
		if (auto ptr = env_.lock();ptr != nullptr) {
			return ptr;
		}

		return nullptr;
	}
}


