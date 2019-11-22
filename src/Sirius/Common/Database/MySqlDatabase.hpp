//
// Created by 404Polaris on 2019/11/21.
//


#include <Sirius/Common/Database/DatabaseCommon.hpp>
#include <utility>
#include <mysql.h>

namespace Sirius::Database {
	struct MySqlConfig {
		std::string db_;
		std::string host_;
		std::string port_;
		std::string user_;
		std::string password_;
		std::string pool_size_;
	public:
		[[nodiscard]] auto GetDB() const {
			return db_.c_str();
		}

		[[nodiscard]] auto GetHost() const {
			return host_.c_str();
		}

		[[nodiscard]] auto GetPort() const {
			return std::stoul(port_);
		}

		[[nodiscard]] auto GetUser() const {
			return user_.c_str();
		}

		[[nodiscard]] auto GetPassword() const {
			return password_.c_str();
		}

		[[nodiscard]] auto GetPoolSize() const {
			return std::stoul(pool_size_);
		}
	};

	class MySqlConnection : public ConnectionBase {
		friend class ConnectionPool;
		friend class ConnectionWrapper;
	private:
		MYSQL *conn_ = nullptr;
	public:
		explicit MySqlConnection(MySqlConfig config) : conn_(mysql_init(nullptr)) {
			if (conn_) {
				Connect(config);
			}
		}

		~MySqlConnection() {
			free_ = false;

			if (conn_) {
				mysql_close(conn_);
				conn_ = nullptr;
			}
		}
	private:
		void Connect(MySqlConfig &config) {
			conn_ = mysql_real_connect(conn_,
									   config.GetHost(),
									   config.GetUser(),
									   config.GetPassword(),
									   config.GetDB(),
									   config.GetPort(),
									   nullptr,
									   0);

			if (conn_) {
				EnableReconnect();
			}
		}

		void EnableReconnect() {
			bool value = true;
			mysql_options(conn_, MYSQL_OPT_RECONNECT, (char *) &value);
			free_ = true;
		}
	};

	class MySqlDatabase {
	public:
		using Config_type = MySqlConfig;
		using Connection_type = MySqlConnection;
	public:
		ConnectionPool<MySqlDatabase> connection_pool_;
	public:
		explicit MySqlDatabase(const Config_type &config) : connection_pool_(config) {
			connection_pool_.Connect();
		}
	};
}
