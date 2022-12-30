#ifndef HUDDRAWOVERRIDE_H
#define HUDDRAWOVERRIDE_H

#include "HudNode.h"

#include <maya/MPxDrawOverride.h>
#include <maya/MDrawRegistry.h>
#include <maya/MUserData.h>

using namespace MHWRender;

namespace DynamoMayaHud
{
    class HudDrawOverride : public MPxDrawOverride
    {
    public:
        static MString name;
        static MPxDrawOverride* creator(const MObject& obj);

    public:
        ~HudDrawOverride() override {}

        DrawAPI supportedDrawAPIs() const override;

        bool isBounded(
            const MDagPath& objPath,
            const MDagPath& cameraPath) const override;

        MBoundingBox boundingBox(
            const MDagPath& objPath,
            const MDagPath& cameraPath) const override;

        bool disableInternalBoundingBoxDraw() const override;

        MUserData* prepareForDraw(
            const MDagPath& objPath,
            const MDagPath& cameraPath,
            const MFrameContext& frameContext,
            MUserData* oldData) override;

        bool hasUIDrawables() const override;
        void addUIDrawables(
            const MDagPath& objPath,
            MHWRender::MUIDrawManager& drawManager,
            const MHWRender::MFrameContext& frameContext,
            const MUserData* data) override;

    private:
        HudDrawOverride(const MObject& obj);
        bool m_loadDefault;
        bool m_isDirty;
    };
}
#endif
