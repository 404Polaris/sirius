//
// Created by 404Polaris on 2019/9/18.
//

#pragma once

#include <sirius/common/noncopyable.h>

#include <memory>
#include <functional>
#include <asio.hpp>

namespace sirius::common {

	class TCPServer : public NonCopyable {
	protected:
		std::atomic_bool run_ = false;
		std::unique_ptr<asio::io_context> io_context_;
		std::unique_ptr<asio::ip::tcp::endpoint> endpoint_;
		std::unique_ptr<asio::ip::tcp::acceptor> acceptor_;
	public:
		explicit TCPServer(int port);
		~ TCPServer();
	protected:
		void Start();
		void Accept();
	public:
		void Run();
		void Stop();
	};
}
