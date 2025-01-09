// main.cpp - Main entry point.
//// Jun 17, 2021
// Chris M.
// https://github.com/RealTimeChris

#include <rtc_rest/Index.hpp>

int32_t main() {
	rtc_rest::https_client client{};
	rtc_rest::https_workload_data workload{};
	workload.baseUrl					   = "https://www.youtube.com";
	workload.workloadClass							= rtc_rest::https_workload_class::Get;
	workload.relativePath				   = "/results?search_query=wintersun+beautiful+death";
	workload.headersToInsert["User-Agent"] = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/131.0.0.0 Safari/537.36";
	workload.headersToInsert["Accept"] = "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7";
	auto result							   = client.submitWorkloadAndGetResult(workload);
	std::cout << "RESULT CODE: " << result.responseCode << std::endl;
	//std::cout << "RESULT DATA: " << result.responseHeaders << std::endl;
	for (auto& [key, value]: result.responseHeaders) {
		std::cout << "RESULT KEY: " << key << std::endl;
		std::cout << "RESULT VALUE: " << value << std::endl;
	}
	return 0;
};
