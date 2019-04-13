/*
Responsible for producing an ActionDetails from
an XINPUT_STATE for consumption by the rest of the code.
*/
#pragma once
#include "stdafx.h"
#include "ButtonAction.h"
#include "TriggerAction.h"
#include "ThumbstickAction.h"

namespace sds
{
	class XInputTranslater
	{
		ButtonAction *btn;
		TriggerAction *trg;
		ThumbstickAction *thmb;
		PlayerInfo *player;
	public:
		XInputTranslater()
		{
			btn = new ButtonAction();
			trg = new TriggerAction();
			thmb = new ThumbstickAction();
		}
		~XInputTranslater(){
			delete btn;
			delete trg;
			delete thmb;
		}
		ActionDetails ProcessState(const XINPUT_STATE &state){
			ActionDetails details;
			//Buttons
			for(auto it = sds::sdsActionDescriptors.xin_buttons.begin(); it != sds::sdsActionDescriptors.xin_buttons.end(); ++it){
				if( btn->Down(state,it->first ) ) {
					details += it->first + sds::sdsActionDescriptors.delimiter;
				}
			}
			//Triggers
			if( trg->Down(state,sds::sdsActionDescriptors.lTrigger) ){
				details += sds::sdsActionDescriptors.lTrigger + sds::sdsActionDescriptors.delimiter;
			}
			if( trg->Down(state,sds::sdsActionDescriptors.rTrigger) ){
				details += sds::sdsActionDescriptors.rTrigger + sds::sdsActionDescriptors.delimiter;
			}
			//Thumbsticks
			std::string thumb = sds::sdsActionDescriptors.lThumb + sds::sdsActionDescriptors.moreInfo;
			//lThumb
			if( thmb->Down(state,thumb + sds::sdsActionDescriptors.up) ){
				details += thumb + sds::sdsActionDescriptors.up + sds::sdsActionDescriptors.delimiter;
			}
			if( thmb->Down(state,thumb + sds::sdsActionDescriptors.down) ){
				details += thumb + sds::sdsActionDescriptors.down + sds::sdsActionDescriptors.delimiter;
			}
			if( thmb->Down(state, thumb + sds::sdsActionDescriptors.left) ){
				details += thumb + sds::sdsActionDescriptors.left + sds::sdsActionDescriptors.delimiter;
			}
			if( thmb->Down(state, thumb + sds::sdsActionDescriptors.right) ){
				details += thumb + sds::sdsActionDescriptors.right + sds::sdsActionDescriptors.delimiter;
			}

			//rThumb
			thumb = sds::sdsActionDescriptors.rThumb + sds::sdsActionDescriptors.moreInfo;
			if( thmb->Down(state,thumb + sds::sdsActionDescriptors.up) ){
				details += thumb + sds::sdsActionDescriptors.up + sds::sdsActionDescriptors.delimiter;
			}
			if( thmb->Down(state,thumb + sds::sdsActionDescriptors.down) ){
				details += thumb + sds::sdsActionDescriptors.down + sds::sdsActionDescriptors.delimiter;
			}
			if( thmb->Down(state, thumb + sds::sdsActionDescriptors.left) ){
				details += thumb + sds::sdsActionDescriptors.left + sds::sdsActionDescriptors.delimiter;
			}
			if( thmb->Down(state, thumb + sds::sdsActionDescriptors.right) ){
				details += thumb + sds::sdsActionDescriptors.right + sds::sdsActionDescriptors.delimiter;
			}
			return details;
		}
	};

}