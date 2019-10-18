//
// Created by 404Polaris on 2019/10/15.
//


#pragma once

#include <sirius/core/nocopyable.hpp>
#include <sirius/core/enable_sirius_env.hpp>
#include <sirius/core/network/message_buffer.hpp>

#include <memory>
#include <queue>
#include <asio.hpp>

namespace sirius::core {
	namespace net_lib {
		using namespace asio;
		using tcp = asio::ip::tcp;
	}

	template<typename _pkt_reader_type, typename _env_type>
	class tcp_session : public std::enable_shared_from_this<tcp_session<_pkt_reader_type, _env_type>>,
						public enable_sirius_env<_env_type>,
						public nocopyable {
	protected:
		bool work_fine_;
		_pkt_reader_type reader_;
		net_lib::tcp::socket socket_;
		std::atomic_bool write_flag_;
		net_lib::io_context::strand strand_;
		std::queue<message_buffer> write_buffer_queue_;
	public:
		explicit tcp_session(net_lib::tcp::socket socket, net_lib::io_context &io_context);
		~tcp_session();
	public:
		void start();
		bool work_fine();
		void write(message_buffer buffer);
	protected:
		void read_pkt();
		void write_pkt(message_buffer);
	};

	template<typename _pkt_reader_type, typename _env_type>
	tcp_session<_pkt_reader_type, _env_type>::tcp_session(net_lib::tcp::socket socket, net_lib::io_context &io_context)
		:socket_(std::move(socket)), strand_(io_context), write_flag_(false) {
	}

	template<typename _pkt_reader_type, typename _env_type>
	tcp_session<_pkt_reader_type, _env_type>::~tcp_session() {
		socket_.close();
	}

	template<typename _pkt_reader_type, typename _env_type>
	void tcp_session<_pkt_reader_type, _env_type>::start() {
		work_fine_ = true;
		read_pkt();
	}

	template<typename _pkt_reader_type, typename _env_type>
	void tcp_session<_pkt_reader_type, _env_type>::read_pkt() {
		auto[no_error, length] = reader_.should_read();

		if (!no_error)return;

		auto that = this->shared_from_this();

		asio::async_read(socket_,
						 asio::buffer(reader_.tail(), length),
						 asio::bind_executor(strand_, [that](std::error_code ec, std::size_t n) {
							 if (!ec) {
								 that->reader_.write(n);
								 that->read_pkt();
							 } else {
								 that->work_fine_ = false;
							 }
						 }));
	}

	template<typename _pkt_reader_type, typename _env_type>
	void tcp_session<_pkt_reader_type, _env_type>::write_pkt(message_buffer buffer) {
		auto that = this->shared_from_this();

		asio::async_write(socket_,
						  asio::buffer(buffer.data(), buffer.size()),
						  asio::bind_executor(strand_, [that](std::error_code ec, std::size_t n) {
							  if (!ec) {
								  if (that->write_buffer_queue_.empty()) {
									  that->write_flag = false;
								  } else {
									  auto next_buffer = std::move(that->write_buffer_queue_.front());
									  that->write_buffer_queue_.pop();
									  that->write_pkt(std::move(next_buffer));
								  }
							  } else {
								  that->work_fine_ = false;
							  }
						  }));
	}

	template<typename _pkt_reader_type, typename _env_type>
	void tcp_session<_pkt_reader_type, _env_type>::write(message_buffer buffer) {
		auto that = this->shared_from_this();

		asio::post(asio::bind_executor(strand_, [that, buffer]() {
			if (!(that->write_flag_)) {
				that->write_flag = true;
				that->write_pkt(buffer);
			} else {
				that->write_buffer_queue_.push(buffer);
			}
		}));
	}

	template<typename _pkt_reader_type, typename _env_type>
	bool tcp_session<_pkt_reader_type, _env_type>::work_fine() {
		return work_fine_;
	}
}


