#include "HudDrawOverride.h"

#include <maya/MGlobal.h>
#include <maya/MFnDependencyNode.h>

namespace DynamoMayaHud
{
	MString HudDrawOverride::name("dynamoHUDDrawOverride");

	HudDrawOverride::HudDrawOverride(const MObject& obj) :
		MPxDrawOverride(obj, NULL),
		m_loadDefault(true),
		m_isDirty(true)
	{
	}

	MPxDrawOverride* HudDrawOverride::creator(const MObject& obj)
	{
		return new HudDrawOverride(obj);
	}

	DrawAPI HudDrawOverride::supportedDrawAPIs() const
	{
		// this plugin supports both GL and DX
		return (kOpenGL | kDirectX11 | kOpenGLCoreProfile);
	}

	bool HudDrawOverride::isBounded(const MDagPath& objPath, const MDagPath& cameraPath) const
	{
		return false;
	}

	MBoundingBox HudDrawOverride::boundingBox(const MDagPath& objPath, const MDagPath& cameraPath) const
	{
		return MBoundingBox();
	}

	bool HudDrawOverride::disableInternalBoundingBoxDraw() const
	{
		return true;
	}

	bool HudDrawOverride::hasUIDrawables() const
	{
		return true;
	}

	MUserData* HudDrawOverride::prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MFrameContext& frameContext, MUserData* oldData)
	{
		MStatus status;

		HudUserData* data = dynamic_cast<HudUserData*>(oldData);

		if (!data)
		{
			data = new HudUserData();
		}

		MObject mObject = objPath.node(&status);
		MFnDependencyNode depNodeFn(mObject, &status);
		HudNode* hudNode = dynamic_cast<HudNode*>(depNodeFn.userNode(&status));

		if (status != MStatus::kSuccess || hudNode == NULL)
		{
			MGlobal::displayError("Failed to get hud user node");
			return data;
		}

		hudNode->getDrawData(*data, cameraPath, frameContext);

		return data;
	}

	void HudDrawOverride::addUIDrawables(const MDagPath& objPath, MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext, const MUserData* data)
	{
		MStatus status;

		const HudUserData* hudData = dynamic_cast<const HudUserData*>(data);

		if (!hudData)
		{
			return;
		}

		MObject mObject = objPath.node(&status);
		MFnDependencyNode depNodeFn(mObject, &status);
		HudNode* hudNode = dynamic_cast<HudNode*>(depNodeFn.userNode(&status));

		if (status != MStatus::kSuccess || hudNode == NULL)
		{
			MGlobal::displayError("Failed to add hud drawables.");
			return;
		}

		hudNode->drawData(*hudData, drawManager);
	}
}