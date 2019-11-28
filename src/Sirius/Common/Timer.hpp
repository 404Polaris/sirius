//
// Created by 404Polaris on 2019/10/31.
//

#pragma once

#include <Sirius/Common/NoCopyAble.hpp>

#include <queue>
#include <memory>
#include <chrono>
#include <functional>
#include <type_traits>

namespace Sirius {

	class Timer : NoCopyAble {
		using _Time_point_type = std::chrono::steady_clock::time_point;
		friend class TimerManager;
	private:
		bool expired_ = false;
		_Time_point_type deadline_;
		std::function<void()> func_;
	public:
		template<typename _Func_type>
		Timer(_Time_point_type deadline, _Func_type &&func):deadline_(deadline), func_(std::forward<_Func_type>(func)) {
			static_assert(std::is_invocable<typename std::decay<_Func_type>::type>::value,
						  "Wrong timer callback type.");
		}

		Timer(Timer &&other) noexcept  : deadline_(other.deadline_), func_(std::move(other.func_)) {
			other.deadline_ = _Time_point_type();
		}

		Timer &operator=(Timer &&other) noexcept {
			this->deadline_ = other.deadline_;
			this->func_ = std::move(other.func_);
			other.deadline_ = _Time_point_type();
			return *this;
		}
	public:
		void Execute() {
			if (!expired_) {
				func_();
				expired_ = true;
			}
		}

		void Cancel() {
			expired_ = true;
		}

		friend bool operator<(const Timer &l, const Timer &r) {
			return l.deadline_ > r.deadline_;
		}
	};

	class TimerManager : NoCopyAble {
	private:
		std::mutex mutex_{};
		std::thread thread_{};
		std::atomic_bool running_ = false;
		std::priority_queue<std::shared_ptr<Timer>> timer_heap_;
	public:
		~TimerManager() {
			Stop();

			if (thread_.joinable()) {
				thread_.join();
			}
		}

		TimerManager() = default;
		TimerManager(TimerManager &&) = delete;
		TimerManager &operator=(TimerManager &&) = delete;
	public:
		template<typename _Func_type>
		auto AddTimer(_Func_type &&func, size_t delay) {
			auto deadline = std::chrono::steady_clock::now() + std::chrono::microseconds(delay);
			auto timer = std::make_shared<Timer>(deadline, std::forward<_Func_type>(func));

			{
				std::lock_guard<std::mutex> lock(mutex_);
				timer_heap_.push(timer);
			}

			return timer;
		}

	public:
		void Start() {
			running_ = true;
			thread_ = std::thread([this]() { InternalLoop(); });
		}

		void Stop() {
			running_ = false;
		}
	private:
		bool Update() {
			bool do_noting = true;
			std::lock_guard<std::mutex> lock(mutex_);

			while (!timer_heap_.empty()) {
				auto &top = timer_heap_.top();
				if (top->deadline_ <= std::chrono::steady_clock::now()) {
					top->Execute();
					timer_heap_.pop();
					do_noting = false;
				}
			}

			return do_noting;
		}

		void InternalLoop() {
			const static auto sleep_duration = std::chrono::nanoseconds(1);

			while (running_) {
				if (Update()) {
					std::this_thread::sleep_for(sleep_duration);
				}
			}
		}
	};
}