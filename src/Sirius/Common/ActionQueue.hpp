//
// Created by 404Polaris on 2019/11/1.
//


#pragma once

#include <Sirius/Common/NoCopyAble.hpp>
#include <queue>
#include <mutex>
#include <optional>
#include <functional>

namespace Sirius {
	class ActionQueue : public NoCopyAble {
		using Action_type = std::function<void()>;
	private:
		std::mutex mutex_;
		std::queue<Action_type> action_queue_{};
	public:
		ActionQueue() = default;
		ActionQueue(ActionQueue &&other) noexcept;
		ActionQueue &operator=(ActionQueue &&other) noexcept;

	public:
		template<typename _Fun_type>
		void Push(_Fun_type &&action);

		std::optional<Action_type> Pop();
	};

	inline ActionQueue &ActionQueue::operator=(ActionQueue &&other) noexcept {
		this->action_queue_ = std::move(other.action_queue_);
		return *this;
	}

	inline ActionQueue::ActionQueue(ActionQueue &&other) noexcept : action_queue_(std::move(other.action_queue_)) {}

	template<typename _Fun_type>
	inline void ActionQueue::Push(_Fun_type &&action) {
		std::unique_lock<std::mutex> lock(mutex_);
		action_queue_.push(std::forward<_Fun_type>(action));
	}

	inline std::optional<ActionQueue::Action_type> ActionQueue::Pop() {
		std::unique_lock<std::mutex> lock(mutex_);

		if (action_queue_.empty()) {
			return std::nullopt;
		}

		auto action = std::move(action_queue_.front());
		action_queue_.pop();
		return std::move(action);
	}
}



