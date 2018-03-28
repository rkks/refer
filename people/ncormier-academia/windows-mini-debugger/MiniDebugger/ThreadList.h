#pragma once

namespace MiniDebugger
{
	class Thread;
}

namespace MiniDebugger
{
	class ThreadList
	{
		// Static Members, return the first node.
		static ThreadList*	CreateList(Thread*);
		static ThreadList*	Add(ThreadList*, Thread*);
		static ThreadList*	Del(ThreadList*, Thread*);

		// Object Members
	public:
		ThreadList(MiniDebugger::Thread*);
		bool						IsLastNode();
		MiniDebugger::ThreadList*	GetNext();
	public:
		~ThreadList(void);
	private:
		MiniDebugger::ThreadList* next;
		MiniDebugger::ThreadList* prev;
		MiniDebugger::Thread* me;
		bool last;
	};
}