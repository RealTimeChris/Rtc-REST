Creating a Guild From a Guild Template {#creating_a_guild_from_a_guild_template}
============
- execue the, `rtc_rest::guilds::createGuildFromGuildTemplateAsync()` function, while passing in a value of type `rtc_rest::create_guild_from_guild_template_data`, with a return value of type `auto` or `rtc_rest::guild_data`.
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
				create_guild_from_guild_template_data& dataPackage01;
				dataPackage01.templateCode = "ym_rr2bj5_ny_yx";
				dataPackage01.name = "test guild_data!";

				auto responseData = guilds::createGuildFromGuildTemplateAsync(dataPackage01).get();

				std::cout << "guild_data name: " << responseData.name << std::endl;


			} catch (...) {
				rethrowException("test::execute()");
			}
		}
	};
}
```
