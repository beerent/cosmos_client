#include <Core/Net/RequestBuilder.h>

RequestBuilder::RequestBuilder() {}

void RequestBuilder::SetEndpoint(const std::string& endpoint) {
	m_endpoint = endpoint;
}

void RequestBuilder::AddParameter(const std::string& key, const std::string& value) {
	m_parameters[key] = value;
}

void RequestBuilder::AddUser(const User& user) {
	m_parameters["username"] = user.GetUsername();
	m_parameters["password"] = user.GetPasswordHash();
}

std::string RequestBuilder::GetRequestString() const {
	std::string requestString = m_endpoint + "?";

	bool firstPair = true;
	for (auto pair : m_parameters) {
		if (false == firstPair) {
			requestString += "&";
		} else {
			firstPair = false;
		}

		requestString += (pair.first + "=" + pair.second);
	}

	return requestString;
}