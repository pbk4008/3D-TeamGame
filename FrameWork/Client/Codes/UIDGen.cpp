#include "pch.h"
#include "UIDGen.h"

UIDGen::UIDGen()
{
}

UIDGen::~UIDGen()
{
}

_ulong UIDGen::GetUID(void)
{
	return nextID++;
}

void UIDGen::Free(void)
{
}

_ulong UIDGen::nextID;
