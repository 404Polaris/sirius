//
// Created by 404Polaris on 2019/10/15.
//


#pragma once

#include <sirius/core/nocopyable.hpp>

#include <asio.hpp>

namespace sirius::core {
	namespace net_lib {
		using namespace asio;
		using tcp = asio::ip::tcp;
	}

	template<typename _pkt_reader_type>
	class tcp_session : nocopyable {
	protected:
		_pkt_reader_type reader_;
		net_lib::tcp::socket socket_;
	public:
		explicit tcp_session(net_lib::tcp::socket socket);
		~tcp_session();
	public:
		void read_pkt();
		void read_some(std::unique_ptr<_pkt_reader_type> reader);
	};

	template<typename _pkt_reader_type>
	tcp_session<_pkt_reader_type>::tcp_session(net_lib::tcp::socket socket):socket_(std::move(socket)) {

	}
	template<typename _pkt_reader_type>
	tcp_session<_pkt_reader_type>::~tcp_session() {
		socket_.close();
	}

	template<typename _pkt_reader_type>
	void tcp_session<_pkt_reader_type>::read_pkt() {

	}

	template<typename _pkt_reader_type>
	void tcp_session<_pkt_reader_type>::read_some(std::unique_ptr<_pkt_reader_type> reader) {

	}
}


