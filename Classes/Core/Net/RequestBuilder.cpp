#include <Core/Net/RequestBuilder.h>

void replaceAll(std::string& str, const std::string& from, const std::string& to);

RequestBuilder::RequestBuilder() {}

void RequestBuilder::SetEndpoint(const std::string& endpoint) {
	m_endpoint = endpoint;
}

void RequestBuilder::AddParameter(const std::string& key, const std::string& value) {
	m_parameters[key] = value;
}

void RequestBuilder::AddUser(const User& user) {
    m_parameters["uid"] = user.GetUid();
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
    
    replaceAll(requestString, " ", "%20");

	return requestString;
}

void replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}
