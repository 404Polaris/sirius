//
// Created by 404Polaris on 2019/10/15.
//


#pragma once

#ifndef ASIO_STANDALONE
#define ASIO_STANDALONE
#endif

#include <Yoa/Common/NoCopyAble.hpp>
#include <Yoa/Net/TcpSession.hpp>

#include <functional>
#include <asio.hpp>

namespace Yoa {

	namespace net_lib {
		using namespace asio;
		using tcp = asio::ip::tcp;
	}

	template<typename _Session_t>
	class TcpServer : public NoCopyAble {
	protected:
		std::atomic_bool running_;
		net_lib::io_context io_context_;
		net_lib::tcp::acceptor acceptor_;
		std::vector<std::thread> threads_;
		std::function<void(std::shared_ptr<_Session_t> &)> on_conn_;
	public:
		explicit TcpServer(uint16_t port, decltype(on_conn_) on_conn);
		~TcpServer();
	public:
		void Start();
	private:
		void Accept();
	};

	template<typename _Session_t>
	inline TcpServer<_Session_t>::TcpServer(uint16_t port, decltype(on_conn_) on_conn) :
		running_(false),
		acceptor_(io_context_,
				  net_lib::tcp::endpoint(net_lib::tcp::v4(),
										 static_cast<unsigned short>(port))),
		on_conn_(on_conn) {
	}

	template<typename _Session_t>
	inline void TcpServer<_Session_t>::Start() {
		running_ = true;

		Accept();

		for (size_t i = 0; i < std::thread::hardware_concurrency(); i++) {
			threads_.emplace_back([this]() {
				io_context_.run();
			});
		}
	}

	template<typename _Session_t>
	inline void TcpServer<_Session_t>::Accept() {
		if (!running_)return;

		acceptor_.async_accept(
			[this](std::error_code ec, net_lib::tcp::socket socket) {
				if (!ec) {
					auto session = std::make_shared<_Session_t>(std::move(socket), io_context_);
					on_conn_(session);
					session->Start();
				}

				Accept();
			});
	}

	template<typename _Session_t>
	inline TcpServer<_Session_t>::~TcpServer() {
		bool flag = running_;
		running_ = false;

		if (flag) {
			io_context_.stop();
			acceptor_.release();
		}

		for (auto &t : threads_) {
			t.join();
		}

	}
}

