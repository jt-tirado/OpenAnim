#ifndef MOTIONTRAILDATA_H
#define MOTIONTRAILDATA_H

#include "../core/Data.h"
#include "../core/TransformMatrixCache.h"

#include <vector>
#include <map>

#include <maya\MUserData.h>
#include <maya\MDagPath.h>
#include <maya\MPoint.h>
#include <maya\MTime.h>
#include <maya\MColor.h>
#include <maya\MVector.h>
#include <maya\MMatrixArray.h>
#include <maya\MPointArray.h>

namespace DynamoMayaTrail
{
	class MotionTrailData : public MUserData
	{
	public:
		//// Data
		//ObjectData object;
		//CameraData camera;
		//LineData lineData;
		//RectData rectData;

		TransformMatrixCache objectCache;
		TransformMatrixCache cameraCache;

		bool enable = false;

		float lineWidth = 0;
		int lineStyle = 0;
		int lineStyleFactor = 0;
		short lineStylePattern = 0;

		MColor lineColor;
		MPointArray points;
	};
}

#endif

