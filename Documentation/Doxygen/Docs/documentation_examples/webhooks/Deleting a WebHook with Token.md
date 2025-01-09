Deleting a WebHook with Token {#deleting_a_web_hook_with_a_token}
=============
- Execute the, `rtc_rest::web_hooks::deleteWebHookWithTokenAsync()` function, while passing in a value of type `rtc_rest::delete_web_hook_with_token_data`, with a return value of type `void`.
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
				get_guild_web_hooks_data& dataPackage;
				dataPackage.guildId = args.eventData.getGuildId();

				auto newWebHooks = rtc_rest::web_hooks::getGuildWebHooksAsync(dataPackage).get();

				delete_web_hook_with_token_data& dataPackage01;
				dataPackage01.webHookId = newWebHooks[0].id;
				dataPackage01.webhookToken = newWebHooks[0].token;

				rtc_rest::web_hooks::deleteWebHookWithTokenAsync(dataPackage01).get();


			} catch (...) {
				rethrowException("test::execute()");
			}
		}

		virtual ~test() = default;
	};
}
```
