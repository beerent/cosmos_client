#pragma once

#include <Core/User/User.h>

#include <string>
#include <map>


class RequestBuilder {

public:
	RequestBuilder();

	void SetEndpoint(const std::string& endpoint);
	void AddParameter(const std::string& key, const std::string& value);
	void AddUser(const User& user);
	std::string GetRequestString() const;

private:
	std::string m_endpoint;
	std::map<std::string, std::string> m_parameters;
	
};