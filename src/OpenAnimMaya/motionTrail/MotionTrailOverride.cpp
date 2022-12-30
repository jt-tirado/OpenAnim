#include "MotionTrailOverride.h";
#include "MotionTrailData.h";
#include "MotionTrailNode.h"

#include <maya/MGlobal.h>
#include <maya/MFnDependencyNode.h>

namespace DynamoMayaTrail
{
	MString MotionTrailOverride::name("dynamoTrailDrawOverride");

	MotionTrailOverride::MotionTrailOverride(const MObject& obj) :
		MPxDrawOverride(obj, NULL),
		m_loadDefault(true),
		m_isDirty(true)
	{
	}

	MPxDrawOverride* MotionTrailOverride::creator(const MObject& obj)
	{
		return new MotionTrailOverride(obj);
	}

	DrawAPI MotionTrailOverride::supportedDrawAPIs() const
	{
		// this plugin supports both GL and DX
		return (kOpenGL | kDirectX11 | kOpenGLCoreProfile);
	}

	bool MotionTrailOverride::isBounded(const MDagPath& objPath, const MDagPath& cameraPath) const
	{
		return false;
	}

	MBoundingBox MotionTrailOverride::boundingBox(const MDagPath& objPath, const MDagPath& cameraPath) const
	{
		return MBoundingBox();
	}

	bool MotionTrailOverride::disableInternalBoundingBoxDraw() const
	{
		return true;
	}

	bool MotionTrailOverride::hasUIDrawables() const
	{
		return true;
	}

	MUserData* MotionTrailOverride::prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MFrameContext& frameContext, MUserData* oldData)
	{
		MStatus status;

		// Retrieve data cache (create if does not exist)
		MotionTrailData* data = dynamic_cast<MotionTrailData*>(oldData);

		if (!data)
		{
			data = new MotionTrailData();

			//if (data == NULL)
			//{
			//	MGlobal::displayError("Fail to allocate draw data");
			//	return NULL;
			//}

			//// Load defaults
			//m_loadDefault = 1;

			//// Set Refresh
			//m_isDirty = true;
		}

		MObject node = objPath.node(&status);
		MFnDependencyNode depNodeFn(node, &status);
		MotionTrailNode* mtNode = dynamic_cast<MotionTrailNode*>(depNodeFn.userNode(&status));

		if (status != MStatus::kSuccess || mtNode == NULL)
		{
			MGlobal::displayError("Fail to get user node");
			return data;
		}

		//get draw Data
		mtNode->getDrawData(*data, cameraPath, frameContext);

		return data;
	}

	void MotionTrailOverride::addUIDrawables(const MDagPath& objPath, MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext, const MUserData* data)
	{
		MStatus status;

		const MotionTrailData* thisdata = dynamic_cast<const MotionTrailData*>(data);

		if (!thisdata)
		{
			return;
		}

		MObject mObject = objPath.node(&status);
		MFnDependencyNode depNodeFn(mObject, &status);
		MotionTrailNode* node = dynamic_cast<MotionTrailNode*>(depNodeFn.userNode(&status));

		if (status != MStatus::kSuccess || node == NULL)
		{
			MGlobal::displayError("Failed to add hud drawables.");
			return;
		}

		node->drawData(*thisdata, drawManager);
	}
}



