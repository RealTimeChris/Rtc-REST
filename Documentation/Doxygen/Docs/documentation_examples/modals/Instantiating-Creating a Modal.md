Instantiating/Creating a Modal-Input {#creating_a_modal}
============
- create a data structure of type `rtc_rest::respond_to_input_event_data`.
- add a modal-text-input using the `rtc_rest::respond_to_input_event_data::addModal` function of the `rtc_rest::respond_to_input_event_data` structure.
- call the `rtc_rest::input_events::respondToInputEventAsync` function, passing it the `rtc_rest::respond_to_input_event_data` structure.
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
			try {
				respond_to_input_event_data& dataPackage {args.eventData};
				dataPackage.addModal("test modal", "test_modal", "test modal small", "test_modal", true, 1, 46, text_input_style::paragraph, "test modal",
									 "test_modal");
				dataPackage.type = input_event_response_type::Interaction_Response;
				auto newEvent = input_events::respondToInputEventAsync(dataPackage);


			} catch (...) {
				reportException("test::execute()");
			}
		}
		virtual ~test() = default;
	};
}
```