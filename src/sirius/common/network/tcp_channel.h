//
// Created by 404Polaris on 2019/9/18.
//

#pragma once

#include <deque>
#include <vector>
#include <memory>
#include <cstdint>
#include <asio.hpp>

#include <sirius/common/noncopyable.h>

namespace sirius::common::network {

	template<typename _Msg_type>
	class TCPChannel : public NonCopyable, public std::enable_shared_from_this<TCPChannel<_Msg_type>> {
		using msg_type = _Msg_type;
		using this_type = TCPChannel<_Msg_type>;

	private:
		_Msg_type read_msg_;
		asio::ip::tcp::socket socket_;
		std::atomic_bool closed_ = false;
		std::deque<_Msg_type> write_queue_;
	public:
		explicit TCPChannel(asio::ip::tcp::socket socket);
	public:
		bool IsOpen();
	private:
		void DoWrite();
		void DoReadBody();
		void DoReadHeader();
	};

	template<typename _Msg_type>
	TCPChannel<_Msg_type>::TCPChannel(asio::ip::tcp::socket socket): socket_(std::move(socket)) {}

	template<typename _Msg_type>
	void TCPChannel<_Msg_type>::TCPChannel::DoWrite() {
		if (closed_)return;

		auto self(this->shared_from_this());
		asio::async_write(socket_,
						  asio::buffer(write_queue_.front().data(), write_queue_.front().size()),
						  [this, self](std::error_code ec, std::size_t) {
							  if (!ec) {
								  write_queue_.pop_front();
								  if (!write_queue_.empty()) {
									  DoWrite();
								  }
							  } else {
								  closed_ = true;
							  }
						  });
	}

	template<typename _Msg_type>
	void TCPChannel<_Msg_type>::DoReadHeader() {
		if (closed_)return;

		auto self(this->shared_from_this());
		asio::async_read(socket_,
						 asio::buffer(read_msg_.Data(), _Msg_type::header_length),
						 [this, self](std::error_code ec, std::size_t) {
							 if (!ec && read_msg_.DecodeHeader()) {
								 DoReadBody();
							 } else {
								 closed_ = true;
							 }
						 });
	}

	template<typename _Msg_type>
	void TCPChannel<_Msg_type>::DoReadBody() {
		if (closed_)return;

		auto self(this->shared_from_this());
		asio::async_read(socket_,
						 asio::buffer(read_msg_.Body(), read_msg_.BodyLength()),
						 [this, self](std::error_code ec, std::size_t) {
							 if (!ec) {
								 DoReadHeader();
							 } else {
								 closed_ = true;
							 }
						 });
	}

	template<typename _Msg_type>
	bool TCPChannel<_Msg_type>::IsOpen() {
		return !closed_;
	}
}
