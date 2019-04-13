/*
Further design considerations would include
interpreting an ActionString,
and to allow mapping with the MapString.
Which would mean updating ActionDescriptors to include the new
tokens, and updating this class to interpret an ActionString,
and finally update GamepadUser.

More importantly a better measure of sensitivity should
be used rather than an arbitrary value.  One may get the 
impression Microsoft hath done the same and used a "slider"
for their sensitivity to hide this fact.

Finally, the ability to modify the sensitivity function
is of utmost importance, which may require a math expression
parser.  The class currently uses the graph of [ y = (x^2) / SENSITIVITY ]
which is a parabola with the quality that at the curve's highest points
it is the nearly (or exactly) the size of the viewing window--the SENSITIVITY.
Also need sensitivity for both X and Y axis.
*/
#pragma once
#include "stdafx.h"
#include "XInputBoostAsync.h"

namespace sds
{
	class XInputBoostMouse : public XInputBoostAsync
	{
		typedef boost::mutex::scoped_lock lock;
	public:
		enum MouseMap
		{
			NEITHER_STICK,
			RIGHT_STICK,
			LEFT_STICK
		} volatile stickMapInfo;
	private:
		boost::mutex state_mutex;

		XINPUT_STATE local_state;
		INPUT data;

		volatile SHORT threadX, threadY;
		volatile LONG step;
		volatile int SENSITIVITY;
		volatile bool is_left_stick;

		static const int MOVE_THREAD_SLEEP = 10;//10 ms
		//static const int PRECISION_RANGE_PERCENT = 15;//15%
		//static const int SLEEP_PRECISION = 15;//15 ms
	public:
		XInputBoostMouse() 
			: XInputBoostAsync(),
			SENSITIVITY(30),
			stickMapInfo(NEITHER_STICK)
		{
			memset((LPVOID)&data,0, sizeof(INPUT));
			data.mi.dwFlags = MOUSEEVENTF_MOVE;
		}
		~XInputBoostMouse()
		{ 
			//this->HaltOperation(); 
		}
		void EnableProcessing(MouseMap info)
		{
			stickMapInfo = info;
		}
		void ProcessState(const XINPUT_STATE &state)
		{
			if(stickMapInfo == NEITHER_STICK)
				return;
			int tsx, tsy;
		
			if(stickMapInfo == RIGHT_STICK)
			{
				tsx = state.Gamepad.sThumbRX;
				tsy = state.Gamepad.sThumbRY;
			}
			else
			{
				tsx = state.Gamepad.sThumbLX;
				tsy = state.Gamepad.sThumbLY;
			}
			//Give worker thread new values.
			threadX = tsx;
			threadY = tsy;

			if( doesRequireMove(tsx,tsy) )
			{
				//update state.
				this->updateState(state);

				//check for thread is running.
				if( !isThreadRunning )
					this->startThread();
			}
		}
		void HaltOperation() 
		{
			throw sds::ERR1;
		}
		void SetSensitivity(int new_sens)
		{
			if(new_sens <= 0)
				return;
			SENSITIVITY = new_sens;
		}
		int GetSensitivity()
		{
			return SENSITIVITY;
		}
	private:
		bool doesRequireMove(int x, int y)
		{
			int DEADZONE = stickMapInfo == LEFT_STICK ? sds::sdsPlayerOne.left_dz : sds::sdsPlayerOne.right_dz;
			return ( (x > DEADZONE
				|| x < -DEADZONE)
				|| (y > DEADZONE
				|| y < -DEADZONE) );
		}

		void workThread()
		{
			while(!isStopRequested)//<--Danger! From the base class.
			{
				//attempt do input.
				SHORT tx = threadX;
				SHORT ty = threadY;
				doInput(tx, ty);
				Sleep(MOVE_THREAD_SLEEP);
				//Optionally sleep longer for a certain range.
				//int ranval = std::numeric_limits<SHORT>::max()/PRECISION_RANGE_PERCENT;//% of range.

				//static auto precise_sleep_func = [&ranval] (int x)
				//{
				//	if( x < ranval )
				//	{
				//		Sleep(SLEEP_PRECISION);
				//	}
				//};
				//tx = abs(tx);
				//ty = abs(ty);
				//precise_sleep_func( tx > ty ? tx : ty );
			}
		
			//mark thread status as not running.
			isThreadRunning = false;
		}

		LONG NormalizeAxis( LONG x )
		{
			int t_sens = SENSITIVITY;

			if( x > 0 )
			{
					x -= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
					x = NormalizeRange( 0, SHRT_MAX,
						(LONG)t_sens, x );
			}
			else
			{
					x += XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
					x = -NormalizeRange( 0, SHRT_MAX,
						(LONG)t_sens, -x );
			}
			return x;
		}
		 
		//THIS FUNCTION ONLY WORKS WITH POSITIVE VALUES
		//IF YOU NEED A NEGATIVE ONE NEGATE THE NEG. VALUES
		//AND NEGATE THE RESULT.
		//begin = range begin
		//end = range end
		//rp = range partition value
		//val = val
		//returns: rp of val
		LONG NormalizeRange( LONG begin, LONG end, LONG rp, LONG val )
		{
			step = (end-begin)/rp;
			for(LONG i = begin,j=1; i <= end; i+=step,++j)
			{
				if( val >= i && val < i+step )
				{
					if(val == end)
						return j-1;
					return j;
				}
			}
			return 0;
		}

		//state mutex is locked.
		void doInput(LONG x, LONG y)
		{
			data.mi.dx = 0;
			data.mi.dy = 0;
			data.mi.dwExtraInfo = GetMessageExtraInfo();
			
			LONG x_value;
			LONG y_value;
			x_value = NormalizeAxis(x);

			//Have to be careful inverting signs,
			//can overflow easily!
			if( y <= SHRT_MIN )
			{
				y=SHRT_MIN+1;
			}
			//here y is inverted, to invert the y axis.
			y_value = NormalizeAxis(-y);
	
			if( x_value != 0 )
			{
				if( x_value > 0 )
				{
					//data.mi.dx = 1;//+t_sens;
					data.mi.dx = getFunctionalValue(x_value);
				}
				else
				{
					//data.mi.dx = -1;//-t_sens;
					data.mi.dx = -(LONG)getFunctionalValue(abs(x_value));
				}
			}

			if( y_value != 0 )
			{
				if( y_value > 0 )
				{
					//data.mi.dy = 1;//+t_sens;
					data.mi.dy = getFunctionalValue(y_value);
				}
				else
				{
					//data.mi.dy = -1;//-t_sens;
					data.mi.dy = -(LONG)getFunctionalValue(abs(y_value));
				}
			}
			x_value = abs(x_value);
			y_value = abs(y_value);

			SendInput(1, &data, sizeof(INPUT));
		}

		size_t getFunctionalValue(size_t x)
		{
			//x = static_cast<size_t>(4*x / SENSITIVITY +1);
			x = static_cast<size_t>( (x*x)/SENSITIVITY +1 );
			//x = (size_t)ceil(((1.0/8.0)*pow((double)x,2))/2.0);
			return (x == 0) ? 1 : x;
		}

	};
}