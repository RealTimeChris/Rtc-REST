Getting a WebHook Message using a Discord Bot Library C++ {#getting_a_web_hook_message}
============
- Execute the, `rtc_rest::web_hooks::getWebHookMessageAsync()` function, while passing in a value of type `rtc_rest::get_web_hook_data`, with a return value of type `auto` or `rtc_rest::message_data`.
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

				execute_web_hook_data& dataPackage01 {newWebHooks[0]};
				dataPackage01.addContent("test content");
				dataPackage01.wait = true;

				auto newMessage = rtc_rest::web_hooks::executeWebHookAsync(dataPackage01).get();

				get_web_hook_data& dataPackage02;
				dataPackage02.webHookId = newWebHooks[0].id;
				dataPackage02.webhookToken = newWebHooks[0].token;
				dataPackage02.messageId = newMessage.id;

				auto newMessage02 = rtc_rest::web_hooks::getWebHookMessageAsync(dataPackage02).get();

				std::cout << "message id: " << newMessage02.id << std::endl;


			} catch (...) {
				rethrowException("test::execute()");
			}
		}

		virtual ~test() = default;
	};
}
```
