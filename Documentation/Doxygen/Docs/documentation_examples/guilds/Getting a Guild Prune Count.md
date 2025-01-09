Getting a Guild Prune Count {#getting_a_guild_prune_count}
============
- select, from the `rtc_rest::guilds::getGuildPruneCountAsync()` function, while passing in a value of type `rtc_rest::get_guild_prune_count_data`, with a return value of type `auto` or `rtc_rest::guild_prune_count_data`.
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
				get_guild_prune_count_data& dataPackage01;
				dataPackage01.days = 28;
				dataPackage01.roles.emplace_back("860075141818744853");
				dataPackage01.guildId = args.eventData.getGuildId();

				auto responseData = guilds::getGuildPruneCountAsync(dataPackage01).get();

				std::cout << responseData.count << std::endl;


			} catch (...) {
				rethrowException("test::execute()");
			}
		}
	};
}


```
