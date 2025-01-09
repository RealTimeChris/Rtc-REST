Getting Channel Invites {#getting_channel_invites}
============
- Execute the `rtc_rest::channels::getChannelInvitesAsync()` function, while passing in a data structure of type `rtc_rest::get_channel_invites_data`, with a return value of `auto` or `std::vector<rtc_rest::invite_data>`.
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
				get_channel_invites_data dataPackage;
				dataPackage.channelId = args.eventData.getChannelId();

				vector<rtc_rest::invite_data> channelInvites = rtc_rest::channels::getChannelInvitesAsync(dataPackage).get();

				std::cout << "the invites: " << std::endl;
				for (const auto& value: channelInvites) {
					std::cout << value.inviter.userName << std::endl;
				}


			} catch (...) {
				rethrowException("test::execute()");
			}
		}
	};
}
```
