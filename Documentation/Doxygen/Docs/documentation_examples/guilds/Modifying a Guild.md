Modifying a Guild {#modifying_a_guild}
============
- Execute the, `rtc_rest::guilds::modifyGuildAsync()` function, while passing in a value of type `rtc_rest::modify_guild_data`, with a return value of type `auto` or `rtc_rest::guild_data`.
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
			embed_data msgEmbed;
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
				vector<guild_data> guilds = guilds::getAllGuildsAsync().get();
				for (const auto& value: guilds) {
					if (value.name == "test guild_data") {
						modify_guild_data& dataPackage00 {value};
						dataPackage00.name = "newer test guild_data!";
						dataPackage00.reason = "testing purposes!";

						auto guild_data = guilds::modifyGuildAsync(dataPackage00).get();

						std::cout << "the name: " << guild_data.name << std::endl;
					}
				}



			} catch (...) {
				rethrowException("test::execute()");
			}
		}
	};
}
```
