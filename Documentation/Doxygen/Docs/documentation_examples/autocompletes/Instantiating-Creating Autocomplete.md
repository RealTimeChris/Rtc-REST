Instantiating/Creating an Autocomplete {#creating_an_auto_complete}
============ 
- create a data structure of type `rtc_rest::create_global_application_command_data` or `rtc_rest::create_guild_application_command_data`, and be sure to set one or more of the `rtc_rest::application_command_option_data`'s settings for `rtc_rest::autocomplete` to true.
- use this data structure to create the application command.

```cpp
// Test.hpp- header for the "test" command.
// https://github.com/RealTimeChris

#pragma once

#include "HelperFunctions.hpp"

namespace rtc_rest {

	class test : public base_function {
	  public:
		test() {
			commandName = "test";
			helpDescription = "testing purposes!";
			embed_data msgEmbed{};
			msgEmbed.setDescription("------\nSimply enter /test!\n------");
			msgEmbed.setTitle("__**test usage:**__");
			msgEmbed.setTimeStamp(getTimeAndDate());
			msgEmbed.setColor("fe_fe_fe");
			helpEmbed = msgEmbed;
		}

		std::unique_ptr<base_function> create() {
			return std::make_unique<test>();
		}

		virtual void execute(base_function_arguments& newArgs) {
			try {
				
				create_global_application_command_data createTestData;
				createTestData.dmPermission = true;
				createTestData.applicationId = discord_core_client::getInstance()->getBotUser().id;
				createTestData.type = application_command_type::Chat_Input;
				createTestData.name = "test";
				createTestData.description = "test command.";
				application_command_option_data testOptionOne;
				testOptionOne.type = application_command_option_type::attachment;
				testOptionOne.name = "attachment";
				testOptionOne.required = false;
				testOptionOne.description = "test attachment!";
				createTestData.options.emplace_back(testOptionOne);
				application_command_option_data testOptionTwo;
				testOptionTwo.type = application_command_option_type::std::string;
				testOptionTwo.name = "test_string";
				testOptionTwo.required = false;
				testOptionTwo.autocomplete = true;
				testOptionTwo.description = "test string!";
				createTestData.options.emplace_back(testOptionTwo);
				application_commands::createGlobalApplicationCommandAsync(createTestData).get();

			} catch (...) {
				reportException("test::execute()");
			}
		}
		virtual ~test(){};
	};
}
```
