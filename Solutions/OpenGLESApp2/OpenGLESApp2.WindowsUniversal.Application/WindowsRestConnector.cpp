#include <WindowsRestConnector.h>
#include "pch.h"

#include <ppltasks.h>

using namespace concurrency;
using namespace Windows::Foundation;
using namespace Windows::Web::Http;
using namespace Platform;

//const std::string URL_PREFIX = "http://127.0.0.1:8081";
const std::string URL_PREFIX = "https://knowyourcosmos.com:13213";

String^ ToSystemString(const std::string& string) {
	std::wstring wid_str = std::wstring(string.begin(), string.end());
	const wchar_t* w_char = wid_str.c_str();
	Platform::String^ systemString = ref new Platform::String(w_char);

	return systemString;
}

std::string WindowsRestConnector::SendRequest(const std::string& requestUrl, IRestReceiver* restReceiver) {
	std::string fullRequestUrl = URL_PREFIX + "/" + requestUrl;
	auto uri = ref new Uri(ToSystemString(fullRequestUrl));

	HttpClient^ httpClient = ref new HttpClient();
	auto cancellationTokenSource = cancellation_token_source();

	std::string requestId = std::to_string(std::intptr_t(restReceiver)) + requestUrl;
	AddRequest(requestId);

	auto task = create_task(httpClient->GetAsync(uri), cancellationTokenSource.get_token()).then([cancellationTokenSource, restReceiver, requestId, this](HttpResponseMessage^ response) {
		create_task(response->Content->ReadAsStringAsync(), cancellationTokenSource.get_token()).then([restReceiver, requestId, this](String^ contentString) {
			std::wstring widestring(contentString->Begin());
			std::string str(widestring.begin(), widestring.end());

			if (ContainsRequest(requestId)) {
				restReceiver->RestReceived(str);
				CloseRequest(requestId);
			}
		});
	});

	return requestId;
}