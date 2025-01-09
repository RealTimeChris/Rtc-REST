Getting a Stage Instance {#getting_a_stage_instance}
============
- Execute the, `rtc_rest::stage_instances::getStageInstanceAsync()` function, while passing in a value of type `rtc_rest::get_stage_instance_data`, with a return value of type `auto` or `rtc_rest::stage_instance`.
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
				get_stage_instance_data& dataPackage;
				dataPackage.channelId = "914726178022101052";

				auto responseData = stage_instances::getStageInstanceAsync(dataPackage).get();

				std::cout << "the topic: " << responseData.topic << std::endl;


			} catch (...) {
				rethrowException("test::execute()");
			}
		}
	};
}
```