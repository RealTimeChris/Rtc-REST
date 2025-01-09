Deleting a Guild Emoji {#deleting_a_guild_emoji}
============
- Execute the, `rtc_rest::reactions::deleteGuildEmojiAsync()` function, while passing in a value of type `rtc_rest::delete_guild_emoji_data`, with a return value of type `void`.
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

				delete_guild_emoji_data& dataPackage;
				dataPackage.guildId = args.eventData.getGuildId();
				dataPackage.emojiId = emojiList[0].id;
				dataPackage.reason = "reason for deleting!";
				reactions::deleteGuildEmojiAsync(dataPackage).get();


			} catch (...) {
				rethrowException("test::execute()");
			}
		}
	};
}
```
