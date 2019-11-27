#pragma once

class IEntityAnimator
{
public:
	virtual void Play() = 0;
	virtual void Stop() = 0;
	virtual void Pump(float delta) = 0;
};