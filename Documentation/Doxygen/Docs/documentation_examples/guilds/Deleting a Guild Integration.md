Deleting a Guild Integration {#deleting_a_guild_integration}
============
- Execute the, `rtc_rest::guilds::deleteGuildIntegrationAsync()` function, while passing in a value of type `rtc_rest::delete_guild_integration_data`, with a return value of type `void`.
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
				delete_guild_integration_data& dataPackage01;
				dataPackage01.guildId = args.eventData.getGuildId();
				dataPackage01.integrationId = "INTEGRATION_ID_HERE";
				dataPackage01.reason = "testing purposes!";

				guilds::deleteGuildIntegrationAsync(dataPackage01).get();


			} catch (...) {
				rethrowException("test::execute()");
			}
		}
	};
}
```
