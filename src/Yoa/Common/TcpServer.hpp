//
// Created by 404Polaris on 2019/10/15.
//


#pragma once

#ifndef ASIO_STANDALONE
#define ASIO_STANDALONE
#endif

#include <Yoa/Common/NoCopyAble.hpp>
#include <Yoa/Common/TcpSession.hpp>
#include <Yoa/Common/EnableYoaEnv.hpp>

#include <asio.hpp>

namespace Yoa {

	namespace net_lib {
		using namespace asio;
		using tcp = asio::ip::tcp;
	}

	template<typename _Session_type, typename _Env_type>
	class TcpServer : public EnableYoaEnv<_Env_type>,
					  public NoCopyAble {
	protected:
		std::atomic_bool running_;
		net_lib::io_context io_context_;
		net_lib::tcp::acceptor acceptor_;
		std::vector<std::thread> threads_;
	public:
		explicit TcpServer(unsigned short port);
		~TcpServer();
	public:
		void Start();
	private:
		void Accept();
	};

	template<typename _Session_type, typename _Env_type>
	inline TcpServer<_Session_type, _Env_type>::TcpServer(unsigned short port) :
		running_(false),
		acceptor_(io_context_, net_lib::tcp::endpoint(net_lib::tcp::v4(), static_cast<unsigned short>(port))) {
	}

	template<typename _Session_type, typename _Env_type>
	inline void TcpServer<_Session_type, _Env_type>::Start() {
		running_ = true;

		Accept();

		for (size_t i = 0; i < std::thread::hardware_concurrency(); i++) {
			threads_.emplace_back([this]() {
				io_context_.run();
			});
		}
	}

	template<typename _Session_type, typename _Env_type>
	inline void TcpServer<_Session_type, _Env_type>::Accept() {
		if (!running_)return;

		acceptor_.async_accept(
			[this](std::error_code ec, net_lib::tcp::socket socket) {
				if (!ec) {
					auto session = std::make_shared<_Session_type>(std::move(socket), io_context_);
					this->Env()->OnConnect(session);
					session->Start();
				}

				Accept();
			});
	}

	template<typename _Session_type, typename _Env_type>
	inline TcpServer<_Session_type, _Env_type>::~TcpServer() {
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

