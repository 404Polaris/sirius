//
// Created by 404Polaris on 2019/12/10.
//


#pragma once

#include <list>
#include <functional>
#include <unordered_map>

namespace Yoa {
	template<typename _Base_event_t>
	class EventDispatcher {
	public:
		using Func_ptr_type = void (*)(_Base_event_t *);
		using Id_type =  typename _Base_event_t::Id_type;
		using Stored_Handler_type =  std::function<void(_Base_event_t *)>;
	private:
		std::unordered_map<Id_type, std::list<Stored_Handler_type>> handler_map_;
	public:
		void RegisterEvent(Id_type id, Func_ptr_type f) {
			if (!f)return;

			auto &list = handler_map_[id];

			if (std::find_if(list.begin(), list.end(), [&f](auto &h) {
				return (f == *(h.template target<Func_ptr_type>()));
			}) == list.end()) {
				Stored_Handler_type handler = f;
				list.push_front(std::move(f));
			}
		}

		void UnRegisterEvent(Id_type id, Func_ptr_type f) {
			if (!f)return;

			auto &list = handler_map_[id];
			list.remove_if([&f](auto &h) { return (f == *(h.template target<Func_ptr_type>())); });
		}

		void TriggerEvent(Id_type id, _Base_event_t *evt) {
			auto &list = handler_map_[id];
			for (auto &h : list) {
				h(evt);
			}
		}
	};
}


