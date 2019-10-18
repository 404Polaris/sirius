//
// Created by 404Polaris on 2019/10/15.
//


#pragma once

#include <sirius/core/nocopyable.hpp>
#include <sirius/core/network/message_buffer.hpp>
#include <sirius/server/login-server/game/network/protocol.h>

#include <vector>
#include <cstddef>
#include <functional>
#include <unordered_map>
#include <fmt/format.h>

namespace sirius::login_server::game {

	class message_reader : public core::nocopyable {
	protected:
		core::message_buffer buffer_;
	public:
		std::byte *data();
		std::tuple<bool, size_t> should_read();
	protected:
		template<typename msg_type>
		std::tuple<bool, size_t> read_check();

		template<typename msg_type>
		void read_complete();
	};

	std::byte *message_reader::data() {
		return buffer_.data();
	}

	template<typename msg_type>
	std::tuple<bool, size_t> message_reader::read_check() {
		const auto message_size = sizeof(msg_type);

		std::tuple<bool, size_t> result{true, message_size - buffer_.size()};

		if (buffer_.size() < message_size)
			return result;

		if (std::get<bool>(result) && std::get<size_t>(result) == 0)
			read_complete<msg_type>();

		return result;
	}

	template<>
	std::tuple<bool, size_t> message_reader::read_check<s_auth_logon_challenge_c>() {
		static const size_t initial_size = 4;
		static const size_t max_size = sizeof(s_auth_logon_challenge_c) + 16;

		if (buffer_.size() < initial_size)
			return {true, initial_size - buffer_.size()};

		auto *challenge = reinterpret_cast<s_auth_logon_challenge_c *>(data());

		if (challenge->size + initial_size > max_size)
			return {false, 0};

		if (challenge->size < sizeof(s_auth_logon_challenge_c) - initial_size)
			return {true, challenge->size};

		auto legal = (buffer_.size() - sizeof(s_auth_logon_challenge_c) == challenge->i_len + 1);
		std::tuple<bool, size_t> result{legal, 0};

		if (std::get<bool>(result) && std::get<size_t>(result) == 0)
			read_complete<s_auth_logon_challenge_c>();

		return result;
	}

	template<typename msg_type>
	void message_reader::read_complete() {
		auto buf = std::move(buffer_);
		fmt::print("Read: {0} Complete! Size: {1} \n", typeid(msg_type).name(), buf.size());
	}
}


