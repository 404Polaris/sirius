//
// Created by 404Polaris on 2019/10/15.
//


#pragma once

#include <sirius/login/core/nocopyable.hpp>

#include <queue>
#include <thread>
#include <vector>

namespace sirius::core {

	class thread_pool : public nocopyable {
		using task_type = std::function<void()>;
	protected:
		std::mutex mutex_{};
		std::atomic_bool running_{};
		std::vector<std::thread> threads_{};
		std::queue<task_type> task_queue_{};
	public:
		thread_pool();
		~thread_pool();
		explicit thread_pool(size_t num);
	public:
		template<typename _type>
		void push_task(_type &&task);
	protected:
		void internal_loop();
		std::optional<task_type> get_task();
	};

	thread_pool::thread_pool(size_t num) : running_(true) {
		for (size_t i = 0; i < num; i++) {
			threads_.emplace_back(std::thread([this]() { internal_loop(); }));
		}
	}

	thread_pool::thread_pool() : thread_pool(std::thread::hardware_concurrency()) {

	}

	template<typename _type>
	void thread_pool::push_task(_type &&task) {
		task_queue_.push(std::forward<_type>(task));
	}

	void thread_pool::internal_loop() {

		while (running_) {
			auto &&task = get_task();

			if (!task) {
				static auto one_ms = std::chrono::milliseconds(1);
				std::this_thread::sleep_for(one_ms);
				continue;
			}

			task.value()();
		}
	}

	std::optional<thread_pool::task_type> thread_pool::get_task() {
		std::lock_guard<std::mutex> lock(mutex_);
		if (task_queue_.empty())return std::nullopt;
		task_type task = std::move(task_queue_.front());
		task_queue_.pop();
		return std::move(task);
	}

	thread_pool::~thread_pool() {
		running_ = false;

		for (auto &thread : threads_) {
			thread.join();
		}
	}
}


