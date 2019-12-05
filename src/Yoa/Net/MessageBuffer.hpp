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
		explicit MessageBuffer(size_t n) {
			buffer_.resize(n);
		}

		MessageBuffer(const MessageBuffer &o) : size_(o.size_), buffer_(o.buffer_) {
			fmt::print("Copy Construct.");
		}

		MessageBuffer &operator=(const MessageBuffer &o) {
			fmt::print("Copy Assign.");
			this->size_ = o.size_;
			this->buffer_ = o.buffer_;
			return *this;
		}

		MessageBuffer(MessageBuffer &&o) noexcept : size_(o.size_),
													buffer_(std::move((o.buffer_))) {
			o.size_ = 0;
		}

		MessageBuffer &operator=(MessageBuffer &&o) noexcept {
			this->size_ = o.size_;
			this->buffer_ = std::move(o.buffer_);
			o.size_ = 0;
			return *this;
		}
	private:
		void Resize(size_t n) {
			buffer_.resize(n, std::byte(0));
		}
	public:

		std::byte *Data() {
			return buffer_.data();
		}

		std::byte *Tail() {
			return Data() + size_;
		}

		size_t Size() {
			return size_;
		}

		void ExpandSize(size_t n) {
			if (buffer_.size() < n)
				Resize(n + size_);
		}

		void Write(size_t n) {
			size_ += n;
		}
	};

}

