// main.cpp - Main entry point.
//// Jun 17, 2021
// Chris M.
// https://github.com/RealTimeChris

#include <rtc_rest/Index.hpp>

int32_t main() {
	rtc_rest::https_client client{};
	rtc_rest::https_workload_data workload{};
	workload.baseUrl = "google.com";
	workload.workloadClass = rtc_rest::https_workload_class::Get;
	workload.headersToInsert["User-Agent"] = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/131.0.0.0 Safari/537.36";
	auto result							   = client.submitWorkloadAndGetResult(workload);
	std::cout << "RESULT CODE: " << result.responseCode << std::endl;
	std::cout << "RESULT DATA: " << result.responseData << std::endl;
	return 0;
};
