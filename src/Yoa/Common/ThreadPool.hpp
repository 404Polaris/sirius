//
// Created by 404Polaris on 2019/10/15.
//


#pragma once

#include <Yoa/Common/NoCopyAble.hpp>

#include <asio.hpp>
#include <memory>
#include <atomic>
#include <fmt/format.h>

namespace Yoa {
	class ThreadPool : NoCopyAble {
	protected:
		const size_t thread_num_;
		std::atomic_bool running_;
		std::unique_ptr<asio::thread_pool> thread_pool_;
	public:
		ThreadPool();
		~ThreadPool();
		explicit ThreadPool(size_t num);
	public:
		template<typename _Task_type>
		void Post(_Task_type &&task);
		void Stop();
		size_t Size();
	};

	inline ThreadPool::ThreadPool(size_t num) : thread_num_(num), running_(true) {
		thread_pool_ = std::make_unique<asio::thread_pool>(num);
	}

	inline ThreadPool::ThreadPool() : ThreadPool(std::thread::hardware_concurrency()) {

	}

	template<typename _Task_type>
	inline void ThreadPool::Post(_Task_type &&task) {
		if (!running_)return;
		asio::post(*thread_pool_, std::forward<_Task_type>(task));
	}

	inline size_t ThreadPool::Size() {
		return thread_num_;
	}

	inline ThreadPool::~ThreadPool() {
		Stop();
		thread_pool_.reset();
	}

	inline void ThreadPool::Stop() {
		running_ = false;
		thread_pool_->stop();
		thread_pool_->join();
	}
}


