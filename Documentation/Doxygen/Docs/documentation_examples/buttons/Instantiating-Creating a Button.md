Instantiating/Creating a Button {#creating_a_button}
============ 
- create a data structure of type `rtc_rest::respond_to_input_event_data`, and add either some content or a message embed.- add one or more buttons by using the `rtc_rest::respond_to_input_event_data::addButton` of the `rtc_rest::respond_to_input_event_data` structure.
- call the `rtc_rest::input_events::respondToInputEventAsync()` function, passing it the `rtc_rest::respond_to_input_event_data` structure.
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
			dataPackage.addButton(false, "test_button", "test button", "✅", button_style::danger);
			dataPackage.addContent("test response");
			dataPackage.addMessageEmbed(embed_data {.description = "testing!", .title = "test title"});
			dataPackage.type = input_event_response_type::Interaction_Response;
			auto inputEventData = input_events::respondToInputEventAsync(dataPackage);
		}
	};
}
```
