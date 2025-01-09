Creating a Guild Ban {#creating_a_guild_ban}
============
- Execute the, `rtc_rest::guilds::createGuildBanAsync()` function, while passing in a value of type `rtc_rest::create_guild_ban_data`, with a return value of type `void`.
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
				create_guild_ban_data& dataPackage01;
				dataPackage01.guildId = args.eventData.getGuildId();
				dataPackage01.guildMemberId = "851629399514415106";
				dataPackage01.deleteMessageDays = 3;
				dataPackage01.reason = "testing purposes!";

				guilds::createGuildBanAsync(dataPackage01).get();


			} catch (...) {
				rethrowException("test::execute()");
			}
		}
	};
}
```
