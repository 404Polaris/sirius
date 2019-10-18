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
		core::message_buffer buffer_{cmd_size_};
		inline static const auto cmd_size_ = sizeof(uint8_t);
	public:
		std::byte *tail();
		void write(size_t n);
		std::tuple<bool, size_t> should_read();
	protected:
		template<typename msg_type>
		std::tuple<bool, size_t> check();

		template<typename msg_type>
		void read_complete();
	private:
		void reset_buffer();
	};

	inline std::byte *message_reader::tail() {
		return buffer_.tail();
	}

	template<typename msg_type>
	std::tuple<bool, size_t> message_reader::check() {
		const auto message_size = sizeof(msg_type);

		std::tuple<bool, size_t> result{true, message_size - buffer_.size()};

		if (buffer_.size() < message_size)
			return result;

		if (std::get<bool>(result) && std::get<size_t>(result) == 0)
			read_complete<msg_type>();

		return result;
	}

	template<>
	inline std::tuple<bool, size_t> message_reader::check<s_auth_logon_challenge_c>() {
		static const size_t initial_size = 4;
		static const size_t max_size = sizeof(s_auth_logon_challenge_c) + 16;

		if (buffer_.size() < initial_size)
			return {true, initial_size - buffer_.size()};

		auto *challenge = reinterpret_cast<s_auth_logon_challenge_c *>(buffer_.data());

		if (challenge->size + initial_size > max_size)
			return {false, 0};

		if (challenge->size + initial_size > buffer_.size())
			return {true, (challenge->size + initial_size) - buffer_.size()};

		auto legal = (buffer_.size() + 1 - sizeof(s_auth_logon_challenge_c) == challenge->i_len);
		std::tuple<bool, size_t> result{legal, 0};

		if (std::get<bool>(result) && std::get<size_t>(result) == 0)
			read_complete<s_auth_logon_challenge_c>();

		return result;
	}

	template<typename msg_type>
	void message_reader::read_complete() {
		auto buf = std::move(buffer_);
		reset_buffer();
		fmt::print("Read: {0} Complete! Size: {1} \n", typeid(msg_type).name(), buf.size());
	}

	inline void message_reader::write(size_t n) {
		buffer_.write(n);
	}

	inline void message_reader::reset_buffer() {
		buffer_ = core::message_buffer(cmd_size_);
	}
}


