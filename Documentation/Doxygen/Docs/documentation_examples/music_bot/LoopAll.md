Loop All {#Loop-All}
============
```cpp

#pragma once

#include "../HelperFunctions.hpp"

namespace rtc_rest {
	class loop_all : public base_function {
	  public:
		loop_all() {
			commandName = "loopall";
			helpDescription = "enables or disables looping of the current queue.";
			embed_data msgEmbed;
			msgEmbed.setDescription("------\nSimply enter /loopall!\n------");
			msgEmbed.setTitle("__**loop-all usage:**__");
			msgEmbed.setTimeStamp(getTimeAndDate());
			msgEmbed.setColor("fe_fe_fe");
			helpEmbed = msgEmbed;
		}

		std::unique_ptr<base_function> create() {
			return std::make_unique<loop_all>();
		}

		void execute(base_function_arguments& newArgs) {
			try {
				channel channel = rtc_rest::channels::getCachedChannel({ .channelId = newArgs.eventData.getChannelId() }).get();

				guild_data guild_data = guilds::getCachedGuild({ .guildId = newArgs.eventData.getGuildId() }).get();

				discord_guild discordGuild(guild_data);

				bool areWeAllowed = checkIfAllowedPlayingInChannel(newArgs.eventData, discordGuild);

				if (!areWeAllowed) {
					return;
				}

				guild_member_data guildMember =
					guild_members::getCachedGuildMember({ .guildMemberId = newArgs.eventData.getAuthorId(), .guildId = newArgs.eventData.getGuildId() }).get();

				bool doWeHaveControl = checkIfWeHaveControl(newArgs.eventData, discordGuild, guildMember);

				if (!doWeHaveControl) {
					return;
				}

				if (song_api::isLoopAllEnabled(guild_data.id)) {
					song_api::setLoopAllStatus(false, guild_data.id);
				} else {
					song_api::setLoopAllStatus(true, guild_data.id);
				}
				embed_data msgEmbed;
				msgEmbed.setAuthor(newArgs.eventData.getUserName(), newArgs.eventData.getAvatarUrl());
				msgEmbed.setColor(discordGuild.data.borderColor);
				if (song_api::isLoopAllEnabled(guild_data.id)) {
					msgEmbed.setDescription("\n------\n__**looping-all has been enabled!**__\n------\n");
				} else {
					msgEmbed.setDescription("\n------\n__**looping-all has been disabled!**__\n------\n");
				}
				savePlaylist(discordGuild);
				msgEmbed.setTimeStamp(getTimeAndDate());
				msgEmbed.setTitle("__**looping-all change:**__");
				respond_to_input_event_data& dataPackage(newArgs.eventData);
				dataPackage.setResponseType(input_event_response_type::Ephemeral_Interaction_Response);
				dataPackage.addMessageEmbed(msgEmbed);
				auto newEvent = input_events::respondToInputEventAsync(dataPackage).get();

				return;
			} catch (...) {
				reportException("loop_all::execute()");
			}
		}
		~loop_all(){};
	};

}
```