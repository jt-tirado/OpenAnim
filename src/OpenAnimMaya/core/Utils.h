#ifndef UTILS_H
#define UTILS_H

#include <maya/MPlug.h>
#include <maya/MObject.h>

class Utils
{
public:
	static MObject getConnectedTransformFromPlug(MPlug& plug);
};
#endif

