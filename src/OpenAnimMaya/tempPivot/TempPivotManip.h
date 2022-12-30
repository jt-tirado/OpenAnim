#ifndef TEMPPIVOTMANIP_HEADER
#define TEMPPIVOTMANIP_HEADER

#include "TempPivotToolCmd.h"

#include <maya/MPxManipContainer.h>
#include <maya/MGlobal.h>
#include <maya/MDagPath.h>
#include <maya/MManipData.h>
#include <maya/M3dView.h>
#include <maya/MPoint.h>
#include <maya/MDagPathArray.h>

class TempPivotManip : public MPxManipContainer
{
public:
    static MTypeId  id;
    static const MString name() { return "tempPivotManip"; }
    static void* creator() { return new TempPivotManip(); }
    static MStatus initialize() { return MPxManipContainer::initialize(); }

    // Constructos
    TempPivotManip(){}
    ~TempPivotManip() override {}

    //Viewport 2.0 manipulator draw overrides
    void preDrawUI(const M3dView& view) override;
    void drawUI(MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext) const override;

    MStatus createChildren() override;
    MStatus connectToDependNode(const MObject& node) override;

    MStatus doPress() override;
    MStatus doDrag() override;
    MStatus doRelease() override;

    //
    void setPoint(MPoint point);
    MPoint getPoint();
    
    
    // Callback function
    MManipData translateChangedCallback(unsigned index);
    MManipData rotationChangedCallback(unsigned index);

    // Helpers
    MDagPath rotateDagPath() { return mRotatePath; }

private:
    MPlug mPlug;
    MDagPath mNodePath;
    MDagPathArray mChildren;

    MDagPath mRotatePath;
    MDagPath mTranslatePath;
    MDagPath mTogglePath;

    unsigned translatePlugIndex;
    unsigned rotatePlugIndex;
};

#endif