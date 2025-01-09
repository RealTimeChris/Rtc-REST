Deleting All Reactions {#deleting_all_reactions}
============
- Execute the, `rtc_rest::reactions::deleteAllReactionsAsync()` function, while passing in a data structure of type `rtc_rest::delete_all_reactions_data`, with a return value of type `void`.
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
				delete_all_reactions_data& dataPackage;
				dataPackage.messageId = args.eventData.getMessageId();
				dataPackage.channelId = args.eventData.getChannelId();

				reactions::deleteAllReactionsAsync(dataPackage).get();


			} catch (...) {
				rethrowException("test::execute()");
			}
		}
	};
}
```
