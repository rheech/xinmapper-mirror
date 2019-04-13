#pragma once
#include "stdafx.h"

namespace sds
{
	class TriggerAction
	{
		PlayerInfo *player;
	public:
		bool Down(const XINPUT_STATE &state, std::string token){
			if( token == sds::sdsActionDescriptors.lTrigger ){
				if( state.Gamepad.bLeftTrigger > sds::sdsPlayerOne.left_trigger_dz ){
					return true;
				}
			}
			if( token == sds::sdsActionDescriptors.rTrigger ){
				if( state.Gamepad.bRightTrigger > sds::sdsPlayerOne.right_trigger_dz ) {
					return true;
				}
			}
			return false;
		}
	};
}