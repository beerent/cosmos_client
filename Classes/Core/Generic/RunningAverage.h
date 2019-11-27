#ifndef RUNNING_AVERAGE
#define RUNNING_AVERAGE
#include <vector>

class RunningAverage {
public:
	RunningAverage(unsigned int samples, float clampValue) : m_count(samples),
		m_clampValue(clampValue), m_rollingIndex(0) {
		m_samples.reserve(m_count);
	}

	float average(float sample) {
		float cappedTimeStep = sample > m_clampValue ? m_clampValue : sample;

		if (m_samples.size() < m_count) {
			m_samples.push_back(cappedTimeStep);
		}
		else
		{
			m_samples[m_rollingIndex] = cappedTimeStep;
			m_rollingIndex++;
			if (m_rollingIndex >= m_count) {
				m_rollingIndex = 0;
			}
		}

		float totalTime = 0;
		for (int i = 0; i < m_samples.size(); ++i)
		{
			totalTime += m_samples[i];
		}

		return totalTime / m_samples.size();
	}

private:
	std::vector<float> m_samples;
	float m_clampValue;
	unsigned int m_count;
	unsigned int m_rollingIndex;
};

#endif
