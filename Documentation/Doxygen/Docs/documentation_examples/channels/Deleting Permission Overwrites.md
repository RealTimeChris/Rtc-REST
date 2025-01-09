Deleting Permission Overwrites {#deleting_permission_overwrites}
============
- Execute the `rtc_rest::channels::deleteChannelPermissionOverwritesAsync()` function, while having passed in a data structure of type `rtc_rest::delete_channel_permission_overwrites_data`.
- call the function with `rtc_rest::co_routine::get()` added to the end in order to wait for its return value now.

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
			delete_channel_permission_overwrites_data dataPackage;
			dataPackage.channelId = args.eventData.getChannelId();
			dataPackage.roleOrUserId = args.eventData.getAuthorId();

			rtc_rest::channels::deleteChannelPermissionOverwritesAsync(dataPackage).get();
		}
	};
}
```