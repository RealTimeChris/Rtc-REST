Getting a Role {#getting_a_role}
=============
- Execute the, `rtc_rest::roles::getCachedRole()` (which collects it from the cache), or `rtc_rest::roles::getRoleAsync()` (which collects it from the discord servers) function, while passing in a value of type `rtc_rest::get_role_data`.
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
			get_role_data& dataPackage;
			dataPackage.guildId = args.eventData.getGuildId();
			dataPackage.roleId = "YOUR_ROLE_ID_HERE";

			role role = roles::getRoleAsync(dataPackage).get();

			role role = roles::getCachedRole(dataPackage).get();
		}
	};
}
```
