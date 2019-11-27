#pragma once

#include <string>

class Answer {
public:
	Answer(int answerId, std::string answer, bool correct);
	std::string GetAnswer() const;
	int GetId() const;
	bool IsCorrect() const;

private:
	int m_answerId;
	std::string m_answer;
	bool m_correct;
};