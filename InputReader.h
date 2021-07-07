#pragma once
// demo code by J. Kaltenmark

#include "EngineThread.h"
#include <string>
#include <iostream>

class InputReader : public EngineThread
{
public:
	InputReader() : EngineThread("inputReader") {}
	virtual ~InputReader() {}

	void Run();
	bool WaitForInputMT(std::string& output, uint32_t milliseconds = 100);

private:
	mutable std::mutex m_Mutex;
	std::condition_variable m_ReadInput;
	std::string m_Line;
};
