//
// Created by 404Polaris on 2019/10/15.
//


#pragma once

#include <sirius/core/nocopyable.hpp>
#include <sirius/core/enable_sirius_env.hpp>

#include <memory>
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
		_pkt_reader_type reader_;
		net_lib::tcp::socket socket_;
		net_lib::io_context::strand strand_;
	public:
		explicit tcp_session(net_lib::tcp::socket socket, net_lib::io_context &io_context);
		~tcp_session();
	public:
		void start();
	protected:
		void read_pkt();
	};

	template<typename _pkt_reader_type, typename _env_type>
	tcp_session<_pkt_reader_type, _env_type>::tcp_session(net_lib::tcp::socket socket, net_lib::io_context &io_context)
		:socket_(std::move(socket)), strand_(io_context) {
	}

	template<typename _pkt_reader_type, typename _env_type>
	tcp_session<_pkt_reader_type, _env_type>::~tcp_session() {
		socket_.close();
	}

	template<typename _pkt_reader_type, typename _env_type>
	void tcp_session<_pkt_reader_type, _env_type>::start() {
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
							 }
						 }));
	}
}


