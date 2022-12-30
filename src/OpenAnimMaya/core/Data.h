#ifndef COREDATA_H
#define COREDATA_H

#include <vector>

#include <maya\MUserData.h>
#include <maya\MDagPath.h>
#include <maya\MPoint.h>
#include <maya\MColor.h>
#include <maya\MVector.h>
#include <maya\MMatrixArray.h>

struct DrawData
{
	bool		enable{ false };
	int			index{ 0 };
	MPoint		position{ 0.0, 0.0, 0.001 };
	int			type{ 0 };
	MColor		color{ 1.0f, 0.0f, 0.0f, 1.0f };
	MVector		scale{ 1.0f, 1.0f, 1.0f };
};

struct CameraData : DrawData
{

};

struct ObjectData : DrawData
{
	MDagPath dag;
	MMatrixArray matrixArray;
};

struct LineData : DrawData
{
	float		width{ 1.0f };
	MPoint		startPoint{ 0.0, 0.0, 0.0 };
	MPoint		endPoint{ 1.0, 1.0, 1.0 };
};

struct RectData
{
	MVector		up{ 0.0, 1.0, 0.0 };
	MVector		normal{ 0.0, 0.0, 1.0 };
	bool		isFilled{ true };
	float		width{ 1.0f };
	float		height{ 1.0f };
};

#endif
