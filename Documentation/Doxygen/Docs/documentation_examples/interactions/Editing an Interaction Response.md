Editing an Interaction Response {#editing_an_interaction_response}
============
- Execute the, `rtc_rest::input_events::respondToInputEventAsync()` function, while passing in a data structure of type `rtc_rest::respond_to_input_event_data` with a type set	to `rtc_rest::input_event_response_type::Edit_Interaction_Response`, with a return value of type `auto` or `rtc_rest::input_event_data`.

```cpp
/// Test.hpp -header for the "test" command.
/// https://github.com/RealTimeChris/Rtc-REST

#pragma once

#include "index.hpp"

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
			input_events::deleteInputEventResponseAsync(args.eventData).get();

			respond_to_input_event_data& dataPackage02 {args.eventData};
			dataPackage02.type = input_event_response_type::Ephemeral_Interaction_Response;
			dataPackage02.addContent("test response");
			auto inputEventData = input_events::respondToInputEventAsync(dataPackage02).get();

			respond_to_input_event_data& dataPackage03 {inputEventData};
			dataPackage03.type = input_event_response_type::Edit_Interaction_Response;
			dataPackage03.addContent("test response, edited!");
			input_events::respondToInputEventAsync(dataPackage03).get();
		}
	};
}
```
