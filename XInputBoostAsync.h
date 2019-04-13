/*
Base class for async operations.
*/
#pragma once
#include "stdafx.h"

namespace sds
{
	class XInputBoostAsync
	{
		typedef boost::mutex::scoped_lock lock;
	public:
		boost::shared_ptr<boost::thread> thread;
	protected:
		volatile bool isThreadRunning;
		volatile bool isStopRequested;
		XINPUT_STATE local_state;
		boost::mutex stateMutex;

	protected://member functions
		void startThread()
		{
			if( ! this->isThreadRunning )
			{
				this->isStopRequested =(false);
				this->isThreadRunning =(true);
				this->thread = boost::shared_ptr<boost::thread>
					(new boost::thread(boost::bind(&XInputBoostAsync::workThread,this)));
			}
		}

		void requestStop()
		{
			if( this->thread != nullptr ){
				if(! this->isThreadRunning )
					return;
				this->isStopRequested =(true);
			}
		}

		void stopThread()
		{
			if(this->thread != nullptr)
			{
				if(! this->isThreadRunning )
					return;
				this->isStopRequested =(true);
				this->thread->join();
				this->isThreadRunning =(false);
			}
		}

		void updateState(const XINPUT_STATE &state) 
		{
			lock(stateMutex);
			local_state = state;
		}

		XINPUT_STATE getCurrentState()
		{
			lock(stateMutex);
			return local_state;
		}
		
		virtual void workThread() = 0;//<<-- thread to be running.

	public://public member functions
		XInputBoostAsync()
		{
			isThreadRunning = false;
			isStopRequested = false;
			lock(stateMutex);
			memset(&local_state,0,sizeof(XINPUT_STATE));
		}
		
		//This function does block when it is called by default.
		virtual ~XInputBoostAsync()
		{
			if( this->thread != nullptr )
			{
				this->isStopRequested = true;
				this->thread->join();
			}
		}
		virtual void ProcessState(const XINPUT_STATE &state) = 0;
		virtual void HaltOperation() = 0;
	};
}