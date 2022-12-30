#ifndef VECTOR3_H
#define VECTOR3_H

#include <vector>

namespace OpenAnim
{
	class Vector3
	{
	public:
		Vector3(
			double x = 0, 
			double y = 0, 
			double z = 0
		)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}

		~Vector3() {};

		double x, y, z = 0;

	public:
		std::string asString()
		{
			std::string result = "(";
			result += std::to_string(x);
			result += ", ";
			result += std::to_string(y);
			result += ", ";
			result += std::to_string(z);
			result += ")";
			return result;
		}
	};
}

#endif
