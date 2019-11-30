//
// Created by 404Polaris on 2019/10/15.
//

#pragma once

#include <vector>
#include <cstddef>

#include <fmt/format.h>

namespace Yoa {
	class MessageBuffer {
	private:
		size_t size_ = 0;
		std::vector<std::byte> buffer_;
	public:
		explicit MessageBuffer(size_t n);
		MessageBuffer(const MessageBuffer &other);
		MessageBuffer &operator=(const MessageBuffer &other);
		MessageBuffer(MessageBuffer &&other) noexcept;
		MessageBuffer &operator=(MessageBuffer &&other) noexcept;
	private:
		void Resize(size_t n);
	public:
		size_t Size();
		std::byte *Data();
		std::byte *Tail();
		void Write(size_t n);
		void ExpandSize(size_t n);
	};

	inline void MessageBuffer::Resize(size_t n) {
		buffer_.resize(n, std::byte(0));
	}

	inline std::byte *MessageBuffer::Data() {
		return buffer_.data();
	}

	inline size_t MessageBuffer::Size() {
		return size_;
	}

	inline MessageBuffer &MessageBuffer::operator=(MessageBuffer &&other) noexcept {
		this->size_ = other.size_;
		this->buffer_ = std::move(other.buffer_);
		other.size_ = 0;
		return *this;
	}

	inline MessageBuffer::MessageBuffer(MessageBuffer &&other) noexcept : size_(other.size_),
																		  buffer_(std::move((other.buffer_))) {
		other.size_ = 0;
	}

	inline void MessageBuffer::ExpandSize(size_t n) {
		if (buffer_.size() < n)
			Resize(n + size_);
	}

	inline void MessageBuffer::Write(size_t n) {
		size_ += n;
	}

	inline MessageBuffer::MessageBuffer(size_t n) {
		buffer_.resize(n);
	}

	inline std::byte *MessageBuffer::Tail() {
		return Data() + size_;
	}

	inline MessageBuffer::MessageBuffer(const MessageBuffer &other) : size_(other.size_), buffer_(other.buffer_) {
		fmt::print("Copy Construct.");
	}

	inline MessageBuffer &MessageBuffer::operator=(const MessageBuffer &other) {
		fmt::print("Copy Assign.");
		this->size_ = other.size_;
		this->buffer_ = other.buffer_;
		return *this;
	}
}

