#include "TempPivotToolCmd.h"
#include "macros.h"

#include <maya/MDGModifier.h>
#include <maya/MArgDatabase.h>
#include <maya/MString.h>
#include <maya/MGlobal.h>
#include <maya/MItSelectionList.h>
#include <maya/MSelectionList.h>
#include <maya/MDagPath.h>
#include <maya/MFnTransform.h>
#include <maya/MFnRotateManip.h>
#include <maya/MTransformationMatrix.h>


MStatus TempPivotToolCmd::doIt(const MArgList& args)
{
	MStatus status = MS::kSuccess;

	// Do It
	if (status == MS::kSuccess)
	{
		status = redoIt();
	}

	return status;
}

MStatus TempPivotToolCmd::redoIt()
{
	MStatus status = MS::kSuccess;
	
	// Check rotate manipulator.
	MFnRotateManip fnRotateManip(rotateManip, &status);

	if (!status)
	{
		displayError("Unable to retrieve rotate manipulator.");
		return MS::kFailure;
	}

	mRotateMatrix = rotateManip.inclusiveMatrix();

	// Check Translate???

	// Check children
	if (children.length() == 0)
	{
		//displayError("No children provided.");
		//return MS::kFailure;
	}

	return status;
}

MStatus TempPivotToolCmd::undoIt()
{
	MStatus status = MS::kSuccess;

	displayInfo("Undo some shit.");

	if (children.length() > 0)
	{

	}

	// Undo rotate manip
	MFnRotateManip fnRotateManip(rotateManip, &status);

	if (status)
	{
		displayInfo("Is undoing some rotate shit.");

		//MFnTransform xform(rotateManip, &status);
		//xform.set(MTransformationMatrix(mRotateMatrix));

		MTransformationMatrix transform(mRotateMatrix);
		
		double rotation[3];
		MTransformationMatrix::RotationOrder order;

		transform.getRotation(rotation, order);
		fnRotateManip.setRotation(rotation, order);
	}

	return status;
}

MStatus TempPivotToolCmd::finalize()
{
	MArgList command;
	command.addArg(commandString());
	return MPxToolCommand::doFinalize(command);
}


