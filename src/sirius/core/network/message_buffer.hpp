//
// Created by 404Polaris on 2019/10/15.
//

#pragma once

#include <vector>
#include <cstddef>

#include <fmt/format.h>

namespace sirius::core {
	class message_buffer {
	private:
		size_t size_ = 0;
		std::vector<std::byte> buffer_;
	public:
		explicit message_buffer(size_t n);
		message_buffer(const message_buffer &other);
		message_buffer &operator=(const message_buffer &other);
		message_buffer(message_buffer &&other) noexcept;
		message_buffer &operator=(message_buffer &&other) noexcept;
	private:
		void resize(size_t n);
	public:
		size_t size();
		std::byte *data();
		std::byte *tail();
		void write(size_t n);
		void expand_size(size_t n);
	};

	inline void message_buffer::resize(size_t n) {
		buffer_.resize(n, std::byte(0));
	}

	inline std::byte *message_buffer::data() {
		return buffer_.data();
	}

	inline size_t message_buffer::size() {
		return size_;
	}

	inline message_buffer &message_buffer::operator=(message_buffer &&other) noexcept {
		this->buffer_ = std::move(other.buffer_);
		return *this;
	}

	inline message_buffer::message_buffer(message_buffer &&other) noexcept : buffer_(std::move((other.buffer_))),
																			 size_(other.size_) {
		other.size_ = 0;
	}

	inline void message_buffer::expand_size(size_t n) {
		if (buffer_.size() < n)
			resize(n + size_);
	}

	inline void message_buffer::write(size_t n) {
		size_ += n;
	}

	inline message_buffer::message_buffer(size_t n) {
		buffer_.resize(n);
	}

	inline std::byte *message_buffer::tail() {
		return data() + size_;
	}

	inline message_buffer::message_buffer(const message_buffer &other) : buffer_(other.buffer_) {
		fmt::print("Copy Construct.");
	}

	inline message_buffer &message_buffer::operator=(const message_buffer &other) {
		fmt::print("Copy Assign.");
		this->buffer_ = other.buffer_;
		return *this;
	}
}

