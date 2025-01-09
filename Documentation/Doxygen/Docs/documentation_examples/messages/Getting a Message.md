Getting a Message {#getting_a_message}
============
- Execute the, `rtc_rest::messages::getMessageAsync()` function, while passing in a data structure of type `rtc_rest::get_message_data`, with a return value of type `auto`, or, `rtc_rest::message_data`.
- call the function with `rtc_rest::co_routine::get()` added to the end in order to wait for its return value now.

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
				get_message_data& dataPackage;
				dataPackage.channelId = args.eventData.getChannelId();
				dataPackage.id = args.eventData.getMessageId();

				message responseMessages = messages::getMessageAsync(dataPackage).get();


			} catch (...) {
				rethrowException("test::execute()");
			}
		}
	};
}
```
