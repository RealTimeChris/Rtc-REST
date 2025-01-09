Getting a Channel {#getting_channel}
============
- Execute the `rtc_rest::channels::getCachedChannel()` (which collects it from the cache), or `rtc_rest::getChannelAsync()` (which collects it from the discord servers) function, while passing to it a data structure of type `rtc_rest::get_channel_data`, with a return type of `auto` or `rtc_rest::channel_data`.
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
			msgEmbed.setTitle("__**test usage:**__");
			msgEmbed.setTimeStamp(getTimeAndDate());
			msgEmbed.setColor("fe_fe_fe");
			helpEmbed = msgEmbed;
		}

		std::unique_ptr<base_function> create() {
			return std::make_unique<test>();
		}

		virtual void execute(base_function_arguments& args) {
			channel channel = rtc_rest::channels::getCachedChannel({args.eventData.getChannelId()}).get();

			channel channel = rtc_rest::channels::getChannelAsync({args.eventData.getChannelId()}).get();
		}
	};
}
```
