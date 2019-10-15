//
// Created by 404Polaris on 2019/10/15.
//


#pragma once

#include <sirius/login/core/nocopyable.hpp>

#include <asio.hpp>
#include <memory>
#include <fmt/format.h>

namespace sirius::core {
	class thread_pool : nocopyable {
	protected:
		const size_t thread_num_;
		std::unique_ptr<asio::thread_pool> thread_pool_;
	public:
		thread_pool();
		~thread_pool();
		explicit thread_pool(size_t num);
	public:
		template<typename task_type>
		void push_task(task_type &&task);

		size_t size();
	};

	thread_pool::thread_pool(size_t num) : thread_num_(num) {
		thread_pool_ = std::make_unique<asio::thread_pool>(num);
	}

	thread_pool::thread_pool() : thread_pool(std::thread::hardware_concurrency()) {

	}

	template<typename task_type>
	void thread_pool::push_task(task_type &&task) {
		asio::post(*thread_pool_, std::forward<task_type>(task));
	}

	size_t thread_pool::size() {
		return thread_num_;
	}

	thread_pool::~thread_pool() {
		thread_pool_->stop();
		thread_pool_->join();
		thread_pool_.reset();
	}
}


