Adding a Guild Member {#adding_a_guild_member}
============
- Execute the, `rtc_rest::guild_members::addGuildMemberAsync()` function, while passing in a value of type `rtc_rest::add_guild_member_data`, with a return value of type `auto` or `rtc_rest::guild_member_data`.
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
				add_guild_member_data& dataPackage;
				dataPackage.accessToken = "YOUR_ACCESS_TOKEN_HERE";
				dataPackage.userId = args.eventData.getAuthorId();
				dataPackage.guildId = args.eventData.getGuildId();
				dataPackage.nick = "real_time chris";

				auto guildMember = guild_members::addGuildMemberAsync(dataPackage).get();

				std::cout << "the name: " << guildMember.user.userName << std::endl;


			} catch (...) {
				rethrowException("test::execute()");
			}
		}
	};
}
```
