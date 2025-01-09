Bulk Overwriting Guild Application Commands {#bulk_overwrite_guild_commands}
============
- Execute the `rtc_rest::application_commands::bulkOverwriteGuildApplicationCommandsAsync()` function and execute it, while passing in a data structure of type `rtc_rest::bulk_overwrite_guild_application_commands_data`, with a return value of `auto` or `std::vector<rtc_rest::application_command_data>`.
- call the function with `rtc_rest::co_routine::get()` added to the end in order to wait for the results now.

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
			vector<create_guild_application_command_data> newVector;
			create_guild_application_command_data createSellDrugsCommandData;
			createSellDrugsCommandData.description = "sell drugs in exchange for some currency!";
			createSellDrugsCommandData.name = "selldrugs";
			createSellDrugsCommandData.type = application_command_type::Chat_Input;
			newVector.emplace_back(createSellDrugsCommandData);

			create_guild_application_command_data registerSlashCommandsCommandData;
			registerSlashCommandsCommandData.description = "register the programmatically designated slash commands.";
			registerSlashCommandsCommandData.name = "registerslashcommands";
			registerSlashCommandsCommandData.type = application_command_type::Chat_Input;
			newVector.emplace_back(registerSlashCommandsCommandData);

			bulk_overwrite_guild_application_commands_data& dataPackage;
			dataPackage.responseData = newVector;
			dataPackage.guildId = args.eventData.getGuildId();

			auto returnValue = application_commands::bulkOverwriteGuildApplicationCommandsAsync(dataPackage).get();

			for (const auto& value: returnValue) {
				std::cout << "command name: " << value.name << std::endl;
			}
		}
	};
}
```