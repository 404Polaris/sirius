//
// Created by 404Polaris on 2019/12/4.
//

#pragma once

#include <Yoa/Common/Hash.hpp>
#include <Yoa/Server/LoginServer/Component/Session.h>

namespace Yoa::LoginServer {
	class Environment;
}

namespace Yoa::LoginServer::Event {
	struct Event_Id_type {
		size_t first_;
		size_t second_;

		constexpr Event_Id_type() : first_(0), second_(0) {}
		explicit constexpr Event_Id_type(size_t first) : first_(first), second_(0) {}
		explicit constexpr Event_Id_type(size_t first, size_t second) : first_(first), second_(second) {}

		bool operator==(const Event_Id_type &o) const {
			return o.first_ == first_ && o.second_ == second_;
		}

		static size_t GetHashValue(const Event_Id_type &o) {
			auto result = std::hash<size_t>()(o.first_);
			HashCombine<size_t>(result, o.second_);
			return result;
		}
	};
}

namespace std {
	template<>
	class std::hash<Yoa::LoginServer::Event::Event_Id_type> {
		using Id_type = Yoa::LoginServer::Event::Event_Id_type;
	public :
		size_t operator()(const Id_type &id) const {
			return Id_type::GetHashValue(id);
		}
	};
}

namespace Yoa::LoginServer::Event {
	struct NetCmdEvent;

	struct EventBase {
	public:
		using Id_type = Event_Id_type;

		template<typename _Event_t>
		constexpr static Id_type GetEventId(_Event_t &event) {
			if constexpr (std::is_same_v<_Event_t, NetCmdEvent>) {
				return _Event_t::GetNetCmdEventId(event.cmd_);
			} else {
				return _Event_t::GetNormalEventId();
			}
		}

		template<typename _Event_t, RemoteCommand CMD>
		constexpr static Id_type GetEventId() {
			if constexpr (std::is_same_v<_Event_t, NetCmdEvent>) {
				return _Event_t::GetNetCmdEventId(CMD);
			} else {
				static_assert(CMD == RemoteCommand::kVoid, "Not NetCmdEvent but use RemoteCommand.");
				return _Event_t::GetNormalEventId();
			}
		}
	};

	template<size_t N, size_t I = 0>
	struct EventWithId : EventBase {
	protected:
		constexpr static Event_Id_type event_id_{N, I};
	public:
		constexpr static auto GetNormalEventId() {
			return event_id_;
		}
	};

	struct UpdateEvent : public EventWithId<0> {
		Environment &env_;
	public:
		explicit UpdateEvent(Environment &env) : env_(env) {}
	};

	struct NetCmdEvent : public EventWithId<1> {
		Environment &env_;
		RemoteCommand cmd_;
		mutable MessageBuffer buffer_;
		Yoa::LoginServer::Component::Session &session_;
	public:
		explicit NetCmdEvent(Environment &env, RemoteCommand cmd, MessageBuffer msg_buffer, Component::Session &session)
			: env_(env),
			  cmd_(cmd),
			  buffer_(std::move(msg_buffer)),
			  session_(session) {
		}

		static constexpr Id_type GetNetCmdEventId(RemoteCommand cmd) {
			return Event_Id_type{event_id_.first_, (size_t) cmd};
		}
	};
}