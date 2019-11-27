#include <Core/GameLogic/Question/Flag/QuestionFlagger.h>
#include <Core/Net/RequestBuilder.h>
#include <IEngine.h>

void QuestionFlagger::RestReceived(const std::string& rest) {}

void QuestionFlagger::FlagQuestion(int questionId) {
	RequestBuilder requestBuilder;
	requestBuilder.SetEndpoint("flagQuestion");
	requestBuilder.AddUser(IEngine::getEngine()->GetUserProvider()->GetUser());
	requestBuilder.AddParameter("question_id", std::to_string(questionId));

	std::string requestString = requestBuilder.GetRequestString();
	IEngine::getEngine()->GetRestConnector()->SendRequest(requestString, this);
}