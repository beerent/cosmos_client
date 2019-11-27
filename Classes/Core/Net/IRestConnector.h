#pragma once

#include "IRestReceiver.h"
#include <string>
#include <unordered_set>

typedef std::unordered_set<std::string> ActiveRestRequests;

class IRestConnector {
public:
	virtual std::string SendRequest(const std::string& request, IRestReceiver* restReceiver) = 0;
	
	void CloseRequest(const std::string& id) {
		if (id == "") {
			return;
		}

		if (ContainsRequest(id) == false) {
			return;
		}

		m_activeRequests.erase(m_activeRequests.find(id));
	}

protected:
	ActiveRestRequests m_activeRequests;
	
	void AddRequest(const std::string& id) {
		m_activeRequests.insert(id);
	}

	bool ContainsRequest(const std::string& id) {
		return m_activeRequests.find(id) != m_activeRequests.end();
	}
};
