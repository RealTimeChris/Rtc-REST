Modifying a Guild Sticker {#modifying_a_guild_sticker}
============
- Execute the, `rtc_rest::stickers::modifyGuildStickerAsync()` function, while passing in a value of type `rtc_rest::modify_guild_sticker_data`, with a return value of type `auto` or `rtc_rest::sticker`.
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
				get_guild_stickers_data& dataPackage01;
				dataPackage01.guildId = args.eventData.getGuildId();
				auto resultVector = stickers::getGuildStickersAsync(dataPackage01).get();

				modify_guild_sticker_data& dataPackage;
				dataPackage.reason = "testing purposes!";
				dataPackage.description = "testing sticker";
				dataPackage.guildId = args.eventData.getGuildId();
				dataPackage.name = "test_sticker";
				dataPackage.tags = "testing";
				dataPackage.stickerId = resultVector[0].id;

				auto responseData = stickers::modifyGuildStickerAsync(dataPackage).get();

				std::cout << "the name: " << responseData.name << std::endl;


			} catch (...) {
				rethrowException("test::execute()");
			}
		}

		virtual ~test() = default;
	};
}
```
