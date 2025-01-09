Getting an Invite {#getting_an_invite}
============
- Execute the, `rtc_rest::guilds::getInviteAsync()` function, while passing in a value of type `rtc_rest::get_invite_data`, with a return value of type `auto` or `rtc_rest::invite_data`.
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
				get_guild_invites_data& dataPackage;
				dataPackage.guildId = args.eventData.getGuildId();

				auto responseVector = guilds::getGuildInvitesAsync(dataPackage).get();

				get_invite_data& dataPackage01;
				dataPackage01.withExpiration = true;
				dataPackage01.withCount = true;
				dataPackage01.inviteId = responseVector[0].code;

				auto responseData = guilds::getInviteAsync(dataPackage01).get();

				std::cout << "the code: " << responseData.code << std::endl;


			} catch (...) {
				rethrowException("test::execute()");
			}
		}
	};
}
```
