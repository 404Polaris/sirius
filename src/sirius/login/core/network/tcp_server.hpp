//
// Created by 404Polaris on 2019/10/15.
//


#pragma once

#ifndef ASIO_STANDALONE
#define ASIO_STANDALONE
#endif

#include <sirius/login/core/nocopyable.hpp>
#include <sirius/login/core/network/tcp_session.hpp>

#include <asio.hpp>

#include <iostream>

namespace sirius::core {

	namespace net_lib {
		using namespace asio;
		using tcp = asio::ip::tcp;
	}

	class tcp_server : public nocopyable {
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
		void do_accept();
	};

	tcp_server::tcp_server(unsigned short port) :
		running_(false),
		acceptor_(io_context_, net_lib::tcp::endpoint(net_lib::tcp::v4(), port)) {
	}

	void tcp_server::start() {
		running_ = true;

		do_accept();

		for (size_t i = 0; i < std::thread::hardware_concurrency(); i++) {
			threads_.emplace_back([this]() {
				io_context_.run();
			});
		}
	}

	void tcp_server::do_accept() {
		if (!running_)return;

		acceptor_.async_accept(
			[this](std::error_code ec, net_lib::tcp::socket socket) {

				if (!ec) {

				}

				do_accept();
			});

	}

	tcp_server::~tcp_server() {
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

