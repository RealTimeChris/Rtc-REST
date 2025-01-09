Getting a Guild {#getting_a_guild}
============
- Execute the, `rtc_rest::guilds::getCachedGuild()` (which collects it from the cache), or `rtc_rest::guilds::getGuildAsync()` (which collects it from the discord servers) function, while passing to it a data structure of type `rtc_rest::get_guild_data`, with a return value of `rtc_rest::guild_data`.
- call the function with `rtc_rest::co_routine::get()` added to the end in order to wait for the results now.

```cpp
/// Test.hpp -header for the "test" command.
/// https://github.com/RealTimeChris/Rtc-REST

#pragma once

#include "index.hpp"

namespace rtc_rest {

	class test : public base_function {
	  public:
		test() {
			commandName = "test";
			helpDescription = "testing purposes!";
			embed_data msgEmbed;
			msgEmbed.setDescription("------\nSimply enter !test or /test!\n------");
			msgEmbed.setTitle("__**test Usage__");
			msgEmbed.setTimeStamp(getTimeAndDate());
			msgEmbed.setColor("fe_fe_fe");
			helpEmbed = msgEmbed;
		}

		std::unique_ptr<base_function> create() {
			return std::make_unique<test>();
		}

		virtual void execute(base_function_arguments& args) {
			guild_data guild01 = guilds::getCachedGuild({args.eventData.getGuildId()}).get();

			guild_data guild02 = guilds::getGuildAsync({args.eventData.getGuildId()}).get();
		}
	};
}
```
