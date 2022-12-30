#ifndef TRANSFORMMATRIXCACHE_H
#define TRANSFORMMATRIXCACHE_H

#include <map>

#include <maya/MFloatMatrix.h>
#include <maya/MMatrix.h>
#include <maya/MFnCamera.h>
#include <maya/MDagPath.h>
#include <maya/MPlug.h>

// https://github.com/Toolchefs/motionPath/blob/master/include/CameraCache.h
class TransformMatrixCache
{

public:
	TransformMatrixCache();
	~TransformMatrixCache() {}

	void initialize(const MObject& object);
	void generateCache(double start, double end);
	std::map<double, MMatrix> data() { return matrixCache; }
	bool isCaching() { return caching; }
	bool isInitialized() { return initialized; }
	void clear() { matrixCache.clear(); }
	int count() { return matrixCache.size(); }
	bool has(double frame);
	MMatrix at(double frame);
	double start() { return startFrame; }
	double end() { return endFrame; }
	bool isValid();
	void ensureMatricesAtTime(const double time, const bool force = false);
	void checkRangeIsCached();
	TransformMatrixCache inverse();

	// Operators
	bool isEqual(TransformMatrixCache& obj);
	bool operator==(TransformMatrixCache& obj);
	bool operator!=(TransformMatrixCache& obj);
	TransformMatrixCache operator+(TransformMatrixCache& obj);
	TransformMatrixCache operator-(TransformMatrixCache& obj);
	TransformMatrixCache operator*(TransformMatrixCache& obj);
	

private:
	MDagPath dag;
	bool caching;
	bool initialized;
	std::map<double, MMatrix> matrixCache;
	double startFrame;
	double endFrame;
	MPlug worldMatrixPlug;
	MPlug txPlug, tyPlug, tzPlug;
	MPlug rxPlug, ryPlug, rzPlug;
};

#endif

