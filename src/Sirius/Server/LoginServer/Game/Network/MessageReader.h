//
// Created by 404Polaris on 2019/10/15.
//


#pragma once

#include <Sirius/Common/NoCopyAble.hpp>
#include <Sirius/Common/MessageBuffer.hpp>
#include <Sirius/Server/LoginServer/Game/Network/Protocol.h>

#include <vector>
#include <cstddef>
#include <functional>
#include <unordered_map>
#include <fmt/format.h>

namespace Sirius::LoginServer::Game {

	class MessageReader : public NoCopyAble {
	protected:
		MessageBuffer buffer_{cmd_size_};
		inline static const auto cmd_size_ = sizeof(uint8_t);
	public:
		void Init();
		std::byte *Tail();
		void Write(size_t n);
		MessageBuffer PopBuffer();
		std::tuple<bool, size_t> ShouldRead();
	protected:
		template<typename msg_type>
		std::tuple<bool, size_t> Check();
	};

	inline std::byte *MessageReader::Tail() {
		return buffer_.Tail();
	}

	template<typename msg_type>
	inline std::tuple<bool, size_t> MessageReader::Check() {
		const auto message_size = sizeof(msg_type);

		std::tuple<bool, size_t> result{true, message_size - buffer_.Size()};

		if (buffer_.Size() < message_size)
			return result;

		return result;
	}

	template<>
	inline std::tuple<bool, size_t> MessageReader::Check<S_AuthLogonChallenge_C>() {
		static const size_t initial_size = 4;
		static const size_t max_size = sizeof(S_AuthLogonChallenge_C) + 16;

		if (buffer_.Size() < initial_size)
			return {true, initial_size - buffer_.Size()};

		auto *challenge = reinterpret_cast<S_AuthLogonChallenge_C *>(buffer_.Data());

		if (challenge->size + initial_size > max_size)
			return {false, 0};

		if (challenge->size + initial_size > buffer_.Size())
			return {true, (challenge->size + initial_size) - buffer_.Size()};

		auto legal = (buffer_.Size() + 1 - sizeof(S_AuthLogonChallenge_C) == challenge->i_len);
		std::tuple<bool, size_t> result{legal, 0};

		return result;
	}

	inline void MessageReader::Write(size_t n) {
		buffer_.Write(n);
	}

	inline MessageBuffer MessageReader::PopBuffer() {
		auto buffer = std::move(buffer_);
		return buffer;
	}

	inline void MessageReader::Init() {
		buffer_ = MessageBuffer(cmd_size_);
	}
}


