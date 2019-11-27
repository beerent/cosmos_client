#pragma once

#include <Core/Net/IRestConnector.h>
#include <Core/Net/IRestReceiver.h>

class QuestionFlagger : public IRestReceiver {
public:
	virtual void RestReceived(const std::string& rest);
	void FlagQuestion(int questionId);
};