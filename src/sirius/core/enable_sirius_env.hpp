//
// Created by 404Polaris on 2019/10/18.
//


#pragma once

#include <memory>

namespace sirius::core {

	template<typename _env_type>
	class enable_sirius_env {
	private:
		std::weak_ptr<_env_type> env_;
	public:
		enable_sirius_env() = default;
		~enable_sirius_env() = default;
	public:
		void register_env(const std::shared_ptr<_env_type> &env);
	protected:
		std::shared_ptr<_env_type> env();
	};

	template<typename _env_type>
	inline void enable_sirius_env<_env_type>::register_env(const std::shared_ptr<_env_type> &env) {
		this->env_ = env;
	}

	template<typename _env_type>
	inline std::shared_ptr<_env_type> enable_sirius_env<_env_type>::env() {
		if (auto ptr = env_.lock();ptr != nullptr) {
			return ptr;
		}

		return nullptr;
	}
}


