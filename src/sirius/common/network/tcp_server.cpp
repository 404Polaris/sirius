//
// Created by 404Polaris on 2019/9/18.
//

#include <sirius/common/network/tcp_server.h>
#include <sirius/common/common.h>

#include <fmt/format.h>
#include <iostream>

namespace sirius::common {
	TCPServer::TCPServer(const int port) {
		io_context_ = std::make_unique<asio::io_context>();
		endpoint_ = std::make_unique<asio::ip::tcp::endpoint>(asio::ip::tcp::v4(), port);
		acceptor_ = std::make_unique<asio::ip::tcp::acceptor>(*io_context_, *endpoint_);
	}

	void TCPServer::Accept() {
		if (!run_) {
			return;;
		}

		acceptor_->async_accept([this](std::error_code ec, asio::ip::tcp::socket socket) {
		  if (!ec) {
			  Accept();
		  }
		});
	}

	void TCPServer::Start() {
		run_ = true;
		Accept();
	}

	void TCPServer::Run() {
		Start();
		io_context_->run();
	}

	void TCPServer::Stop() {
		run_ = false;
		io_context_->stop();
		ReleaseCPUUntil([this]() {
		  return io_context_->stopped();
		});
	}

	TCPServer::~TCPServer() {
		Stop();
		acceptor_.reset();
		endpoint_.reset();
	}
}
