#include "MotionTrailNode.h"

#include <maya/MGlobal.h>
#include <maya/MFnMessageAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericData.h>

#include <maya/MPlugArray.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnTransform.h>
#include <maya/MFnDagNode.h>
#include <maya/MDagPath.h>
#include <maya/MTime.h>
#include <maya/MDGContext.h>
#include <maya/MDGContextGuard.h>
#include <maya/MFnMatrixData.h>
#include <maya/MMatrix.h>
#include <maya/MDistance.h>
#include <maya/MItMeshVertex.h>

// https://help.autodesk.com/view/MAYAUL/2022/ENU/?guid=Maya_SDK_cpp_ref_maya_plugin_for_spreticle_2sp_reticle_loc_8cpp_example_html

#define McheckStatus(stat,msg)  \
    if (!stat) {                \
        stat.perror(msg);       \
        return stat;            \
    }

#define McheckVoid(stat,msg)    \
    if (!stat) {                \
        stat.perror(msg);       \
        return;                 \
    }

namespace DynamoMayaTrail
{
	MString MotionTrailNode::name("dynamoTrail");
	MTypeId MotionTrailNode::id(0x0013a380);
	MString MotionTrailNode::drawDbClassification("drawdb/geometry/dynamoTrail");
	MString MotionTrailNode::drawRegistrantId("dynamoTrailPlugin");

	MObject MotionTrailNode::enable;
	MObject MotionTrailNode::isDirty;
	
	MObject MotionTrailNode::timeCurrent;
	MObject MotionTrailNode::timeStart;
	MObject MotionTrailNode::timeEnd;
	MObject MotionTrailNode::timeIncrement;

	MObject MotionTrailNode::object;
	MObject MotionTrailNode::objectType;
	MObject MotionTrailNode::objectIndex;
	MObject MotionTrailNode::camera;
	MObject MotionTrailNode::drawType;

	MObject MotionTrailNode::lineStyle;
	MObject MotionTrailNode::lineStyleFactor;
	MObject MotionTrailNode::lineStylePattern;
	MObject MotionTrailNode::lineWidth;
	MObject MotionTrailNode::lineColor;
	MObject MotionTrailNode::lineAlpha;

	void* MotionTrailNode::creator()
	{
		return new MotionTrailNode();
	}

	MStatus MotionTrailNode::initialize()
	{
		MStatus status;

		MFnNumericAttribute nAttr;
		MFnEnumAttribute eAttr;
		MFnUnitAttribute uAttr;
		MFnMessageAttribute mAttr;

		// Default
		enable = nAttr.create("enable", "enable", MFnNumericData::kBoolean, 1);
		nAttr.setKeyable(true);
		nAttr.setStorable(true);
		nAttr.setWritable(true);
		nAttr.setReadable(true);
		addAttribute(enable);

		// Object
		object = mAttr.create("object", "object");
		mAttr.setWritable(true);
		mAttr.setStorable(true);
		addAttribute(object);

		objectType = eAttr.create("objectType", "objectType", 0);
		eAttr.addField("Transform", TrailObjectType::Transform);
		eAttr.addField("Vertex", TrailObjectType::Vertex);
		eAttr.setKeyable(true);
		eAttr.setStorable(true);
		eAttr.setWritable(true);
		eAttr.setReadable(true);
		addAttribute(objectType);

		objectIndex = nAttr.create("objectIndex", "objectIndex", MFnNumericData::kInt, 0);
		nAttr.setMin(0);
		nAttr.setKeyable(true);
		nAttr.setStorable(true);
		nAttr.setWritable(true);
		nAttr.setReadable(true);
		addAttribute(objectIndex);

		// Type
		drawType = eAttr.create("drawType", "drawType", 0);
		eAttr.addField("Constant", TrailType::Constant);
		eAttr.addField("Alternating", TrailType::Alternating);
		eAttr.addField("Past / Future", TrailType::PastFuture);
		eAttr.setKeyable(true);
		eAttr.setStorable(true);
		eAttr.setWritable(true);
		eAttr.setReadable(true);
		addAttribute(drawType);

		// Time
		timeCurrent = nAttr.create("timeCurrent", "timeCurrent", MFnNumericData::kFloat, 1.0f);
		nAttr.setKeyable(true);
		nAttr.setStorable(true);
		nAttr.setWritable(true);
		nAttr.setReadable(true);
		addAttribute(timeCurrent);

		timeStart = nAttr.create("timeStart", "timeStart", MFnNumericData::kFloat, 0.0f);
		nAttr.setKeyable(true);
		nAttr.setStorable(true);
		nAttr.setWritable(true);
		nAttr.setReadable(true);
		addAttribute(timeStart);

		timeEnd = nAttr.create("timeEnd", "timeEnd", MFnNumericData::kFloat, 0.0f);
		nAttr.setKeyable(true);
		nAttr.setStorable(true);
		nAttr.setWritable(true);
		nAttr.setReadable(true);
		addAttribute(timeEnd);

		timeIncrement = nAttr.create("timeIncrement", "timeIncrement", MFnNumericData::kFloat, 1.0f);
		nAttr.setKeyable(true);
		nAttr.setStorable(true);
		nAttr.setWritable(true);
		nAttr.setReadable(true);
		addAttribute(timeIncrement);

		// Line Style
		lineStyle = eAttr.create("lineStyle", "lineStyle", 0, &status);
		eAttr.setKeyable(true);
		eAttr.setStorable(true);
		eAttr.setWritable(true);
		eAttr.setReadable(true);
		eAttr.addField("solid", 0);
		eAttr.addField("shortDotted", 1);
		eAttr.addField("shortDashed", 2);
		eAttr.addField("dashed", 3);
		eAttr.addField("dotted", 4);
		eAttr.addField("custom", 5);
		addAttribute(lineStyle);

		// Line Factor
		lineStyleFactor = nAttr.create("lineStyleFactor", "lineStyleFactor", MFnNumericData::kInt, 1, &status);
		nAttr.setMin(0);
		nAttr.setKeyable(true);
		nAttr.setStorable(true);
		nAttr.setWritable(true);
		nAttr.setReadable(true);
		addAttribute(lineStyleFactor);

		// Line Pattern
		lineStylePattern = nAttr.create("lineStylePattern", "lineStylePattern", MFnNumericData::kInt, 1, &status);
		nAttr.setMin(-32768);
		nAttr.setMax(32768);
		nAttr.setKeyable(true);
		nAttr.setStorable(true);
		nAttr.setWritable(true);
		nAttr.setReadable(true);
		addAttribute(lineStylePattern);

		// Line Width
		lineWidth = nAttr.create("lineWidth", "lineWidth", MFnNumericData::kFloat, 1.0f);
		nAttr.setMin(0.0f);
		nAttr.setDefault(1.0f);
		nAttr.setKeyable(true);
		nAttr.setStorable(true);
		nAttr.setWritable(true);
		nAttr.setReadable(true);
		addAttribute(lineWidth);

		// Line Color
		lineColor = nAttr.createColor("lineColor", "lineColor");
		nAttr.setDefault(1.0f, 1.0f, 1.0f);
		nAttr.setUsedAsColor(true);
		nAttr.setKeyable(true);
		nAttr.setStorable(true);
		nAttr.setWritable(true);
		nAttr.setReadable(true);
		addAttribute(lineColor);

		lineAlpha = nAttr.create("lineAlpha", "lineAlpha", MFnNumericData::kFloat, 1.0f);
		nAttr.setMin(0.0f);
		nAttr.setMax(1.0f);
		nAttr.setDefault(1.0f);
		nAttr.setKeyable(true);
		nAttr.setStorable(true);
		nAttr.setWritable(true);
		nAttr.setReadable(true);
		addAttribute(lineAlpha);

		// Line
		return MS::kSuccess;
	}

	MotionTrailNode::MotionTrailNode() :
		m_loadDefault(1),
		m_isDirty(true),
		m_isVisible(true)
	{
	}

	bool MotionTrailNode::excludeAsLocator() const
	{
		MObject thisNode = thisMObject();
		return false;
	}

	void MotionTrailNode::postConstructor()
	{
		m_isDirty = true;
	}

	//MStatus MotionTrailNode::compute(const MPlug& plug, MDataBlock& data)
	//{
	//	return MS::kSuccess;
	//}

	//bool MotionTrailNode::setInternalValue(const MPlug& plug, const MDataHandle& handle)
	//{
	//	if (plug == worldInverseMatrix || plug == isTemplated)
	//	{
	//		return false;
	//	}

	//	m_isDirty = true;
	//	return false;
	//}


	//void MotionTrailNode::draw(M3dView& view, const MDagPath& path, M3dView::DisplayStyle style, M3dView::DisplayStatus status)
	//{
	//	m_isVisible = M3dView::DisplayObjects::kExcludeMotionTrails == (M3dView::DisplayObjects::kExcludeMotionTrails && view.objectDisplay());
	//}

	//MMatrix getVertexMatrix(MObject& mesh, int vertexIndex)
	//{
	//	MStatus status;

	//	MItMeshVertex vertexIter(mesh, &status);
	//	if (status != MS::kSuccess)
	//	{
	//		MGlobal::displayError("Failed to create MItMeshVertex for mesh");
	//		return MMatrix();
	//	}

	//	vertexIter.setIndex(vertexIndex, vertexIndex);

	//	MMatrix matrix = vertexIter.transformationMatrix(&status);
	//	if (status != MS::kSuccess)
	//	{
	//		MGlobal::displayError("Failed to get transformation matrix for vertex");
	//		return MMatrix();
	//	}

	//	return matrix;
	//}

	void MotionTrailNode::getPointData(MObject& thisNode, MotionTrailData& data)
	{
		MStatus status;
		data.points.clear();
		MPlug p, c;

		// Get Time
		double start;
		double end;
		double increment;

		p = MPlug(thisNode, timeStart);
		p.getValue(start);

		p = MPlug(thisNode, timeEnd);
		p.getValue(end);

		p = MPlug(thisNode, timeIncrement);
		p.getValue(increment);

		// Get the connected object
		p = MPlug(thisNode, object);

		MPlugArray connections;
		p.connectedTo(connections, true, false, &status);

		if (status != MS::kSuccess)
		{
			return;
		}

		if (connections.length() == 0)
		{
			return;
		}

		MObject transform = connections[0].node();

		MFnDependencyNode depNodeFn(transform, &status);
		MPlug plugArray = depNodeFn.findPlug("worldMatrix", false, &status);
		
		if (status != MS::kSuccess)
		{
			return;
		}

		plugArray.evaluateNumElements();
		MPlug matrixPlug = plugArray.elementByPhysicalIndex(0);
		MDistance::Unit worldUnit = MDistance::uiUnit();

		for (double i = start; i <= end; i += increment)
		{
			MTime evalTime(i, MTime::uiUnit());
			MDGContext context(evalTime);

			//MObject matrixObj = matrixPlug.asMObject(context);

			MDGContextGuard guard(context);
			MObject matrixObj = matrixPlug.asMObject();

			MFnMatrixData matrixDataFn(matrixObj);
			MTransformationMatrix transformMatrix = matrixDataFn.transformation();

			MVector translation = transformMatrix.getTranslation(MSpace::kWorld);
			double x = MDistance(translation.x).asUnits(worldUnit);
			double y = MDistance(translation.y).asUnits(worldUnit);
			double z = MDistance(translation.z).asUnits(worldUnit);

			MPoint point(x, y, z);
			data.points.append(point);
		}
	}

	void MotionTrailNode::debugData(MotionTrailData& data)
	{
		MGlobal::displayInfo("------------------------------------------------");
		MGlobal::displayInfo(MString("enable: ") + data.enable);

		MGlobal::displayInfo(MString("lineWidth: ") + data.lineWidth);
		MGlobal::displayInfo(MString("lineStyle: ") + data.lineStyle);
		MGlobal::displayInfo(MString("lineStyleFactor: ") + data.lineStyleFactor);
		MGlobal::displayInfo(MString("lineStylePattern: ") + data.lineStylePattern);

		MGlobal::displayInfo(MString("lineColorR: ") + data.lineColor.r);
		MGlobal::displayInfo(MString("lineColorG: ") + data.lineColor.g);
		MGlobal::displayInfo(MString("lineColorB: ") + data.lineColor.b);
		MGlobal::displayInfo(MString("lineColorA: ") + data.lineColor.a);

		MGlobal::displayInfo(MString("pointsCount: ") + data.points.length());

		for (int i=0; i < data.points.length(); i++)
		{
			debugPoint(i, data.points[i]);
		}
	}

	void MotionTrailNode::debugPoint(int index, MPoint& point)
	{
		MString message = MString("point[") + index + MString("]: ");
		message += MString() + point.x + MString(", ");
		message += MString() + point.y + MString(", ");
		message += MString() + point.z;

		MGlobal::displayInfo(message);
	}

	void MotionTrailNode::getDrawData(MotionTrailData& data, const MDagPath& cameraPath, const MFrameContext& frameContext)
	{
		//Clear the data
		data = MotionTrailData();

		MStatus status;
		MPlug p, c;
		MObject thisNode = thisMObject();

		// Enable
		p = MPlug(thisNode, enable);
		McheckVoid(p.getValue(data.enable), "MotionTrailData::getDrawData enable");

		//data.enable = data.enable && m_isVisible;

		if (!data.enable)
		{
			return;
		}

		// Get Line Data
		p = MPlug(thisNode, lineWidth);
		McheckVoid(p.getValue(data.lineWidth), "MotionTrailData::getDrawData lineWidth");

		p = MPlug(thisNode, lineStyle);
		McheckVoid(p.getValue(data.lineStyle), "MotionTrailData::getDrawData lineStyle");

		p = MPlug(thisNode, lineStyleFactor);
		McheckVoid(p.getValue(data.lineStyleFactor), "MotionTrailData::getDrawData lineStyleFactor");

		p = MPlug(thisNode, lineStylePattern);
		McheckVoid(p.getValue(data.lineStylePattern), "MotionTrailData::getDrawData lineStylePattern");

		p = MPlug(thisNode, lineColor);
		c = p.child(0);
		McheckVoid(c.getValue(data.lineColor.r), "MotionTrailData::getDrawData lineColorR");
		c = p.child(1);
		McheckVoid(c.getValue(data.lineColor.g), "MotionTrailData::getDrawData lineColorG");
		c = p.child(2);
		McheckVoid(c.getValue(data.lineColor.b), "MotionTrailData::getDrawData lineColorB");

		p = MPlug(thisNode, lineAlpha);
		McheckVoid(p.getValue(data.lineColor.a), "MotionTrailData::getDrawData lineColorA");

		// Get Points
		getPointData(thisNode, data);
		//debugData(data);

		// If this is the first time it's being draw, load the default values
		if (m_loadDefault == 1)
		{
			/*MString tag;
			p = MPlug(thisNode, Tag);
			McheckVoid(p.getValue(tag), "spReticleLoc::draw get tag");
			MString cmd = "if (exists(\"spReticleLocSetDefault\")) spReticleLocSetDefault(\"" + name() + "\",\"" + tag + "\")";
			MGlobal::executeCommand(cmd);*/
			m_loadDefault = 0;
		}

		// Get 
		if (m_isDirty)
		{
			//data.compute();
			m_isDirty = false;
		}
	}

	void MotionTrailNode::drawData(const MotionTrailData& data, MHWRender::MUIDrawManager& drawManager)
	{
		// Drawing not enabled, return
		if (!data.enable)
		{
			return;
		}

		// FIXME: Not sure how this works.
		//if (M3dView::DisplayObjects::kExcludeMotionTrails != (M3dView::DisplayObjects::kExcludeMotionTrails && view.objectDisplay()))
		//{
		//	return;
		//}

		if (data.points.length() == 0)
		{
			return;
		}

		drawManager.beginDrawable();
		drawManager.setLineWidth(data.lineWidth);
		drawManager.setColor(data.lineColor);

		if (data.lineStyle == 5)
		{
			drawManager.setLineStyle(data.lineStyleFactor, data.lineStylePattern);
		}
		else
		{
			drawManager.setLineStyle((MHWRender::MUIDrawManager::LineStyle)data.lineStyle);
		}


		for (int i = 0; i < data.points.length() - 1; i++)
		{
			drawManager.line(data.points[i], data.points[i + 1]);
		}
		//drawManager.lineList(data.points, false);

		drawManager.endDrawable();

	}
}


