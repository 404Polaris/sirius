//
// Created by 404Polaris on 2019/10/15.
//


#pragma once

#ifndef ASIO_STANDALONE
#define ASIO_STANDALONE
#endif

#include <sirius/core/nocopyable.hpp>
#include <sirius/core/network/tcp_session.hpp>
#include <sirius/core/enable_sirius_env.hpp>

#include <entt/entt.hpp>
#include <asio.hpp>

namespace sirius::core {

	namespace net_lib {
		using namespace asio;
		using tcp = asio::ip::tcp;
	}

	template<typename _session_type, typename _env_type>
	class tcp_server : public enable_sirius_env<_env_type>,
					   public nocopyable {
	protected:
		std::atomic_bool running_;
		net_lib::io_context io_context_;
		net_lib::tcp::acceptor acceptor_;
		std::vector<std::thread> threads_;
	public:
		explicit tcp_server(unsigned short port);
		~tcp_server();
	public:
		void start();
	private:
		void accept();
	};

	template<typename _session_type, typename _env_type>
	inline tcp_server<_session_type, _env_type>::tcp_server(unsigned short port) :
		running_(false),
		acceptor_(io_context_, net_lib::tcp::endpoint(net_lib::tcp::v4(), static_cast<unsigned short>(port))) {
	}

	template<typename _session_type, typename _env_type>
	inline void tcp_server<_session_type, _env_type>::start() {
		running_ = true;

		accept();

		for (size_t i = 0; i < std::thread::hardware_concurrency(); i++) {
			threads_.emplace_back([this]() {
				io_context_.run();
			});
		}
	}

	template<typename _session_type, typename _env_type>
	inline void tcp_server<_session_type, _env_type>::accept() {
		if (!running_)return;

		acceptor_.async_accept(
			[this](std::error_code ec, net_lib::tcp::socket socket) {
				if (!ec) {
					auto session = std::make_shared<_session_type>(std::move(socket), io_context_);
					this->env()->on_connect(session);
					session->start();
				}

				accept();
			});
	}

	template<typename _session_type, typename _env_type>
	inline tcp_server<_session_type, _env_type>::~tcp_server() {
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

