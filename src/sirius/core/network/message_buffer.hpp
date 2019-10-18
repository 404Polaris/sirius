//
// Created by 404Polaris on 2019/10/15.
//

#pragma once

#include <vector>
#include <cstddef>

namespace sirius::core {
	class message_buffer {
	private:
		std::vector<std::byte> buffer_;
	public:
		message_buffer(const message_buffer &other) = default;
		message_buffer &operator=(const message_buffer &other) = default;
		message_buffer(message_buffer &&other) noexcept;
		message_buffer &operator=(message_buffer &&other);
	public:
		size_t size();
		std::byte *data();
		void resize(size_t n);
	};

	void message_buffer::resize(size_t n) {
		buffer_.resize(n, std::byte(0));
	}

	std::byte *message_buffer::data() {
		return buffer_.data();
	}

	size_t message_buffer::size() {
		return buffer_.size();
	}

	message_buffer &message_buffer::operator=(message_buffer &&other) {
		this->buffer_ = std::move(other.buffer_);
		return *this;
	}

	message_buffer::message_buffer(message_buffer &&other) noexcept : buffer_(std::move((other.buffer_))) {

	}
}

