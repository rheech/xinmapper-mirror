/*
Responsible for tokens used in an ActionDetails
string.
Inevitably tokens must be mapped to values (some of them).
*/

#pragma once
#include "stdafx.h"

namespace sds
{
	struct ActionDescriptors
	{
		std::string x, y, a, b;
		std::string lThumb, rThumb;
		std::string lTrigger, rTrigger;
		std::string lShoulder, rShoulder;
		std::string dpad;
		std::string left, down, up, right, none;
		std::string start, back;
		std::string vk;
		char moreInfo;
		//std::string moreInfo;
		//std::string delimiter
		char delimiter;
		std::string mappedTo;
		std::string norm, toggle, rapid;

		std::map<std::string,int> xin_buttons;
		
		// Kind of like using this class to hold a bunch of constants
		// C++ won't allow me to create.
		ActionDescriptors()
		{
			x = "X";y = "Y"; a="A"; b="B";
			lThumb = "LTHUMB"; rThumb = "RTHUMB";
			lTrigger = "LTRIGGER"; rTrigger = "RTRIGGER";
			lShoulder = "LSHOULDER"; rShoulder = "RSHOULDER";
			dpad = "DPAD";
			left = "LEFT"; down = "DOWN"; up = "UP"; right = "RIGHT"; none = "NONE";
			start = "START"; back = "BACK";
			norm = "NORM"; toggle = "TOGGLE"; rapid = "RAPID";
			vk = "VK";
			moreInfo = ':';
			delimiter = ' ';
			mappedTo = "=";
			xin_buttons[x] = XINPUT_GAMEPAD_X;
			xin_buttons[y] = XINPUT_GAMEPAD_Y;
			xin_buttons[a] = XINPUT_GAMEPAD_A;
			xin_buttons[b] = XINPUT_GAMEPAD_B;
			xin_buttons[lShoulder] = XINPUT_GAMEPAD_LEFT_SHOULDER;
			xin_buttons[rShoulder] = XINPUT_GAMEPAD_RIGHT_SHOULDER;
			xin_buttons[dpad + moreInfo + left] = XINPUT_GAMEPAD_DPAD_LEFT;
			xin_buttons[dpad + moreInfo + right] = XINPUT_GAMEPAD_DPAD_RIGHT;
			xin_buttons[dpad + moreInfo + up] = XINPUT_GAMEPAD_DPAD_UP;
			xin_buttons[dpad + moreInfo + down] = XINPUT_GAMEPAD_DPAD_DOWN;
			xin_buttons[start] = XINPUT_GAMEPAD_START;
			xin_buttons[back] = XINPUT_GAMEPAD_BACK;
			xin_buttons[lThumb] = XINPUT_GAMEPAD_LEFT_THUMB;
			xin_buttons[rThumb] = XINPUT_GAMEPAD_RIGHT_THUMB;

		}
	};

}