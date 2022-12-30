#ifndef MOTIONTRAILNODE_H
#define MOTIONTRAILNODE_H

#include "MotionTrailData.h"

#include <maya/MPxLocatorNode.h>
#include <maya/MPlug.h>
#include <maya/MFrameContext.h>
#include <maya/MUIDrawManager.h>
#include <maya/MPointArray.h>
#include <maya/MPoint.h>

// https://help.autodesk.com/view/MAYAUL/2022/ENU/?guid=Maya_SDK_cpp_ref_maya_plugin_for_spreticle_2sp_reticle_loc_8h_example_html

//#define kCameraNodeMessage "cameraNodeMessage"
//#define kCameraSpaceMessage "cameraSpaceMessage"
//#define kCameraSpaceMatrix "cameraSpaceMatrix"
//#define kCameraSpaceMatrix "cameraSpaceMatrix"
//
//#define kObjectSpaceMessage "objectSpaceMessage"
//#define kWorldSpacePoints "worldSpacePoints"
//#define kOrientationWorldSpacePointsX "orientationWorldSpacePointsX"
//#define kOrientationWorldSpacePointsY "orientationWorldSpacePointsY"
//#define kOrientationWorldSpacePointsZ "orientationWorldSpacePointsZ"
//
//#define kObjectSpaceMatrix "objectSpaceMatrix"
//#define kDirtyDots "dirtyDots"
//#define kFrameTimes "frameTimes"
//#define kGimbalLockScore "gimbalLockScore"
//#define kBigDots "bigDots"
//#define kLineWidth "lineWidth"
//
//#define kSmallDotSize "smallDotSize"
//#define kbigDotSize "bigDotSize"
//
//#define kPresentSmallDotColor "presentSmallDotColor"
//#define kPresentBigDotColor "presentBigDotColor"
//
//#define kPastDotsColor "pastDotsColor"
//#define kPastDirtyDotsColor "pastDotsColor"
//#define kFutureDotsColor "futureDotsColor"
//#define kFutureDirtyDotsColor "futureDirtyDotsColor"
//
//#define kLineColorScheme "lineColorScheme"
//
//#define kLineColorSchemePastFuture "Past and Future"
//#define kLineColorSchemeSpacing "Spacing Gradient"
//#define kLineColorSchemeGimbal "Gimbal Lock Gradient"
//#define kLineColorSchemeNoise "Noise Gradient"
//#define kLineColorSchemeDepth "Depth Gradient"
//
//#define kLineColor1 "lineColor1"
//#define kLineColor2 "lineColor2"
//#define kLineColor3 "lineColor3"
//#define kLineColor4 "lineColor4"
//
//#define kFretLineColorX1 "fretLineColorX1"
//#define kFretLineColorX2 "fretLineColorX2"
//#define kFretLineColorX3 "fretLineColorX3"
//
//#define kFretLineColorY1 "fretLineColorY1"
//#define kFretLineColorY2 "fretLineColorY2"
//#define kFretLineColorY3 "fretLineColorY3"
//
//#define kFretLineColorZ1 "fretLineColorZ1"
//#define kFretLineColorZ2 "fretLineColorZ2"
//#define kFretLineColorZ3 "fretLineColorZ3"
//
//#define kTime "time"
//#define kFramesBeforeAfter "framesBeforeAfter"
//#define kOrientationTrackingX "orientationTrackingX"
//#define kOrientationTrackingY "orientationTrackingY"
//#define kOrientationTrackingZ "orientationTrackingZ"
//

// ---------------------------------------------------

//#define kTransparencyFalloff "transparencyFalloff"
//#define kIsDirty "isDirty"
//#define kIsCameraDirty "isCameraDirty"
//
////
//#define kEnable "enable"
//#define kTimeCurrent "timeCurrent"
//#define kTimeStart "timeStart"
//#define kTimeEnd "timeEnd"
//
//#define kObject "object"
//#define kObjectType "objectType"
//#define kObjectTypeTransform "transform"
//#define kObjectTypeVert "vertex"
//#define kObjectTypeEdge "edge"
//#define kObjectTypeFace "face"
//#define kObjectIndex "objectIndex"
//
//#define kCamera "camera"
//#define kViewType "drawType"
//#define kViewTypeNone "none"
//#define kViewTypeAll "all"
//#define kViewTypeCamera "camera"
//
//#define kRectEnable "rectEnable"
//#define kRectColor "rectColor"
//#define kRectType "rectType"
//#define kRectTypeAll "allFrames"
//#define kRectTypeKeys "keyFrames"
//#define kRectSolid "rectIsSolid"
//
//#define kLineEnable "lineEnable"
//#define kLineType "lineType"
//#define kLineWidth "lineWidth"
//#define kLineColor "lineColor"

namespace DynamoMayaTrail
{
	enum TrailType
	{
		Constant = 0,
		Alternating = 1,
		PastFuture = 2,
	};

	enum TrailObjectType
	{
		Transform = 0,
		Vertex = 1,
	};

	class MotionTrailNode : public MPxLocatorNode
	{
	public:
		
		static void* creator();
		static MStatus initialize();

		// Static Plugin Attributes
		static MString name;
		static MTypeId id;
		static MString drawDbClassification;
		static MString drawRegistrantId;

		// Static Node Attributes
		static MObject enable;
		static MObject isDirty;

		static MObject timeCurrent;
		static MObject timeStart;
		static MObject timeEnd;
		static MObject timeIncrement;

		static MObject object;
		static MObject objectType;
		static MObject objectIndex;

		static MObject camera;
		static MObject drawType;

		static MObject lineWidth;
		static MObject lineStyle;
		static MObject lineStyleFactor;
		static MObject lineStylePattern;
		static MObject lineColor;
		static MObject lineAlpha;

	public:
		MotionTrailNode();
		~MotionTrailNode() override {};

		bool excludeAsLocator() const override;
		//bool drawLast() const override { return true; }
		//bool isBounded() const override { return false; }

		void postConstructor() override;
		//MStatus compute(const MPlug& plug, MDataBlock& data) override;
		//bool setInternalValue(const MPlug& plug, const MDataHandle& handle) override;

		void getDrawData(MotionTrailData& data, const MDagPath& cameraPath, const MFrameContext& frameContext);

		void drawData(
			const MotionTrailData& data,
			MHWRender::MUIDrawManager& drawManager);

		//void draw(M3dView& view, const MDagPath& path,
		//	M3dView::DisplayStyle style,
		//	M3dView::DisplayStatus status) override;

	private:
		double m_loadDefault;
		bool m_isDirty;
		bool m_isVisible;

		void getPointData(MObject& thisNode, MotionTrailData& data);
		void debugData(MotionTrailData& data);
		void debugPoint(int index, MPoint& point);
	};
}

#endif