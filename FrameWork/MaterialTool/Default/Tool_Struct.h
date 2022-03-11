#pragma once
#ifndef __TOOL_STRUCT_H__
#define __TOOL_STRUCT_H__
BEGIN(Tool)
typedef struct tagTerrainData
{
	_uint iCntX;
	_uint iCntZ;
	_uint iDetail;
	_float fInterval;
	_int iTextureIndex;
}TERRAINDESC;
END
#endif