#include ".\task.h"

Task::Task() {
	state = 0;
}

Task::~Task() {
}

Kernel::Kernel() {
	tasks.clear();
}

Kernel::~Kernel() {
}

bool Kernel::AddTask(Task *tNewTask) {
	tasks.push_back(tNewTask);
	return true;
}

bool Kernel::Init() {
	bool status = true;
	int s = (int)tasks.size();
	for (int i=0;i<s;i++) {
		status &= tasks[i]->init();
	}		
	return status;
}

void Kernel::Run() {
	int s = (int)tasks.size();
	for (int i=0;i<s;i++) {
		tasks[i]->run();
	}
}

void Kernel::Pause(int minPriority) {
	int s = (int)tasks.size();
	for (int i=0;i<s;i++) {
		if (tasks[i]->priority >= minPriority) {
			tasks[i]->pause();
		}
	}
}

bool Kernel::Stop() {
	bool status = true;
	int s = (int)tasks.size();
	for (int i=0;i<s;i++) {
		status &= tasks[i]->stop();
	}		
	return status;
}