//
// Created by 404Polaris on 2019/11/21.
//

namespace Sirius {

	class ConnectionBase : NoCopyAble {
	protected:
		bool free_ = false;
	protected:
		explicit ConnectionBase() = default;
		~ConnectionBase() = default;
	protected:
		void UnLock() {
			free_ = true;
		}

		bool TryLock() {
			if (free_) {
				free_ = false;
				return true;
			}

			return false;
		}
	};

	template<typename _Database_type>
	class ConnectionWrapper : public NoCopyAble {
		using Connection_type = typename _Database_type::Connection_type;
	private:
		const std::shared_ptr<Connection_type> conn_ = nullptr;
	public:
		explicit ConnectionWrapper(std::shared_ptr<Connection_type> conn) : conn_(conn) {

		}

		ConnectionWrapper(ConnectionWrapper &&other) noexcept : conn_(std::move(other.conn_)) {

		}

		ConnectionWrapper &operator=(ConnectionWrapper &&other) noexcept {
			conn_ = std::move(other.conn_);
		}

		~ConnectionWrapper() {
			if (conn_) {
				conn_->unlock();
			}
		};

	public:
		std::shared_ptr<Connection_type> GetConnection() {
			return conn_;
		}

		explicit operator bool() {
			return conn_.get() != nullptr;
		}

		Connection_type *operator->() {
			return conn_.get();
		}
	};

	template<typename _Database_type>
	class ConnectionPool : public NoCopyAble {
		using Config_type = typename _Database_type::Config_type;
		using Connection_type = typename _Database_type::Connection_type;
	private:
		std::mutex mutex_{};
		const Config_type config_;
		const size_t pool_size_ = 0;
		std::vector<std::shared_ptr<Connection_type>> connections_;
	public:
		explicit ConnectionPool(Config_type config) :
			pool_size_(config.GetPoolSize()),
			config_(std::move(config)) {
		}

		ConnectionPool(ConnectionPool &&o) noexcept
			: pool_size_(o.pool_size_),
			  config_(std::move(o.config_)),
			  connections_(std::move(o.connections_)) {
		}

		ConnectionPool &operator=(ConnectionPool &&o) noexcept {
			config_ = std::move(o.config_);
			pool_size_ = o.pool_size_;
			connections_ = std::move(o.connections_);
			return *this;
		}
	public:
		void Connect(size_t pool_size) {
			connections_.clear();

			for (size_t i = 0; i < pool_size; i++) {
				connections_.push_back(std::make_shared<Connection_type>(config_));
			}
		}

		ConnectionWrapper<Connection_type> GetConnection() {
			std::lock_guard<std::mutex> lock(mutex_);

			for (size_t i = 0; i < pool_size_; i++) {
				if (connections_[i]->try_lock()) {
					return ConnectionWrapper<Connection_type>(connections_[i]);
				}
			}

			return ConnectionWrapper<Connection_type>(nullptr);
		}
	};
}
