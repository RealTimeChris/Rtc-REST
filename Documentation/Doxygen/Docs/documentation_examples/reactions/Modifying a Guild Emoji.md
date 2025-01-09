Modifying a Guild Emoji {#modifying_a_guild_emoji}
============
- Execute the, `rtc_rest::reactions::modifyGuildEmojiAsync()` function, while passing in a value of type `rtc_rest::modify_guild_emoji_data`, with a return value of type `auto` or `rtc_rest::emoji_data`.
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
				get_emoji_list_data& dataPackage00 { };
				dataPackage00.guildId = args.eventData.getGuildId();

				auto emojiList = reactions::getEmojiListAsync(dataPackage00).get();

				modify_guild_emoji_data& dataPackage;
				dataPackage.guildId = args.eventData.getGuildId();
				dataPackage.emojiId = emojiList[0].id;
				dataPackage.reason = "testing purposes";
				dataPackage.name = "testemoji";
				auto emoji = reactions::modifyGuildEmojiAsync(dataPackage).get();

				std::cout << "the name: " << emoji.name << std::endl;


			} catch (...) {
				rethrowException("test::execute()");
			}
		}
	};
}
```
