Getting a Thread Member {#getting_a_thread_member}
============
- Execute the, `rtc_rest::threads::getThreadMemberAsync()` function, while passing in a value of type `rtc_rest::get_thread_member_data`, with a return value of type `auto` or `rtc_rest::thread_member_data`.
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
				get_thread_member_data& dataPackage;
				dataPackage.channelId = "909435143561809953";
				dataPackage.userId = args.eventData.getAuthorId();

				auto newThreadMember = threads::getThreadMemberAsync(dataPackage).get();


			} catch (...) {
				rethrowException("test::execute()");
			}
		}
	};
}
```
