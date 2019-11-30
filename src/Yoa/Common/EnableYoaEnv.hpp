//
// Created by 404Polaris on 2019/10/18.
//


#pragma once

#include <memory>

namespace Yoa {

	template<typename _Env_type>
	class EnableYoaEnv {
	private:
		std::weak_ptr<_Env_type> env_;
	public:
		EnableYoaEnv() = default;
		~EnableYoaEnv() = default;
	public:
		void RegisterEnv(const std::shared_ptr<_Env_type> &env);
	protected:
		std::shared_ptr<_Env_type> Env();
	};

	template<typename _Env_type>
	inline void EnableYoaEnv<_Env_type>::RegisterEnv(const std::shared_ptr<_Env_type> &env) {
		this->env_ = env;
	}

	template<typename _Env_type>
	inline std::shared_ptr<_Env_type> EnableYoaEnv<_Env_type>::Env() {
		if (auto ptr = env_.lock();ptr != nullptr) {
			return ptr;
		}

		return nullptr;
	}
}


