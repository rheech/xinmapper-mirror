/* Google "Finite State Machine" */
#pragma once

namespace sds
{
	struct MultiBool // =D
	{
		enum STATE
		{
			STATE_ONE,
			STATE_TWO,
			STATE_THREE,
			STATE_FOUR,
			STATE_FIVE,
			STATE_SIX
		} current_state;

		MultiBool() : current_state(STATE_ONE) {}
		MultiBool( const MultiBool &rhs )
		{
			this->current_state = rhs.current_state;
		}
		MultiBool( STATE s )
		{
			s = current_state;
		}
		//void IncrementState();
		void ResetState()
		{
			current_state = STATE_ONE;
		}
	};

}//end namespace