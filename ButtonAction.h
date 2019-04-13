#pragma once
#include "stdafx.h"

namespace sds
{
	class ButtonAction
	{
	public:
		bool Down(const XINPUT_STATE &state, std::string token)
		{
			for(auto it = sds::sdsActionDescriptors.xin_buttons.begin(); it != sds::sdsActionDescriptors.xin_buttons.end(); ++it)
			{
				if( it->first == token )
				{
					if( state.Gamepad.wButtons & it->second )
						return true;
				}
			}
			return false;
		}
	};
}