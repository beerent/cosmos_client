#include <Core/GameLogic/Question/Question.h>
#include <chrono>
#include <random>

Question::Question(int questionId, std::string question, std::vector<Answer> wrongAnswers, Answer correctAnswer) : m_questionId(questionId), m_question(question), m_wrongAnswers(wrongAnswers), m_correctAnswer(correctAnswer) {}

int Question::GetQuestionId() const {
	return m_questionId;
}
std::string Question::GetQuestion() const {
	return m_question;
}

std::vector<Answer> Question::GetWrongAnswers() const {
	return m_wrongAnswers;
}

std::vector<Answer> Question::GetRandomizedAnswers() const {
	std::vector<Answer> randomAnswers;
	std::vector<Answer> orderedAnswers = GetWrongAnswers();
	
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 generator(seed);

	while (randomAnswers.size() !=3 && orderedAnswers.size() != 0) {
		int randomNumber = (generator() % (orderedAnswers.size()));
		randomAnswers.push_back(orderedAnswers[randomNumber]);
		auto it = orderedAnswers.begin() + randomNumber;
		orderedAnswers.erase(it);
	}
	
	int randomNumber = (generator() % (randomAnswers.size() + 1));
	randomAnswers.insert(randomAnswers.begin() + randomNumber, GetCorrectAnswer());

	return randomAnswers;
}

Answer Question::GetCorrectAnswer() const {
	return m_correctAnswer;
}

bool Question::IsCorrectAnswer(int id) const {
	return id == m_correctAnswer.GetId();
}