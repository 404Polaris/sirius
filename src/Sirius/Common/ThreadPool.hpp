//
// Created by 404Polaris on 2019/10/15.
//


#pragma once

#include <Sirius/Common/NoCopyAble.hpp>

#include <asio.hpp>
#include <memory>
#include <fmt/format.h>

namespace sirius {
	class ThreadPool : NoCopyAble {
	protected:
		const size_t thread_num_;
		std::unique_ptr<asio::thread_pool> thread_pool_;
	public:
		ThreadPool();
		~ThreadPool();
		explicit ThreadPool(size_t num);
	public:
		template<typename _Task_type>
		void Post(_Task_type &&task);

		size_t size();
	};

	inline ThreadPool::ThreadPool(size_t num) : thread_num_(num) {
		thread_pool_ = std::make_unique<asio::thread_pool>(num);
	}

	inline ThreadPool::ThreadPool() : ThreadPool(std::thread::hardware_concurrency()) {

	}

	template<typename _Task_type>
	inline void ThreadPool::Post(_Task_type &&task) {
		asio::post(*thread_pool_, std::forward<_Task_type>(task));
	}

	inline size_t ThreadPool::Size() {
		return thread_num_;
	}

	inline ThreadPool::~thread_pool() {
		thread_pool_->stop();
		thread_pool_->join();
		thread_pool_.reset();
	}
}


