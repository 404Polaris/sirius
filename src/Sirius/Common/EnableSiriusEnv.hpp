//
// Created by 404Polaris on 2019/10/18.
//


#pragma once

#include <memory>

namespace Sirius {

	template<typename _Env_type>
	class EnableSiriusEnv {
	private:
		std::weak_ptr<_Env_type> env_;
	public:
		EnableSiriusEnv() = default;
		~EnableSiriusEnv() = default;
	public:
		void RegisterEnv(const std::shared_ptr<_Env_type> &env);
	protected:
		std::shared_ptr<_Env_type> Env();
	};

	template<typename _Env_type>
	inline void EnableSiriusEnv<_Env_type>::RegisterEnv(const std::shared_ptr<_Env_type> &env) {
		this->env_ = env;
	}

	template<typename _Env_type>
	inline std::shared_ptr<_Env_type> EnableSiriusEnv<_Env_type>::Env() {
		if (auto ptr = env_.lock();ptr != nullptr) {
			return ptr;
		}

		return nullptr;
	}
}


