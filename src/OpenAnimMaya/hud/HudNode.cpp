#include "HudNode.h"
#include "HudData.h"
#include "HudDefines.h"

#include <string>
#include <iostream>
#include <ctime>
#include <cmath>

#include <maya/MFnStringData.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnMessageAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericData.h>
#include <maya/MUIDrawManager.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MGlobal.h>
#include <maya/MString.h>

#include <maya/MSelectionList.h>
#include <maya/MObject.h>
#include <maya/MPlug.h>
#include <maya/MFnCamera.h>

#include <maya/MFileIO.h>
#include <maya/MFileObject.h>
#include <maya/MCommonSystemUtils.h>
#include <maya/MTime.h>
#include <maya/MAnimControl.h>

#include <maya/MViewport2Renderer.h>
#include <maya/MTextureManager.h>

namespace DynamoMayaHud
{
	MString HudNode::name("dynamoHud");
	MTypeId HudNode::id(0x0013a381);
	MString HudNode::drawDbClassification("drawdb/geometry/dynamoHud");
	MString HudNode::drawRegistrantId("dynamoHudPlugin");

	MStringArray HudNode::fontList;

	MObject HudNode::enable;
	MObject HudNode::displayType;
	MObject HudNode::camera;

	MObject HudNode::textList;
	MObject HudNode::textEnable;
	MObject HudNode::textType;
	MObject HudNode::textString;
	MObject HudNode::textInt;
	MObject HudNode::textFloat;
	MObject HudNode::textBool;
	MObject HudNode::textAlignment;
	MObject HudNode::textArea;
	MObject HudNode::textAnchor;
	MObject HudNode::textPosition;
	MObject HudNode::textScale;
	MObject HudNode::textDimensions;
	MObject HudNode::textFontIncline;
	MObject HudNode::textFontWeight;
	MObject HudNode::textFontStretch;
	MObject HudNode::textFontLine;
	MObject HudNode::textFontSize;
	MObject HudNode::textFontIndex;
	MObject HudNode::textColor;
	MObject HudNode::textAlpha;
	MObject HudNode::textEnableBackground;
	MObject HudNode::textBackgroundColor;
	MObject HudNode::textBackgroundAlpha;
	MObject HudNode::textIs2D;
	MObject HudNode::textDynamic;

	MObject HudNode::lineList;
	MObject HudNode::lineEnable;
	MObject HudNode::lineIs2D;
	MObject HudNode::lineArea;
	MObject HudNode::lineAnchor;
	MObject HudNode::linePosition;
	MObject HudNode::lineScale;
	MObject HudNode::lineStyle;
	MObject HudNode::lineStyleFactor;
	MObject HudNode::lineStylePattern;
	MObject HudNode::lineWidth;
	MObject HudNode::lineStart;
	MObject HudNode::lineEnd;
	MObject HudNode::lineColor;
	MObject HudNode::lineAlpha;

	MObject HudNode::rectList;
	MObject HudNode::rectEnable;
	MObject HudNode::rectIs2D;
	MObject HudNode::rectArea;
	MObject HudNode::rectAnchor;
	MObject HudNode::rectPosition;
	MObject HudNode::rectDimensions;
	MObject HudNode::rectUp;
	MObject HudNode::rectNormal;
	MObject HudNode::rectScale;
	MObject HudNode::rectIsFilled;
	MObject HudNode::rectColor;
	MObject HudNode::rectAlpha;
	MObject HudNode::rectLineStyle;
	MObject HudNode::rectLineStyleFactor;
	MObject HudNode::rectLineStylePattern;
	MObject HudNode::rectLineWidth;

	MObject HudNode::imageList;
	MObject HudNode::imageEnable;
	MObject HudNode::imagePath;
	MObject HudNode::imageIs2D;
	MObject HudNode::imageArea;
	MObject HudNode::imageAnchor;
	MObject HudNode::imagePosition;
	MObject HudNode::imageDimensions;
	MObject HudNode::imageUp;
	MObject HudNode::imageNormal;
	MObject HudNode::imageScale;
	MObject HudNode::imageIsFilled;
	MObject HudNode::imageColor;
	MObject HudNode::imageAlpha;

	MString HudNode::getUserName()
	{
		return MCommonSystemUtils::getEnv("USER");
	}
	MString HudNode::getFilePath()
	{
		MFileObject fo;
		fo.setRawFullName(MFileIO::currentFile());
		return fo.expandedPath();
	}
	MString HudNode::getFileName()
	{
		MFileObject fo;
		fo.setRawFullName(MFileIO::currentFile());
		return fo.resolvedName();
	}
	MString HudNode::getSceneName()
	{
		MString fileName = getFileName();

		if (fileName.indexW(".") != -1)
		{
			MStringArray strList;
			fileName.split('.', strList);
			fileName = strList[0];
		}

		return fileName;
	}
	MString HudNode::getFileVersion()
	{
		MString fileName = getFileName();
		MString version = kStrUnknown;

		if (fileName.indexW(".") != -1)
		{
			MStringArray strList;
			fileName.split('.', strList);
			
			for (int i=0; i < strList.length(); i++)
			{
				std::string str = strList[i].asChar();

				if (str.find_first_not_of("0123456789") == std::string::npos)
				{
					version = strList[i];
					break;
				}
			}
		}

		return version;
	}
	MString HudNode::getProject()
	{
		return kStrUnknown;
	}

	// TODO: Support All FPS values.
	MString HudNode::getFps()
	{
		MTime time = MAnimControl::currentTime();

		if (time.unit() == MTime::k24FPS)
		{
			return "24fps";
		}
		else if (time.unit() == MTime::k30FPS)
		{
			return "30fps";
		}
		else if (time.unit() == MTime::k60FPS)
		{
			return "60fps";
		}

		return kStrUnknown;
	}
	MString HudNode::getTime()
	{
		MTime time = MAnimControl::currentTime();
		time.setUnit(MTime::kSeconds);
		return MString() + time.value();
	}
	MString HudNode::getTimecode()
	{
		MTime time = MAnimControl::currentTime();
		//time.setUnit(MTime::kSeconds);
		double val = time.value();

		const char* sign = "";

		if (val < 0)
		{
			sign = "-";
			val = -val;
		}
		int valInt = (int)(val + .5);
		int sec = valInt / 24;
		int frame = valInt - sec * 24;
		int min = sec / 60;
		sec -= min * 60;
		int hour = min / 60;
		min -= hour * 60;
		char buffer[90];

		if (hour > 0)
		{
			sprintf_s(buffer, "%s%d:%02d:%02d.%02d",
				sign, hour, min, sec, frame);
		}
		else
		{
			sprintf_s(buffer, "%s%02d:%02d.%02d",
				sign, min, sec, frame);
		}

		return MString(buffer);


		//return kStrUnknown;
	}
	MString HudNode::getFrame()
	{
		MTime time = MAnimControl::currentTime();
		return MString() + time.value();
	}
	MString HudNode::getDay()
	{
		struct std::tm newtime;
		std::time_t now = time(0);
		localtime_s(&newtime, &now);

		return MString() + newtime.tm_mday;

		//return kStrUnknown;
	}
	MString HudNode::getMonth()
	{
		struct std::tm newtime;
		std::time_t now = time(0);
		localtime_s(&newtime, &now);

		return MString() + (newtime.tm_mon + 1);

		//return kStrUnknown;
	}
	MString HudNode::getYear()
	{
		struct std::tm newtime;
		std::time_t now = time(0);
		localtime_s(&newtime, &now);

		return MString() + (newtime.tm_year + 1900);

		//return kStrUnknown;
	}

	MString HudNode::getYear2()
	{
		MString year = getYear();
		return year.substringW(2, 3);
		//return kStrUnknown;
	}

	MString HudNode::getDate()
	{
		struct std::tm newtime;
		std::time_t now = time(0);
		localtime_s(&newtime, &now);

		MString day = MString() + newtime.tm_mday;
		MString month = MString() + (newtime.tm_mon + 1);
		MString year = MString() + (newtime.tm_year + 1900);

		return MString() + day + "/" + month + "/" + year;
	}

	HudNode::HudNode() :
		m_loadDefault(true),
		m_isDirty(true)
	{
	}

	void* HudNode::creator()
	{
		return new HudNode();
	}

	MStatus HudNode::initialize()
	{
		MStatus status;
		MFnNumericAttribute  nAttr;
		MFnEnumAttribute     eAttr;
		MFnCompoundAttribute cAttr;
		MFnUnitAttribute     uAttr;
		MFnTypedAttribute    tAttr;
		MFnMessageAttribute	 mAttr;

		// Enable
		enable = nAttr.create("enable", "enable", MFnNumericData::kBoolean, 1, &status);
		nAttr.setReadable(true);
		nAttr.setWritable(true);
		nAttr.setStorable(true);
		nAttr.setChannelBox(true);
		addAttribute(enable);

		// Diplay Type
		displayType = eAttr.create("displayType", "displayType", DisplayType::kAllCameras, &status);
		eAttr.setReadable(true);
		eAttr.setWritable(true);
		eAttr.setStorable(true);
		eAttr.setChannelBox(true);
		eAttr.addField("none", DisplayType::kDisplayNone);
		eAttr.addField("all", DisplayType::kAllCameras);
		eAttr.addField("connectedOnly", DisplayType::kConnectedCamerasOnly);
		eAttr.addField("hudAttributeOnly", DisplayType::kCamerasWithHudAttribute);
		addAttribute(displayType);

		// Camera
		camera = mAttr.create("camera", "camera");
		mAttr.setReadable(true);
		mAttr.setWritable(true);
		mAttr.setStorable(true);
		addAttribute(camera);

		// Add Text Attributes
		addTextAttributes();

		// Add Line Attributes
		addLineAttributes();

		// Add Rect Attributes
		addRectAttributes();

		// Add Image Attributes
		addImageAttributes();

		//
		return MS::kSuccess;
	}

	void HudNode::addTextAttributes()
	{
		MStatus status;
		MFnNumericAttribute  nAttr;
		MFnEnumAttribute     eAttr;
		MFnCompoundAttribute cAttr;
		MFnUnitAttribute     uAttr;
		MFnTypedAttribute    tAttr;
		MFnMessageAttribute	 mAttr;

		// Text Attributes
		textList = cAttr.create("textList", "textList", &status);
		cAttr.setArray(true);
		cAttr.setIndexMatters(true);
		cAttr.setWritable(true);
		cAttr.setReadable(true);

		textType = eAttr.create("textType", "textType", TextType::kString, &status);
		eAttr.setReadable(true);
		eAttr.setWritable(true);
		eAttr.setStorable(true);

		eAttr.addField("string", TextType::kString);
		eAttr.addField("integer", TextType::kInt);
		eAttr.addField("float", TextType::kFloat);
		eAttr.addField("boolean", TextType::kBool);
		eAttr.addField("mel", TextType::kMel);
		eAttr.addField("python", TextType::kPython);

		eAttr.addField("user", TextType::kUser);
		eAttr.addField("file", TextType::kFile);
		eAttr.addField("scene", TextType::kScene);
		eAttr.addField("camera", TextType::kCamera);
		eAttr.addField("focal", TextType::kFocal);
		eAttr.addField("fps", TextType::kFps);

		eAttr.addField("frame", TextType::kFrame);
		eAttr.addField("time", TextType::kTime);
		eAttr.addField("timecode", TextType::kTimecode);
		eAttr.addField("date", TextType::kDate);

		cAttr.addChild(textType);

		MFnStringData stringFn;
		//MObject defaultString = stringFn.create("text");
		MObject defaultString = stringFn.create(kStrDebug);
		textString = tAttr.create("textString", "textString", MFnData::kString, defaultString, &status);
		tAttr.setStorable(true);
		cAttr.addChild(textString);

		textInt = nAttr.create("textInteger", "textInteger", MFnNumericData::kInt, 0, &status);
		nAttr.setReadable(true);
		nAttr.setWritable(true);
		nAttr.setStorable(true);
		cAttr.addChild(textInt);

		textFloat = nAttr.create("textFloat", "textFloat", MFnNumericData::kFloat, 0.0f, &status);
		nAttr.setReadable(true);
		nAttr.setWritable(true);
		nAttr.setStorable(true);
		cAttr.addChild(textFloat);

		textBool = nAttr.create("textBoolean", "textBoolean", MFnNumericData::kBoolean, 0, &status);
		nAttr.setReadable(true);
		nAttr.setWritable(true);
		nAttr.setStorable(true);
		cAttr.addChild(textBool);

		textAlignment = eAttr.create("textAlignment", "textAlignment", MUIDrawManager::kCenter, &status);
		eAttr.setReadable(true);
		eAttr.setWritable(true);
		eAttr.setStorable(true);
		eAttr.addField("left", MUIDrawManager::kLeft);
		eAttr.addField("center", MUIDrawManager::kCenter);
		eAttr.addField("right", MUIDrawManager::kRight);
		cAttr.addChild(textAlignment);

		textAnchor = eAttr.create("textAnchorPosition", "textAnchorPosition", AnchorType::kCenter, &status);
		eAttr.setReadable(true);
		eAttr.setWritable(true);
		eAttr.setStorable(true);
		eAttr.addField("none", AnchorType::kAnchorNone);
		eAttr.addField("topLeft", AnchorType::kTopLeft);
		eAttr.addField("topCenter", AnchorType::kTopCenter);
		eAttr.addField("topRight", AnchorType::kTopRight);
		eAttr.addField("centerLeft", AnchorType::kCenterLeft);
		eAttr.addField("center", AnchorType::kCenter);
		eAttr.addField("centerRight", AnchorType::kCenterRight);
		eAttr.addField("bottomLeft", AnchorType::kBottomLeft);
		eAttr.addField("bottomCenter", AnchorType::kBottomCenter);
		eAttr.addField("bottomRight", AnchorType::kBottomRight);
		cAttr.addChild(textAnchor);

		textPosition = nAttr.create("textPosition", "textPosition", MFnNumericData::k3Double, 0, &status);
		nAttr.setDefault(0.0, 0.0, 0.001);
		nAttr.setReadable(true);
		nAttr.setWritable(true);
		nAttr.setStorable(true);
		cAttr.addChild(textPosition);

		textDimensions = nAttr.create("textDimensions", "textDimensions", MFnNumericData::k3Double, 100.0, &status);
		nAttr.setDefault(100.0, 100.0, 0.0);
		nAttr.setMin(0);
		nAttr.setReadable(true);
		nAttr.setWritable(true);
		nAttr.setStorable(true);
		cAttr.addChild(textDimensions);

		textFontIncline = nAttr.create("textFontIncline", "textFontIncline", MFnNumericData::kInt, 0, &status);
		nAttr.setMin(0);
		nAttr.setReadable(true);
		nAttr.setWritable(true);
		nAttr.setStorable(true);
		cAttr.addChild(textFontIncline);

		textFontWeight = nAttr.create("textFontWeight", "textFontWeight", MFnNumericData::kInt, 0, &status);
		nAttr.setMin(0);
		nAttr.setReadable(true);
		nAttr.setWritable(true);
		nAttr.setStorable(true);
		cAttr.addChild(textFontWeight);

		textFontStretch = nAttr.create("textFontStretch", "textFontStretch", MFnNumericData::kInt, 0, &status);
		nAttr.setMin(0);
		nAttr.setReadable(true);
		nAttr.setWritable(true);
		nAttr.setStorable(true);
		cAttr.addChild(textFontStretch);

		textFontLine = nAttr.create("textFontLine", "textFontLine", MFnNumericData::kInt, 0, &status);
		nAttr.setMin(0);
		nAttr.setReadable(true);
		nAttr.setWritable(true);
		nAttr.setStorable(true);
		cAttr.addChild(textFontLine);

		textFontSize = nAttr.create("textFontSize", "textFontSize", MFnNumericData::kInt, 1, &status);
		nAttr.setMin(0);
		nAttr.setReadable(true);
		nAttr.setWritable(true);
		nAttr.setStorable(true);
		cAttr.addChild(textFontSize);

		textFontIndex = eAttr.create("textFontName", "textFontName", 0);
		eAttr.setReadable(true);
		eAttr.setWritable(true);
		eAttr.setStorable(true);

		unsigned int nFont = MUIDrawManager::getFontList(HudNode::fontList);

		//if (nFont == 0)
		//{
		//	perror("No font available!");
		//}

		for (unsigned int i = 0; i < nFont; i++)
		{
			MString str = HudNode::fontList[i];
			eAttr.addField(str, (short)i);
		}

		cAttr.addChild(textFontIndex);

		textColor = nAttr.createColor("textColor", "textColor");
		nAttr.setDefault(1.0f, 1.0f, 1.0f);
		nAttr.setUsedAsColor(true);
		nAttr.setReadable(true);
		nAttr.setWritable(true);
		nAttr.setStorable(true);
		cAttr.addChild(textColor);

		textAlpha = nAttr.create("textAlpha", "textAlpha", MFnNumericData::kFloat, 1.0f);
		nAttr.setMin(0.0f);
		nAttr.setMax(1.0f);
		nAttr.setDefault(1.0f);
		nAttr.setReadable(true);
		nAttr.setWritable(true);
		nAttr.setStorable(true);
		cAttr.addChild(textAlpha);

		textBackgroundColor = nAttr.createColor("textBackgroundColor", "textBackgroundColor");
		nAttr.setDefault(0.0f, 0.0f, 0.0f);
		nAttr.setUsedAsColor(true);
		nAttr.setReadable(true);
		nAttr.setWritable(true);
		nAttr.setStorable(true);
		cAttr.addChild(textBackgroundColor);

		textBackgroundAlpha = nAttr.create("textBackgroundAlpha", "textBackgroundAlpha", MFnNumericData::kFloat, 1.0f);
		nAttr.setMin(0.0f);
		nAttr.setMax(1.0f);
		nAttr.setReadable(true);
		nAttr.setWritable(true);
		nAttr.setStorable(true);
		cAttr.addChild(textBackgroundAlpha);

		textIs2D = nAttr.create("text2D", "text2D", MFnNumericData::kBoolean, 1, &status);
		nAttr.setReadable(true);
		nAttr.setWritable(true);
		nAttr.setStorable(true);
		cAttr.addChild(textIs2D);

		textDynamic = nAttr.create("textDynamic", "textDynamic", MFnNumericData::kBoolean, 0, &status);
		nAttr.setReadable(true);
		nAttr.setWritable(true);
		nAttr.setStorable(true);
		cAttr.addChild(textDynamic);

		textScale = nAttr.create("textScale", "textScale", MFnNumericData::k3Double, 1, &status);
		nAttr.setDefault(1, 1, 1);
		nAttr.setReadable(true);
		nAttr.setWritable(true);
		nAttr.setStorable(true);
		cAttr.addChild(textScale);

		textArea = eAttr.create("textAnchorArea", "textAnchorArea", AreaType::kResolution, &status);
		eAttr.setReadable(true);
		eAttr.setWritable(true);
		eAttr.setStorable(true);
		eAttr.addField("none", AreaType::kAreaNone);
		eAttr.addField("resolutionGate", AreaType::kResolution);
		eAttr.addField("safeAction", AreaType::kSafeAction);
		eAttr.addField("safeTitle", AreaType::kSafeTitle);
		cAttr.addChild(textArea);

		textEnable = nAttr.create("textEnable", "textEnable", MFnNumericData::kBoolean, 1, &status);
		nAttr.setReadable(true);
		nAttr.setWritable(true);
		nAttr.setStorable(true);
		cAttr.addChild(textEnable);

		textEnableBackground = nAttr.create("textEnableBackground", "textEnableBackground", MFnNumericData::kBoolean, 1, &status);
		nAttr.setReadable(true);
		nAttr.setWritable(true);
		nAttr.setStorable(true);
		cAttr.addChild(textEnableBackground);

		addAttribute(textList);
	}


	void HudNode::addLineAttributes()
	{
		MStatus status;
		MFnNumericAttribute  nAttr;
		MFnEnumAttribute     eAttr;
		MFnCompoundAttribute cAttr;
		MFnUnitAttribute     uAttr;
		MFnTypedAttribute    tAttr;
		MFnMessageAttribute	 mAttr;

		// Line Attributes
		lineList = cAttr.create("lineList", "lineList", &status);
		cAttr.setArray(true);
		cAttr.setIndexMatters(true);
		cAttr.setWritable(true);
		cAttr.setReadable(true);

		// Line Enable
		lineEnable = nAttr.create("lineEnable", "lineEnable", MFnNumericData::kBoolean, 1, &status);
		nAttr.setReadable(true);
		nAttr.setWritable(true);
		nAttr.setStorable(true);
		cAttr.addChild(lineEnable);

		// Line is 2D
		lineIs2D = nAttr.create("lineIs2D", "lineIs2D", MFnNumericData::kBoolean, 1, &status);
		nAttr.setReadable(true);
		nAttr.setWritable(true);
		nAttr.setStorable(true);
		cAttr.addChild(lineIs2D);

		// Line Area
		lineArea = eAttr.create("lineArea", "lineArea", AreaType::kResolution, &status);
		eAttr.setReadable(true);
		eAttr.setWritable(true);
		eAttr.setStorable(true);
		eAttr.addField("none", AreaType::kAreaNone);
		eAttr.addField("resolutionGate", AreaType::kResolution);
		eAttr.addField("safeAction", AreaType::kSafeAction);
		eAttr.addField("safeTitle", AreaType::kSafeTitle);
		cAttr.addChild(lineArea);

		// Line Anchor
		lineAnchor = eAttr.create("lineAnchor", "lineAnchor", AnchorType::kCenter, &status);
		eAttr.setReadable(true);
		eAttr.setWritable(true);
		eAttr.setStorable(true);
		eAttr.addField("none", AnchorType::kAnchorNone);
		eAttr.addField("topLeft", AnchorType::kTopLeft);
		eAttr.addField("topCenter", AnchorType::kTopCenter);
		eAttr.addField("topRight", AnchorType::kTopRight);
		eAttr.addField("centerLeft", AnchorType::kCenterLeft);
		eAttr.addField("center", AnchorType::kCenter);
		eAttr.addField("centerRight", AnchorType::kCenterRight);
		eAttr.addField("bottomLeft", AnchorType::kBottomLeft);
		eAttr.addField("bottomCenter", AnchorType::kBottomCenter);
		eAttr.addField("bottomRight", AnchorType::kBottomRight);
		cAttr.addChild(lineAnchor);

		// Line Position
		linePosition = nAttr.create("linePosition", "linePosition", MFnNumericData::k3Double, 0, &status);
		nAttr.setDefault(0.0, 0.0, 0.0);
		nAttr.setReadable(true);
		nAttr.setWritable(true);
		nAttr.setStorable(true);
		cAttr.addChild(linePosition);

		// Line Style
		lineStyle = eAttr.create("lineStyle", "lineStyle", 0, &status);
		eAttr.setReadable(true);
		eAttr.setWritable(true);
		eAttr.setStorable(true);
		eAttr.addField("solid", 0);
		eAttr.addField("shortDotted", 1);
		eAttr.addField("shortDashed", 2);
		eAttr.addField("dashed", 3);
		eAttr.addField("dotted", 4);
		eAttr.addField("custom", 5);
		cAttr.addChild(lineStyle);

		// Line Factor
		lineStyleFactor = nAttr.create("lineStyleFactor", "lineStyleFactor", MFnNumericData::kInt, 1, &status);
		nAttr.setMin(0);
		nAttr.setReadable(true);
		nAttr.setWritable(true);
		nAttr.setStorable(true);
		cAttr.addChild(lineStyleFactor);

		// Line Pattern
		lineStylePattern = nAttr.create("lineStylePattern", "lineStylePattern", MFnNumericData::kInt, 1, &status);
		nAttr.setMin(-32768);
		nAttr.setMax(32768);
		nAttr.setReadable(true);
		nAttr.setWritable(true);
		nAttr.setStorable(true);
		cAttr.addChild(lineStylePattern);

		// Line Width
		lineWidth = nAttr.create("lineWidth", "lineWidth", MFnNumericData::kFloat, 1.0f);
		nAttr.setMin(0.0f);
		nAttr.setDefault(1.0f);
		nAttr.setReadable(true);
		nAttr.setWritable(true);
		nAttr.setStorable(true);
		cAttr.addChild(lineWidth);

		// Line Start
		lineStart = nAttr.create("lineStart", "lineStart", MFnNumericData::k3Double, 0, &status);
		nAttr.setDefault(0.0, 0.0, 0.0);
		nAttr.setReadable(true);
		nAttr.setWritable(true);
		nAttr.setStorable(true);
		cAttr.addChild(lineStart);

		// Line End
		lineEnd = nAttr.create("lineEnd", "lineEnd", MFnNumericData::k3Double, 0, &status);
		nAttr.setDefault(100.0, 100.0, 100.0);
		nAttr.setReadable(true);
		nAttr.setWritable(true);
		nAttr.setStorable(true);
		cAttr.addChild(lineEnd);

		// Line Scale
		lineScale = nAttr.create("lineScale", "lineScale", MFnNumericData::k3Double, 1, &status);
		nAttr.setDefault(1, 1, 1);
		nAttr.setReadable(true);
		nAttr.setWritable(true);
		nAttr.setStorable(true);
		cAttr.addChild(lineScale);

		// Line Color
		lineColor = nAttr.createColor("lineColor", "lineColor");
		nAttr.setDefault(1.0f, 1.0f, 1.0f);
		nAttr.setUsedAsColor(true);
		nAttr.setReadable(true);
		nAttr.setWritable(true);
		nAttr.setStorable(true);
		cAttr.addChild(lineColor);

		lineAlpha = nAttr.create("lineAlpha", "lineAlpha", MFnNumericData::kFloat, 1.0f);
		nAttr.setMin(0.0f);
		nAttr.setMax(1.0f);
		nAttr.setDefault(1.0f);
		nAttr.setReadable(true);
		nAttr.setWritable(true);
		nAttr.setStorable(true);
		cAttr.addChild(lineAlpha);

		//
		addAttribute(lineList);
	}

	void HudNode::addRectAttributes()
	{
		MStatus status;
		MFnNumericAttribute  nAttr;
		MFnEnumAttribute     eAttr;
		MFnCompoundAttribute cAttr;
		MFnUnitAttribute     uAttr;
		MFnTypedAttribute    tAttr;
		MFnMessageAttribute	 mAttr;

		// Rect Attributes
		rectList = cAttr.create("rectList", "rectList", &status);
		cAttr.setArray(true);
		cAttr.setIndexMatters(true);
		cAttr.setWritable(true);
		cAttr.setReadable(true);

		// Rect Enable
		rectEnable = nAttr.create("rectEnable", "rectEnable", MFnNumericData::kBoolean, 1, &status);
		nAttr.setReadable(true);
		nAttr.setWritable(true);
		nAttr.setStorable(true);
		cAttr.addChild(rectEnable);

		// Rect is 2D
		rectIs2D = nAttr.create("rectIs2D", "rectIs2D", MFnNumericData::kBoolean, 1, &status);
		nAttr.setReadable(true);
		nAttr.setWritable(true);
		nAttr.setStorable(true);
		cAttr.addChild(rectIs2D);

		// Rect Area
		rectArea = eAttr.create("rectArea", "rectArea", AreaType::kResolution, &status);
		eAttr.setReadable(true);
		eAttr.setWritable(true);
		eAttr.setStorable(true);
		eAttr.addField("none", AreaType::kAreaNone);
		eAttr.addField("resolutionGate", AreaType::kResolution);
		eAttr.addField("safeAction", AreaType::kSafeAction);
		eAttr.addField("safeTitle", AreaType::kSafeTitle);
		cAttr.addChild(rectArea);

		// Rect Anchor
		rectAnchor = eAttr.create("rectAnchor", "rectAnchor", AnchorType::kCenter, &status);
		eAttr.setReadable(true);
		eAttr.setWritable(true);
		eAttr.setStorable(true);
		eAttr.addField("none", AnchorType::kAnchorNone);
		eAttr.addField("topLeft", AnchorType::kTopLeft);
		eAttr.addField("topCenter", AnchorType::kTopCenter);
		eAttr.addField("topRight", AnchorType::kTopRight);
		eAttr.addField("centerLeft", AnchorType::kCenterLeft);
		eAttr.addField("center", AnchorType::kCenter);
		eAttr.addField("centerRight", AnchorType::kCenterRight);
		eAttr.addField("bottomLeft", AnchorType::kBottomLeft);
		eAttr.addField("bottomCenter", AnchorType::kBottomCenter);
		eAttr.addField("bottomRight", AnchorType::kBottomRight);
		cAttr.addChild(rectAnchor);

		// Rect Position
		rectPosition = nAttr.create("rectPosition", "rectPosition", MFnNumericData::k3Double, 0, &status);
		nAttr.setDefault(0.0, 0.0, 0.0);
		nAttr.setReadable(true);
		nAttr.setWritable(true);
		nAttr.setStorable(true);
		cAttr.addChild(rectPosition);

		// Rect Dimensions
		rectDimensions = nAttr.create("rectDimension", "rectDimension", MFnNumericData::k3Double, 100.0, &status);
		nAttr.setDefault(10.0, 10.0, 0.0);
		nAttr.setMin(0);
		nAttr.setReadable(true);
		nAttr.setWritable(true);
		nAttr.setStorable(true);
		cAttr.addChild(rectDimensions);

		// Rect Up
		rectUp = nAttr.create("rectUp", "rectUp", MFnNumericData::k3Double, 0, &status);
		nAttr.setDefault(0.0, 1.0, 0.0);
		nAttr.setReadable(true);
		nAttr.setWritable(true);
		nAttr.setStorable(true);
		cAttr.addChild(rectUp);

		// Rect Normal
		rectNormal = nAttr.create("rectNormal", "rectNormal", MFnNumericData::k3Double, 0, &status);
		nAttr.setDefault(1.0, 0.0, 0.0);
		nAttr.setReadable(true);
		nAttr.setWritable(true);
		nAttr.setStorable(true);
		cAttr.addChild(rectNormal);

		// Rect Scale
		rectScale = nAttr.create("rectScale", "rectScale", MFnNumericData::k3Double, 1, &status);
		nAttr.setDefault(1, 1, 1);
		nAttr.setReadable(true);
		nAttr.setWritable(true);
		nAttr.setStorable(true);
		cAttr.addChild(rectScale);

		// Rect Is Filled
		rectIsFilled = nAttr.create("rectIsFilled", "rectIsFilled", MFnNumericData::kBoolean, 1, &status);
		nAttr.setReadable(true);
		nAttr.setWritable(true);
		nAttr.setStorable(true);
		cAttr.addChild(rectIsFilled);

		// Rect Color
		rectColor = nAttr.createColor("rectColor", "rectColor");
		nAttr.setDefault(1.0f, 1.0f, 1.0f);
		nAttr.setUsedAsColor(true);
		nAttr.setReadable(true);
		nAttr.setWritable(true);
		nAttr.setStorable(true);
		cAttr.addChild(rectColor);

		rectAlpha = nAttr.create("rectAlpha", "rectAlpha", MFnNumericData::kFloat, 1.0f);
		nAttr.setMin(0.0f);
		nAttr.setMax(1.0f);
		nAttr.setDefault(1.0f);
		nAttr.setReadable(true);
		nAttr.setWritable(true);
		nAttr.setStorable(true);
		cAttr.addChild(rectAlpha);

		// Rect Line Style
		rectLineStyle = eAttr.create("rectLineStyle", "rectLineStyle", 0, &status);
		eAttr.setReadable(true);
		eAttr.setWritable(true);
		eAttr.setStorable(true);
		eAttr.addField("solid", 0);
		eAttr.addField("shortDotted", 1);
		eAttr.addField("shortDashed", 2);
		eAttr.addField("dashed", 3);
		eAttr.addField("dotted", 4);
		eAttr.addField("custom", 5);
		cAttr.addChild(rectLineStyle);

		// Rect Line Factor
		rectLineStyleFactor = nAttr.create("rectLineStyleFactor", "rectLineStyleFactor", MFnNumericData::kInt, 1, &status);
		nAttr.setMin(0);
		nAttr.setReadable(true);
		nAttr.setWritable(true);
		nAttr.setStorable(true);
		cAttr.addChild(rectLineStyleFactor);

		// Rect Line Pattern
		rectLineStylePattern = nAttr.create("rectLineStylePattern", "rectLineStylePattern", MFnNumericData::kInt, 1, &status);
		nAttr.setMin(-32768);
		nAttr.setMax(32768);
		nAttr.setReadable(true);
		nAttr.setWritable(true);
		nAttr.setStorable(true);
		cAttr.addChild(rectLineStylePattern);

		// Rect Line Width
		rectLineWidth = nAttr.create("rectLineWidth", "rectLineWidth", MFnNumericData::kFloat, 1.0f);
		nAttr.setMin(0.0f);
		nAttr.setDefault(1.0f);
		nAttr.setReadable(true);
		nAttr.setWritable(true);
		nAttr.setStorable(true);
		cAttr.addChild(rectLineWidth);

		//
		addAttribute(rectList);
	}

	void HudNode::addImageAttributes()
	{
		MStatus status;
		MFnNumericAttribute  nAttr;
		MFnEnumAttribute     eAttr;
		MFnCompoundAttribute cAttr;
		MFnUnitAttribute     uAttr;
		MFnTypedAttribute    tAttr;
		MFnMessageAttribute	 mAttr;

		// Image Attributes
		imageList = cAttr.create("imageList", "imageList", &status);
		cAttr.setArray(true);
		cAttr.setIndexMatters(true);
		cAttr.setWritable(true);
		cAttr.setReadable(true);
		cAttr.setHidden(false);

		// Image Enable
		imageEnable = nAttr.create("imageEnable", "imageEnable", MFnNumericData::kBoolean, 1, &status);
		nAttr.setReadable(true);
		nAttr.setWritable(true);
		nAttr.setStorable(true);
		cAttr.addChild(imageEnable);

		// Image is 2D
		imageIs2D = nAttr.create("imageIs2D", "imageIs2D", MFnNumericData::kBoolean, 1, &status);
		nAttr.setReadable(true);
		nAttr.setWritable(true);
		nAttr.setStorable(true);
		cAttr.addChild(imageIs2D);

		// Image Path
		MFnStringData stringFn;
		MObject defaultString = stringFn.create("");
		imagePath = tAttr.create("imagePath", "imagePath", MFnData::kString, defaultString, &status);
		tAttr.setStorable(true);
		tAttr.setUsedAsFilename(true);
		cAttr.addChild(imagePath);

		// Image Area
		imageArea = eAttr.create("imageArea", "imageArea", AreaType::kResolution, &status);
		eAttr.setReadable(true);
		eAttr.setWritable(true);
		eAttr.setStorable(true);
		eAttr.addField("none", AreaType::kAreaNone);
		eAttr.addField("resolutionGate", AreaType::kResolution);
		eAttr.addField("safeAction", AreaType::kSafeAction);
		eAttr.addField("safeTitle", AreaType::kSafeTitle);
		cAttr.addChild(imageArea);

		// Image Anchor
		imageAnchor = eAttr.create("imageAnchor", "imageAnchor", AnchorType::kCenter, &status);
		eAttr.setReadable(true);
		eAttr.setWritable(true);
		eAttr.setStorable(true);
		eAttr.addField("none", AnchorType::kAnchorNone);
		eAttr.addField("topLeft", AnchorType::kTopLeft);
		eAttr.addField("topCenter", AnchorType::kTopCenter);
		eAttr.addField("topRight", AnchorType::kTopRight);
		eAttr.addField("centerLeft", AnchorType::kCenterLeft);
		eAttr.addField("center", AnchorType::kCenter);
		eAttr.addField("centerRight", AnchorType::kCenterRight);
		eAttr.addField("bottomLeft", AnchorType::kBottomLeft);
		eAttr.addField("bottomCenter", AnchorType::kBottomCenter);
		eAttr.addField("bottomRight", AnchorType::kBottomRight);
		cAttr.addChild(imageAnchor);

		// Image Position
		imagePosition = nAttr.create("imagePosition", "imagePosition", MFnNumericData::k3Double, 0, &status);
		nAttr.setDefault(0.0, 0.0, 0.0);
		nAttr.setReadable(true);
		nAttr.setWritable(true);
		nAttr.setStorable(true);
		cAttr.addChild(imagePosition);

		// Image Dimensions
		imageDimensions = nAttr.create("imageDimensions", "imageDimensions", MFnNumericData::k3Double, 100.0, &status);
		nAttr.setDefault(10.0, 10.0, 0.0);
		nAttr.setMin(0);
		nAttr.setReadable(true);
		nAttr.setWritable(true);
		nAttr.setStorable(true);
		cAttr.addChild(imageDimensions);

		// Image Up
		imageUp = nAttr.create("imageUp", "imageUp", MFnNumericData::k3Double, 0, &status);
		nAttr.setDefault(0.0, 1.0, 0.0);
		nAttr.setReadable(true);
		nAttr.setWritable(true);
		nAttr.setStorable(true);
		cAttr.addChild(imageUp);

		// Image Normal
		imageNormal = nAttr.create("imageNormal", "imageNormal", MFnNumericData::k3Double, 0, &status);
		nAttr.setDefault(1.0, 0.0, 0.0);
		nAttr.setReadable(true);
		nAttr.setWritable(true);
		nAttr.setStorable(true);
		cAttr.addChild(imageNormal);

		// Image Scale
		imageScale = nAttr.create("imageScale", "imageScale", MFnNumericData::k3Double, 1, &status);
		nAttr.setDefault(1, 1, 1);
		nAttr.setReadable(true);
		nAttr.setWritable(true);
		nAttr.setStorable(true);
		cAttr.addChild(imageScale);

		// Image Rect Is Filled
		imageIsFilled = nAttr.create("imageIsFilled", "imageIsFilled", MFnNumericData::kBoolean, 1, &status);
		nAttr.setReadable(true);
		nAttr.setWritable(true);
		nAttr.setStorable(true);
		cAttr.addChild(imageIsFilled);

		// Image Color
		imageColor = nAttr.createColor("imageColor", "imageColor");
		nAttr.setDefault(1.0f, 1.0f, 1.0f);
		nAttr.setUsedAsColor(true);
		nAttr.setReadable(true);
		nAttr.setWritable(true);
		nAttr.setStorable(true);
		cAttr.addChild(imageColor);

		imageAlpha = nAttr.create("imageAlpha", "imageAlpha", MFnNumericData::kFloat, 1.0f);
		nAttr.setMin(0.0f);
		nAttr.setMax(1.0f);
		nAttr.setDefault(1.0f);
		nAttr.setReadable(true);
		nAttr.setWritable(true);
		nAttr.setStorable(true);
		cAttr.addChild(imageAlpha);

		//
		addAttribute(imageList);
	}

	void HudNode::postConstructor()
	{
		MStatus status;
		m_loadDefault = true;
		m_isDirty = true;

		MFnDependencyNode nodeFn(thisMObject(), &status);

		if (!status)
		{
			return;
		}

		nodeFn.findPlug("castsShadows", false).setBool(false);
		nodeFn.findPlug("receiveShadows", false).setBool(false);
		nodeFn.findPlug("motionBlur", false).setBool(false);
	}

	// TODO: Get updated position.
	MVector HudNode::getOffset(HudUserData& data, BaseData& item)
	{
		MVector offset(0.0, 0.0);

		if (!item.enable || !item.is2D)
		{
			return offset;
		}
		MVector itemHalf(item.dimensions.x * 0.5, item.dimensions.y * 0.5);

		// Area
		MVector origin(0.0, 0.0);
		MVector dimensions(0.0, 0.0);

		AreaType area = static_cast<AreaType>(item.areaType);
		
		if (area == AreaType::kAreaNone)
		{
			dimensions.x = (double)data.viewportWidth;
			dimensions.y = (double)data.viewportHeight;
		}
		else if (area == AreaType::kResolution)
		{
			origin = data.maskOrigin;
			dimensions = data.maskDimension;
		}

		else if (area == AreaType::kSafeAction)
		{
			origin = data.safeActionOrigin;
			dimensions = data.safeActionDimension;
		}

		else if (area == AreaType::kSafeTitle)
		{
			origin = data.safeTitleOrigin;
			dimensions = data.safeTitleDimension;
		}

		// Anchor
		AnchorType anchor = static_cast<AnchorType>(item.anchorType);

		if (anchor == AnchorType::kAnchorNone)
		{
			//offset.x = 0;
			//offset.y = 0;
		}
		else if (anchor == AnchorType::kTopLeft)
		{
			offset.x = origin.x;
			offset.y = origin.y + dimensions.y - item.dimensions.y;
		}
		else if (anchor == AnchorType::kTopCenter)
		{
			offset.x = origin.x + (dimensions.x * 0.5);
			offset.y = origin.y + dimensions.y - item.dimensions.y;
		}
		else if (anchor == AnchorType::kTopRight)
		{
			offset.x = origin.x + dimensions.x;
			offset.y = origin.y + dimensions.y - item.dimensions.y;
		}
		else if (anchor == AnchorType::kCenterLeft)
		{
			offset.x = origin.x;
			offset.y = origin.y + (dimensions.y * 0.5) - itemHalf.y;
		}
		else if (anchor == AnchorType::kCenter)
		{
			offset.x = origin.x + (dimensions.x * 0.5);
			offset.y = origin.y + (dimensions.y * 0.5) - itemHalf.y;
		}
		else if (anchor == AnchorType::kCenterRight)
		{
			offset.x = origin.x + dimensions.x;
			offset.y = origin.y + (dimensions.y * 0.5) - itemHalf.y;
		}
		else if (anchor == AnchorType::kBottomLeft)
		{
			offset.x = origin.x;
			offset.y = origin.y;
		}
		else if (anchor == AnchorType::kBottomCenter)
		{
			offset.x = origin.x + (dimensions.x * 0.5);
			offset.y = origin.y;
		}
		else if (anchor == AnchorType::kBottomRight)
		{
			offset.x = origin.x + dimensions.x;
			offset.y = origin.y;
		}

		//MGlobal::displayInfo(MString("Area: ") + area);
		//MGlobal::displayInfo(MString("Anchor: ") + anchor);
		//MGlobal::displayInfo(MString("Offset: ") + offset.x + MString(", ") + offset.y);

		// Update Position
		return offset;
	}


	void HudNode::formatString(HudUserData& data, TextData& textData)
	{
		textData.textResult = textData.textString;

		if (!textData.enable)
		{
			return;
		}

		TextType type = static_cast<TextType>(textData.textType);

		if (type == TextType::kString)
		{
			if (textData.textString.length() == 0)
			{
				textData.textResult = " ";
				return;
			}

			textData.textResult = textData.textString;

			if (textData.textString.indexW("{") == -1 || textData.textString.indexW("}") == -1)
			{
				return;
			}

			// Replace User
			if (textData.textString.indexW(kStrUser) != -1)
			{
				textData.textResult.substitute(kStrUser, getUserName());
			}

			// Replace File
			if (textData.textString.indexW(kStrFile) != -1)
			{
				textData.textResult.substitute(kStrFile, getFileName());
			}

			// Replace Scene
			if (textData.textString.indexW(kStrScene) != -1)
			{
				textData.textResult.substitute(kStrScene, getSceneName());
			}

			// Replace Version
			if (textData.textString.indexW(kStrVersion) != -1)
			{
				textData.textResult.substitute(kStrVersion, getFileVersion());
			}

			// Replace Project
			if (textData.textString.indexW(kStrProject) != -1)
			{
				textData.textResult.substitute(kStrProject, getProject());
			}

			// Replace Camera
			if (textData.textString.indexW(kStrCamera) != -1)
			{
				textData.textResult.substitute(kStrCamera, data.cameraName);
			}

			// Replace Focal
			if (textData.textString.indexW(kStrFocal) != -1)
			{
				textData.textResult.substitute(kStrFocal, MString() + data.cameraFocal);
			}

			// Replace Fps
			if (textData.textString.indexW(kStrFPS) != -1)
			{
				textData.textResult.substitute(kStrFPS, getFps());
			}

			// Replace Frame
			if (textData.textString.indexW(kStrFrame) != -1)
			{
				textData.textResult.substitute(kStrFrame, getFrame());
			}

			// Replace Time
			if (textData.textString.indexW(kStrTime) != -1)
			{
				textData.textResult.substitute(kStrTime, getTime());
			}

			// Replace Timecode
			if (textData.textString.indexW(kStrTimecode) != -1)
			{
				textData.textResult.substitute(kStrTimecode, getTimecode());
			}

			// Replace Date
			if (textData.textString.indexW(kStrDate) != -1)
			{
				textData.textResult.substitute(kStrDate, getDate());
			}

			// Replace Month
			if (textData.textString.indexW(kStrMonth) != -1)
			{
				textData.textResult.substitute(kStrMonth, getMonth());
			}

			// Replace Day
			if (textData.textString.indexW(kStrDay) != -1)
			{
				textData.textResult.substitute(kStrDay, getDay());
			}

			// Replace Year
			if (textData.textString.indexW(kStrYear2) != -1)
			{
				textData.textResult.substitute(kStrYear2, getYear2());
			}

			// Replace Year
			if (textData.textString.indexW(kStrYear4) != -1)
			{
				textData.textResult.substitute(kStrYear4, getYear());
			}
			
			return;
		}
		else if (type == TextType::kInt)
		{
			textData.textResult = "";
			textData.textResult += textData.textInt;
			return;
		}
		else if (type == TextType::kFloat)
		{
			textData.textResult = "";
			textData.textResult += textData.textFloat;
			return;
		}
		else if (type == TextType::kBool)
		{
			MString boolStr = "False";

			if (textData.textBool)
			{
				boolStr = "True";
			}

			textData.textResult = boolStr;
			return;
		}
		else if (type == TextType::kMel)
		{
			//lineData.textResult = kStrUnknown;
			MGlobal::executeCommand(textData.textString, textData.textResult);
			return;
		}
		else if (type == TextType::kPython)
		{
			//lineData.textResult = kStrUnknown;
			MGlobal::executePythonCommand(textData.textString, textData.textResult);
			return;
		}
		else if (type == TextType::kUser)
		{
			textData.textResult = getUserName();
			return;
		}
		else if (type == TextType::kFile)
		{
			textData.textResult = getFileName();
			return;
		}
		else if (type == TextType::kScene)
		{
			textData.textResult = getSceneName();
			return;
		}
		else if (type == TextType::kProject)
		{
			textData.textResult = getProject();
			return;
		}
		else if (type == TextType::kCamera)
		{
			textData.textResult = data.cameraName;
			return;
		}
		else if (type == TextType::kFocal)
		{
			textData.textResult = data.cameraFocal;
			return;
		}
		else if (type == TextType::kFps)
		{
			textData.textResult = getFps();
			return;
		}
		else if (type == TextType::kFrame)
		{
			textData.textResult = getFrame();
			return;
		}
		else if (type == TextType::kTime)
		{
			textData.textResult = getTime();
			return;
		}
		else if (type == TextType::kTimecode)
		{
			textData.textResult = getTimecode();
			return;
		}
		else if (type == TextType::kDate)
		{
			textData.textResult = getDate();
			return;
		}
		else
		{
			textData.textResult = kStrUnknown;
			return;
		}
	}

	void HudNode::debugText(TextData& textData)
	{
		MGlobal::displayInfo("------------------------------------------------");
		MString message;

		MGlobal::displayInfo(MString("textType: ") + textData.textType);
		MGlobal::displayInfo(MString("textString: ") + textData.textString);
		MGlobal::displayInfo(MString("textInt: ") + textData.textInt);
		MGlobal::displayInfo(MString("textFloat: ") + textData.textFloat);
		MGlobal::displayInfo(MString("textBool: ") + textData.textBool);
		MGlobal::displayInfo(MString("textResult: ") + textData.textResult);

		MGlobal::displayInfo(MString("textAlign: ") + textData.textAlignment);
		MGlobal::displayInfo(MString("textAnchor: ") + textData.anchorType);

		MGlobal::displayInfo(MString("textPosition: ") + textData.position.x + MString(", ") + textData.position.y + MString(", ") + textData.position.z);
		MGlobal::displayInfo(MString("textDimension: ") + textData.dimensions.x + MString(", ") + textData.dimensions.y + MString(", ") + textData.dimensions.z);

		MGlobal::displayInfo(MString("textFontIncline: ") + textData.textFontIncline);
		MGlobal::displayInfo(MString("textFontWeight: ") + textData.textFontWeight);
		MGlobal::displayInfo(MString("textFontStretch: ") + textData.textFontStretch);
		MGlobal::displayInfo(MString("textFontLine: ") + textData.textFontLine);
		MGlobal::displayInfo(MString("textFontSize: ") + textData.textFontSize);
		MGlobal::displayInfo(MString("textFontIndex: ") + textData.textFontIndex);

		MGlobal::displayInfo(MString("textColor: ") + textData.color.r + MString(", ") + textData.color.b + MString(", ") + textData.color.g + MString(", ") + textData.color.a);

		MGlobal::displayInfo(MString("textBackgroundColor: ") + textData.backgroundColor.r + MString(", ") + textData.backgroundColor.b + MString(", ") + textData.backgroundColor.g + MString(", ") + textData.backgroundColor.a);

		MGlobal::displayInfo(MString("text2D: ") + textData.is2D);
		MGlobal::displayInfo(MString("textDynamic: ") + textData.dynamic);

		MGlobal::displayInfo(MString("textScale: ") + textData.scale.x + MString(", ") + textData.scale.y + MString(", ") + textData.scale.z);

		MGlobal::displayInfo(MString("textArea: ") + textData.areaType);
		MGlobal::displayInfo(MString("textEnable: ") + textData.enable);
		MGlobal::displayInfo(MString("textEnableBackground: ") + textData.enableBackground);
	}

	MStatus HudNode::getTextData(HudUserData& data)
	{
		data.textArray.clear();

		MStatus status;
		MPlug p, tp, cp;
		p = MPlug(thisMObject(), textList);
		int numPlugs = p.numElements();

		for (int i = 0; i < numPlugs; i++)
		{
			TextData td;
			tp = p.elementByPhysicalIndex(i, &status);
			status = getTextDataChild(tp, td);

			// Update Position
			MVector offset = getOffset(data, td);
			td.position.x += offset.x;
			td.position.y += offset.y;

			// Cleanup Text
			formatString(data, td);

			//// Debug
			//debugText(hudData);

			data.textArray.push_back(td);
		}

		return MS::kSuccess;
	}

	MStatus HudNode::getTextDataChild(MPlug textPlug, TextData& td)
	{
		MStatus stat;
		MPlug p, c;

		// Data Type / Value
		p = textPlug.child(0, &stat);
		McheckStatus(p.getValue(td.textType), "HudNode::getTextDataChild - textType");

		p = textPlug.child(1, &stat);
		McheckStatus(p.getValue(td.textString), "HudNode::getTextDataChild - textString");

		p = textPlug.child(2, &stat);
		McheckStatus(p.getValue(td.textInt), "HudNode::getTextDataChild - textInt");

		p = textPlug.child(3, &stat);
		McheckStatus(p.getValue(td.textFloat), "HudNode::getTextDataChild - textFloat");

		p = textPlug.child(4, &stat);
		McheckStatus(p.getValue(td.textBool), "HudNode::getTextDataChild - textBool");

		// Alignment
		p = textPlug.child(5, &stat);
		McheckStatus(p.getValue(td.textAlignment), "HudNode::getTextDataChild - textAlignment");

		p = textPlug.child(6, &stat);
		McheckStatus(p.getValue(td.anchorType), "HudNode::getTextDataChild - anchorType");

		// Position
		p = textPlug.child(7, &stat);
		c = p.child(0, &stat);
		McheckStatus(c.getValue(td.position.x), "HudNode::getTextDataChild - positionX");

		c = p.child(1, &stat);
		McheckStatus(c.getValue(td.position.y), "HudNode::getTextDataChild - positionY");

		c = p.child(2, &stat);
		McheckStatus(c.getValue(td.position.z), "HudNode::getTextDataChild - positionZ");

		// Dimensions
		p = textPlug.child(8, &stat);
		c = p.child(0, &stat);
		McheckStatus(c.getValue(td.dimensions.x), "HudNode::getTextDataChild - backgroundSizeX");

		c = p.child(1, &stat);
		McheckStatus(c.getValue(td.dimensions.y), "HudNode::getTextDataChild - backgroundSizeY");

		// Font
		p = textPlug.child(9, &stat);
		McheckStatus(p.getValue(td.textFontIncline), "HudNode::getTextDataChild - fontIncline");

		p = textPlug.child(10, &stat);
		McheckStatus(p.getValue(td.textFontWeight), "HudNode::getTextDataChild - fontWeight");

		p = textPlug.child(11, &stat);
		McheckStatus(p.getValue(td.textFontStretch), "HudNode::getTextDataChild - fontStretch");

		p = textPlug.child(12, &stat);
		McheckStatus(p.getValue(td.textFontLine), "HudNode::getTextDataChild - fontLine");

		p = textPlug.child(13, &stat);
		McheckStatus(p.getValue(td.textFontSize), "HudNode::getTextDataChild - fontSize");

		p = textPlug.child(14, &stat);
		McheckStatus(p.getValue(td.textFontIndex), "HudNode::getTextDataChild - fontName");

		// Color
		p = textPlug.child(15, &stat);
		c = p.child(0, &stat);
		McheckStatus(c.getValue(td.color.r), "HudNode::getTextDataChild - textColorR");

		c = p.child(1, &stat);
		McheckStatus(c.getValue(td.color.g), "HudNode::getTextDataChild - textColorG");

		c = p.child(2, &stat);
		McheckStatus(c.getValue(td.color.b), "HudNode::getTextDataChild - textColorB");

		p = textPlug.child(16, &stat);
		McheckStatus(p.getValue(td.color.a), "HudNode::getTextDataChild - textColorA");

		// Background Color
		p = textPlug.child(17, &stat);
		c = p.child(0, &stat);
		McheckStatus(c.getValue(td.backgroundColor.r), "HudNode::getTextDataChild - backgroundColorR");

		c = p.child(1, &stat);
		McheckStatus(c.getValue(td.backgroundColor.g), "HudNode::getTextDataChild - backgroundColorG");

		c = p.child(2, &stat);
		McheckStatus(c.getValue(td.backgroundColor.b), "HudNode::getTextDataChild - backgroundColorB");

		p = textPlug.child(18, &stat);
		McheckStatus(p.getValue(td.backgroundColor.a), "HudNode::getTextDataChild - backgroundColorA");

		// Is2D
		p = textPlug.child(19, &stat);
		McheckStatus(p.getValue(td.is2D), "HudNode::getTextDataChild - is2D");

		// Dynamic
		p = textPlug.child(20, &stat);
		McheckStatus(p.getValue(td.dynamic), "HudNode::getTextDataChild - dynamic");

		// Scale
		p = textPlug.child(21, &stat);
		c = p.child(0, &stat);
		McheckStatus(c.getValue(td.scale.x), "HudNode::getTextDataChild - scaleX");

		c = p.child(1, &stat);
		McheckStatus(c.getValue(td.scale.y), "HudNode::getTextDataChild - scaleY");

		// Area
		p = textPlug.child(22, &stat);
		McheckStatus(p.getValue(td.areaType), "HudNode::getTextDataChild - textArea");

		// Enable
		p = textPlug.child(23, &stat);
		McheckStatus(p.getValue(td.enable), "HudNode::getTextDataChild - textEnable");

		// Enable Background
		p = textPlug.child(24, &stat);
		McheckStatus(p.getValue(td.enableBackground), "HudNode::getTextDataChild - textEnableBackground");


		return MS::kSuccess;
	}

	MStatus HudNode::getLineData(HudUserData& hudData)
	{
		hudData.lineArray.clear();

		MStatus status;
		MPlug plug, linePlug, cp;
		plug = MPlug(thisMObject(), lineList);
		int numPlugs = plug.numElements();

		for (int i = 0; i < numPlugs; i++)
		{
			LineData lineData;
			linePlug = plug.elementByPhysicalIndex(i, &status);
			status = getLinePlugData(linePlug, lineData);

			// Update Position
			lineData.lineStart.x += lineData.position.x;
			lineData.lineStart.y += lineData.position.y;
			lineData.lineStart.z += lineData.position.z;

			lineData.lineEnd.x += lineData.position.x;
			lineData.lineEnd.y += lineData.position.y;
			lineData.lineEnd.z += lineData.position.z;

			if (lineData.is2D)
			{
				MVector offset = getOffset(hudData, lineData);
				lineData.lineStart.x += offset.x;
				lineData.lineStart.y += offset.y;
				lineData.lineStart.z = 0.0;

				lineData.lineEnd.x += offset.x;
				lineData.lineEnd.y += offset.y;
				lineData.lineEnd.z = 0.0;
			}

			// Update Scale
			lineData.lineEnd.x *= lineData.scale.x;
			lineData.lineEnd.y *= lineData.scale.y;
			
			if (!lineData.is2D)
			{
				lineData.lineEnd.z *= lineData.scale.z;
			}
			
			//// Debug
			//debugText(hudData);

			hudData.lineArray.push_back(lineData);
		}

		return MS::kSuccess;
	}

	MStatus HudNode::getLinePlugData(MPlug plug, LineData& lineData)
	{
		MStatus stat;
		MPlug p, c;

		// Enable
		p = plug.child(0, &stat);
		McheckStatus(p.getValue(lineData.enable), "HudNode::getTextDataChild - lineEnable");

		// Is2D
		p = plug.child(1, &stat);
		McheckStatus(p.getValue(lineData.is2D), "HudNode::getTextDataChild - lineIs2D");

		// Area
		p = plug.child(2, &stat);
		McheckStatus(p.getValue(lineData.areaType), "HudNode::getTextDataChild - lineArea");

		// Anchor
		p = plug.child(3, &stat);
		McheckStatus(p.getValue(lineData.anchorType), "HudNode::getTextDataChild - lineAnchor");

		// Position
		p = plug.child(4, &stat);
		c = p.child(0, &stat);
		McheckStatus(c.getValue(lineData.position.x), "HudNode::getTextDataChild - linePositionX");

		c = p.child(1, &stat);
		McheckStatus(c.getValue(lineData.position.y), "HudNode::getTextDataChild - linePositionY");

		c = p.child(2, &stat);
		McheckStatus(c.getValue(lineData.position.z), "HudNode::getTextDataChild - linePositionZ");

		// Style
		p = plug.child(5, &stat);
		McheckStatus(p.getValue(lineData.lineStyle), "HudNode::getTextDataChild - lineStyle");

		// Factor
		p = plug.child(6, &stat);
		McheckStatus(p.getValue(lineData.lineStyleFactor), "HudNode::getTextDataChild - lineStyleFactor");

		// Pattern
		p = plug.child(7, &stat);
		McheckStatus(p.getValue(lineData.lineStylePattern), "HudNode::getTextDataChild - lineStyle");

		// Width
		p = plug.child(8, &stat);
		McheckStatus(p.getValue(lineData.lineWidth), "HudNode::getTextDataChild - lineWidth");

		// Start
		p = plug.child(9, &stat);
		c = p.child(0, &stat);
		McheckStatus(c.getValue(lineData.lineStart.x), "HudNode::getTextDataChild - startX");

		c = p.child(1, &stat);
		McheckStatus(c.getValue(lineData.lineStart.y), "HudNode::getTextDataChild - startY");

		c = p.child(2, &stat);
		McheckStatus(c.getValue(lineData.lineStart.z), "HudNode::getTextDataChild - startZ");

		// End
		p = plug.child(10, &stat);
		c = p.child(0, &stat);
		McheckStatus(c.getValue(lineData.lineEnd.x), "HudNode::getTextDataChild - endX");

		c = p.child(1, &stat);
		McheckStatus(c.getValue(lineData.lineEnd.y), "HudNode::getTextDataChild - endY");

		c = p.child(2, &stat);
		McheckStatus(c.getValue(lineData.lineEnd.z), "HudNode::getTextDataChild - endZ");

		// Scale
		p = plug.child(11, &stat);
		c = p.child(0, &stat);
		McheckStatus(c.getValue(lineData.scale.x), "HudNode::getTextDataChild - scaleX");

		c = p.child(1, &stat);
		McheckStatus(c.getValue(lineData.scale.y), "HudNode::getTextDataChild - scaleY");

		c = p.child(2, &stat);
		McheckStatus(c.getValue(lineData.scale.z), "HudNode::getTextDataChild - scaleZ");

		// Color
		p = plug.child(12, &stat);
		c = p.child(0, &stat);
		McheckStatus(c.getValue(lineData.color.r), "HudNode::getTextDataChild - textColorR");

		c = p.child(1, &stat);
		McheckStatus(c.getValue(lineData.color.g), "HudNode::getTextDataChild - textColorG");

		c = p.child(2, &stat);
		McheckStatus(c.getValue(lineData.color.b), "HudNode::getTextDataChild - textColorB");

		p = plug.child(13, &stat);
		McheckStatus(p.getValue(lineData.color.a), "HudNode::getTextDataChild - textColorA");

		return MS::kSuccess;
	}

	MStatus HudNode::getRectData(HudUserData& hudData)
	{
		hudData.rectArray.clear();

		MStatus status;
		MPlug plug, rectPlug, cp;
		plug = MPlug(thisMObject(), rectList);
		int numPlugs = plug.numElements();

		for (int i = 0; i < numPlugs; i++)
		{
			RectData rectData;
			rectPlug = plug.elementByPhysicalIndex(i, &status);
			status = getRectPlugData(rectPlug, rectData);

			// Update Dimensions
			rectData.dimensions.x *= rectData.scale.x;
			rectData.dimensions.y *= rectData.scale.y;
			rectData.dimensions.z *= rectData.scale.z;

			if (rectData.is2D)
			{
				MVector offset = getOffset(hudData, rectData);
				rectData.position.x += offset.x;
				rectData.position.y += offset.y;
				rectData.position.z = 0.0;
			}

			//// Debug
			//debugText(hudData);

			hudData.rectArray.push_back(rectData);
		}

		return MS::kSuccess;
	}

	MStatus HudNode::getRectPlugData(MPlug plug, RectData& rectData)
	{
		MStatus stat;
		MPlug p, c;

		// Enable
		p = plug.child(0, &stat);
		McheckStatus(p.getValue(rectData.enable), "HudNode::getRectPlugData - rectEnable");

		// Is2D
		p = plug.child(1, &stat);
		McheckStatus(p.getValue(rectData.is2D), "HudNode::getRectPlugData - rectIs2D");

		// Area
		p = plug.child(2, &stat);
		McheckStatus(p.getValue(rectData.areaType), "HudNode::getRectPlugData - rectArea");

		// Anchor
		p = plug.child(3, &stat);
		McheckStatus(p.getValue(rectData.anchorType), "HudNode::getRectPlugData - rectAnchor");

		// Position
		p = plug.child(4, &stat);
		c = p.child(0, &stat);
		McheckStatus(c.getValue(rectData.position.x), "HudNode::getRectPlugData - rectPositionX");

		c = p.child(1, &stat);
		McheckStatus(c.getValue(rectData.position.y), "HudNode::getRectPlugData - rectPositionY");

		c = p.child(2, &stat);
		McheckStatus(c.getValue(rectData.position.z), "HudNode::getRectPlugData - rectPositionZ");

		// Dimensions
		p = plug.child(5, &stat);
		c = p.child(0, &stat);
		McheckStatus(c.getValue(rectData.dimensions.x), "HudNode::getRectPlugData - rectDimensionX");

		c = p.child(1, &stat);
		McheckStatus(c.getValue(rectData.dimensions.y), "HudNode::getRectPlugData - rectDimensionY");

		c = p.child(2, &stat);
		McheckStatus(c.getValue(rectData.dimensions.z), "HudNode::getRectPlugData - rectDimensionZ");

		// Up
		p = plug.child(6, &stat);
		c = p.child(0, &stat);
		McheckStatus(c.getValue(rectData.rectUp.x), "HudNode::getRectPlugData - rectUpX");

		c = p.child(1, &stat);
		McheckStatus(c.getValue(rectData.rectUp.y), "HudNode::getRectPlugData - rectUpY");

		c = p.child(2, &stat);
		McheckStatus(c.getValue(rectData.rectUp.z), "HudNode::getRectPlugData - rectUpZ");

		// Normal
		p = plug.child(7, &stat);
		c = p.child(0, &stat);
		McheckStatus(c.getValue(rectData.rectNormal.x), "HudNode::getRectPlugData - rectNormalX");

		c = p.child(1, &stat);
		McheckStatus(c.getValue(rectData.rectNormal.y), "HudNode::getRectPlugData - rectNormalY");

		c = p.child(2, &stat);
		McheckStatus(c.getValue(rectData.rectNormal.z), "HudNode::getRectPlugData - rectNormalZ");

		// Scale
		p = plug.child(8, &stat);
		c = p.child(0, &stat);
		McheckStatus(c.getValue(rectData.scale.x), "HudNode::getRectPlugData - rectScaleX");

		c = p.child(1, &stat);
		McheckStatus(c.getValue(rectData.scale.y), "HudNode::getRectPlugData - rectScaleY");

		c = p.child(2, &stat);
		McheckStatus(c.getValue(rectData.scale.z), "HudNode::getRectPlugData - rectScaleZ");

		// Is Filled
		p = plug.child(9, &stat);
		McheckStatus(p.getValue(rectData.rectIsFilled), "HudNode::getRectPlugData - rectIsFilled");

		// Color
		p = plug.child(10, &stat);
		c = p.child(0, &stat);
		McheckStatus(c.getValue(rectData.color.r), "HudNode::getRectPlugData - rectColorR");

		c = p.child(1, &stat);
		McheckStatus(c.getValue(rectData.color.g), "HudNode::getRectPlugData - rectColorG");

		c = p.child(2, &stat);
		McheckStatus(c.getValue(rectData.color.b), "HudNode::getRectPlugData - rectColorB");

		p = plug.child(11, &stat);
		McheckStatus(p.getValue(rectData.color.a), "HudNode::getRectPlugData - rectColorA");

		// Line Style
		p = plug.child(12, &stat);
		McheckStatus(p.getValue(rectData.lineStyle), "HudNode::getRectPlugData - lineStyle");

		// Line Factor
		p = plug.child(13, &stat);
		McheckStatus(p.getValue(rectData.lineStyleFactor), "HudNode::getRectPlugData - lineStyleFactor");

		// Line Pattern
		p = plug.child(14, &stat);
		McheckStatus(p.getValue(rectData.lineStylePattern), "HudNode::getRectPlugData - lineStyle");

		// Line Width
		p = plug.child(15, &stat);
		McheckStatus(p.getValue(rectData.lineWidth), "HudNode::getRectPlugData - lineWidth");


		return MS::kSuccess;
	}

	MStatus HudNode::getImageData(HudUserData& hudData)
	{
		hudData.imageArray.clear();

		MStatus status;
		MPlug plug, imagePlug, cp;
		plug = MPlug(thisMObject(), imageList);
		int numPlugs = plug.numElements();

		for (int i = 0; i < numPlugs; i++)
		{
			ImageData imageData;
			imagePlug = plug.elementByPhysicalIndex(i, &status);
			status = getImagePlugData(imagePlug, imageData);

			// Update Dimensions
			imageData.dimensions.x *= imageData.scale.x;
			imageData.dimensions.y *= imageData.scale.y;
			imageData.dimensions.z *= imageData.scale.z;

			if (imageData.is2D)
			{
				MVector offset = getOffset(hudData, imageData);
				imageData.position.x += offset.x;
				imageData.position.y += offset.y;
				imageData.position.z = 0.0;
			}

			hudData.imageArray.push_back(imageData);
		}

		return MS::kSuccess;
	}

	MStatus HudNode::getImagePlugData(MPlug plug, ImageData& imageData)
	{
		MStatus stat;
		MPlug p, c;

		// Enable
		p = plug.child(0, &stat);
		McheckStatus(p.getValue(imageData.enable), "HudNode::getImagePlugData - enable");

		// Is2D
		p = plug.child(1, &stat);
		McheckStatus(p.getValue(imageData.is2D), "HudNode::getImagePlugData - rectIs2D");

		// Path
		p = plug.child(2, &stat);
		McheckStatus(p.getValue(imageData.imagePath), "HudNode::getImagePlugData - imagePath");

		// Area
		p = plug.child(3, &stat);
		McheckStatus(p.getValue(imageData.areaType), "HudNode::getImagePlugData - rectArea");

		// Anchor
		p = plug.child(4, &stat);
		McheckStatus(p.getValue(imageData.anchorType), "HudNode::getImagePlugData - rectAnchor");

		// Position
		p = plug.child(5, &stat);
		c = p.child(0, &stat);
		McheckStatus(c.getValue(imageData.position.x), "HudNode::getImagePlugData - rectPositionX");

		c = p.child(1, &stat);
		McheckStatus(c.getValue(imageData.position.y), "HudNode::getImagePlugData - rectPositionY");

		c = p.child(2, &stat);
		McheckStatus(c.getValue(imageData.position.z), "HudNode::getImagePlugData - rectPositionZ");

		// Dimensions
		p = plug.child(6, &stat);
		c = p.child(0, &stat);
		McheckStatus(c.getValue(imageData.dimensions.x), "HudNode::getImagePlugData - rectDimensionX");

		c = p.child(1, &stat);
		McheckStatus(c.getValue(imageData.dimensions.y), "HudNode::getImagePlugData - rectDimensionY");

		c = p.child(2, &stat);
		McheckStatus(c.getValue(imageData.dimensions.z), "HudNode::getImagePlugData - rectDimensionZ");

		// Up
		p = plug.child(7, &stat);
		c = p.child(0, &stat);
		McheckStatus(c.getValue(imageData.rectUp.x), "HudNode::getImagePlugData - rectUpX");

		c = p.child(1, &stat);
		McheckStatus(c.getValue(imageData.rectUp.y), "HudNode::getImagePlugData - rectUpY");

		c = p.child(2, &stat);
		McheckStatus(c.getValue(imageData.rectUp.z), "HudNode::getImagePlugData - rectUpZ");

		// Normal
		p = plug.child(8, &stat);
		c = p.child(0, &stat);
		McheckStatus(c.getValue(imageData.rectNormal.x), "HudNode::getImagePlugData - rectNormalX");

		c = p.child(1, &stat);
		McheckStatus(c.getValue(imageData.rectNormal.y), "HudNode::getImagePlugData - rectNormalY");

		c = p.child(2, &stat);
		McheckStatus(c.getValue(imageData.rectNormal.z), "HudNode::getImagePlugData - rectNormalZ");

		// Scale
		p = plug.child(9, &stat);
		c = p.child(0, &stat);
		McheckStatus(c.getValue(imageData.scale.x), "HudNode::getImagePlugData - rectScaleX");

		c = p.child(1, &stat);
		McheckStatus(c.getValue(imageData.scale.y), "HudNode::getImagePlugData - rectScaleY");

		c = p.child(2, &stat);
		McheckStatus(c.getValue(imageData.scale.z), "HudNode::getImagePlugData - rectScaleZ");

		// Is Filled
		p = plug.child(10, &stat);
		McheckStatus(p.getValue(imageData.rectIsFilled), "HudNode::getImagePlugData - rectIsFilled");

		// Color
		p = plug.child(11, &stat);
		c = p.child(0, &stat);
		McheckStatus(c.getValue(imageData.color.r), "HudNode::getImagePlugData - rectColorR");

		c = p.child(1, &stat);
		McheckStatus(c.getValue(imageData.color.g), "HudNode::getImagePlugData - rectColorG");

		c = p.child(2, &stat);
		McheckStatus(c.getValue(imageData.color.b), "HudNode::getImagePlugData - rectColorB");

		p = plug.child(12, &stat);
		McheckStatus(p.getValue(imageData.color.a), "HudNode::getImagePlugData - rectColorA");

		return MS::kSuccess;
	}

	MStatus HudNode::getViewportData(HudUserData& data, const MDagPath& cameraPath, const MFrameContext& frameContext)
	{
		// Viewport Data
		frameContext.getViewportDimensions(data.viewportX, data.viewportX, data.viewportWidth, data.viewportHeight);

		// Device Aspect Ratio
		MSelectionList selection;
		selection.add("defaultResolution");
		MObject mObject;
		selection.getDependNode(0, mObject);
		MFnDependencyNode fnNode(mObject);
		data.deviceAspectRatio = fnNode.findPlug("deviceAspectRatio", false).asDouble();

		// Camera Data
		MFnCamera fnCamera(cameraPath);
		
		data.cameraName = fnCamera.name();
		data.cameraAspectRatio = fnCamera.aspectRatio();
		data.cameraPanZoomEnabled = fnCamera.panZoomEnabled();
		data.cameraRenderPanZoom = fnCamera.renderPanZoom();
		data.cameraZoom = fnCamera.zoom();
		data.cameraPan.x = fnCamera.horizontalPan();
		data.cameraPan.y = fnCamera.verticalPan();
		data.cameraFocal = fnCamera.focalLength();

		// TODO: Get Connected camera from "camera" message attribute

		// Calculate
		data.viewportAspectRatio = data.viewportWidth / data.viewportHeight;
		double scale = 1.0;

		if (fnCamera.filmFit() == MFnCamera::kHorizontalFilmFit)
		{
			data.maskDimension.x = (double)data.viewportWidth / fnCamera.overscan();
			data.maskDimension.y = data.maskDimension.x / data.deviceAspectRatio;
		}
		else if (fnCamera.filmFit() == MFnCamera::kVerticalFilmFit)
		{
			data.maskDimension.y = (double)data.viewportHeight / fnCamera.overscan();
			data.maskDimension.x = data.maskDimension.y * data.deviceAspectRatio;
		}
		else if (fnCamera.filmFit() == MFnCamera::kFillFilmFit)
		{
			if (data.viewportAspectRatio < data.cameraAspectRatio)
			{
				if (data.cameraAspectRatio < data.deviceAspectRatio)
				{
					scale = data.cameraAspectRatio / data.viewportAspectRatio;
				}
				else
				{
					scale = data.deviceAspectRatio / data.viewportAspectRatio;
				}
			}
			else if (data.cameraAspectRatio > data.deviceAspectRatio)
			{
				scale = data.deviceAspectRatio / data.cameraAspectRatio;
			}

			data.maskDimension.x = (double)data.viewportWidth / fnCamera.overscan() * scale;
			data.maskDimension.y = data.maskDimension.x / data.deviceAspectRatio;
		}
		else if (fnCamera.filmFit() == MFnCamera::kOverscanFilmFit)
		{
			if (data.viewportAspectRatio < data.cameraAspectRatio)
			{
				if (data.cameraAspectRatio < data.deviceAspectRatio)
				{
					scale = data.cameraAspectRatio / data.viewportAspectRatio;
				}
				else
				{
					scale = data.deviceAspectRatio / data.viewportAspectRatio;
				}
			}
			else if (data.cameraAspectRatio > data.deviceAspectRatio)
			{
				scale = data.deviceAspectRatio / data.cameraAspectRatio;
			}

			data.maskDimension.y = (double)data.viewportHeight / fnCamera.overscan() / scale;
			data.maskDimension.x = data.maskDimension.y * data.deviceAspectRatio;
		}
		else
		{
			MGlobal::displayError("Unknown film fit value.");
			data.enable = false;
			return MS::kFailure;
		}

		// Update Mask
		data.viewportCenter.x = 0.5 * (double)data.viewportWidth;
		data.viewportCenter.y = 0.5 * (double)data.viewportHeight;

		data.maskOrigin.x = data.viewportCenter.x - (0.5 * data.maskDimension.x);
		data.maskOrigin.y = data.viewportCenter.y - (0.5 * data.maskDimension.y);

		// Safe Action
		data.safeActionDimension = data.maskDimension * 0.9;
		data.safeActionOrigin.x = data.maskOrigin.x + ((data.maskDimension.x - data.safeActionDimension.x) * 0.5);
		data.safeActionOrigin.y = data.maskOrigin.y + ((data.maskDimension.y - data.safeActionDimension.y) * 0.5);

		// Safe Title
		data.safeTitleDimension = data.maskDimension * 0.8;
		data.safeTitleOrigin.x = data.maskOrigin.x + ((data.maskDimension.x - data.safeTitleDimension.x) * 0.5);
		data.safeTitleOrigin.y = data.maskOrigin.y + ((data.maskDimension.y - data.safeTitleDimension.y) * 0.5);

		return MS::kSuccess;
	}

	void HudNode::debugData(HudUserData& data)
	{
		MGlobal::displayInfo("------------------------------------------------");

		MGlobal::displayInfo(MString("enable: ") + data.enable);
		MGlobal::displayInfo(MString("viewportX: ") + data.viewportX);
		MGlobal::displayInfo(MString("viewportY: ") + data.viewportY);
		MGlobal::displayInfo(MString("viewportWidth: ") + data.viewportWidth);
		MGlobal::displayInfo(MString("viewportHeight: ") + data.viewportHeight);

		MGlobal::displayInfo(MString("viewportCenter: ") + data.viewportCenter.x + MString(", ") + data.viewportCenter.y);

		MGlobal::displayInfo(MString("deviceAspectRatio: ") + data.deviceAspectRatio);
		MGlobal::displayInfo(MString("viewportAspectRatio: ") + data.viewportAspectRatio);

		MGlobal::displayInfo(MString("cameraName: ") + data.cameraName);
		MGlobal::displayInfo(MString("cameraAspectRatio: ") + data.cameraAspectRatio);
		MGlobal::displayInfo(MString("cameraPanZoomEnabled: ") + data.cameraPanZoomEnabled);
		MGlobal::displayInfo(MString("cameraRenderPanZoom: ") + data.cameraRenderPanZoom);
		MGlobal::displayInfo(MString("cameraPan: ") + data.cameraPan.x + MString(", ") + data.cameraPan.y);
		MGlobal::displayInfo(MString("cameraZoom: ") + data.cameraZoom);

		MGlobal::displayInfo(MString("maskOrigin: ") + data.maskOrigin.x + MString(", ") + data.maskOrigin.y);
		MGlobal::displayInfo(MString("maskDimension: ") + data.maskDimension.x + MString(", ") + data.maskDimension.y);

		MGlobal::displayInfo(MString("safeActionOrigin: ") + data.safeActionOrigin.x + MString(", ") + data.safeActionOrigin.y);
		MGlobal::displayInfo(MString("safeActionDimension: ") + data.safeActionDimension.x + MString(", ") + data.safeActionDimension.y);

		MGlobal::displayInfo(MString("safeTitleOrigin: ") + data.safeTitleOrigin.x + MString(", ") + data.safeTitleOrigin.y);
		MGlobal::displayInfo(MString("safeTitleDimension: ") + data.safeTitleDimension.x + MString(", ") + data.safeTitleDimension.y);
	}

	void HudNode::getDrawData(HudUserData& data, const MDagPath& cameraPath, const MFrameContext& frameContext)
	{
		// Clear
		data = HudUserData();

		MStatus stat;
		MPlug p;
		MObject thisNode = thisMObject();

		// Enable
		p = MPlug(thisNode, enable);
		McheckVoid(p.getValue(data.enable), "HudNode::getDrawData enable");

		if (!data.enable)
		{
			return;
		}

		// Viewport
		getViewportData(data, cameraPath, frameContext);
		//debugData(hudData);

		// Text
		getTextData(data);

		// Line
		getLineData(data);

		// Rect
		getRectData(data);

		// Image
		getImageData(data);
	}

	void HudNode::drawData(const HudUserData& data, MHWRender::MUIDrawManager& drawManager)
	{
		if (!data.enable)
		{
			return;
		}

		// Draw Text
		for (int i = 0; i < data.textArray.size(); i++)
		{
			TextData textData = data.textArray[i];

			if (textData.enable)
			{
				drawManager.beginDrawable();

				drawManager.setFontIncline(textData.textFontIncline);
				drawManager.setFontWeight(textData.textFontWeight);
				drawManager.setFontStretch(textData.textFontStretch);
				drawManager.setFontLine(textData.textFontLine);
				drawManager.setFontSize(textData.textFontSize);
				drawManager.setFontName(HudNode::fontList[textData.textFontIndex]);
				drawManager.setColor(textData.color);

				const int backgroundSize[2] = { (int)textData.dimensions.x, (int)textData.dimensions.y };

				if (textData.is2D)
				{
					drawManager.text2d(
						textData.position, 
						textData.textResult, 
						(MHWRender::MUIDrawManager::TextAlignment)textData.textAlignment, 
						textData.enableBackground ? backgroundSize : NULL, 
						textData.enableBackground ? &textData.backgroundColor : NULL,
						textData.dynamic
					);
				}
				else
				{
					drawManager.text(
						textData.position,
						textData.textResult,
						(MHWRender::MUIDrawManager::TextAlignment)textData.textAlignment,
						textData.enableBackground ? backgroundSize : NULL,
						textData.enableBackground ? &textData.backgroundColor : NULL,
						textData.dynamic
					);
				}

				drawManager.endDrawable();
			}
		}
	
		// Draw Line
		for (int i = 0; i < data.lineArray.size(); i++)
		{
			LineData lineData = data.lineArray[i];

			if (lineData.enable)
			{

				drawManager.beginDrawable();
				drawManager.setLineWidth(lineData.lineWidth);
				drawManager.setColor(lineData.color);

				if (lineData.lineStyle == 5)
				{
					drawManager.setLineStyle(lineData.lineStyleFactor, lineData.lineStylePattern);
				}
				else
				{
					drawManager.setLineStyle((MHWRender::MUIDrawManager::LineStyle)lineData.lineStyle);
				}
				
				if (lineData.is2D)
				{
					drawManager.line2d(
						lineData.lineStart,
						lineData.lineEnd
					);
				}
				else
				{
					drawManager.line(
						lineData.lineStart,
						lineData.lineEnd
					);
				}
				drawManager.endDrawable();
			}
		}

		// Draw Rect
		for (int i = 0; i < data.rectArray.size(); i++)
		{
			RectData rectData = data.rectArray[i];

			if (rectData.enable)
			{

				drawManager.beginDrawable();

				if (!rectData.rectIsFilled)
				{
					drawManager.setLineWidth(rectData.lineWidth);

					if (rectData.lineStyle == 5)
					{
						drawManager.setLineStyle(rectData.lineStyleFactor, rectData.lineStylePattern);
					}
					else
					{
						drawManager.setLineStyle((MHWRender::MUIDrawManager::LineStyle)rectData.lineStyle);
					}
				}
				
				drawManager.setColor(rectData.color);

				if (rectData.is2D)
				{
					drawManager.rect2d(
						rectData.position,
						rectData.rectUp,
						rectData.dimensions.x,
						rectData.dimensions.y,
						rectData.rectIsFilled
					);
				}
				else
				{
					drawManager.rect(
						rectData.position,
						rectData.rectUp,
						rectData.rectNormal,
						rectData.dimensions.x,
						rectData.dimensions.y,
						rectData.rectIsFilled
					);
				}
				drawManager.endDrawable();
			}
		}

		// Draw Image
		for (int i = 0; i < data.imageArray.size(); i++)
		{
			ImageData imageData = data.imageArray[i];

			if (imageData.enable)
			{

				MHWRender::MRenderer* renderer = MHWRender::MRenderer::theRenderer();

				if (!renderer)
				{
					//MGlobal::displayError("Unable to get renderer.");
					return;
				}

				MTexture* texture = renderer->getTextureManager()->acquireTexture(imageData.imagePath, imageData.imagePath);

				if (!texture)
				{
					//MGlobal::displayError("Unable to get texture.");
					continue;
				}

				drawManager.beginDrawable();

				drawManager.setTexture(texture);
				drawManager.setTextureSampler(MHWRender::MSamplerState::kMinMagMipLinear, MHWRender::MSamplerState::kTexClamp);
				drawManager.setTextureMask(MHWRender::MBlendState::kRGBAChannels);
				drawManager.setColor(imageData.color);

				// Scale the image based on the dimensions
				MTextureDescription textureDescription;
				texture->textureDescription(textureDescription);
				//double scaleY = (0.5 * imageData.dimensions.y) - 4;
				//double scaleX = scaleY / textureDescription.fHeight * textureDescription.fWidth;

				//imageData.dimensions.x += scaleX;
				//imageData.dimensions.y += scaleY;
				imageData.dimensions.x = textureDescription.fWidth * imageData.scale.x;
				imageData.dimensions.y = textureDescription.fHeight * imageData.scale.y;

				

				if (imageData.is2D)
				{
					drawManager.rect2d(
						imageData.position,
						imageData.rectUp,
						imageData.dimensions.x,
						imageData.dimensions.y,
						imageData.rectIsFilled
					);
				}
				else
				{
					drawManager.rect(
						imageData.position,
						imageData.rectUp,
						imageData.rectNormal,
						imageData.dimensions.x,
						imageData.dimensions.y,
						imageData.rectIsFilled
					);
				}
				drawManager.endDrawable();
			}
		}

		
	}

}
