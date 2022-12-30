#ifndef FRAME_H
#define FRAME_H

#include "time.h"

namespace OpenAnim
{
	class Frame
	{
	public:
		Frame() {}
		~Frame() {}

	private:
		Time m_time;
	};
}

#endif
