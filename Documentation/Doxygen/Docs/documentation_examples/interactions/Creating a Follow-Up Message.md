Creating a Follow-Up Message {#creating_a_follow_up_message}
============
- Execute the, `rtc_rest::input_events::respondToInputEventAsync()` function, while passing in a data structure of type `rtc_rest::respond_to_input_event_data` with a type set	to `rtc_rest::input_event_response_type::Follow_Up_Message`, with a return value of type `auto` or `rtc_rest::std::unique_ptr<input_event_data>`.

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
				respond_to_input_event_data& dataPackage {args.eventData};
				dataPackage.type = input_event_response_type::Follow_Up_Message;
				dataPackage.addContent("this is a test response message!");
				auto responseData = input_events::respondToInputEventAsync(dataPackage);

			} catch (...) {
				rethrowException("test::execute() error: ");
			}
		}
	};
}
```
