/*
Contains the logic for using the ActionDetails
based on the MapInformation.

Further design considerations may incorporate a
queue for sending input.  SendInput will allow
an array to be sent in one fell swoop of the 
function.
*/
#pragma once
#include "stdafx.h"
#include "MultiBool.h"
#include "SendKey.h"

namespace sds
{
	class Mapper
	{
		/*
		Each map has a FSM type unique.
		*/
		struct WordData
		{
			std::string control, info, sim_type, value;
			//char value;
			sds::MultiBool fsm;
			bool down;
			WordData() : down(false) {}
		};

		SendKey keySend;
		std::vector<WordData> mapTokenInfo;
		MapInformation map;
	public:
		void ProcessActionDetails(ActionDetails details)
		{
			std::vector<std::string> tokens;
			//Get input tokens.
			GetTokens(details,tokens);
			//Delegate processing.
			ProcessTokens(tokens);
		}

		MapInformation GetMapInfo() const
		{
			return map;
		}
		bool SetMapInfo(const MapInformation &newMap)
		{
			//Todo, validate.
			assert( newMap.size() > 0 );
			//Reset map token info.
			mapTokenInfo.clear();
			//Set MapInformation
			map = newMap;
			//Set WordData vector.
			std::string t;
			std::stringstream ss(newMap);
			while( ss >> t ){
				WordData data;
				assert(t.size() > 0);
				std::replace(t.begin(), t.end(), sds::sdsActionDescriptors.moreInfo, ' ');
				std::stringstream(t) >> data.control >> data.info >> data.sim_type >> data.value;
				mapTokenInfo.push_back(data);
			}
			return true;
		}
	private:
		void ProcessTokens(std::vector<std::string> &detail)
		{
			typedef std::vector<WordData>::iterator wit_type;
			typedef std::vector<std::string>::iterator sit_type;
			//This section sets a bool if the map is in the input.
			using std::string;
			std::for_each(mapTokenInfo.begin(),mapTokenInfo.end(), [](WordData &d){
				d.down = false;
			});
			string first, second;
			for(sit_type it = detail.begin(); it != detail.end(); ++it)
			{
				std::replace(it->begin(), it->end(), sds::sdsActionDescriptors.moreInfo, ' ');
				std::stringstream(*it) >> first >> second;
				for(wit_type ij = mapTokenInfo.begin(); ij != mapTokenInfo.end(); ++ij){
					if( first == ij->control
						&& (second+sds::sdsActionDescriptors.none == ij->info 
						|| second == ij->info ) )
						ij->down = true;
				}
			}
			ProcessStates(this->mapTokenInfo);
		}
		void ProcessStates(std::vector<WordData> &states)
		{
			for(auto it = states.begin(); it != states.end(); ++it)
			{
				//Update this if more sim types are added.
				if( it->sim_type == sds::sdsActionDescriptors.norm )
					Normal(*it);
				if( it->sim_type == sds::sdsActionDescriptors.toggle )
					Toggle(*it);
				if( it->sim_type == sds::sdsActionDescriptors.rapid )
					Rapid(*it);
			}
		}
		void Normal(WordData &detail) 
		{
			/*
			Normal keypress logic.
			The bool down member is important.
			*/
			std::string strVal;
			if( detail.down )
			{
				if( detail.fsm.current_state == detail.fsm.STATE_ONE )
				{
					//Check for VK.
					int keyCode = GetVkFromString(detail.value);
					if( keyCode >= 0 )
					{
						keySend.Send(keyCode,true);
					}
					else
						keySend.Send(detail.value,true);
					detail.fsm.current_state = detail.fsm.STATE_TWO;
				}
			}
			else
			{
				if( detail.fsm.current_state == detail.fsm.STATE_TWO )
				{
					//Check for VK.
					int keyCode = GetVkFromString(detail.value);
					if( keyCode >= 0 )
					{
						keySend.Send(keyCode,false);
					}
					else
						keySend.Send(detail.value.c_str(),false);
					detail.fsm.ResetState();
				}
			}
		}
		void Toggle(WordData &detail) 
		{
			//Toggle keypress logic.
			std::string strVal;
			if( detail.down )
			{
				if( detail.fsm.current_state == detail.fsm.STATE_ONE )
				{
					//Check for VK
					int keyCode = GetVkFromString(detail.value);
					if(keyCode >= 0)
						keySend.Send(keyCode,true);
					else
						keySend.Send(detail.value,true);
					detail.fsm.current_state = detail.fsm.STATE_TWO;
				}
				if( detail.fsm.current_state == detail.fsm.STATE_THREE )
				{
					int keyCode = GetVkFromString(detail.value);
					if(keyCode >= 0)
						keySend.Send(keyCode,false);
					else
						keySend.Send(detail.value,false);
					detail.fsm.current_state = detail.fsm.STATE_FOUR;
				}
			}
			else
			{
				if( detail.fsm.current_state == detail.fsm.STATE_TWO )
				{
					detail.fsm.current_state = detail.fsm.STATE_THREE;
				}
				if( detail.fsm.current_state == detail.fsm.STATE_FOUR )
				{
					detail.fsm.ResetState();
				}
			}
		}
		void Rapid(WordData &detail) 
		{
			//Rapid keypress logic.
			std::string strVal;
			if(detail.down)
			{
				int keyCode = GetVkFromString(detail.value);
				if( keyCode >= 0 )
				{
					keySend.Send(keyCode,true);
					keySend.Send(keyCode,false);
				}
				else
				{
					keySend.Send(detail.value,true);
					keySend.Send(detail.value,false);
				}

			}
		}
		void GetTokens(const std::string &details, std::vector<std::string> &tokenOut)
		{
			tokenOut.clear();
			std::string t;
			std::stringstream ss(details);
			while( ss >> t )
				tokenOut.push_back(t);
		}
		int GetVkFromString(std::string in)//Returns -1 if no value.
		{
			int keyCode = -1;
			if( in.find(sds::sdsActionDescriptors.vk) != std::string::npos )
			{
				std::string strVal;
				strVal = in.substr(sds::sdsActionDescriptors.vk.size());
				std::stringstream(strVal) >> keyCode;
			}
			return keyCode;
		}
	};

}