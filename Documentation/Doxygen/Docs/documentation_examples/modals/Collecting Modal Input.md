Collecting Modal Input {#collecting_modal_input}
=============
- after creating a modal-text-input, create an object of the `rtc_rest::modal_collector` class, passing into its constructor the `rtc_rest::input_event_data` that resulted from the call to `rtc_rest::input_events::respondToInputEventAsync`, when the modal was created.- call the `rtc_rest::modal_collector::collectModalData` function, from the instance of modal_collector.
- collect a result of type `rtc_rest::modal_response_data` and deal with the modal response as you see fit!
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
				auto newEvent = input_events::respondToInputEventAsync(dataPackage).get();
				modal_collector modalCollector {newEvent};
				auto modalReturnData = modalCollector.collectModalData(120000).get();
				respond_to_input_event_data& dataPackage03 {modalReturnData};
				dataPackage03.addContent(modalReturnData.value);
				dataPackage03.type = input_event_response_type::Interaction_Response;
				input_events::respondToInputEventAsync(dataPackage03);


			} catch (...) {
				reportException("test::execute()");
			}
		}
		virtual ~test() = default;
	};
}
```
