#include "Utils.h"

#include <maya/MGlobal.h>

#include <maya/MPlugArray.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnTransform.h>
#include <maya/MFnDagNode.h>
#include <maya/MDagPath.h>

MObject Utils::getConnectedTransformFromPlug(MPlug& plug)
{
	MStatus status;

	MPlugArray plugs;
	plug.connectedTo(plugs, true, false);

	if (plugs.length() > 0)
	{
		MObject node = plugs[0].node();
		MFnDagNode fnDag(node);
		MDagPath dagPath;
		status = fnDag.getPath(dagPath);

		if (!status)
		{
			MGlobal::displayError("Unable to retreive DagPath.");
		}
		else
		{
			MObject result = dagPath.transform(&status);

			if (!status)
			{
				MGlobal::displayError("Unable to retreive Transform Object.");
			}
			else
			{
				return result;
			}
		}
	}

	return MObject();
}
