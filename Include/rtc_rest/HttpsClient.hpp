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
/// HttpsClient.hpp - Header file for the "Https stuff".
/// May 12, 2021
/// https://rtc_rest.com
/// \file HttpsClient.hpp
#pragma once

#include <rtc_rest/RateLimitQueue.hpp>
#include <jsonifier/Index.hpp>
#include <source_location>

namespace rtc_rest {

	/// \brief Voice websocket close codes.
	class https_response_code {
	  public:
		/// \brief Voice websocket close codes.
		enum class https_response_codes : uint64_t {
			Unset				= std::numeric_limits<uint64_t>::max(),
			ok					= 200,///< The request completed successfully.
			created				= 201,///< The entity was created successfully.
			No_Content			= 204,///< The request completed successfully but returned no content.
			Not_Modifies		= 304,///< The entity was not modified (no action was taken).
			Bad_Request			= 400,///< The request was improperly formatted, or the server couldn't understand it.
			unauthorized		= 401,///< The authorization header was missing or invalid.
			forbidden			= 403,///< The authorization token you passed did not have permission to the resource.
			Not_Found			= 404,///< The resource at the location specified doesn't exist.
			Method_Not_Allowed	= 405,///< The https method used is not valid for the location specified.
			Too_Many_Requests	= 429,///< You are being rate limited, see rate limits.
			Gateway_Unavailable = 502,///< There was not a gateway available to process your request. wait a bit and retry.
		};

		RTC_REST_INLINE static std::unordered_map<https_response_codes, std::string> outputErrorValues{
			{ static_cast<https_response_codes>(200), "The request completed successfully" }, { static_cast<https_response_codes>(201), "The entity was created successfully" },
			{ static_cast<https_response_codes>(204), "The request completed successfully but returned no content" },
			{ static_cast<https_response_codes>(304), "The entity was not modified (no action was taken)" },
			{ static_cast<https_response_codes>(400), "The request was improperly formatted, or the server couldn't understand it" },
			{ static_cast<https_response_codes>(401), "The authorization header was missing or invalid" },
			{ static_cast<https_response_codes>(403), "The authorization token you passed did not have permission to the resource" },
			{ static_cast<https_response_codes>(404), "The resource at the location specified doesn't exist" },
			{ static_cast<https_response_codes>(405), "The https method used is not valid for the location specified" },
			{ static_cast<https_response_codes>(429), "you are being rate limited, see rate limits" },
			{ static_cast<https_response_codes>(502), "There was not a gateway available to process your request.wait a bit and retry" },
			{ static_cast<https_response_codes>(500), "The server had an error processing your request(these are rare)" }
		};

		https_response_codes value{};

		RTC_REST_INLINE https_response_code() = default;

		RTC_REST_INLINE https_response_code& operator=(uint64_t valueNew) {
			value = static_cast<https_response_codes>(valueNew);
			return *this;
		}

		RTC_REST_INLINE https_response_code(uint64_t value) {
			*this = value;
		}

		RTC_REST_INLINE operator std::string() {
			return std::string{ "Code: " + std::to_string(static_cast<uint32_t>(value)) + std::string{ ", message: " } +
				static_cast<std::string>(https_response_code::outputErrorValues[value]) };
		}

		RTC_REST_INLINE operator uint64_t() {
			return static_cast<uint64_t>(value);
		}
	};

	class https_connection_manager;
	struct rate_limit_data;

	enum class https_state { Collecting_Headers = 0, Collecting_Contents = 1, Collecting_Chunked_Contents = 2, complete = 3 };

	class https_error : public std::exception {
	  public:
		https_response_code errorCode{};
		RTC_REST_INLINE https_error(const std::string_view& message, std::source_location location = std::source_location::current()) : std::exception{ message.data() } {};
	};

	struct https_response_data {
		friend class https_rnr_builder;
		friend class https_connection;
		friend class https_client;

		https_response_code responseCode{ std::numeric_limits<uint32_t>::max() };
		std::unordered_map<std::string, std::string> responseHeaders{};
		https_state currentState{ https_state::Collecting_Headers };
		std::string responseData{};
		uint64_t contentLength{};

	  protected:
		bool isItChunked{};
	};

	class https_rnr_builder {
	  public:
		friend class https_client;

		https_rnr_builder() = default;

		https_response_data finalizeReturnValues(rate_limit_data& rateLimitData);

		std::string buildRequest(const https_workload_data& workload);

		void updateRateLimitData(rate_limit_data& rateLimitData);

		bool parseHeaders();

		virtual ~https_rnr_builder() = default;

	  protected:
		bool parseContents();

		bool parseChunk();
	};

	class https_connection : public https_rnr_builder, public tcp_connection<https_connection> {
	  public:
		template<typename value_type> friend class https_tcp_connection;

		rate_limit_data* currentRateLimitData{};
		const int32_t maxReconnectTries{ 3 };
		std::string inputBufferReal{};
		std::string currentBaseUrl{};
		int32_t currentReconnectTries{};
		https_workload_data workload{};
		https_response_data data{};

		https_connection() = default;

		https_connection(const std::string& baseUrlNew, const uint16_t portNew);

		void resetValues(https_workload_data&& workloadNew, rate_limit_data* newRateLimitData);

		void handleBuffer() override;

		bool areWeConnected();

		void disconnect();

		virtual ~https_connection() = default;
	};

	/// @class https_connection_manager.
	/// @brief For managing the collection of Https connections.
	class https_connection_manager {
	  public:
		friend class https_client;

		https_connection_manager() = default;

		https_connection_manager(rate_limit_queue*);

		https_connection& getConnection(https_workload_type workloadType);

		rate_limit_queue& getRateLimitQueue();

	  protected:
		std::unordered_map<https_workload_type, std::unique_ptr<https_connection>> httpsConnections{};///< Collection of Https connections.
		rate_limit_queue* rateLimitQueue{};
		std::mutex accessMutex{};
	};

	class https_connection_stack_holder {
	  public:
		https_connection_stack_holder(https_connection_manager& connectionManager, https_workload_data&& workload);

		https_connection& getConnection();

		~https_connection_stack_holder();

	  protected:
		rate_limit_queue* rateLimitQueue{};
		https_connection* connection{};
	};

	class https_client_core {
	  public:
		https_client_core() = default;

		RTC_REST_INLINE https_response_data submitWorkloadAndGetResult(https_workload_data&& workloadNew) {
			https_connection connection{};
			rate_limit_data rateLimitData{};
			connection.resetValues(std::move(workloadNew), &rateLimitData);
			auto returnData = httpsRequestInternal(connection);
			if (returnData.responseCode != 200 && returnData.responseCode != 204 && returnData.responseCode != 201) {
				std::string errorMessage{};
				if (connection.workload.callStack != "") {
					errorMessage += connection.workload.callStack + " ";
				}
				errorMessage += "Https error: " + returnData.responseCode.operator std::string() + "\nThe request: base url: " + connection.workload.baseUrl + "\n";
				if (!connection.workload.relativePath.empty()) {
					errorMessage += "Relative Url: " + connection.workload.relativePath + "\n";
				}
				if (!connection.workload.content.empty()) {
					errorMessage += "Content: " + connection.workload.content + "\n";
				}
				if (!returnData.responseData.empty()) {
					errorMessage += "The Response: " + static_cast<std::string>(returnData.responseData);
				}
				https_error theError{ errorMessage };
				theError.errorCode = returnData.responseCode;
			}
			return returnData;
		}

	  protected:
		std::string botToken{};

		https_response_data httpsRequestInternal(https_connection& connection);

		https_response_data recoverFromError(https_connection& connection);

		https_response_data getResponse(https_connection& connection);
	};

	/**
		 * \addtogroup discord_core_internal
		 * @{
		 */

	/// @class https_client
	/// @brief For sending Https requests.
	class https_client : public https_client_core {
	  public:
		https_client();

		template<typename value_type, typename string_type> void getParseErrors(jsonifier::jsonifier_core<false>& parser, value_type& value, string_type& stringNew) {
			parser.parseJson(value, parser.minifyJson(parser.prettifyJson(stringNew)));
			if (auto result = parser.getErrors(); result.size() > 0) {
				for (auto& valueNew: result) {
					//message_printer::printError<print_message_type::websocket>(valueNew.reportError());
				}
			}
		}

		template<typename workload_type, typename... args> auto submitWorkloadAndGetResult(workload_type&& workload, args&... argsNew) {
			https_connection_stack_holder stackHolder{ connectionManager, std::move(workload) };
			https_response_data returnData = httpsRequest(stackHolder.getConnection());
			if (static_cast<uint32_t>(returnData.responseCode) != 200 && static_cast<uint32_t>(returnData.responseCode) != 204 &&
				static_cast<uint32_t>(returnData.responseCode) != 201) {
				std::string errorMessage{};
				if (stackHolder.getConnection().workload.callStack != "") {
					errorMessage += stackHolder.getConnection().workload.callStack + " ";
				}
				errorMessage +=
					"Https error: " + returnData.responseCode.operator std::string() + "\nThe request: base url: " + stackHolder.getConnection().workload.baseUrl + "\n";
				if (!stackHolder.getConnection().workload.relativePath.empty()) {
					errorMessage += "Relative Url: " + stackHolder.getConnection().workload.relativePath + "\n";
				}
				if (!stackHolder.getConnection().workload.content.empty()) {
					errorMessage += "Content: " + stackHolder.getConnection().workload.content + "\n";
				}
				if (!returnData.responseData.empty()) {
					errorMessage += "The Response: " + static_cast<std::string>(returnData.responseData);
				}
				https_error theError{ errorMessage };
				theError.errorCode = returnData.responseCode;
			}

			if constexpr ((( !std::is_void_v<args> ) || ...)) {
				if (returnData.responseData.size() > 0) {
					(getParseErrors(parser, argsNew, returnData.responseData), ...);
				}
			}
			return returnData;
		}

	  protected:
		inline static wsadata_wrapper wsaData{};
		inline static ssl_context_holder sslContext{};
		https_connection_manager connectionManager{};
		rate_limit_queue rateLimitQueue{};

		https_response_data executeByRateLimitData(https_connection& connection);

		https_response_data httpsRequest(https_connection& connection);
	};

	using sys_clock = std::chrono::system_clock;

	RTC_REST_INLINE void rate_limit_queue::initialize() {
		for (int64_t enumOne = static_cast<int64_t>(https_workload_type::Unset); enumOne != static_cast<int64_t>(https_workload_type::Last); enumOne++) {
			auto tempBucket = std::to_string(std::chrono::duration_cast<nanoseconds>(sys_clock::now().time_since_epoch()).count());
			buckets.emplace(static_cast<https_workload_type>(enumOne), tempBucket);
			rateLimits.emplace(tempBucket, std::make_unique<rate_limit_data>())
				.first->second->sampledTimeInMs.store(std::chrono::duration_cast<milliseconds>(sys_clock::now().time_since_epoch()));
			std::this_thread::sleep_for(std::chrono::milliseconds{ 1 });
		}
	}

	RTC_REST_INLINE rate_limit_data* rate_limit_queue::getEndpointAccess(https_workload_type workloadType) {
		jsonifier_internal::stop_watch<milliseconds> stopWatch{ milliseconds{ 25000 } };
		stopWatch.reset();
		auto targetTime =
			std::chrono::duration_cast<std::chrono::duration<int64_t, std::milli>>(rateLimits[buckets[workloadType]]->sampledTimeInMs.load(std::memory_order_acquire)) +
			std::chrono::duration_cast<std::chrono::duration<int64_t, std::milli>>(rateLimits[buckets[workloadType]]->sRemain.load(std::memory_order_acquire));
		if (rateLimits[buckets[workloadType]]->getsRemaining.load(std::memory_order_acquire) <= 0) {
			auto newNow = std::chrono::duration_cast<std::chrono::duration<int64_t, std::milli>>(sys_clock::now().time_since_epoch());
			while ((newNow - targetTime).count() <= 0) {
				if (stopWatch.hasTimeElapsed()) {
					return nullptr;
				}
				newNow = std::chrono::duration_cast<std::chrono::duration<int64_t, std::milli>>(sys_clock::now().time_since_epoch());
				std::this_thread::sleep_for(std::chrono::nanoseconds{ 1 });
			}
		}
		stopWatch.reset();
		while (!rateLimits[buckets[workloadType]]->accessMutex.try_lock()) {
			std::this_thread::sleep_for(std::chrono::nanoseconds{ 1 });
			if (stopWatch.hasTimeElapsed()) {
				return nullptr;
			}
		}
		return rateLimits.at(buckets.at(workloadType)).get();
	}

	RTC_REST_INLINE void rate_limit_queue::releaseEndPointAccess(https_workload_type type) {
		rateLimits.at(buckets.at(type))->accessMutex.unlock();
	}

	std::vector<std::string_view> tokenize(std::string_view in, const char* sep = "\r\n") {
		std::vector<std::string_view> result{};
		std::string_view::size_type b = 0;
		std::string_view::size_type e = 0;
		while ((b = in.find_first_not_of(sep, e)) != std::string_view::npos) {
			e = in.find_first_of(sep, b);
			if (e == std::string_view::npos) {
				break;
			}
			result.emplace_back(in.substr(b, e - b));
		}
		return result;
	}

	uint64_t parseCode(std::string_view string) {
		uint64_t start = string.find(' ');
		if (start == std::string_view::npos) {
			return 0;
		}

		while (isspace(string[start])) {
			start++;
		}

		uint64_t end = start;
		while (isdigit(string[end])) {
			end++;
		}
		std::string_view codeStr = string.substr(start, end - start);
		uint64_t code			 = jsonifier::strToUint64(codeStr.data());
		return code;
	}

	https_connection::https_connection(const std::string& baseUrlNew, const uint16_t portNew) : tcp_connection<https_connection>{ baseUrlNew, portNew } {
	}

	void https_connection::handleBuffer() {
		jsonifier_internal::stop_watch<milliseconds> stopWatch{ 9500 };
		std::basic_string_view<uint8_t> stringNew{};
		stopWatch.reset();
		do {
			stringNew		  = getInputBuffer();
			auto originalSize = inputBufferReal.size();
			inputBufferReal.resize(inputBufferReal.size() + stringNew.size());
			std::memcpy(inputBufferReal.data() + originalSize, stringNew.data(), stringNew.size());
			switch (data.currentState) {
				case https_state::Collecting_Headers: {
					if (!parseHeaders()) {
						return;
					}
					break;
				}
				case https_state::Collecting_Contents: {
					if (!parseContents()) {
						return;
					}
					break;
				}
				case https_state::Collecting_Chunked_Contents: {
					if (!parseChunk()) {
						return;
					}
					break;
				}
				case https_state::complete: {
					inputBufferReal.clear();
					return;
				}
			}
		} while (stringNew.size() > 0 && !stopWatch.hasTimeElapsed());
		return;
	}

	bool https_connection::areWeConnected() {
		return tcp_connection::areWeStillConnected();
	}

	void https_connection::disconnect() {
		tcp_connection::disconnect();
		tcp_connection::reset();
	}

	void https_connection::resetValues(https_workload_data&& workloadDataNew, rate_limit_data* rateLimitDataNew) {
		currentRateLimitData = rateLimitDataNew;
		if (currentBaseUrl != workloadDataNew.baseUrl) {
			tcp_connection::reset();
			currentBaseUrl = workloadDataNew.baseUrl;
		}
		workload = std::move(workloadDataNew);
		if (workload.baseUrl == "") {
			workload.baseUrl = "https://discord.com/api/v10";
		}
		inputBufferReal.clear();
		data = https_response_data{};
	}

	void https_rnr_builder::updateRateLimitData(rate_limit_data& rateLimitData) {
		auto connection{ static_cast<https_connection*>(this) };
		if (connection->data.responseHeaders.contains("x-ratelimit-bucket")) {
			rateLimitData.bucket = connection->data.responseHeaders.at("x-ratelimit-bucket");
		}
		if (connection->data.responseHeaders.contains("x-ratelimit-reset-after")) {
			rateLimitData.sRemain.store(seconds{ static_cast<int64_t>(ceil(jsonifier::strToDouble(connection->data.responseHeaders.at("x-ratelimit-reset-after").data()))) },
				std::memory_order_release);
		}
		if (connection->data.responseHeaders.contains("x-ratelimit-remaining")) {
			rateLimitData.getsRemaining.store(static_cast<int64_t>(jsonifier::strToInt64(connection->data.responseHeaders.at("x-ratelimit-remaining").data())),
				std::memory_order_release);
		}
		if (rateLimitData.getsRemaining.load(std::memory_order_acquire) <= 1 || rateLimitData.areWeASpecialBucket.load(std::memory_order_acquire)) {
			rateLimitData.doWeWait.store(true, std::memory_order_release);
		}
	}

	https_response_data https_rnr_builder::finalizeReturnValues(rate_limit_data& rateLimitData) {
		auto connection{ static_cast<https_connection*>(this) };
		if (connection->data.responseData.size() >= connection->data.contentLength && connection->data.contentLength > 0) {
			connection->data.responseData = connection->data.responseData.substr(0, connection->data.contentLength);
		} else {
			auto pos1 = connection->data.responseData.find_first_of('{');
			auto pos2 = connection->data.responseData.find_last_of('}');
			auto pos3 = connection->data.responseData.find_first_of('[');
			auto pos4 = connection->data.responseData.find_last_of(']');
			if (pos1 != std::string_view::npos && pos2 != std::string_view::npos && pos1 < pos3) {
				connection->data.responseData = connection->data.responseData.substr(pos1, pos2 + 1);
			} else if (pos3 != std::string_view::npos && pos4 != std::string_view::npos) {
				connection->data.responseData = connection->data.responseData.substr(pos3, pos4 + 1);
			}
		}
		updateRateLimitData(rateLimitData);
		if (connection->data.responseCode != 204 && connection->data.responseCode != 200 && connection->data.responseCode != 201) {
			std::cerr << std::string{ "Sorry, but that https request threw the following error: " + connection->data.responseCode.operator std::string() +
				connection->data.responseData };
		}
		return std::move(connection->data);
	}

	std::string https_rnr_builder::buildRequest(const https_workload_data& workload) {
		std::string baseUrlNew{};
		if (workload.baseUrl.find(".com") != std::string_view::npos) {
			baseUrlNew = workload.baseUrl.substr(workload.baseUrl.find("https://") + std::string_view("https://").size(),
				workload.baseUrl.find(".com") + std::string_view(".com").size() - std::string_view("https://").size());
		} else if (workload.baseUrl.find(".org") != std::string_view::npos) {
			baseUrlNew = workload.baseUrl.substr(workload.baseUrl.find("https://") + std::string_view("https://").size(),
				workload.baseUrl.find(".org") + std::string_view(".org").size() - std::string_view("https://").size());
		}
		std::string returnString{};
		if (workload.workloadClass == https_workload_class::Get || workload.workloadClass == https_workload_class::Delete) {
			if (workload.workloadClass == https_workload_class::Get) {
				returnString += "GET " + workload.baseUrl + workload.relativePath + " HTTP/1.1\r\n";
			} else if (workload.workloadClass == https_workload_class::Delete) {
				returnString += "DELETE " + workload.baseUrl + workload.relativePath + " HTTP/1.1\r\n";
			}
			for (auto& [key, value]: workload.headersToInsert) {
				returnString += key + ": " + value + "\r\n";
			}
			returnString += "Pragma: no-cache\r\n";
			returnString += "Connection: keep-alive\r\n";
			returnString += "Host: " + baseUrlNew + "\r\n\r\n";
		} else {
			if (workload.workloadClass == https_workload_class::Patch) {
				returnString += "PATCH " + workload.baseUrl + workload.relativePath + " HTTP/1.1\r\n";
			} else if (workload.workloadClass == https_workload_class::Post) {
				returnString += "POST " + workload.baseUrl + workload.relativePath + " HTTP/1.1\r\n";
			} else if (workload.workloadClass == https_workload_class::Put) {
				returnString = "PUT " + workload.baseUrl + workload.relativePath + " HTTP/1.1\r\n";
			}
			for (auto& [key, value]: workload.headersToInsert) {
				returnString += key + ": " + value + "\r\n";
			}
			returnString += "Pragma: no-cache\r\n";
			returnString += "Connection: keep-alive\r\n";
			returnString += "Host: " + baseUrlNew + "\r\n";
			returnString += "Content-Length: " + std::to_string(workload.content.size()) + "\r\n\r\n";
			returnString += workload.content + "\r\n\r\n";
		}
		return returnString;
	}

	bool https_rnr_builder::parseHeaders() {
		auto connection{ static_cast<https_connection*>(this) };
		std::string& stringViewNew = connection->inputBufferReal;
		if (stringViewNew.find("\r\n\r\n") != std::string_view::npos) {
			auto headers = tokenize(stringViewNew);
			if (headers.size() && (headers.at(0).find("HTTP/1") != std::string_view::npos)) {
				uint64_t parseCodeNew{};
				try {
					parseCodeNew = parseCode(headers.at(0));
				} catch (const std::invalid_argument& error) {
					//message_printer::printError<print_message_type::https>(error.what());
					connection->data.currentState = https_state::complete;
				}
				headers.erase(headers.begin());
				if (headers.size() >= 3 && parseCodeNew) {
					for (uint64_t x = 0; x < headers.size(); ++x) {
						std::string_view::size_type sep = headers.at(x).find(": ");
						if (sep != std::string_view::npos) {
							std::string key		   = static_cast<std::string>(headers.at(x).substr(0, sep));
							std::string_view value = headers.at(x).substr(sep + 2, headers.at(x).size());
							for (auto& valueNew: key) {
								valueNew = static_cast<char>(tolower(static_cast<int32_t>(valueNew)));
							}
							connection->data.responseHeaders.emplace(key, value);
						}
					}
					connection->data.responseCode = parseCodeNew;
					if (connection->data.responseCode == 302) {
						connection->workload.baseUrl = connection->data.responseHeaders.at("location");
						connection->disconnect();
						return false;
					}
					if (connection->data.responseCode == 204) {
						connection->data.currentState = https_state::complete;
					} else if (connection->data.responseHeaders.contains("content-length")) {
						connection->data.contentLength = jsonifier::strToUint64(connection->data.responseHeaders.at("content-length").data());
						connection->data.currentState  = https_state::Collecting_Contents;
					} else {
						connection->data.isItChunked   = true;
						connection->data.contentLength = std::numeric_limits<uint32_t>::max();
						connection->data.currentState  = https_state::Collecting_Chunked_Contents;
					}
					connection->inputBufferReal.erase(connection->inputBufferReal.begin() + static_cast<int64_t>(stringViewNew.find("\r\n\r\n")) + 4);
				}
			}
			return true;
		}
		return false;
	}

	bool https_rnr_builder::parseChunk() {
		auto connection{ static_cast<https_connection*>(this) };
		std::string_view stringViewNew01{ connection->inputBufferReal };
		if (auto finalPosition = stringViewNew01.find("\r\n0\r\n\r\n"); finalPosition != std::string_view::npos) {
			uint64_t pos{ 0 };
			while (pos < stringViewNew01.size() || connection->data.responseData.size() < connection->data.contentLength) {
				uint64_t lineEnd = stringViewNew01.find("\r\n", pos);
				if (lineEnd == std::string_view::npos) {
					break;
				}

				std::string_view sizeLine{ stringViewNew01.data() + pos, lineEnd - pos };
				uint64_t chunkSize = jsonifier::strToUint64<16>(static_cast<std::string>(sizeLine));
				connection->data.contentLength += chunkSize;

				if (chunkSize == 0) {
					break;
				}

				pos = lineEnd + 2;

				std::string_view newString{ stringViewNew01.data() + pos, chunkSize };
				connection->data.responseData += newString;
				pos += chunkSize + 2;
			}
			connection->data.currentState = https_state::complete;
			return true;
		}
		return false;
	}

	bool https_rnr_builder::parseContents() {
		auto connection{ static_cast<https_connection*>(this) };
		if (connection->inputBufferReal.size() >= connection->data.contentLength || !connection->data.contentLength) {
			connection->data.responseData += std::string_view{ connection->inputBufferReal.data(), connection->data.contentLength };
			connection->data.currentState = https_state::complete;
			return true;
		} else {
			return false;
		}
	}

	https_connection_manager::https_connection_manager(rate_limit_queue* rateLimitDataQueueNew) {
		rateLimitQueue = rateLimitDataQueueNew;
	}

	rate_limit_queue& https_connection_manager::getRateLimitQueue() {
		return *rateLimitQueue;
	}

	https_connection& https_connection_manager::getConnection(https_workload_type workloadType) {
		std::unique_lock lock{ accessMutex };
		if (!httpsConnections.contains(workloadType)) {
			httpsConnections.emplace(workloadType, std::make_unique<https_connection>());
		}
		httpsConnections.at(workloadType)->currentReconnectTries = 0;
		return *httpsConnections.at(workloadType).get();
	}

	https_connection_stack_holder::https_connection_stack_holder(https_connection_manager& connectionManager, https_workload_data&& workload) {
		connection		   = &connectionManager.getConnection(workload.getWorkloadType());
		rateLimitQueue	   = &connectionManager.getRateLimitQueue();
		auto rateLimitData = connectionManager.getRateLimitQueue().getEndpointAccess(workload.getWorkloadType());
		if (!rateLimitData) {
			throw std::runtime_error{ "Failed to gain endpoint access." };
		}
		connection->resetValues(std::move(workload), rateLimitData);
		if (!connection->areWeConnected()) {
			*static_cast<tcp_connection<https_connection>*>(connection) = https_connection{ connection->workload.baseUrl, static_cast<uint16_t>(443) };
		}
	}

	https_connection_stack_holder::~https_connection_stack_holder() {
		rateLimitQueue->releaseEndPointAccess(connection->workload.getWorkloadType());
	}

	https_connection& https_connection_stack_holder::getConnection() {
		return *connection;
	}

	https_client::https_client() : https_client_core(), connectionManager(&rateLimitQueue) {
		rateLimitQueue.initialize();
	}

	https_response_data https_client::httpsRequest(https_connection& connection) {
		https_response_data resultData = executeByRateLimitData(connection);
		return resultData;
	}

	https_response_data https_client::executeByRateLimitData(https_connection& connection) {
		https_response_data returnData{};
		milliseconds timeRemaining{};
		milliseconds currentTime = std::chrono::duration_cast<milliseconds>(sys_clock::now().time_since_epoch());
		if (connection.workload.workloadType == https_workload_type::Delete_Message_Old) {
			connection.currentRateLimitData->sRemain.store(seconds{ 4 }, std::memory_order_release);
		}
		if (connection.workload.workloadType == https_workload_type::Post_Message || connection.workload.workloadType == https_workload_type::Patch_Message) {
			connection.currentRateLimitData->areWeASpecialBucket.store(true, std::memory_order_release);
		}
		if (connection.currentRateLimitData->areWeASpecialBucket.load(std::memory_order_acquire)) {
			connection.currentRateLimitData->sRemain.store(seconds{ static_cast<int64_t>(ceil(4.0f / 4.0f)) }, std::memory_order_release);
			milliseconds targetTime{ connection.currentRateLimitData->sampledTimeInMs.load(std::memory_order_acquire) +
				std::chrono::duration_cast<std::chrono::milliseconds>(connection.currentRateLimitData->sRemain.load(std::memory_order_acquire)) };
			timeRemaining = targetTime - currentTime;
		} else if (connection.currentRateLimitData->doWeWait.load(std::memory_order_acquire)) {
			milliseconds targetTime{ connection.currentRateLimitData->sampledTimeInMs.load(std::memory_order_acquire) +
				std::chrono::duration_cast<std::chrono::milliseconds>(connection.currentRateLimitData->sRemain.load(std::memory_order_acquire)) };
			timeRemaining = targetTime - currentTime;
			connection.currentRateLimitData->doWeWait.store(false, std::memory_order_release);
		}
		if (timeRemaining.count() > 0) {
			//message_printer::printSuccess<print_message_type::https>("we're waiting on rate-limit: " + std::to_string(timeRemaining.count()));
			milliseconds targetTime{ currentTime + timeRemaining };
			while (targetTime > currentTime && targetTime.count() > 0 && currentTime.count() > 0 && timeRemaining.count() > 0) {
				currentTime	  = std::chrono::duration_cast<milliseconds>(sys_clock::now().time_since_epoch());
				timeRemaining = targetTime - currentTime;
				if (timeRemaining.count() <= 20) {
					continue;
				} else {
					std::this_thread::sleep_for(milliseconds{ static_cast<int64_t>(static_cast<double>(timeRemaining.count()) * 80.0f / 100.0f) });
				}
			}
		}
		returnData = https_client::httpsRequestInternal(connection);
		connection.currentRateLimitData->sampledTimeInMs.store(std::chrono::duration_cast<std::chrono::duration<int64_t, std::milli>>(sys_clock::now().time_since_epoch()),
			std::memory_order_release);

		if (returnData.responseCode == 204 || returnData.responseCode == 201 || returnData.responseCode == 200) {
			//message_printer::printSuccess<print_message_type::https>(
			//connection.workload.callStack + " success: " + static_cast<std::string>(returnData.responseCode) + ": " + returnData.responseData);
		} else if (returnData.responseCode == 429) {
			if (connection.data.responseHeaders.contains("x-ratelimit-retry-after")) {
				connection.currentRateLimitData->sRemain.store(seconds{ jsonifier::strToInt64(connection.data.responseHeaders.at("x-ratelimit-retry-after").data()) / 1000LL },
					std::memory_order_release);
			}
			connection.currentRateLimitData->doWeWait.store(true, std::memory_order_release);
			connection.currentRateLimitData->sampledTimeInMs.store(std::chrono::duration_cast<milliseconds>(sys_clock::now().time_since_epoch()), std::memory_order_release);
			//message_printer::printError<print_message_type::https>(connection.workload.callStack +
			//"::httpsRequest(), we've hit rate limit! time remaining: " + std::to_string(connection.currentRateLimitData->sRemain.load(std::memory_order_acquire).count()));
			connection.resetValues(std::move(connection.workload), connection.currentRateLimitData);
			returnData = executeByRateLimitData(connection);
		}
		return returnData;
	}

	https_response_data https_client_core::httpsRequestInternal(https_connection& connection) {
		if (connection.workload.baseUrl == "https://discord.com/api/v10") {
			connection.workload.headersToInsert.emplace("Authorization", "Bot " + botToken);
			connection.workload.headersToInsert.emplace("User-Agent", "Rtc-REST (https://rtc_rest.com/1.0)");
			if (connection.workload.payloadType == payload_type::Application_Json) {
				connection.workload.headersToInsert.emplace("Content-Type", "application/json");
			} else if (connection.workload.payloadType == payload_type::Multipart_Form) {
				connection.workload.headersToInsert.emplace("Content-Type", "multipart/form-data; boundary=boundary25");
			}
		}
		if (connection.currentReconnectTries >= connection.maxReconnectTries) {
			connection.disconnect();
			return https_response_data{};
		}
		if (!connection.areWeConnected()) {
			connection.currentBaseUrl									 = connection.workload.baseUrl;
			*static_cast<tcp_connection<https_connection>*>(&connection) = https_connection{ connection.workload.baseUrl, static_cast<uint16_t>(443) };
			if (connection.currentStatus != connection_status::NO_Error || !connection.areWeConnected()) {
				++connection.currentReconnectTries;
				connection.disconnect();
				return httpsRequestInternal(connection);
			}
		}
		auto request = connection.buildRequest(connection.workload);
		if (connection.areWeConnected()) {
			connection.writeData(static_cast<std::string_view>(request), true);
			if (connection.currentStatus != connection_status::NO_Error || !connection.areWeConnected()) {
				++connection.currentReconnectTries;
				connection.disconnect();
				return httpsRequestInternal(connection);
			}
			auto result = getResponse(connection);
			if (static_cast<int64_t>(result.responseCode) == -1 || !connection.areWeConnected()) {
				++connection.currentReconnectTries;
				connection.disconnect();
				return httpsRequestInternal(connection);
			} else {
				return result;
			}
		} else {
			++connection.currentReconnectTries;
			connection.disconnect();
			return httpsRequestInternal(connection);
		}
	}

	https_response_data https_client_core::recoverFromError(https_connection& connection) {
		if (connection.currentReconnectTries >= connection.maxReconnectTries) {
			connection.disconnect();
			return connection.finalizeReturnValues(*connection.currentRateLimitData);
		}
		++connection.currentReconnectTries;
		connection.disconnect();
		std::this_thread::sleep_for(std::chrono::milliseconds{ 150 });
		return httpsRequestInternal(connection);
	}

	https_response_data https_client_core::getResponse(https_connection& connection) {
		while (connection.data.currentState != https_state::complete) {
			if (connection.areWeConnected()) {
				auto newState = connection.processIO(10);
				switch (newState) {
					case connection_status::NO_Error: {
						continue;
					}
					case connection_status::CONNECTION_Error:
						[[fallthrough]];
					case connection_status::POLLERR_Error:
						[[fallthrough]];
					case connection_status::POLLHUP_Error:
						[[fallthrough]];
					case connection_status::POLLNVAL_Error:
						[[fallthrough]];
					case connection_status::READ_Error:
						[[fallthrough]];
					case connection_status::WRITE_Error:
						[[fallthrough]];
					case connection_status::SOCKET_Error:
						[[fallthrough]];
					default: {
						return recoverFromError(connection);
					}
				}
			} else {
				return recoverFromError(connection);
			}
		}
		return connection.finalizeReturnValues(*connection.currentRateLimitData);
	}
}