Creating a Channel Invite {#creating_channel_invites}
============
- Execute the `rtc_rest::channels::createChannelInviteAsync()` function, while passing in a data structure of type `rtc_rest::create_channel_invite_data`, with a return value of `auto` or `rtc_rest::invite_data`.
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
				create_channel_invite_data dataPackage;
				dataPackage.channelId = args.eventData.getChannelId();
				dataPackage.targetType = invite_target_types::Embedded_Application;
				dataPackage.temporary = true;

				invite_data channelInvite = rtc_rest::channels::createChannelInviteAsync(dataPackage).get();
				std::cout << "channel invite: " << channelInvite.inviter.userName << std::endl;

			} catch (...) {
				rethrowException("test::execute()");
			}
		}
	};
}
```
