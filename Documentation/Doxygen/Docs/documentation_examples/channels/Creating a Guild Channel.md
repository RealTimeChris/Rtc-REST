Creating a Guild Channel {#creating_guild_channel}
============
- Execute the, `rtc_rest::channels::createGuildChannelAsync()` function, while passing in a data structure of type `rtc_rest::create_guild_channel_data`, with a return value of type `auto` or `rtc_rest::channel_data`.
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
				create_guild_channel_data dataPackage;
				dataPackage.type = channel_type::Guild_Text;
				dataPackage.name = "test channel";
				dataPackage.guildId = args.eventData.getGuildId();
				dataPackage.reason = "testing purposes!";

				vector<rtc_rest::channel_data> channels = rtc_rest::channels::getGuildChannelsAsync({.guildId = args.eventData.getGuildId()}).get();

				for (const auto& value: channels) {
					if (value.type == channel_type::GUILD_CATEGORY) {
						dataPackage.parentId = value.id;
						break;
					}
				}

				channel channel = rtc_rest::channels::createGuildChannelAsync(dataPackage).get();

				std::cout << "the name: " << channel.name << std::endl;


			} catch (...) {
				rethrowException("test::execute()");
			}
		}
	};
}
```
