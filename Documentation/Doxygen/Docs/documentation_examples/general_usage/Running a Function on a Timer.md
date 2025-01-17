Running a Function on a Timer {#running_a_function_on_a_timer}
============
- create a function who's argument is of type `rtc_rest::discord_core_client*`, with a return type of `void`.
- create a data value of type `std::vector<rtc_rest::repeated_function_data>`.
- create a data value of type `rtc_rest::repeated_function_data`, and fill out its members, and stuff it inside the vector. be sure to set `rtc_rest::repeated` to true if you would like the function call to be recurring.
- pass this vector as a member that is a part of the `rtc_rest::discord_core_client_config` structure as an argument to the `rtc_rest::discord_core_client` constructor - now the functions it contains will run on a schedule defined by the `rtc_rest::intervalInMs` member of the data structure.
- do this with as many functions as you would like!

```cpp
// main.cpp - main entry point.
// jun 17, 2021
// real_time_chris
// https://github.com/RealTimeChris

#include "commands/commands_list.hpp"

void onBoot00(discord_core_client* args) {
	auto botUser = args->getBotUser();
	database_manager_agent::initialize(botUser.id);
	discord_user user{ botUser.userName, botUser.id };
}

int32_t main() {
	std::vector<rtc_rest::repeated_function_data> functionVector{};
	repeated_function_data function01{};
	function01.function = &onBoot00;
	function01.intervalInMs = 150;
	function01.repeated = false;
	functionVector.pushBack(function01);
	discord_core_client_config clientConfig{};
	clientConfig.functionsToExecute = functionVector;
	auto ptr = std::make_unique<discord_core_client>(clientConfig);
	ptr->runBot();
	return 0;
}

```
