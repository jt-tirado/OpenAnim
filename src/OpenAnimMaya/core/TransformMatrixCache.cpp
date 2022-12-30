#include "TransformMatrixCache.h"

#include <maya/MAnimControl.h>
#include <maya/MFnMatrixData.h>
#include <maya/MGlobal.h>
#include <maya/MDagPath.h>
#include <maya/MDGContextGuard.h>

TransformMatrixCache::TransformMatrixCache() :
    caching(false),
    initialized(false),
    startFrame(0),
    endFrame(0)
{
}

bool TransformMatrixCache::has(double frame)
{
    if (matrixCache.count(frame) > 0)
    {
        return true;
    }

    return false;
}

MMatrix TransformMatrixCache::at(double frame)
{
    if (has(frame))
    {
        return matrixCache.at(frame);
    }

    return MMatrix();
}

bool TransformMatrixCache::isValid()
{
    if (!caching && startFrame != endFrame && initialized && count() > 0)
    {
        return true;
    }

    return false;
}

void TransformMatrixCache::initialize(const MObject& object)
{
    MFnDependencyNode fnNode(object);
    MPlug worldMatrixPlugs = fnNode.findPlug("worldMatrix", false);
    worldMatrixPlugs.evaluateNumElements();
    worldMatrixPlug = worldMatrixPlugs[0];

    caching = false;
    initialized = true;
}

void TransformMatrixCache::generateCache(double start, double end)
{
	// 
    if (worldMatrixPlug.isNull())
    {
        return;
    }
        
    caching = true;
	matrixCache.clear();
    startFrame = start;
    endFrame = end;

    // 
    for (double i = startFrame; i <= endFrame; ++i)
    {
        MTime time(i, MTime::uiUnit());
        MDGContext context(time);

        MObject value;
        MDGContextGuard guard(context);
        worldMatrixPlug.getValue(value);
        matrixCache[i] = MFnMatrixData(value).matrix();
    }

    caching = false;
}

void TransformMatrixCache::checkRangeIsCached()
{
    if (worldMatrixPlug.isNull())
        return;

    caching = true;

    for (double i = startFrame; i <= endFrame; ++i)
    {
        if (matrixCache.find(i) == matrixCache.end())
        {
            MTime evalTime(i, MTime::uiUnit());
            MDGContext context(evalTime);

            MObject val;
            MDGContextGuard guard(context);
            worldMatrixPlug.getValue(val);
            matrixCache[i] = MFnMatrixData(val).matrix();
        }
    }
    caching = false;
}

TransformMatrixCache TransformMatrixCache::inverse()
{
    return TransformMatrixCache();
}

bool TransformMatrixCache::isEqual(TransformMatrixCache& obj)
{
    if (count() != obj.count())
    {
        return false;
    }

    if (startFrame != obj.startFrame)
    {
        return false;
    }

    if (endFrame != obj.endFrame)
    {
        return false;
    }
    
    for (auto iter = matrixCache.begin(); iter != matrixCache.end(); ++iter)
    {
        if (!obj.has(iter->first))
        {
            return false;
        }

        if (iter->second != obj.at(iter->first))
        {
            return false;
        }

    }

    return true;
}

bool TransformMatrixCache::operator==(TransformMatrixCache& obj)
{
    return isEqual(obj);
}

bool TransformMatrixCache::operator!=(TransformMatrixCache& obj)
{
    return !isEqual(obj);
}

TransformMatrixCache TransformMatrixCache::operator+(TransformMatrixCache& obj)
{
    return TransformMatrixCache();
}

TransformMatrixCache TransformMatrixCache::operator-(TransformMatrixCache& obj)
{
    return TransformMatrixCache();
}

TransformMatrixCache TransformMatrixCache::operator*(TransformMatrixCache& obj)
{
    return TransformMatrixCache();
}


void TransformMatrixCache::ensureMatricesAtTime(const double time, const bool force)
{
    if (matrixCache.find(time) == matrixCache.end() || force)
    {
        if (worldMatrixPlug.isNull())
        {
            return;
        }

        MTime evalTime(time, MTime::uiUnit());
        MDGContext context(evalTime);

        MObject value;
        MDGContextGuard guard(context);
        worldMatrixPlug.getValue(value);
        matrixCache[time] = MFnMatrixData(value).matrix().inverse();
    }
}
