#ifndef TEMPPIVOTTOOLCMD_HEADER
#define TEMPPIVOTTOOLCMD_HEADER

#include <vector>

#include <maya/MPxToolCommand.h>
#include <maya/MStatus.h>
#include <maya/MVector.h>
#include <maya/MPoint.h>
#include <maya/MSyntax.h>
#include <maya/MArgList.h>
#include <maya/MMatrix.h>
#include <maya/MMatrixArray.h>
#include <maya/MObject.h>
#include <maya/MObjectArray.h>
#include <maya/MDagPath.h>
#include <maya/MDagPathArray.h>


class TempPivotToolCmd : public MPxToolCommand
{
public:
    static MString name() { return "tempPivot"; }
    static void* creator() { return new TempPivotToolCmd; }

    TempPivotToolCmd() { setCommandString(TempPivotToolCmd::name()); }
    ~TempPivotToolCmd() override {}

    MStatus doIt(const MArgList& args);
    MStatus redoIt();
    MStatus undoIt();
    bool isUndoable() const { return true; }
    MStatus finalize() override;

    MDagPathArray children;

    MDagPath rotateManip;
    MDagPath translateManip;

private:
    MMatrix mRotateMatrix;
    MMatrix mTranslateMatrix;
};

#endif