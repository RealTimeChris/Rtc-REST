Creating a Direct-Message {#creating_a_dm}
============
- Execute the, `rtc_rest::channels::createDMChannelAsync()` function, while passing in a data structure of type `rtc_rest::create_dmchannel_data`, with a return value of type `auto` or `rtc_rest::channel_data`.
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
				create_dmchannel_data dataPackage;
				dataPackage.userId = args.eventData.getAuthorId();

				channel dmChannel = rtc_rest::channels::createDMChannelAsync(dataPackage).get();


			} catch (...) {
				rethrowException("test::execute()");
			}
		}

		virtual ~test() = default;
	};
}
```
