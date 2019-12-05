//
// Created by 404Polaris on 2019/11/1.
//


#pragma once

#include <Yoa/Common/NoCopyAble.hpp>
#include <queue>
#include <mutex>
#include <optional>
#include <functional>

namespace Yoa {
	class SyncExecutor : public NoCopyAble {
		using Task_type = std::function<void()>;
	private:
		std::mutex mutex_{};
		std::thread thread_{};
		std::atomic_bool running_ = false;
		std::queue<Task_type> task_queue_{};
	public:
		SyncExecutor() = default;

		~SyncExecutor() {
			Stop();
			thread_.join();
		}

		SyncExecutor &operator=(SyncExecutor &&other) noexcept {
			this->task_queue_ = std::move(other.task_queue_);
			return *this;
		}

		SyncExecutor(SyncExecutor &&other) noexcept :
			task_queue_(std::move(other.task_queue_)) {
		}

	private:
		void Update() {
			using namespace std::chrono_literals;

			while (running_) {
				bool free_ = true;
				while (auto task = Pop()) {
					task.value()();
					free_ = false;
				}

				if (free_) {
					std::this_thread::sleep_for(1ns);
				}
			}
		}

		std::optional<Task_type> Pop() {
			std::unique_lock<std::mutex> lock(mutex_);

			if (task_queue_.empty()) {
				return std::nullopt;
			}

			auto task = std::move(task_queue_.front());
			task_queue_.pop();
			return task;
		}
	public:
		void Start() {
			running_ = true;
			thread_ = std::thread([this]() { Update(); });
		}

		void Stop() {
			running_ = false;
		}

		template<typename _Fun_t>
		void Push(_Fun_t &&task) {
			{
				std::unique_lock<std::mutex> lock(mutex_);
				task_queue_.push(std::forward<_Fun_t>(task));
			}
		}
	};
}



