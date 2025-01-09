Beginner's Notes {#beginner_notes}
============
- <b><u>Important:</u></b> You will need to enable the following settings in your bot's control panel: enter your "developer control panel" and select a bot, then go to the "bot" section-in here, enable "presence intent", "server members intent", "message content intent" - and hit "save".   
<br>
- All of the central class managers are classes that are filled with static methods that are a part of the `rtc_rest` namespace, they are as follows:   

    `rtc_rest::application_commands`

    `rtc_rest::auto_moderation_rules`
  
    `rtc_rest::channels`
    
    `rtc_rest::event_manager`
    
    `rtc_rest::guilds`
    
    `rtc_rest::guild_members`
    
    `rtc_rest::guild_scheduled_events`
    
    `rtc_rest::input_events`
    
    `rtc_rest::interactions`
    
    `rtc_rest::messages`
    
    `rtc_rest::reactions`
    
    `rtc_rest::roles`
    
    `rtc_rest::stage_instances`
    
    `rtc_rest::stickers`
    
    `rtc_rest::threads`
    
    `rtc_rest::users`
    
    `rtc_rest::web_hooks`   

<br>
- Each of the functions in this library takes an argument that is of a type who's name is derived from the function-for example, the data structure that gets passed into `rtc_rest::guilds::getGuildAsync()` is `rtc_rest::get_guild_data`.   
<br>
- Most of the functions in this library are `rtc_rest::async`, meaning that they return a value of type `rtc_rest::co_routine<rtc_rest::co_routine::return_type>`, which you call `rtc_rest::co_routine::get()` on to retrieve the returned value, and to suspend the current thread in the process of waiting for it. alternatively, you have the option of leaving the task's completion up to the background thread that is handling it, by not calling `rtc_rest::co_routine::get()`.
