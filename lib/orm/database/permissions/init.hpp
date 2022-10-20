#pragma once

namespace ORM {
	namespace Permissions {
		using flag = unsigned int;

		static constexpr flag  NONE = 0;
		static constexpr flag  Read = 1 << 0; // requires  read permissions
		static constexpr flag Write = 1 << 1; // requires write permissions
		static constexpr flag Admin = 1 << 2; // requires admin permissions
		static constexpr flag   ALL = (1 << 0) & (1 << 1) & (1 << 2);
	}
}