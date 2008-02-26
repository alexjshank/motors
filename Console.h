#include "task.h"
#include <list>
#include <vector>
#ifndef _CONSOLE_
#define _CONSOLE_

class Console :
	public Task
{
public:
	Console(void);
	~Console(void);

	bool init();
	void run();

	void RunLine(const char *line);
	void RunLinef(const char *format, ...);
	bool LoadScript(const char *scriptFilename);

	void ProcessInput(char input);
	void ProcessLine();
	void Toggle() { toggled = !toggled; }

	void Print(const char *line);
	void Printf(const char *format, ...);

	bool enabled;
	bool toggled;
	std::string currentline;
	std::vector<std::string> history;
};

#endif