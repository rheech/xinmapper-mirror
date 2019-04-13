/*
Small helper class for simulating input.
*/

#pragma once
#include "stdafx.h"

namespace sds
{
	class SendKey
	{
		INPUT keyInput;
		INPUT mouseInput;
	public:
		SendKey()
		{
			memset(&keyInput,0,sizeof(INPUT));
			memset(&mouseInput,0,sizeof(INPUT));
			keyInput.type = INPUT_KEYBOARD;
			mouseInput.type = INPUT_MOUSE;
		}
		void Send(int vk, bool down)
		{
			if( down )
				keyInput.ki.dwFlags = KEYEVENTF_SCANCODE;
			else
				keyInput.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
		
			WORD scanCode = GetScanCode(vk);
			keyInput.ki.wScan = scanCode;
			if( scanCode == 0 )
			{
				//Assume mouse.
				switch( vk )
				{
				case VK_LBUTTON:
					if(down)
						mouseInput.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
					else
						mouseInput.mi.dwFlags = MOUSEEVENTF_LEFTUP;
					break;
				case VK_RBUTTON:
					if(down)
						mouseInput.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
					else
						mouseInput.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
					break;
				case VK_MBUTTON:
					if(down)
						mouseInput.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
					else
						mouseInput.mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
					break;
				default:
					break;
				}
			}
		
			mouseInput.mi.dwExtraInfo = GetMessageExtraInfo();
			keyInput.ki.dwExtraInfo = GetMessageExtraInfo();
			UINT ret = SendInput(1,(scanCode != 0 ? &keyInput : &mouseInput),sizeof(INPUT));
			assert(ret != 0);
		}
		void Send(std::string str, bool down){
			for(auto it = str.begin(); it != str.end(); ++it)
				Send(*it,down);
		}
	private:
		WORD GetScanCode(int vk)
		{
			WORD ret =
				(MapVirtualKeyExA(VkKeyScanA(isalpha(vk) ? tolower(vk) : vk), MAPVK_VK_TO_VSC,GetKeyboardLayout(0)));
			if( ret == 0 )
				ret = static_cast<WORD> (MapVirtualKeyExA(vk, MAPVK_VK_TO_VSC,GetKeyboardLayout(0)));
			
			return ret;
		}
	};
}