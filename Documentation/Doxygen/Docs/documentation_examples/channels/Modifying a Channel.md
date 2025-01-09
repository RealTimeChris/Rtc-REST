Modifying a Channel {#modifying_channel}
============
- Execute the `rtc_rest::channels::modifyChannelAsync()` function, while passing to it a completed data structure `rtc_rest::modify_channel_data`, with a return value of `auto` or `rtc_rest::channel_data`.
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
				channel channel = rtc_rest::channels::getCachedChannel({.channelId = args.eventData.getChannelId()}).get();
				modify_channel_data dataPackage {channel};
				dataPackage.channelData.name = "test update";
				channel channelNew = rtc_rest::channels::modifyChannelAsync(dataPackage).get();

			} catch (...) {
				rethrowException("test::execute() error: ");
			}
		}
	};
}
```
