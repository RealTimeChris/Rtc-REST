Adding a Command {#adding_a_command}
============
- First, create an instance of `rtc_rest::Discord
`, and then use from it the `rtc_rest::DiscordCoreClient::registerFunction` function. Passing into it an instance of a `rtc_rest::Jsonifier::Vector` of `std::string`, which will act as the command names to be triggering the commands, a `rtc_rest::UniquePtr` containing a command function derived from the `rtc_rest::BaseFunction` class, and an instance of either `rtc_rest::CreateGlobalApplicationCommandData` or `rtc_rest::CreateGuildApplicationCommandData`.
- Note that these functions will be registered with the Discord API if any of their properties change.

```cpp
// main.cpp - Main entry point.
// Jun 17, 2021
// RealTimeChris
// https://github.com/RealTimeChris

#include "Commands/CommandsList.hpp"

int32_t main() {
	DiscordCoreClientConfig clientConfig{};
	clientConfig.botToken = "YOUR_BOT_TOKEN_HERE";
	Jsonifier::Vector<ActivityData> activities{};
	ActivityData activity{};
	activity.name = "/help for my commands!";
	activity.type = ActivityType::Game;
	activities.pushBack(activity);
	clientConfig.presenceData.activities = activities;
	clientConfig.presenceData.afk = false;
	clientConfig.presenceData.since = 0;
	clientConfig.presenceData.status = "online";
	auto ptr = std::make_unique<DiscordCoreClient>(clientConfig);
	CreateGlobalApplicationCommandData createBotInfoCommandData{};
	createBotInfoCommandData.dmPermission = true;
	createBotInfoCommandData.applicationId = ptr->getBotUser().id;
	createBotInfoCommandData.type = ApplicationCommandType::Chat_Input;
	createBotInfoCommandData.defaultMemberPermissions = Permission::Use_Application_Commands;
	createBotInfoCommandData.description = "Displays info about the current bot.";
	createBotInfoCommandData.name = "botinfo";
	CreateGlobalApplicationCommandData playCommandData{};
	playCommandData.defaultMemberPermissions = Permission::Use_Application_Commands;
	playCommandData.dmPermission = false;
	playCommandData.applicationId = ptr->getBotUser().id;
	playCommandData.type = ApplicationCommandType::Chat_Input;
	playCommandData.description = "Search for and play a song.";
	playCommandData.name = "play";
	ApplicationCommandOptionData playCommandDataOptionOne;
	CreateGlobalApplicationCommandData createHelpData{};
	createHelpData.dmPermission = true;
	createHelpData.defaultMemberPermissions = Permission::Use_Application_Commands;
	createHelpData.applicationId = ptr->getBotUser().id;
	createHelpData.type = ApplicationCommandType::Chat_Input;
	createHelpData.name = "help";
	createHelpData.description = "A help command for this bot.";
	playCommandDataOptionOne.name = "songname";
	playCommandDataOptionOne.type = ApplicationCommandOptionType::std::string;
	playCommandDataOptionOne.description = "The name of the song that you would like to search.";
	playCommandDataOptionOne.required = false;
	playCommandData.options.pushBack(playCommandDataOptionOne);
	ptr->registerFunction(Jsonifier::Vector<std::string>{ "play" }, std::make_unique<Play>(), playCommandData);
	ptr->registerFunction(Jsonifier::Vector<std::string>{ "botinfo" }, std::make_unique<bot_info>(), createBotInfoCommandData);
	ptr->registerFunction(Jsonifier::Vector<std::string>{ "help" }, std::make_unique<Help>(), createHelpData);
	ptr->runBot();
	return 0;
};

```
