Deleting a Guild Scheduled Event {#deleting_a_guild_scheduled_event}
============
- Execute the, from the `rtc_rest::guild_scheduled_events::deleteGuildScheduledEventAsync()` function, while passing in a value of type `rtc_rest::delete_guild_scheduled_event_data`, with a return value of type `void`.
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
				get_guild_scheduled_events_data& dataPackage01;
				dataPackage01.guildId = args.eventData.getGuildId();
				dataPackage01.withUserCount = true;
				auto responseData01 = guild_scheduled_events::getGuildScheduledEventsAsync(dataPackage01).get();

				delete_guild_scheduled_event_data& dataPackage;
				dataPackage.guildId = args.eventData.getGuildId();
				dataPackage.guildScheduledEventId = responseData01[0].id;

				guild_scheduled_events::deleteGuildScheduledEventAsync(dataPackage).get();


			} catch (...) {
				rethrowException("test::execute()");
			}
		}
	};
}
```
