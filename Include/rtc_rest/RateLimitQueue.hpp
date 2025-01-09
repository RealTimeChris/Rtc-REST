/*
	MIT License

	Rtc-REST, A bot library for Discord, written in C++, and featuring explicit multithreading through the usage of custom, asynchronous C++ CoRoutines.

	Copyright 2022, 2023 Chris M. (RealTimeChris)

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/
/// RateLimitQueue.hpp - Header file for the "rate_limit_queue stuff".
/// May 12, 2021
/// https://rtc_rest.com
/// \file RateLimitQueue.hpp
#pragma once

#include <rtc_rest/TCPConnection.hpp>
#include <memory>
#include <mutex>

namespace rtc_rest {

	struct rate_limit_data {
		friend class https_connection_stack_holder;
		friend class https_connection_manager;
		friend class rate_limit_stack_holder;
		friend class https_rnr_builder;
		friend class rate_limit_queue;
		friend class https_client;

	  protected:
		std::unique_lock<std::mutex> lock{ accessMutex, std::defer_lock };
		std::atomic<milliseconds> sampledTimeInMs{ milliseconds{} };
		std::atomic<seconds> sRemain{ seconds{} };
		std::atomic_int64_t getsRemaining{ 1 };
		std::atomic_bool areWeASpecialBucket{};
		std::atomic_bool didWeHitRateLimit{};
		std::atomic_bool doWeWait{};
		std::string bucket{};
		std::mutex accessMutex{};
	};

	enum class https_workload_class : uint8_t { Get = 0, Put = 1, Post = 2, Patch = 3, Delete = 4 };

	enum class payload_type : uint8_t { Application_Json = 1, Multipart_Form = 2 };

	enum class https_workload_type : uint8_t {
		Unset											= 0,
		Get_Global_Application_Commands					= 1,
		Post_Global_Application_Command					= 2,
		Get_Global_Application_Command					= 3,
		Patch_Global_Application_Command				= 4,
		Delete_Global_Application_Command				= 5,
		Bulk_Put_Global_Application_Commands			= 6,
		Get_Guild_Application_Commands					= 7,
		Post_Guild_Application_Command					= 8,
		Get_Guild_Application_Command					= 9,
		Patch_Guild_Application_Command					= 10,
		Delete_Guild_Application_Command				= 11,
		Bulk_Put_Guild_Application_Commands				= 12,
		Get_Guild_Application_Commands_Permissions		= 13,
		Get_Guild_Application_Command_Permissions		= 14,
		Put_Guild_Application_Command_Permissions		= 15,
		Batch_Put_Guild_Application_Command_Permissions = 16,
		Post_Interaction_Response						= 17,
		Get_Interaction_Response						= 18,
		Patch_Interaction_Response						= 19,
		Delete_Interaction_Response						= 20,
		Post_Followup_Message							= 21,
		Get_Followup_Message							= 22,
		Patch_Followup_Message							= 23,
		Delete_Followup_Message							= 24,
		Get_Guild_Audit_Logs							= 25,
		Get_Channel										= 26,
		Patch_Channel									= 27,
		Delete_Channel									= 28,
		Get_Messages									= 29,
		Get_Message										= 30,
		Post_Message									= 31,
		Crosspost_Message								= 32,
		Put_Reaction									= 33,
		Delete_Own_Reaction								= 34,
		Delete_User_Reaction							= 35,
		Get_Reactions									= 36,
		Delete_All_Reactions							= 37,
		Delete_Reactions_By_Emoji						= 38,
		Patch_Message									= 39,
		Delete_Message									= 40,
		Delete_Message_Old								= 41,
		Bulk_Delete_Messages							= 42,
		Put_Channel_Permission_Overwrites				= 43,
		Get_Channel_Invites								= 44,
		Post_Channel_Invite								= 45,
		Delete_Channel_Permission_Overwrites			= 46,
		Post_Follow_News_Channel						= 47,
		Post_Trigger_Typing_Indicator					= 48,
		Get_Pinned_Messages								= 49,
		Put_Pin_Message									= 50,
		Delete_Pin_Message								= 51,
		Put_Recipient_To_Group_Dm						= 52,
		Delete_Recipient_From_Group_Dm					= 53,
		Post_Thread_With_Message						= 54,
		Post_Thread_Without_Message						= 55,
		Put_Self_In_Thread								= 56,
		Put_Thread_Member								= 57,
		Delete_Self_From_Thread							= 58,
		Delete_Thread_Member							= 59,
		Get_Thread_Member								= 60,
		Get_Thread_Members								= 61,
		Get_Active_Threads								= 62,
		Get_Public_Archived_Threads						= 63,
		Get_Private_Archived_Threads					= 64,
		Get_Joined_Private_Archived_Threads				= 65,
		Get_Emoji_List									= 66,
		Get_Guild_Emoji									= 67,
		Post_Guild_Emoji								= 68,
		Patch_Guild_Emoji								= 69,
		Delete_Guild_Emoji								= 70,
		Post_Guild										= 71,
		Get_Guild										= 72,
		Get_Guild_Preview								= 73,
		Patch_Guild										= 74,
		Delete_Guild									= 75,
		Get_Guild_Channels								= 76,
		Post_Guild_Channel								= 77,
		Patch_Guild_Channel_Positions					= 78,
		Get_Guild_Active_Threads						= 79,
		Get_Guild_Member								= 80,
		Get_Guild_Members								= 81,
		Get_Search_Guild_Members						= 82,
		Put_Guild_Member								= 83,
		Patch_Guild_Member								= 84,
		Patch_Current_Guild_Member						= 85,
		Put_Guild_Member_Role							= 86,
		Delete_Guild_Member_Role						= 87,
		Delete_Guild_Member								= 88,
		Get_Guild_Bans									= 89,
		Get_Guild_Ban									= 90,
		Put_Guild_Ban									= 91,
		Delete_Guild_Ban								= 92,
		Get_Guild_Roles									= 93,
		Post_Guild_Role									= 94,
		Patch_Guild_Role_Positions						= 95,
		Patch_Guild_Role								= 96,
		Delete_Guild_Role								= 97,
		Get_Guild_Prune_Count							= 98,
		Post_Guild_Prune								= 99,
		Get_Guild_Voice_Regions							= 100,
		Get_Guild_Invites								= 101,
		Get_Guild_Integrations							= 102,
		Delete_Guild_Integration						= 103,
		Get_Guild_Widget_Settings						= 104,
		Patch_Guild_Widget								= 105,
		Get_Guild_Widget								= 106,
		Get_Vanity_Invite								= 107,
		Get_Guild_Widget_Image							= 108,
		Get_Guild_Welcome_Screen						= 109,
		Patch_Guild_Welcome_Screen						= 110,
		Patch_Current_User_Voice_State					= 111,
		Patch_User_Voice_State							= 112,
		Get_Guild_Scheduled_Events						= 113,
		Post_Guild_Scheduled_Event						= 114,
		Get_Guild_Scheduled_Event						= 115,
		Patch_Guild_Scheduled_Event						= 116,
		Delete_Guild_Scheduled_Event					= 117,
		Get_Guild_Scheduled_Event_Users					= 118,
		Get_Guild_Template								= 119,
		Post_Guild_From_Guild_Template					= 120,
		Get_Guild_Templates								= 121,
		Post_Guild_Template								= 122,
		Put_Guild_Template								= 123,
		Patch_Guild_Template							= 124,
		Delete_Guild_Template							= 125,
		Get_Invite										= 126,
		Delete_Invite									= 127,
		Post_Stage_Instance								= 128,
		Get_Stage_Instance								= 129,
		Patch_Stage_Instance							= 130,
		Delete_Stage_Instance							= 131,
		Get_Sticker										= 132,
		Get_Nitro_Sticker_Packs							= 133,
		Get_Guild_Stickers								= 134,
		Post_Guild_Sticker								= 135,
		Patch_Guild_Sticker								= 136,
		Delete_Guild_Sticker							= 137,
		Get_Current_User								= 138,
		Get_User										= 139,
		Patch_Current_User								= 140,
		Get_Current_User_Guilds							= 141,
		Delete_Leave_Guild								= 142,
		Post_Create_User_Dm								= 143,
		Get_User_Connections							= 144,
		Get_Voice_Regions								= 145,
		Post_Webhook									= 146,
		Get_Channel_Webhooks							= 147,
		Get_Guild_Webhooks								= 148,
		Get_Webhook										= 149,
		Get_Webhook_With_Token							= 150,
		Patch_Webhook									= 151,
		Patch_Webhook_With_Token						= 152,
		Delete_Webhook									= 153,
		Delete_Webhook_With_Token						= 154,
		Post_Execute_Webhook							= 155,
		Get_Webhook_Message								= 156,
		Patch_Webhook_Message							= 157,
		Delete_Webhook_Message							= 158,
		Get_Application_Info							= 159,
		Get_Authorization_Info							= 160,
		Get_Gateway_Bot									= 161,
		Post_Thread_In_Forum_Channel					= 162,
		Get_Auto_Moderation_Rules						= 163,
		Get_Auto_Moderation_Rule						= 164,
		Post_Auto_Moderation_Rule						= 165,
		Patch_Auto_Moderation_Rule						= 166,
		Delete_Auto_Moderation_Rule						= 167,
		YouTube_Get_Search_Results						= 168,
		SoundCloud_Get_Search_Results					= 169,
		SoundCloud_Get_Client_Id						= 170,
		SoundCloud_Get_Download_Links					= 171,
		Last											= 172
	};

	class https_workload_data {
	  public:
		friend class https_client;

		std::unordered_map<std::string, std::string> headersToInsert{};
		payload_type payloadType{ payload_type::Application_Json };
		https_workload_class workloadClass{};
		std::string relativePath{};
		std::string callStack{};
		std::string baseUrl{};
		std::string content{};

		https_workload_data() = default;

		https_workload_data& operator=(https_workload_data&& other) noexcept {
			if (this != &other) {
				headersToInsert = std::move(other.headersToInsert);
				relativePath	= std::move(other.relativePath);
				callStack		= std::move(other.callStack);
				baseUrl			= std::move(other.baseUrl);
				content			= std::move(other.content);
				workloadClass	= other.workloadClass;
				workloadType	= other.workloadType;
				payloadType		= other.payloadType;
			}
			return *this;
		}

		https_workload_data(https_workload_data&& other) noexcept {
			*this = std::move(other);
		}

		https_workload_data& operator=(https_workload_type type) {
			workloadType = type;
			return *this;
		}

		https_workload_type getWorkloadType() const {
			return workloadType;
		}

		https_workload_data(https_workload_type type) {
			*this = type;
		}

	  protected:
		https_workload_type workloadType{};
	};

	class rate_limit_queue {
	  public:
		friend class https_client;

		RTC_REST_INLINE rate_limit_queue() = default;

		RTC_REST_INLINE void initialize();

		RTC_REST_INLINE rate_limit_data* getEndpointAccess(https_workload_type workloadType);

		RTC_REST_INLINE void releaseEndPointAccess(https_workload_type type);

	  protected:
		std::unordered_map<std::string, std::unique_ptr<rate_limit_data>> rateLimits{};
		std::unordered_map<https_workload_type, std::string> buckets{};
	};

}
