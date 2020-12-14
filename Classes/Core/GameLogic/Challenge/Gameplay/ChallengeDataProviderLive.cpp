#include <Core/GameLogic/Challenge/Gameplay/ChallengeDataProviderLive.h>

#include <Core/Net/RequestBuilder.h>

#include <IEngine.h>

namespace requests {
	const std::string NEW_CHALLENGE = "newChallenge";
	const std::string GET_CHALLENGE_QUESTIONS = "getChallengeQuestions";
	const std::string REGISTER_CHALLENGE_ANSWER = "registerChallengeAnswer";
}

ChallengeDataProviderLive::ChallengeDataProviderLive() {
	m_restConnector = IEngine::getEngine()->GetRestConnector();
}

ChallengeDataProviderLive::~ChallengeDataProviderLive() {
	m_restConnector->CloseRequest(m_requestChallengeIdRequestId);
	m_restConnector->CloseRequest(m_requestChallengeQuestionsRequestId);
	m_restConnector->CloseRequest(m_submitChallengeAnswerRequestId);
}

void ChallengeDataProviderLive::RequestChallengeId() {
	RequestBuilder requestBuilder;
	requestBuilder.AddUser(IEngine::getEngine()->GetUserProvider()->GetUser());
	requestBuilder.SetEndpoint(requests::NEW_CHALLENGE);

	std::string requestString = requestBuilder.GetRequestString();
	m_requestChallengeIdRequestId = m_restConnector->SendRequest(requestString, this);
}

void ChallengeDataProviderLive::ChallengeIdReceived(int challengeId) {
	m_challengeDataProviderReceiver->ChallengeIdReceived(challengeId);
}

void ChallengeDataProviderLive::ChallengeTimerReceived(int timerSeconds) {
    m_challengeDataProviderReceiver->ChallengeTimerReceived(timerSeconds);
}

void ChallengeDataProviderLive::RequestChallengeQuestions(int challengeId) {
	RequestBuilder requestBuilder;
	requestBuilder.AddUser(IEngine::getEngine()->GetUserProvider()->GetUser());
	requestBuilder.SetEndpoint(requests::GET_CHALLENGE_QUESTIONS);
	requestBuilder.AddParameter("attempt_id", std::to_string(challengeId));

	std::string requestString = requestBuilder.GetRequestString();
	m_requestChallengeQuestionsRequestId = m_restConnector->SendRequest(requestString, this);
}

void ChallengeDataProviderLive::ChallengeQuestionsReceived(const std::queue<Question>& questions) {
	m_challengeDataProviderReceiver->ChallengeQuestionsReceived(questions);
}

void ChallengeDataProviderLive::SubmitChallengeAnswer(int challengeId, int answerId) {
	RequestBuilder requestBuilder;
	requestBuilder.AddUser(IEngine::getEngine()->GetUserProvider()->GetUser());
	requestBuilder.SetEndpoint(requests::REGISTER_CHALLENGE_ANSWER);

	requestBuilder.AddParameter("attempt_id", std::to_string(challengeId));
	requestBuilder.AddParameter("answer_id", std::to_string(answerId));

	std::string requestString = requestBuilder.GetRequestString();
	m_submitChallengeAnswerRequestId = m_restConnector->SendRequest(requestString, this);
}

void ChallengeDataProviderLive::RestReceived(const std::string& rest) {
	json11::Json json = JsonProvider::ParseString(rest);
	std::string request = json["request"].string_value();

	if (request == requests::NEW_CHALLENGE) {
        ChallengeTimerReceived(json["payload"]["challenge_mode_timer_length"].int_value());
		ChallengeIdReceived(json["payload"]["attempt_id"].int_value());
	} else if (request == requests::GET_CHALLENGE_QUESTIONS) {
		std::queue<Question> questions = JsonToQuestions(json["payload"]);
		ChallengeQuestionsReceived(questions);
	}
}

std::queue<Question> ChallengeDataProviderLive::JsonToQuestions(const json11::Json& json) {
	auto questions = json.array_items();

	std::queue<Question> questionsObj;
	for (auto& question : questions) {
		int questionId = question["id"].int_value();
		std::string questionText = question["question"].string_value();

		std::vector<Answer> wrongAnswers;

		int answerId = -1;
		std::string answerText;
		bool answerCorrect(false);
		//get wrong answer
		auto wrongAnswersJson = question["incorrectAnswers"].array_items();
		for (auto& wrongAnswerJson : wrongAnswersJson) {
			answerId = wrongAnswerJson["id"].int_value();
			answerText = wrongAnswerJson["answer"].string_value();
			answerCorrect = wrongAnswerJson["correct"].bool_value();

			Answer wrongAnswer(answerId, answerText, answerCorrect);
			wrongAnswers.push_back(wrongAnswer);
		}

		//get correct answer
		answerId = question["correctAnswer"]["id"].int_value();
		answerText = question["correctAnswer"]["answer"].string_value();
		answerCorrect = question["correctAnswer"]["correct"].bool_value();

		Answer correctAnswer(answerId, answerText, answerCorrect);

		Question questionObj(questionId, questionText, wrongAnswers, correctAnswer);
		questionsObj.push(questionObj);
	}

	return questionsObj;
}
