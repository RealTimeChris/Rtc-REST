Creating a Guild Role {#creating_a_guild_role}
============
- Execute the, `rtc_rest::roles::createGuildRoleAsync()` function, while passing in a value of type `rtc_rest::create_guild_role_data`, with a return value of type `auto` or `rtc_rest::role`.
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
				create_guild_role_data& dataPackage01;
				dataPackage01.name = "test role";
				dataPackage01.hexColorValue = "fefe23";
				dataPackage01.hoist = false;
				dataPackage01.reason = "testing!";
				dataPackage01.guildId = args.eventData.getGuildId();

				auto newRole = roles::createGuildRoleAsync(dataPackage01).get();

				std::cout << "role name: " << newRole.name << std::endl;


			} catch (...) {
				rethrowException("test::execute()");
			}
		}
	};
}


```
