#pragma once
#include "stdafx.h"
#include "Mapper.h"
#include "XInputTranslater.h"
#include "XInputBoostMouse.h"

namespace sds
{
	class InputPoller : public XInputBoostAsync
	{
		const int THREAD_DELAY;
		Mapper *m;
		XInputTranslater *t;
		XInputBoostMouse *mse;

	protected://member functions
		virtual void workThread()
		{
			while( ! this->isStopRequested )
			{
				DWORD error = XInputGetState(sds::sdsPlayerOne.player_id, &local_state);
				if( error != ERROR_SUCCESS )
					break;
				mse->ProcessState(local_state);
				m->ProcessActionDetails( t->ProcessState(local_state) );
				Sleep(THREAD_DELAY);
			}
			this->isThreadRunning = false;
		}

	public://public member functions
		InputPoller(Mapper *mapper, XInputTranslater *trans, XInputBoostMouse *mouse)
			: XInputBoostAsync(), THREAD_DELAY(10), m(mapper), t(trans), mse(mouse)
		{
		}

		bool Start()
		{
			this->startThread();
			return this->isThreadRunning;
		}

		bool Stop()
		{
			this->stopThread();
			return this->isThreadRunning;
		}
		bool IsRunning()
		{
			return this->isThreadRunning;
		}
		bool IsControllerConnected()
		{
			XINPUT_STATE ss = {0};
			return XInputGetState(sds::sdsPlayerOne.player_id, &ss) == ERROR_SUCCESS;
		}
		void ProcessState(const XINPUT_STATE &state)
		{
			throw sds::ERR1;
		}
		void HaltOperation()
		{
			throw sds::ERR1;
		}
	};

}

//class InputPoller
//{
//	static const int THREAD_DELAY;
//	static volatile bool gamepadConnected;// <-- Not yet implemented.
//	static volatile long running;
//	static volatile long stopRequested;
//	static volatile uintptr_t hThread;
//	static XINPUT_STATE localState;
//	static XInputTranslater *tran;
//	static Mapper *mapper;
//	static PlayerInfo *player;
//	static XInputBoostMouse *mouse;
//public:
//	InputPoller(PlayerInfo *p, Mapper *m, XInputTranslater *t, XInputBoostMouse *mse)
//		//: player(p), mapper(m), tran(t), mouse(mse)
//	{
//		//This is simply a bit of an abstraction,
//		//the objects are assumed to exist.
//		player = p;
//		mapper = m;
//		tran = t;
//		mouse = mse;
//		assert( mapper != NULL );
//		assert( player != NULL );
//		assert( tran != NULL );
//		assert( mouse != NULL );
//		running = FALSE;
//		stopRequested = FALSE;
//		gamepadConnected = false;
//		hThread = 0;
//		memset(&localState,0,sizeof(XINPUT_STATE));
//	}
//
//	//Start returns true as long as the thread is running after it returns.
//	bool Start()
//	{
//		if( ::_InterlockedCompareExchange(&running,TRUE,TRUE) )
//			return true;
//		::_InterlockedExchange(&stopRequested,FALSE);
//		::_InterlockedExchange(&running,TRUE);
//		hThread = _beginthread(&InputPoller::ThreadFunc,0,NULL);
//		return hThread != -1L;
//	}
//	void Stop()
//	{
//		::_InterlockedExchange(&stopRequested,TRUE);
//		::WaitForSingleObject((HANDLE)hThread,INFINITE);
//		::_InterlockedExchange(&stopRequested,FALSE);
//	}
//	BOOL IsRunning()
//	{
//		return (::_InterlockedCompareExchange(&running,FALSE,FALSE));
//	}
//private:
//	//void WorkerFunction()
//	//{
//
//	//}
//	static void ThreadFunc(void *args)
//	{
//		HANDLE timer = ::CreateWaitableTimerA(NULL,FALSE, "XInmapper Thread Func Timer");
//		do
//		{
//			DWORD error = XInputGetState(player->player_id,&localState);
//			if( error != ERROR_SUCCESS )
//			{
//				break;
//			}
//			mouse->ProcessState(localState);
//			mapper->ProcessActionDetails( tran->ProcessState(localState) );
//			::WaitForSingleObject(timer,THREAD_DELAY);
//		}while( ! ::_InterlockedCompareExchange(&stopRequested,FALSE,FALSE) );
//		
//		::CloseHandle(timer);
//		::_InterlockedExchange(&running,FALSE);
//		_endthread();
//	}
//};
//
