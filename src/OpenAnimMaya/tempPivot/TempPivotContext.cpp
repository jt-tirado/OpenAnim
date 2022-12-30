#include "TempPivotContext.h"
#include "TempPivotManip.h"

#include <maya/MModelMessage.h>
#include <maya/MGlobal.h>
#include <maya/MItSelectionList.h>
#include <maya/MObjectArray.h>
#include <maya/MDagPathArray.h>

TempPivotContext::TempPivotContext()
{
    setTitleString("Temp Pivot");
}

void TempPivotContext::toolOnSetup(MEvent&)
{
    setHelpString("Rotate the object using the rotation handles.");

    mCmd = 0;
    mHasCmd = false;
    mManip = NULL;

    updateManipulators(this);

    MStatus status;

    mId = MModelMessage::addCallback(
        MModelMessage::kActiveListModified,
        updateManipulators,
        this, 
        &status
    );
    
    if (!status) {
        MGlobal::displayError("Model addCallback failed.");
    }
}

void TempPivotContext::clear()
{
    deleteManipulators();
    mCmd = 0;
    mHasCmd = false;
    mHasManip = false;
}

void TempPivotContext::toolOffCleanup()
{
    MStatus status;
    status = MModelMessage::removeCallback(mId);

    if (!status) {
        MGlobal::displayError("Model remove callback failed");
    }

    clear();

    MPxContext::toolOffCleanup();
}

bool TempPivotContext::isDependFree(MFnDependencyNode& node)
{
    MStatus status;

    // Check Translation
    MPlug tPlug = node.findPlug("translate", true, &status);

    if (!status)
    {
        return false;
    }

    // Check Rotation
    MPlug rPlug = node.findPlug("rotate", true, &status);

    if (!status)
    {
        return false;
    }

    return true;
}

MStatus TempPivotContext::doPress(MEvent& event, MHWRender::MUIDrawManager& drawMgr, const MHWRender::MFrameContext& context)
{
    MStatus status = MPxSelectionContext::doPress(event, drawMgr, context);

    if (!isSelecting())
    {
        if (mHasManip && mHasCmd)
        {
            mCmd->rotateManip = mManip->rotateDagPath();
            mCmd->redoIt();
        }
    }
    return status;
}

MStatus TempPivotContext::doRelease(MEvent& event, MHWRender::MUIDrawManager& drawMgr, const MHWRender::MFrameContext& context)
{
    MStatus status = MPxSelectionContext::doRelease(event, drawMgr, context);

    if (!isSelecting())
    {
        if (mHasManip && mHasCmd)
        {
            mCmd->finalize();
        }
    }
    return status;
}

void TempPivotContext::updateManipulators(void* data)
{
    MStatus status = MStatus::kSuccess;

    // Update Manipulators
    TempPivotContext* ctxPtr = (TempPivotContext*)data;

    //ctxPtr->deleteManipulators();
    //ctxPtr->mHasManip = false;
    ctxPtr->clear();
    
    // Get Active Selected
    MSelectionList list;
    status = MGlobal::getActiveSelectionList(list);
    MItSelectionList iter(list, MFn::kInvalid, &status);

    if (MS::kSuccess == status) 
    {
        MObjectArray objects;

        for (; !iter.isDone(); iter.next()) 
        {
            // Make sure the selection list item is a depend node and has the
            // required plugs before manipulating it.

            MObject dependNode;
            iter.getDependNode(dependNode);

            if (dependNode.isNull() || !dependNode.hasFn(MFn::kDependencyNode))
            {
                MGlobal::displayWarning("Object in selection list is not a depend node.");
                continue;
            }

            // Check Depend Node
            MFnDependencyNode dependNodeFn(dependNode);

            if (!isDependFree(dependNodeFn))
            {
                MGlobal::displayWarning("Object cannot be manipulated: " + dependNodeFn.name());
                continue;
            }

            // Append
            objects.append(dependNode);
        }

        if (objects.length() == 0)
        {
            return;
        }
        else
        {
            // Create manipulator
            MString manipName("tempPivotManip");
            MObject manipObject;

            TempPivotManip* manipulator = (TempPivotManip*)TempPivotManip::newManipulator(manipName, manipObject);

            if (NULL != manipulator)
            {
                // Add the manipulator
                ctxPtr->mManip = manipulator;
                ctxPtr->addManipulator(manipObject);
                ctxPtr->mHasManip = true;
                
                // Add the command
                ctxPtr->mCmd = (TempPivotToolCmd*)ctxPtr->newToolCommand(); 
                ctxPtr->mHasCmd = true;

                for (int x = 0; x < objects.length(); x++)
                {
                    MObject object = objects[x];

                    // Connect the manipulator to the object in the selection list.
                    if (!manipulator->connectToDependNode(object))
                    {
                        MFnDependencyNode dependNodeFn(object);
                        MGlobal::displayWarning("Error connecting manipulator to object: " + dependNodeFn.name());
                    }
                }
            }
        }  
    }
}


