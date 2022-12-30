#include "motionTrail\MotionTrailNode.h"
#include "motionTrail\MotionTrailOverride.h"

#include "hud\HudNode.h"
#include "hud\HudDrawOverride.h"

#include <maya\MGlobal.h>
#include <maya\MFnPlugin.h>

using namespace DynamoMayaHud;
using namespace DynamoMayaTrail;

MStatus initializePlugin(MObject obj)
{
    MStatus status;
    MFnPlugin plugin(obj, "Justin Tirado", "1.0", "Any");

    // Motion Trail
    status = plugin.registerNode(
        MotionTrailNode::name,
        MotionTrailNode::id,
        &MotionTrailNode::creator,
        &MotionTrailNode::initialize,
        MPxNode::kLocatorNode,
        &MotionTrailNode::drawDbClassification
    );

    if (!status)
    {
        MGlobal::displayError("Unable to register: " + MotionTrailNode::name);
        return status;
    }

    status = MHWRender::MDrawRegistry::registerDrawOverrideCreator(
        MotionTrailNode::drawDbClassification,
        MotionTrailNode::drawRegistrantId,
        MotionTrailOverride::creator
    );

    if (!status) {
        MGlobal::displayError("Unable to register: " + MotionTrailOverride::name);
        return status;
    }

    // HUD
    status = plugin.registerNode(
        HudNode::name,
        HudNode::id,
        &HudNode::creator,
        &HudNode::initialize,
        MPxNode::kLocatorNode,
        &HudNode::drawDbClassification
    );

    if (!status)
    {
        MGlobal::displayError("Unable to register: " + HudNode::name);
        return status;
    }

    status = MHWRender::MDrawRegistry::registerDrawOverrideCreator(
        HudNode::drawDbClassification,
        HudNode::drawRegistrantId,
        HudDrawOverride::creator
    );

    if (!status) {
        MGlobal::displayError("Unable to register: " + HudDrawOverride::name);
        return status;
    }
    return status;
}


MStatus uninitializePlugin(MObject obj)
{
    MStatus status;
    MFnPlugin plugin(obj);

    // Motion Trail
    status = MDrawRegistry::deregisterGeometryOverrideCreator(
        MotionTrailNode::drawDbClassification,
        MotionTrailNode::drawRegistrantId
    );

    if (!status) {
        MGlobal::displayError("Unable to deregister: " + MotionTrailOverride::name);
        return status;
    }

    status = plugin.deregisterNode(MotionTrailNode::id);

    if (!status) 
    {
        MGlobal::displayError("Unable to deregister: " + MotionTrailNode::name);
        return status;
    }

    // HUD
    status = MDrawRegistry::deregisterGeometryOverrideCreator(
        HudNode::drawDbClassification,
        HudNode::drawRegistrantId
    );

    if (!status) {
        MGlobal::displayError("Unable to deregister: " + HudDrawOverride::name);
        return status;
    }

    status = plugin.deregisterNode(HudNode::id);

    if (!status)
    {
        MGlobal::displayError("Unable to deregister: " + HudNode::name);
        return status;
    }
    

    return status;
}