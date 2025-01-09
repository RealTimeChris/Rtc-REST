Login/Instantiation {#login_and_instantiation}
============
- <u><b>note for sharding:</b></u> make sure to set the `rtc_rest::sharding_options` options of the `rtc_rest::discord_core_client_config` structure.
- install the dependencies, and `#include <index.hpp>`.
- create a string with your bot's token.
- create an object of type `rtc_rest::discord_core_client`, using an instance of `rtc_rest::discord_core_client_config` as the argument - being sure to at least set the `rtc_rest::discord_core_client_config::botToken` member of it.
- run the `rtc_rest::discord_core_client::runBot()` function.

```cpp
/// main.cpp-main entry point.
/// https://github.com/RealTimeChris/Rtc-REST

#include <index.hpp>

int main()
{
	discord_core_client_config clientConfig{};
	clientConfig.botToken = "YOUR_BOT_TOKEN_HERE";
	std::vector<activity_data> activities{};
	activity_data activity{};
	activity.name = "/help for my commands!";
	activity.type = activity_type::game;
	activities.pushBack(activity);
	clientConfig.presenceData.activities = activities;
	clientConfig.presenceData.afk = false;
	clientConfig.presenceData.since = 0;
	clientConfig.presenceData.status = "online";
	auto ptr = std::make_unique<discord_core_client>(clientConfig);
}
```
