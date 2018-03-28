#include "StdAfx.h"
#include "ThreadList.h"

namespace MiniDebugger
{

	ThreadList::ThreadList(MiniDebugger::Thread* me)
	{
		this->me = me;
	}

	ThreadList::~ThreadList(void)
	{
	}

	bool ThreadList::IsLastNode()
	{
		return (this->last);
	}

	MiniDebugger::ThreadList*	ThreadList::GetNext()
	{
		return (this->next);
	}

	// ------------------------------------------------------
	// ThreadList Manager.
	// ------------------------------------------------------
	//
	ThreadList*	ThreadList::CreateList(Thread* thread)
	{
		ThreadList* threadList = new ThreadList(thread);
		threadList->last = true;
		threadList->me = thread;
		threadList->next = threadList;
		threadList->prev = threadList;
		return (threadList);
	}
	
	ThreadList*	ThreadList::Add(ThreadList* threadList, Thread* thread)
	{
		ThreadList* tlNew = new ThreadList(thread);
		tlNew->me = thread;

		ThreadList* tlLast = threadList;

		while (!tlLast->IsLastNode())
		{
			tlLast = tlLast->next;
		}

		tlLast->last = false;
		tlLast->next = tlNew;

		tlNew->last = true;
		tlNew->next = threadList;
		tlNew->prev = tlLast;

		return (threadList);
	}
	
	ThreadList*	ThreadList::Del(ThreadList* threadList, Thread* thread)
	{
		ThreadList* tlDel = threadList;

		while (!tlDel->IsLastNode() && tlDel->me != thread)
		{
			tlDel = tlDel->next;
		}

		tlDel->prev->next = tlDel->next->prev;
		if (tlDel->last)
			tlDel->prev->last = true;

		ThreadList* ret = threadList;
		if (ret == tlDel)
		{
			ret = tlDel->next;
			if (ret == tlDel)
				ret = NULL;
		}

		delete (tlDel);

		return (ret);
	}
	//
	// ------------------------------------------------------
	// ------------------------------------------------------

}