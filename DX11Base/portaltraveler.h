#pragma once

#include "portal.h"


class OBB;

class PortalTraveler
{
public:
	virtual PortalType GetEntrancePortal() const { return m_entrancePortal; }
	virtual void SetEntrancePortal(PortalType type) { m_entrancePortal = type; }
	virtual OBB* GetOBB() { return &m_obb; }

	virtual void Swap() = 0;
	virtual dx::XMVECTOR GetTravelerPosition() = 0;

protected:
	PortalType m_entrancePortal;
	OBB m_obb;
};