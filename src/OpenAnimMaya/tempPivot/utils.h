#include <string>
#include <maya/MString.h>
#include <maya/MVector.h>
#include <maya/MPoint.h>
#include <maya/MDagPath.h>
#include <maya/MFnDagNode.h>
#include <maya/MFnTransform.h>
#include <maya/MEulerRotation.h>
#include <maya/MTypes.h>

MString toMString(double value)
{
	return MString(std::to_string(value).c_str());
}

MString toMString(int value)
{
	return MString(std::to_string(value).c_str());
}

MString toMString(MVector vector)
{
	MString str = "(";
	str += toMString(vector.x) + ",";
	str += toMString(vector.y) + ",";
	str += toMString(vector.z);
	str += ")";
	return str;
}

MString toMString(MPoint point)
{
	MString str = "(";
	str += toMString(point.x) + ",";
	str += toMString(point.y) + ",";
	str += toMString(point.z) + ",";
	str += toMString(point.w) + ",";
	str += ")";
	return str;
}

MString toMString(MEulerRotation rotation)
{
    MString str = "(";
    str += toMString(rotation.x) + ",";
    str += toMString(rotation.y) + ",";
    str += toMString(rotation.z);
    str += ")";
    return str;
}

MVector vectorPlugValue(const MPlug& plug) {
    if (plug.numChildren() == 3)
    {
        double x, y, z;
        MPlug rx = plug.child(0);
        MPlug ry = plug.child(1);
        MPlug rz = plug.child(2);
        rx.getValue(x);
        ry.getValue(y);
        rz.getValue(z);
        MVector result(x, y, z);
        return result;
    }
    else {
        MGlobal::displayError("Expected 3 children for plug " + MString(plug.name()));
        MVector result(0, 0, 0);
        return result;
    }
}

MVector nodeTranslation(MDagPath dag)
{
    MFnDagNode dagFn(dag);
    MDagPath path;
    dagFn.getPath(path);
    path.pop();  // pop from the shape to the transform
    MFnTransform transformFn(path);
    return transformFn.translation(MSpace::kWorld);
}