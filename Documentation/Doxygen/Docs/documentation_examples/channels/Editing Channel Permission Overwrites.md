Edit Channel Permission Overwrites {#editing_permission_overwrites}
=============
- Execute the `rtc_rest::channels::editChannelPermissionOverwritesAsync()` function, while passing it a data structure of type `rtc_rest::edit_channel_permission_overwrites_data`.
- call the function with `rtc_rest::co_routine::get()` added to the end in order to wait for the results now.

```cpp
/// Test.hpp -header for the "test" command.
/// https://github.com/RealTimeChris/Rtc-REST

#pragma once

#include "index.hpp"

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
			edit_channel_permission_overwrites_data dataPackage;
			dataPackage.allow = permissions_converter::addPermissionsToString(
				dataPackage.allow, vector<permission> {permission::Add_Reactions, permission::Manage_Messages});
			dataPackage.deny = permissions_converter::removePermissionsFromString(
				dataPackage.deny, vector<permission> {permission::Attach_Files, permission::Embed_Links});
			dataPackage.channelId = args.eventData.getChannelId();
			dataPackage.type = permission_overwrites_type::user;
			dataPackage.roleOrUserId = args.eventData.getAuthorId();

			rtc_rest::channels::editChannelPermissionOverwritesAsync(dataPackage).get();
		}
	};
}
```