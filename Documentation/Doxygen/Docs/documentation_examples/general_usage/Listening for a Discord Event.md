Listening for a Discord Event {#listening_for_an_event}
============
- a complete list of possible events is [here](https://discord.com/developers/docs/topics/gateway#commands-and-events-gateway-events).
- create a function that returns `rtc_rest::co_routine<void>`, that takes an argument of a type that derives its name from the type of event that is being listened for, where the argument is of type `rtc_rest::eventname+data`. for example, the argument for the `rtc_rest::event_manager::onGuildCreation` event is `rtc_rest::on_guild_creation_data`.
- the list of events can be found by dereferencing `rtc_rest::discord_core_client::eventManager`, where you pass into the function found here, a pointer to your own created function.

```cpp
/// main.cpp-main entry point.
/// https://github.com/RealTimeChris/Rtc-REST

#include <index.hpp>

co_routine<void> onGuildCreation(on_guild_creation_data dataPackage) {
	co_await newThreadAwaitable<void>();
	std::cout << "guild_data name: " << dataPackage.guild_data.data.name << std::endl;
	co_return;
}

int32_t main() {
	std::string botToken {"YOUR_BOT_TOKEN_HERE"};
	auto ptr = std::make_unique<discord_core_client>(botToken, "!");
	ptr->eventManager->onGuildCreation(&onGuildCreation);
	ptr->runBot();
	return 0;
}
```
