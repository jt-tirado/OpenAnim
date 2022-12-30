#ifndef HUDDATA_H
#define HUDDATA_H

#include <vector>

#include <maya\MUserData.h>
#include <maya\MPoint.h>
#include <maya\MVector.h>
#include <maya\MString.h>
#include <maya\MColor.h>
#include <maya\MUIDrawManager.h>

namespace DynamoMayaHud
{
	enum DisplayType
	{
		kDisplayNone = 0,
		kAllCameras = 1,
		kConnectedCamerasOnly = 2,
		kCamerasWithHudAttribute = 3
	};

	enum AreaType
	{
		kAreaNone = 0,
		kResolution = 1,
		kSafeAction = 2,
		kSafeTitle = 3
	};

	enum AnchorType
	{
		kAnchorNone = 0,
		kTopLeft = 1,
		kTopCenter = 2,
		kTopRight = 3,
		kCenterLeft = 4,
		kCenter = 5,
		kCenterRight = 6,
		kBottomLeft = 7,
		kBottomCenter = 8,
		kBottomRight = 9
	};

	enum TextType
	{
		kString = 0,
		kInt = 1,
		kFloat = 2,
		kBool = 3,
		kMel = 4,
		kPython = 5,
		kUser = 6,
		kFile = 7,
		kScene = 8,
		kProject = 9,
		kCamera = 10,
		kFocal = 11,
		kFps = 12,
		kFrame = 13,
		kTime = 14,
		kTimecode = 15,
		kDate = 16
	};

	struct BaseData
	{
		bool enable = false;
		int areaType = 0;
		int anchorType = 0;
		bool is2D = false;
		MPoint position;
		MVector dimensions;
		MVector scale;
		MColor color;
		unsigned int depthPriority;

		bool equals(const BaseData& other)
		{
			if (enable != other.enable)
				return false;

			if (areaType != other.areaType)
				return false;

			if (anchorType != other.anchorType)
				return false;

			if (is2D != other.is2D)
				return false;

			if (position != other.position)
				return false;

			if (dimensions != other.dimensions)
				return false;

			if (scale != other.scale)
				return false;

			if (color != other.color)
				return false;

			return true;
		}

		bool operator==(const BaseData& other){return equals(other);}
		bool operator!=(const BaseData& other){return !equals(other);}
	};

	struct TextData : public BaseData
	{
		int textType = 0;

		MString textString = " ";
		int textInt = 0;
		double textFloat = 0;
		bool textBool = false;;

		MString textResult = " ";

		int textAlignment = 0;
		bool enableBackground = false;
		MColor backgroundColor;
		bool dynamic = false;

		int textFontIncline = 0;
		int textFontWeight = 0;
		int textFontStretch = 0;
		int textFontLine = 0;
		int textFontSize = 0;
		int textFontIndex = 0;

		bool equals(const TextData& other)
		{
			if (!BaseData::equals(other))
				return false;

			if (textType != other.textType)
				return false;

			if (textResult != other.textResult)
				return false;

			if (textAlignment != other.textAlignment)
				return false;

			if (enableBackground != other.enableBackground)
				return false;

			if (backgroundColor != other.backgroundColor)
				return false;

			if (textFontLine != other.textFontIncline)
				return false;
			
			if (textFontWeight != other.textFontWeight)
				return false;

			if (textFontStretch != other.textFontStretch)
				return false;

			if (textFontLine != other.textFontLine)
				return false;

			if (textFontSize != other.textFontSize)
				return false;

			if (textFontIndex != other.textFontIndex)
				return false;

			return true;
		}

		bool operator==(const TextData& other) { return equals(other); }
		bool operator!=(const TextData& other) { return !equals(other); }
	};

	struct LineData : public BaseData
	{
		float lineWidth;
		int lineStyle;
		int lineStyleFactor;
		short lineStylePattern;
		MPoint lineStart;
		MPoint lineEnd;
	};

	struct RectData : public LineData
	{
		MVector rectUp;
		MVector rectNormal;
		bool rectIsFilled;
	};

	struct PointData : BaseData
	{

	};

	struct CircleData : public BaseData
	{

	};

	struct ArcData : public BaseData
	{

	};

	struct ImageData : public RectData
	{
		MString imagePath;
	};

	struct CameraData
	{

	};

	class HudUserData : public MUserData
	{
	public:
		bool enable = false;

		int displayType = 0;

		int viewportX = 0;
		int viewportY = 0;
		int viewportWidth = 0;
		int viewportHeight = 0;

		MVector viewportCenter;

		int resolutionWidth = 0;
		int resolutionHeight = 0;

		double deviceAspectRatio = 0.0;
		double viewportAspectRatio = 0.0;

		MString cameraName = "";
		double cameraAspectRatio = 0.0;
		bool cameraPanZoomEnabled = false;
		bool cameraRenderPanZoom = false;
		MVector cameraPan;
		double cameraZoom = 0.0;
		double cameraFocal = 0.0;

		MVector maskOrigin;
		MVector maskDimension;

		MVector safeActionOrigin;
		MVector safeActionDimension;

		MVector safeTitleOrigin;
		MVector safeTitleDimension;

		std::vector<TextData> textArray;
		std::vector<RectData> rectArray;
		std::vector<LineData> lineArray;
		std::vector<ImageData> imageArray;

		bool equals(const HudUserData& other)
		{
			if (viewportX != other.viewportX)
				return false;

			if (viewportY != other.viewportY)
				return false;

			if (viewportWidth != other.viewportWidth)
				return false;

			if (viewportHeight != other.viewportHeight)
				return false;

			if (resolutionWidth != other.resolutionWidth)
				return false;

			if (resolutionHeight != other.resolutionHeight)
				return false;

			if (cameraAspectRatio != other.cameraAspectRatio)
				return false;

			if (deviceAspectRatio != other.deviceAspectRatio)
				return false;

			if (viewportAspectRatio != other.viewportAspectRatio)
				return false;

			if (textArray.size() != other.textArray.size())
				return false;

			for (int i=0; i<textArray.size(); i++)
			{
				if (textArray[i] != other.textArray[i])
				{
					return false;
				}
			}

			if (rectArray.size() != other.rectArray.size())
				return false;

			for (int i = 0; i < rectArray.size(); i++)
			{
				if (rectArray[i] != other.rectArray[i])
				{
					return false;
				}
			}

			if (lineArray.size() != other.lineArray.size())
				return false;

			for (int i = 0; i < lineArray.size(); i++)
			{
				if (lineArray[i] != other.lineArray[i])
				{
					return false;
				}
			}

			if (imageArray.size() != other.imageArray.size())
				return false;

			for (int i = 0; i < imageArray.size(); i++)
			{
				if (imageArray[i] != other.imageArray[i])
				{
					return false;
				}
			}

			return true;
		}

		bool operator==(const HudUserData& other) { return equals(other); }
		bool operator!=(const HudUserData& other) { return !equals(other); }
	};
}

#endif