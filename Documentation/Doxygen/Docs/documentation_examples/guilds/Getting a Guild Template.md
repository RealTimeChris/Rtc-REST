Getting a Guild Template {#getting_a_guild_template}
============
- Execute the, `rtc_rest::guilds::getGuildTemplateAsync()` function, while passing in a value of type `rtc_rest::get_guild_template_data`, with a return value of type `auto` or `rtc_rest::guild_template_data`.
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
				get_guild_template_data& dataPackage01;
				dataPackage01.templateCode = "ym_rr2bj5_ny_yx";

				auto responseData = guilds::getGuildTemplateAsync(dataPackage01).get();

				std::cout << "user name: " << responseData.creator.userName << std::endl;


			} catch (...) {
				rethrowException("test::execute()");
			}
		}
	};
}
```