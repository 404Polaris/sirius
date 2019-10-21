//
// Created by 404Polaris on 2019/10/15.
//


#pragma once

#include <sirius/core/nocopyable.hpp>
#include <sirius/core/enable_sirius_env.hpp>
#include <sirius/core/network/message_buffer.hpp>

#include <queue>
#include <mutex>
#include <memory>
#include <atomic>
#include <optional>
#include <asio.hpp>

namespace sirius::core {
	namespace net_lib {
		using namespace asio;
		using tcp = asio::ip::tcp;
	}

	template<typename _msg_reader_type, typename _env_type>
	class tcp_session : public std::enable_shared_from_this<tcp_session<_msg_reader_type, _env_type>>,
						public enable_sirius_env<_env_type>,
						public nocopyable {
	protected:
		bool work_fine_;
		std::mutex read_mutex_;
		_msg_reader_type reader_;
		net_lib::tcp::socket socket_;
		net_lib::io_context::strand strand_;
		std::queue<message_buffer> read_buffer_queue_;
		std::queue<message_buffer> write_buffer_queue_;
	public:
		explicit tcp_session(net_lib::tcp::socket socket, net_lib::io_context &io_context);
		~tcp_session();
	public:
		void start();
		bool work_fine();
		void write(message_buffer buffer);
		std::optional<message_buffer> read();
	protected:
		void read_msg();
		void write_msg();
		void read_msg_complete();
	};

	template<typename _msg_reader_type, typename _env_type>
	inline tcp_session<_msg_reader_type, _env_type>::tcp_session(net_lib::tcp::socket socket,
																 net_lib::io_context &io_context)
		:socket_(std::move(socket)), strand_(io_context), work_fine_(false) {
	}

	template<typename _msg_reader_type, typename _env_type>
	inline tcp_session<_msg_reader_type, _env_type>::~tcp_session() {
		socket_.close();
	}

	template<typename _msg_reader_type, typename _env_type>
	inline void tcp_session<_msg_reader_type, _env_type>::start() {
		work_fine_ = true;
		reader_.init();
		read_msg();
	}

	template<typename _msg_reader_type, typename _env_type>
	inline void tcp_session<_msg_reader_type, _env_type>::read_msg() {
		if (!work_fine_)return;

		auto[no_error, length] = reader_.should_read();

		if (!no_error)return;

		if (no_error && length == 0) {
			read_msg_complete();
			reader_.init();
		}

		auto that = this->shared_from_this();

		asio::async_read(socket_,
						 asio::buffer(reader_.tail(), length),
						 asio::bind_executor(strand_, [that](std::error_code ec, std::size_t n) {
							 if (!ec) {
								 that->reader_.write(n);
								 that->read_msg();
							 } else {
								 that->work_fine_ = false;
							 }
						 }));
	}

	template<typename _msg_reader_type, typename _env_type>
	void tcp_session<_msg_reader_type, _env_type>::read_msg_complete() {
		std::lock_guard<std::mutex> lock(read_mutex_);

		auto &&buffer = reader_.pop_buffer();
		read_buffer_queue_.emplace(std::move(buffer));
	}

	template<typename _msg_reader_type, typename _env_type>
	inline void tcp_session<_msg_reader_type, _env_type>::write_msg() {
		if (!work_fine_)return;

		auto that = this->shared_from_this();

		asio::async_write(socket_,
						  asio::buffer(write_buffer_queue_.front().data(), write_buffer_queue_.front().size()),
						  asio::bind_executor(strand_, [that](std::error_code ec, std::size_t n) {
							  if (!ec) {
								  that->write_buffer_queue_.pop();

								  if (!(that->write_buffer_queue_.empty())) {
									  that->write_msg();
								  }
							  } else {
								  that->work_fine_ = false;
							  }
						  }));
	}

	template<typename _msg_reader_type, typename _env_type>
	inline void tcp_session<_msg_reader_type, _env_type>::write(message_buffer buffer) {
		if (!work_fine_)return;

		auto that = this->shared_from_this();

		asio::post(asio::bind_executor(strand_, [that, local_buffer = std::move(buffer)]() {
			auto write_flag = !(that->write_buffer_queue_.empty());
			that->write_buffer_queue_.push(local_buffer);

			if (!write_flag) {
				that->write_msg();
			}
		}));
	}

	template<typename _msg_reader_type, typename _env_type>
	inline bool tcp_session<_msg_reader_type, _env_type>::work_fine() {
		return work_fine_;
	}

	template<typename _msg_reader_type, typename _env_type>
	std::optional<message_buffer> tcp_session<_msg_reader_type, _env_type>::read() {
		std::lock_guard<std::mutex> lock(read_mutex_);

		if (read_buffer_queue_.empty())return std::nullopt;
		auto buffer = std::move(read_buffer_queue_.front());
		read_buffer_queue_.pop();
		return buffer;
	}
}


