//
// Created by 404Polaris on 2019/10/26.
//

#pragma  once

extern "C" {
#include <mysql.h>
}

#include <string>

namespace FaceMySql {
	template<typename _Object_type, typename _Field_type>
	struct Field {
		std::string field_name_;
		_Field_type _Object_type::* field_;
	};

	template<typename _DbAdapter_type>
	auto MakeFace(typename _DbAdapter_type::_Config_type config) {

	}

	class MySqlAdapter {
		struct Config {};
	public:
		using _Config_type = Config;
	};
}
