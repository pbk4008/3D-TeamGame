#pragma once
#ifndef __TOOL_ENUM_H__
#define __TOOL_ENUM_H__
BEGIN(Tool)
enum class TOOLOBJID {TOOL_STATIC, TOOL_DYNAMIC, TOOL_TERRAIN, TOOL_CAMERA,TOOL_UI};
enum class TOOLLAYERID {TOOL_PROTOTYPE, TOOL_STATICCLONE, TOOL_DYNAMICCLONE};
enum class TOOLSCENEID {TOOL_PROTO, TOOL_DEFORM, TOOL_END};
END
#endif