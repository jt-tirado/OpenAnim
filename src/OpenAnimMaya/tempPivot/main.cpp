#include "TempPivotContextCmd.h"
#include "TempPivotToolCmd.h"
#include "TempPivotManip.h"

#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>


MStatus initializePlugin(MObject obj) {
    MFnPlugin plugin(obj, "Justin Tirado", "1.0", "Any");

    MStatus status;
    status = plugin.registerContextCommand(
        TempPivotContextCmd::name(), 
        TempPivotContextCmd::creator,
        TempPivotToolCmd::name(),
        TempPivotToolCmd::creator
    );

    if (!status)
    {
        MGlobal::displayError("Failed to register context command.");
        return status;
    }

    status = plugin.registerNode(
        TempPivotManip::name(), 
        TempPivotManip::id,
        TempPivotManip::creator,
        TempPivotManip::initialize,
        MPxNode::kManipContainer
    );

    if (!status) {
        MGlobal::displayError("Error registering manipulator node.");
        return status;
    }

    return MS::kSuccess;
}

MStatus uninitializePlugin(MObject obj) {
    MFnPlugin plugin(obj);

    MStatus status;
    status = plugin.deregisterContextCommand(TempPivotContextCmd::name(), TempPivotToolCmd::name());
    
    if (!status)
    {
        MGlobal::displayError("Failed to unregister context command.");
        return status;
    }

    status = plugin.deregisterNode(TempPivotManip::id);

    if (!status) {
        MGlobal::displayError("Error deregistering manipulator node.");
        return status;
    }

    return MS::kSuccess;
}