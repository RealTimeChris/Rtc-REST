Getting an Interaction Response {#getting_an_interaction_response}
============
- Execute the, `rtc_rest::interactions::getInteractionResponseAsync()` function, while passing in a data structure of type `rtc_rest::get_interaction_response_data`, with a return type of `rtc_rest::message_data`.

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
				get_interaction_response_data& dataPackage;
				dataPackage.applicationId = getBotUser().id;
				dataPackage.interactionToken = args.eventData.getInteractionToken();

				auto interactionResponse = interactions::getInteractionResponseAsync(dataPackage).get();


			} catch (...) {
				rethrowException("test::execute() error: ");
			}
		}
	};
}
```
