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
		void resize(size_t n);
		const std::byte *get_ptr();
	};

	void message_buffer::resize(size_t n) {
		buffer_.resize(n, std::byte(0));
	}

	const std::byte *message_buffer::get_ptr() {
		return buffer_.data();
	}
}

