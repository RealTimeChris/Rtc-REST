Modifying Guild Channel Positions {#modifying_guild_channel_positions}
=============
- Execute the, `rtc_rest::channels::modifyGuildChannelPositionsAsync()` function, while passing in a data structure of type `rtc_rest::modify_guild_channel_positions_data`, with a return value of type `void`.
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
				vector<rtc_rest::channel_data> channels = rtc_rest::channels::getGuildChannelsAsync({.guildId = args.eventData.getGuildId()}).get();

				vector<modify_guild_channel_position_data> dataPackage00;

				modify_guild_channel_position_data& dataPackage01;
				dataPackage01.id = channels.at(1).id;
				dataPackage01.lockPermissions = false;
				dataPackage01.parentId = channels.at(1).parentId;
				dataPackage01.position = channels.at(1).position-1;

				dataPackage00.emplace_back(dataPackage01);

				modify_guild_channel_positions_data dataPackage;
				dataPackage.reason = "testing purposes!";
				dataPackage.guildId = args.eventData.getGuildId();
				dataPackage.modifyChannelData = dataPackage00;

				rtc_rest::channels::modifyGuildChannelPositionsAsync(dataPackage).get();


			} catch (...) {
				rethrowException("test::execute()");
			}
		}
	};
}
```
