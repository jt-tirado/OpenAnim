#ifndef MATRIX4_H
#define MATRIX4_H

#include <vector>

/*
A 4x4 matrix class
*/

namespace OpenAnim
{
	class Matrix4
	{
	public:
		Matrix4(){}
		~Matrix4() {}

		double at(unsigned int index)
		{
			return m_data[index];
		}

	private:
		double m_data[16];
	};
}

#endif
