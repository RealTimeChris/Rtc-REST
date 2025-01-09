Creating an Interaction Response {#creating_an_input_event_response}
============ 
- Execute the, from the `rtc_rest::input_events::respondToInputEventAsync()` function, while passing in a data structure of type `rtc_rest::respond_to_input_event_data` with a type set to either `rtc_rest::input_event_response_type::Deferred_Response`, `rtc_rest::input_event_response_type::Ephemeral_Deferred_Response`, `rtc_rest::input_event_response_type::Interaction_Response`, `rtc_rest::input_event_response_type::Follow_Up_Message`, `rtc_rest::input_event_response_type::Ephemeral_Interaction_Response`, or `rtc_rest::input_event_response_type::Ephemeral_Follow_Up_Message`, with a return value of type `auto` or `rtc_rest::input_event_data`.

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

			respond_to_input_event_data& dataPackage {args.eventData};
			dataPackage.type = input_event_response_type::Deferred_Response;
			input_events::respondToInputEventAsync(dataPackage);

			respond_to_input_event_data& dataPackage01 {args.eventData};
			dataPackage01.type = input_event_response_type::Interaction_Response;
			dataPackage01.addContent("test response");
			input_events::respondToInputEventAsync(dataPackage01);

			respond_to_input_event_data& dataPackage02 {args.eventData};
			dataPackage02.type = input_event_response_type::Ephemeral_Interaction_Response;
			dataPackage02.addContent("test response");
			input_events::respondToInputEventAsync(dataPackage02);
		}
	};
}
```
