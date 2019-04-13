#pragma once

#include "InputPoller.h"
#include "Mapper.h"
#include "XInputTranslater.h"
#include "XInputBoostMouse.h"

namespace sds
{
	class GamepadUser
	{
	public:
		InputPoller *poller;
		Mapper *mapper;
		XInputTranslater *trans;
		XInputBoostMouse *mouse;
	public:
		GamepadUser()
		{
			mapper = new Mapper();
			trans = new XInputTranslater();
			mouse = new XInputBoostMouse();
			poller = new InputPoller(mapper,trans,mouse);
		}
		~GamepadUser()
		{
			poller->Stop();
			delete poller;
			delete mapper;
			delete trans;
			delete mouse;
		}
	};
}