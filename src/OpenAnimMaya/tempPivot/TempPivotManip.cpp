#include "TempPivotManip.h"
#include "macros.h"
#include "utils.h"

#include <maya/MFnRotateManip.h>
#include <maya/MFnStateManip.h>
#include <maya/MFnFreePointTriadManip.h>
#include <maya/MFnToggleManip.h>
#include <maya/MGlobal.h>
#include <maya/MEulerRotation.h>
#include <maya/MPlug.h>
#include <maya/MVector.h>
#include <maya/MFnTransform.h>
#include <maya/MTypes.h>

MTypeId TempPivotManip::id(0x00139d00);

void TempPivotManip::setPoint(MPoint point)
{
    MFnFreePointTriadManip freePointTriadManipFn(mTranslatePath);
    freePointTriadManipFn.setPoint(point);
}

MPoint TempPivotManip::getPoint()
{
    MFnFreePointTriadManip freePointTriadManipFn(mTranslatePath);
    unsigned triadPointIndex = freePointTriadManipFn.pointIndex();

    MPoint point;
    if (!getConverterManipValue(freePointTriadManipFn.pointIndex(), point))
    {
        MGlobal::displayError("Unable to get point.");
    }

    return point;
}

void TempPivotManip::preDrawUI(const M3dView& view)
{
    /*MPoint point = getPoint();

    MFnRotateManip rotateManip(mRotatePath);
    rotateManip.setRotationCenter(point);

    MFnToggleManip toggleManip(mTogglePath);
    toggleManip.setStartPoint(point);*/
}

void TempPivotManip::drawUI(MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext) const
{
}

MStatus TempPivotManip::createChildren()
{
    MStatus status = MStatus::kSuccess;

    MGlobal::displayInfo("Create Children.");

    // Add the rotation manip
    //
    mRotatePath = addRotateManip("RotateManip", "rotation");

    //
    mTranslatePath = addFreePointTriadManip("TranslateManip", "translate");

    // Toggle Manip
    mTogglePath = addToggleManip("ToggleManip", "toggle");

    // 
    mChildren.clear();

    return status;
}

MStatus TempPivotManip::connectToDependNode(const MObject& node)
{
    MGlobal::displayInfo("Connect to depend node.");

    MStatus status;

    // Get the Dag Path
    MDagPath nodePath;
    MFnDagNode dagNodeFn(node);
    dagNodeFn.getPath(nodePath);
    MFnTransform transform(nodePath);

    // Find the rotate and rotatePivot plugs on the node.  These plugs will 
    // be attached either directly or indirectly to the manip values on the
    // rotate manip.

    MFnDependencyNode nodeFn(node);

    // Translate Plug
    MPlug tPlug = nodeFn.findPlug("translate", true, &status);

    if (!status)
    {
        MGlobal::displayError("Could not find translate plug on node");
        return status;
    }

    // Rotate Plug
    MPlug rPlug = nodeFn.findPlug("rotate", true, &status);
    if (!status)
    {
        MGlobal::displayError("Could not find rotate plug on node");
        return status;
    }

    // Rotate Pivot Plug
    MPlug rcPlug = nodeFn.findPlug("rotatePivot", true, &status);
    if (!status)
    {
        MGlobal::displayError("Could not find rotatePivot plug on node");
        return status;
    }

    // If the translate pivot exists, it will be used to move the state manip
    // to a convenient location.

    // Append
    mChildren.append(nodePath);

    // FreePointTriadManip
    MFnFreePointTriadManip freePointTriadManipFn(mTranslatePath, &status);
    
    if (!status) 
    {
        MGlobal::displayError("Unable to get FreePointManip.");
        return status;
    }

    //MPoint existingTranslation(vectorPlugValue(tPlug));
    freePointTriadManipFn.setTranslation(transform.getTranslation(MSpace::kWorld), MSpace::kTransform);
    addPlugToManipConversionCallback(freePointTriadManipFn.pointIndex(), (plugToManipConversionCallback)&TempPivotManip::translateChangedCallback);

    // Toggle Manip
    MFnToggleManip toggleManip(mTogglePath);

    // Rotate Manip
    MFnRotateManip rotateManip(mRotatePath, &status);

    if (!status)
    {
        MGlobal::displayError("Unable to get RotateManip.");
        return status;
    }

    rotateManip.setRotateMode(MFnRotateManip::kObjectSpace);
    //rotateManip.displayWithNode(node);

    MEulerRotation existingRotation(vectorPlugValue(rPlug));
    rotateManip.setInitialRotation(existingRotation);
    rotateManip.setTranslation(transform.getTranslation(MSpace::kWorld), MSpace::kTransform);

    //addPlugToManipConversionCallback(rotateManip.rotationIndex(), (plugToManipConversionCallback)&TempPivotManip::rotationChangedCallback);
    rotatePlugIndex = addManipToPlugConversionCallback(mPlug, (manipToPlugConversionCallback)&TempPivotManip::rotationChangedCallback);
    //rotateManip.connectToRotationCenterPlug(rcPlug);
    
    /*MPlug rxPlug = rPlug.child(0);
    addPlugToInViewEditor(rxPlug);
    MPlug ryPlug = rPlug.child(1);
    addPlugToInViewEditor(ryPlug);
    MPlug rzPlug = rPlug.child(2);
    addPlugToInViewEditor(rzPlug);*/
    finishAddingManips();
    MPxManipContainer::connectToDependNode(node);
    return status;
}

MManipData TempPivotManip::translateChangedCallback(unsigned index)
{
    // Get Point Values
    MFnFreePointTriadManip freePointTriadManipFn(mTranslatePath);

    MPoint point;
    getConverterManipValue(freePointTriadManipFn.pointIndex(), point);

    // Update Manipulators
    MFnRotateManip rotateManip(mRotatePath);
    rotateManip.setRotationCenter(point);

    // Return Data
    MFnNumericData numData;
    MObject numDataObj = numData.create(MFnNumericData::k3Double);
    numData.setData(point.x, point.y, point.z);
    return MManipData(numDataObj);
}

MManipData TempPivotManip::rotationChangedCallback(unsigned index) 
{
    static MEulerRotation cache;
    MObject obj = MObject::kNullObj;

    if (index != rotatePlugIndex)
    {
        MGlobal::displayError("Invalid index in rotation changed callback!");
        // For invalid indices, return vector of 0's
        MFnNumericData numericData;
        obj = numericData.create(MFnNumericData::k3Double);
        numericData.setData(0.0, 0.0, 0.0);
        return obj;
    }

    MFnRotateManip rotateManip(mRotatePath);
    MFnNumericData numericData;
    obj = numericData.create(MFnNumericData::k3Double);

    MEulerRotation manipRotation;
    if (!getConverterManipValue(rotateManip.rotationIndex(), manipRotation))
    {
        MGlobal::displayError("Error retrieving manip value");
        numericData.setData(0.0, 0.0, 0.0);
    }
    else {
        numericData.setData(manipRotation.x, manipRotation.y, manipRotation.z);
    }
    return MManipData(obj);
}

MStatus TempPivotManip::doPress()
{
    return MS::kUnknownParameter;
}

MStatus TempPivotManip::doDrag()
{
    return MS::kUnknownParameter;
}

MStatus TempPivotManip::doRelease()
{
    // Custom release handling

    // Let Maya do it's work too
    return MS::kUnknownParameter;
}


