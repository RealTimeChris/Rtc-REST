Modifying Guild Role Positions {#modifying_guild_role_positions}
============
- Execute the, `rtc_rest::roles::modifyGuildRolePositionsAsync()` function, while passing in a value of type `rtc_rest::modify_guild_role_positions_data`, with a return value of type `auto` or `std::vector<role>`.
- call the function with `rtc_rest::co_routine::get()` added to the end in order to wait for the results now.

```cpp
/// Test.hpp -header for the "test" command.
/// https://github.com/RealTimeChris/Rtc-REST

#pragma once

#include <index.hpp>

namespace rtc_rest {

	class test : public base_function {
	  public:
		test() {
			commandName = "test";
			helpDescription = "testing purposes!";
			embed_data msgEmbed { };
			msgEmbed.setDescription("------\nSimply enter !test or /test!\n------");
			msgEmbed.setTitle("__**test usage:**__");
			msgEmbed.setTimeStamp(getTimeAndDate());
			msgEmbed.setColor("fe_fe_fe");
			helpEmbed = msgEmbed;
		}

		std::unique_ptr<base_function> create() {
			return std::make_unique<test>();
		}

		virtual void execute(base_function_arguments& args) {
			try {
				modify_guild_role_positions_data& dataPackage01;
				dataPackage01.newPosition = 12;
				dataPackage01.roleId = "886366417316896799";
				dataPackage01.reason = "testing!";
				dataPackage01.guildId = args.eventData.getGuildId();

				auto newRoles = roles::modifyGuildRolePositionsAsync(dataPackage01).get();

				for (const auto& value: newRoles) {
					std::cout << "role name: " << value.name << std::endl;
				}


			} catch (...) {
				rethrowException("test::execute()");
			}
		}
	};
}


```
