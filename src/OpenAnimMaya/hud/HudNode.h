#ifndef HUDNODE_H
#define HUDNODE_H

#include "HudData.h"

#include <maya/MPxLocatorNode.h>
#include <maya/MPlug.h>
#include <maya/MStringArray.h>
#include <maya/MDagPath.h>
#include <maya/MFrameContext.h>
#include <maya/MUIDrawManager.h>

namespace DynamoMayaHud
{
	class HudNode : public MPxLocatorNode
	{
	public:
		static void* creator();
		static MStatus initialize();

		// Static Plugin Attributes
		static MString name;
		static MTypeId id;
		static MString drawDbClassification;
		static MString drawRegistrantId;

		static MStringArray fontList;

		// Static Node Attributes
		static MObject enable;
		static MObject displayType;
		static MObject camera;

		// Static Text Attributes
		static MObject textList;

		static MObject textEnable;
		static MObject textType;
		static MObject textString;
		static MObject textInt;
		static MObject textFloat;
		static MObject textBool;
		static MObject textAlignment;
		static MObject textArea;
		static MObject textAnchor;
		static MObject textPosition;
		static MObject textDimensions;
		static MObject textScale;
		static MObject textFontIncline;
		static MObject textFontWeight;
		static MObject textFontStretch;
		static MObject textFontLine;
		static MObject textFontSize;
		static MObject textFontIndex;
		static MObject textColor;
		static MObject textAlpha;
		static MObject textEnableBackground;
		static MObject textBackgroundColor;
		static MObject textBackgroundAlpha;
		static MObject textIs2D;
		static MObject textDynamic;

		// Static Line Attributes
		static MObject lineList;
		static MObject lineEnable;
		static MObject lineIs2D;
		static MObject lineArea;
		static MObject lineAnchor;
		static MObject linePosition;
		static MObject lineScale;
		static MObject lineStyle;
		static MObject lineStyleFactor;
		static MObject lineStylePattern;
		static MObject lineWidth;
		static MObject lineStart;
		static MObject lineEnd;
		static MObject lineColor;
		static MObject lineAlpha;

		// Static Rect Attributes
		static MObject rectList;
		static MObject rectEnable;
		static MObject rectIs2D;
		static MObject rectArea;
		static MObject rectAnchor;
		static MObject rectPosition;
		static MObject rectDimensions;
		static MObject rectUp;
		static MObject rectNormal;
		static MObject rectScale;
		static MObject rectIsFilled;
		static MObject rectColor;
		static MObject rectAlpha;
		static MObject rectLineStyle;
		static MObject rectLineStyleFactor;
		static MObject rectLineStylePattern;
		static MObject rectLineWidth;

		// Static Image Attributes
		static MObject imageList;
		static MObject imageEnable;
		static MObject imagePath;
		static MObject imageIs2D;
		static MObject imageArea;
		static MObject imageAnchor;
		static MObject imagePosition;
		static MObject imageDimensions;
		static MObject imageUp;
		static MObject imageNormal;
		static MObject imageScale;
		static MObject imageIsFilled;
		static MObject imageColor;
		static MObject imageAlpha;

		// Static Helpers
		static MString getUserName();
		static MString getFilePath();
		static MString getFileName();
		static MString getSceneName();
		static MString getFileVersion();
		static MString getProject();
		static MString getFps();
		static MString getTime();
		static MString getTimecode();
		static MString getFrame();
		static MString getDay();
		static MString getMonth();
		static MString getYear();
		static MString getYear2();
		static MString getDate();

	public:
		HudNode();
		~HudNode() override {};

		bool excludeAsLocator() const override { return false; };
		void postConstructor() override;

		void getDrawData(HudUserData& data,
			const MDagPath& cameraPath,
			const MFrameContext& frameContext);

		void drawData(
			const HudUserData& data,
			MHWRender::MUIDrawManager& drawManager);

	private:
		bool m_loadDefault;
		bool m_isDirty;

		void formatString(HudUserData& data, TextData& textData);
		MVector getOffset(HudUserData& data, BaseData& item);

		MStatus getViewportData(HudUserData& data, const MDagPath& cameraPath, const MFrameContext& frameContext);
		MStatus getTextData(HudUserData& data);
		MStatus getTextDataChild(MPlug tPlug, TextData& td);

		MStatus getLineData(HudUserData& hudData);
		MStatus getLinePlugData(MPlug plug, LineData& lineData);

		MStatus getRectData(HudUserData& hudData);
		MStatus getRectPlugData(MPlug plug, RectData& rectData);

		MStatus getImageData(HudUserData& hudData);
		MStatus getImagePlugData(MPlug plug, ImageData& imageData);

		void debugText(TextData& textData);
		void debugData(HudUserData& data);

		static void addTextAttributes();
		static void addLineAttributes();
		static void addRectAttributes();
		static void addImageAttributes();
	};
}
#endif

