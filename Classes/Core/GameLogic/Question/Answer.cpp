#include <Core/GameLogic/Question/Answer.h>

Answer::Answer(int answerId, std::string answer, bool correct) : m_answerId(answerId), m_answer(answer), m_correct(correct) {}

std::string Answer::GetAnswer() const {
	return m_answer;
}

int Answer::GetId() const {
	return m_answerId;
}

bool Answer::IsCorrect() const {
	return m_correct;
}