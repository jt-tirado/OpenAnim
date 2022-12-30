#include "TempPivotContextCmd.h"
#include "macros.h"

#include <maya/MGlobal.h>
#include <maya/MPoint.h>
#include <maya/MArgParser.h>
#include <maya/MString.h>

MPxContext* TempPivotContextCmd::makeObj()
{ 
	mCtx = new TempPivotContext();
	return mCtx; 
}

MStatus TempPivotContextCmd::appendSyntax()
{
	MSyntax syntax = this->syntax();

	if (MS::kSuccess != syntax.addFlag(kTypeFlag, kTypeFlagLong, MSyntax::kString))
	{
		return MS::kFailure;
	}

	if (MS::kSuccess != syntax.addFlag(kPositionFlag, kPositionFlagLong, MSyntax::kDouble, MSyntax::kDouble, MSyntax::kDouble))
	{
		return MS::kFailure;
	}

	if (MS::kSuccess != syntax.addFlag(kLastFlag, kLastFlagLong, MSyntax::kBoolean))
	{
		return MS::kFailure;
	}

	if (MS::kSuccess != syntax.addFlag(kAlignFlag, kAlignFlagLong, MSyntax::kString))
	{
		return MS::kFailure;
	}

	return MS::kSuccess;
}

MStatus TempPivotContextCmd::doEditFlags()
{
	MStatus status = MS::kSuccess;

	MArgParser argData = parser();

	// Type
	if (argData.isFlagSet(kTypeFlag))
	{
		MString tmp;
		status = argData.getFlagArgument(kTypeFlag, 0, tmp);

		if (!status)
		{
			MGlobal::displayError("Type flag invalid.");
			return status;
		}

		mCtx->setType(tmp);
	}
	else
	{
		mCtx->setType(MString(kTypeCenter));
	}

	// Position
	if (argData.isFlagSet(kPositionFlag))
	{
		MPoint tmp;

		status = argData.getFlagArgument(kPositionFlag, 0, tmp.x);

		if (!status)
		{
			MGlobal::displayError("Position X flag invalid.");
			return status;
		}

		status = argData.getFlagArgument(kPositionFlag, 1, tmp.y);

		if (!status)
		{
			MGlobal::displayError("Position Y flag invalid.");
			return status;
		}

		status = argData.getFlagArgument(kPositionFlag, 2, tmp.z);

		if (!status)
		{
			MGlobal::displayError("Position Z flag invalid.");
			return status;
		}

		mCtx->setPosition(tmp);
	}

	// Last
	if (argData.isFlagSet(kLastFlag))
	{
		bool tmp;
		status = argData.getFlagArgument(kLastFlag, 0, tmp);

		if (!status)
		{
			MGlobal::displayError("Last flag invalid.");
			return status;
		}

		mCtx->setIsLast(tmp);
	}
	else
	{
		mCtx->setIsLast(true);
	}

	// Align
	if (argData.isFlagSet(kAlignFlag))
	{
		MString tmp;
		status = argData.getFlagArgument(kAlignFlag, 0, tmp);

		if (!status)
		{
			MGlobal::displayError("Align flag invalid.");
			return status;
		}

		mCtx->setAlignType(tmp);
	}
	else
	{
		mCtx->setAlignType(MString(kAlignWorld));
	}

	return status;
}
