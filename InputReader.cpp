#include "InputReader.h"
#include <iostream>

void InputReader::Run()
{
	std::cout << "|Start input Reader thread" << std::endl;
	std::string line;
	while (m_Run)
	{
		std::cin >> line;
		m_Mutex.lock();
		m_Line = line;
		m_Mutex.unlock();
		m_ReadInput.notify_all();
		if (line == "q")
		{
			m_Run = false;
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	}
}

bool InputReader::WaitForInputMT(std::string& output, uint32_t milliseconds)
{
	std::unique_lock<std::mutex> lck(m_Mutex);
	if (m_ReadInput.wait_for(lck, std::chrono::milliseconds(milliseconds)) == std::cv_status::timeout)
	{
		if (m_Line.empty())
		{
			return false;
		}
		output = m_Line;
		m_Line.clear();
		return true;
	}

	output = m_Line;
	m_Line.clear();

	return true;
}

