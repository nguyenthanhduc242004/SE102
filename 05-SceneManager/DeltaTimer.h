#pragma once
class CDeltaTimer
{
	float accumulated = 0.0f;   // in milliseconds
public:
	void Start() { accumulated = 0.00001; }
	void Reset() { accumulated = 0.0f; }
	void Tick(float dt) { if (accumulated >= 0.0f) accumulated += dt; }
	bool IsRunning() { return accumulated > 0.0f; }
	bool HasPassed(float ms) const {
		return accumulated >= ms;
	}
	float getAccumulated() { return accumulated; }
};

