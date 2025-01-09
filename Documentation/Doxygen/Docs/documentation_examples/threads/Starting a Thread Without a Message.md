Starting a Thread without a Message {#starting_a_thread_without_a_message}
============
- Execute the, from the `rtc_rest::threads::startThreadWithoutMessageAsync()` function, while passing in a value of type `rtc_rest::start_thread_without_message_data`, with a return value of type `auto` or `rtc_rest::thread`.
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
				start_thread_without_message_data& dataPackage;
				dataPackage.invitable = true;
				dataPackage.type = thread_type::GUILD_PUBLIC_THREAD;
				dataPackage.reason = "testing reasons!";
				dataPackage.autoArchiveDuration = thread_auto_archive_duration::short;
				dataPackage.channelId = args.eventData.getChannelId();
				dataPackage.threadName = "new thread";

				channel newThread = threads::startThreadWithoutMessageAsync(dataPackage).get();


			} catch (...) {
				rethrowException("test::execute()");
			}
		}
	};
}
```
