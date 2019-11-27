#pragma once

#include <string>
#include <vector>

#include <Core/GameLogic/Question/Answer.h>

class Question {
public:
	Question(int questionId, std::string question, std::vector<Answer> wrongAnswers, Answer correctAnswer);
	int GetQuestionId() const;
	std::string GetQuestion() const;
	std::vector<Answer> GetWrongAnswers() const;
	std::vector<Answer> GetRandomizedAnswers() const;
	Answer GetCorrectAnswer() const;
	bool IsCorrectAnswer(int id) const;

private:
	int m_questionId;
	std::string m_question;
	std::vector<Answer> m_wrongAnswers;
	Answer m_correctAnswer;
};