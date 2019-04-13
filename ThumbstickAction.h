#pragma once
#include "stdafx.h"

namespace sds
{
	class ThumbstickAction
	{
	public:
		bool Down(const XINPUT_STATE &state, std::string token){
			//No string in switch, unfortunate.
			std::string temp = sds::sdsActionDescriptors.lThumb + sds::sdsActionDescriptors.moreInfo;
			if( token == temp + sds::sdsActionDescriptors.left )
			{
				//Test lThumb left.
				if( state.Gamepad.sThumbLX < (-sds::sdsPlayerOne.left_dz) )
					return true;
			}
			else if( token == temp + sds::sdsActionDescriptors.right )
			{
				if( state.Gamepad.sThumbLX > sds::sdsPlayerOne.left_dz )
					return true;
			}
			else if( token == temp + sds::sdsActionDescriptors.down )
			{
				if( state.Gamepad.sThumbLY < (-sds::sdsPlayerOne.left_dz) )
					return true;
			}
			else if( token == temp + sds::sdsActionDescriptors.up )
			{
				if( state.Gamepad.sThumbLY > sds::sdsPlayerOne.left_dz )
					return true;
			}
			temp = sds::sdsActionDescriptors.rThumb + sds::sdsActionDescriptors.moreInfo;
			//Right thumbstick.
			if( token == temp + sds::sdsActionDescriptors.left )
			{
				//Test lThumb left.
				if( state.Gamepad.sThumbRX < (-sds::sdsPlayerOne.left_dz) )
					return true;
			}
			else if( token == temp + sds::sdsActionDescriptors.right )
			{
				if( state.Gamepad.sThumbRX > sds::sdsPlayerOne.left_dz )
					return true;
			}
			else if( token == temp + sds::sdsActionDescriptors.down )
			{
				if( state.Gamepad.sThumbRY < (-sds::sdsPlayerOne.left_dz) )
					return true;
			}
			else if( token == temp + sds::sdsActionDescriptors.up )
			{
				if( state.Gamepad.sThumbRY > sds::sdsPlayerOne.left_dz )
					return true;
			}
			return false;
		}
	};

}