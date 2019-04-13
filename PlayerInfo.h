/*
Holds program information for the specific player,
like deadzone information.
*/
#pragma once
namespace sds
{
	struct PlayerInfo
	{
		int left_dz;
		int right_dz;
		int left_trigger_dz;
		int right_trigger_dz;
		int player_id;
		unsigned x_mouse_pps;
		unsigned y_mouse_pps;
		PlayerInfo() 
			: left_dz(XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE),
			right_dz(XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE),
			player_id(0),
			left_trigger_dz(XINPUT_GAMEPAD_TRIGGER_THRESHOLD),
			right_trigger_dz(XINPUT_GAMEPAD_TRIGGER_THRESHOLD),
			x_mouse_pps(1000u), y_mouse_pps(1000u)
		{}
	};
}