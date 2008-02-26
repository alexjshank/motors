#pragma once

#include <vector>

enum TaskPriority {
	TP_ENGINE = 10,
	TP_GAME = 5
};

class Task
{
	friend class Kernel;
public:
	Task(void);
	~Task(void);

	virtual bool init() { return true; };
	virtual void pause() { };
	virtual bool stop() { return true; };

private:
	virtual void run() {};

	int state;		
	int priority;
};

class Kernel 
{
public:
	Kernel();
	~Kernel();

	bool AddTask(Task *tNewTask);
	bool Init();
	void Run();
	void Pause(int minPriority);
	bool Stop();

private:
	std::vector<Task *> tasks;
};