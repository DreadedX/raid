#pragma once
//----------------------------------------------
#include <iostream>
//----------------------------------------------
class Logger {
	public:
		Logger(std::string m_tag) : tag(m_tag) {}

		template<class T>
		Logger &operator<<(T value) {
			if (tag.length() > 0) {
				std::cout << "[" << tag << "] ";
			}
			std::cout << value;
		}

	private:
		std::string tag;
};
//----------------------------------------------
extern Logger debug;
extern Logger log;
extern Logger error;
//----------------------------------------------
