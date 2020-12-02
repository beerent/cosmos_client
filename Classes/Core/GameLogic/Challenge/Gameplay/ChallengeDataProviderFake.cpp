#include <Core/GameLogic/Challenge/Gameplay/ChallengeDataProviderFake.h>
#include <Core/GameLogic/Question/Question.h>
#include <Core/GameLogic/Question/Answer.h>

#include <queue>
#include <vector>

void ChallengeDataProviderFake::RequestChallengeId() {
	ChallengeIdReceived(1);
}

void ChallengeDataProviderFake::ChallengeIdReceived(int challengeId) {
	m_challengeDataProviderReceiver->ChallengeIdReceived(challengeId);
}

void ChallengeDataProviderFake::ChallengeTimerReceived(int timerSeconds) {
}

void ChallengeDataProviderFake::RequestChallengeQuestions(int challengeId) {
	std::queue<Question> questions;

	Answer correctAnswer1(1, "1", true);
	Answer wrongAnswer1_1(2, "2", false);
	Answer wrongAnswer1_2(3, "3", false);
	Answer wrongAnswer1_3(4, "4", false);
	std::vector<Answer> wrongAnswers1;
	wrongAnswers1.push_back(wrongAnswer1_1);
	wrongAnswers1.push_back(wrongAnswer1_2);
	wrongAnswers1.push_back(wrongAnswer1_3);
	Question question1(1, "How many moons does Earth have?", wrongAnswers1, correctAnswer1);
	questions.push(question1);

	Answer correctAnswer2(5, "2", true);
	Answer wrongAnswer2_1(6, "1", false);
	std::vector<Answer> wrongAnswers2;
	wrongAnswers2.push_back(wrongAnswer2_1);
	Question question2(2, "How many moons does Mars have?", wrongAnswers2, correctAnswer2);
	questions.push(question2);

	Answer correctAnswer3(9, "Jupiter", true);
	Answer wrongAnswer3_1(10, "Uranus", false);
	Answer wrongAnswer3_2(11, "Saturn", false);
	Answer wrongAnswer3_3(12, "Mercury", false);
	std::vector<Answer> wrongAnswers3;
	wrongAnswers3.push_back(wrongAnswer3_1);
	wrongAnswers3.push_back(wrongAnswer3_2);
	wrongAnswers3.push_back(wrongAnswer3_3);
	Question question3(3, "The moon IO orbits around which planet?", wrongAnswers3, correctAnswer3);
	questions.push(question3);

	Answer correctAnswer4(13, "Saturn", true);
	Answer wrongAnswer4_1(14, "Uranus", false);
	Answer wrongAnswer4_2(15, "Jupiter", false);
	Answer wrongAnswer4_3(16, "Mercury", false);
	std::vector<Answer> wrongAnswers4;
	wrongAnswers4.push_back(wrongAnswer4_1);
	wrongAnswers4.push_back(wrongAnswer4_2);
	wrongAnswers4.push_back(wrongAnswer4_3);
	Question question4(4, "The moon Enceladus orbits around which planet?", wrongAnswers4, correctAnswer4);
	questions.push(question4);

	Answer correctAnswer5(17, "Venus", true);
	Answer wrongAnswer5_1(18, "Neptune", false);
	Answer wrongAnswer5_2(19, "Jupiter", false);
	Answer wrongAnswer5_3(20, "Mercury", false);
	std::vector<Answer> wrongAnswers5;
	wrongAnswers5.push_back(wrongAnswer5_1);
	wrongAnswers5.push_back(wrongAnswer5_2);
	wrongAnswers5.push_back(wrongAnswer5_3);
	Question question5(5, "Which planet is considered Earth's 'Sister Planet'?", wrongAnswers5, correctAnswer5);
	questions.push(question5);

	ChallengeQuestionsReceived(questions);
}

void ChallengeDataProviderFake::ChallengeQuestionsReceived(const std::queue<Question>& questions) {
	m_challengeDataProviderReceiver->ChallengeQuestionsReceived(questions);
}

void ChallengeDataProviderFake::SubmitChallengeAnswer(int challengeId, int answerId) {

}
