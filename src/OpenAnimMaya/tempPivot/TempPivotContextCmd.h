#ifndef TEMPPIVOTCONTEXTCMD_HEADER
#define TEMPPIVOTCONTEXTCMD_HEADER

#include "TempPivotContext.h"

#include <maya/MPxContextCommand.h>
#include <maya/MPxContext.h>


class TempPivotContextCmd : public MPxContextCommand
{
	public:
		static const MString name() { return "tempPivotCtx"; }
		static void* creator() { return new TempPivotContextCmd; }

		TempPivotContextCmd() {}
		MPxContext* makeObj();
		MStatus doEditFlags();
		MStatus appendSyntax();
	
	private:
		TempPivotContext* mCtx;
};

#endif