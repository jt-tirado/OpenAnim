#ifndef EULER_H
#define EULER_H

#include "matrix4.h"
#include "vector3.h"
#include "quaternion.h"

#include <string>
#include <vector>
#include <tgmath.h>

/*
Class that manipulates and stores Euler Rotations.

References:
	- https://help.autodesk.com/view/MAYAUL/2022/ENU/?guid=Maya_SDK_py_ref_class_open_maya_1_1_m_euler_rotation_html
	- https://help.autodesk.com/view/MAYAUL/2022/ENU/?guid=Maya_SDK_cpp_ref_class_m_euler_rotation_html
	- https://forums.cgsociety.org/t/4x4-matrix-to-transform-values/1743190/9
*/

namespace OpenAnim
{
	class Euler
	{
	public:
		enum RotateOrder
		{
			kXYZ = 0,
			kXZY = 3,
			kYXZ = 4,
			kYZX = 1,
			kZXY = 2,
			kZYX = 5
		};

		static const double kRadianToDegrees;
		static const double kDegreesToRadian;

	public:
		Euler(double x = 0, double y = 0, double z = 0, RotateOrder rotateOrder = RotateOrder::kXYZ)
		{
			this->x = x;
			this->y = y;
			this->z = z;
			this->rotateOrder = rotateOrder;
		}

		Euler(Matrix4& matrix, RotateOrder rotateOrder = RotateOrder::kXYZ)
		{
			this->x = atan2(matrix.at(6), matrix.at(10)) * kRadianToDegrees;
			this->y = -asin(matrix.at(2)) * kRadianToDegrees;
			this->z = atan2(matrix.at(1), matrix.at(0)) * kRadianToDegrees;
			this->rotateOrder = rotateOrder;
		}

		Euler(Vector3& vector, RotateOrder rotateOrder = RotateOrder::kXYZ)
		{
			this->x = vector.x;
			this->y = vector.y;
			this->z = vector.z;
			this->rotateOrder = rotateOrder;
		}

		~Euler() {}

		double x, y, z;
		RotateOrder rotateOrder;

	public:

		std::vector<double> asVector(){return std::vector<double>({x, y, z});}
		Vector3 asVector3(){return Vector3(x,y,z);}

		std::string asString()
		{
			std::string result = "(";
			result += std::to_string(x);
			result += ", ";
			result += std::to_string(y);
			result += ", ";
			result += std::to_string(z);
			result += ", ";
			result += std::to_string(rotateOrder);
			result += ")";
			return result;
		}

		// Returns an Euler from a 4x4 matrix
		Matrix4 asMatrix()
		{
			return Matrix4();
		}

		Quaternion asQuaternion()
		{
			return Quaternion();
		}
	};

	const double Euler::kRadianToDegrees = 57.2957795;
	const double Euler::kDegreesToRadian = 0.017;
}

#endif
