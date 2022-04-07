#ifndef UIDGen_h__
#define UIDGen_h__

#include "SingleTon.h"

class UIDGen : public CSingleTon<UIDGen>
{
	friend CSingleTon;

private:
	NO_COPY(UIDGen);
	explicit UIDGen();
	virtual ~UIDGen();
public:
	_ulong GetUID(void);
private:
	static _ulong nextID;
	virtual void Free(void) override;
};

#endif // UIDGen_h__