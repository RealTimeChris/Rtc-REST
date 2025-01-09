Getting a User {#getting_a_user}
============
- Execute the, `rtc_rest::users::getCachedUser()` (which collects it from the cache), or `rtc_rest::users::getUserAsync()` (which collects it from the discord servers) function, while passing in a value of type `rtc_rest::get_user_data`, with a return value of `auto` or `rtc_rest::user`.
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
				get_user_data& dataPackage;
				dataPackage.userId = args.eventData.getAuthorId();

				auto currentUser = users::getUserAsync(dataPackage).get();

				std::cout << currentUser.userName << std::endl;


			} catch (...) {
				rethrowException("test::execute()");
			}
		}

		virtual ~test() = default;
	};
}


```
