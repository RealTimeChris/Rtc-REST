Removing a User from a Thread {#removing_a_user_from_a_thread}
============
- Execute the, `rtc_rest::threads::removeThreadMemberAsync()` function, while passing in a value of type `rtc_rest::remove_thread_member_data`, with a return value of type `void`.
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
				remove_thread_member_data& dataPackage;
				dataPackage.channelId = "909407162776834069";
				dataPackage.userId = args.eventData.getAuthorId();

				threads::removeThreadMemberAsync(dataPackage).get();

			} catch (...) {
				rethrowException("test::execute()");
			}
		}
	};
}
```
