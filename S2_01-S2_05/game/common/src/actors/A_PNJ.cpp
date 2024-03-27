#include "A_PNJ.h"
#include "A_Map.h"

Actor* A_PNJ::make() {
	return new A_PNJ();
}

int A_PNJ::onCreate() {
	int type = (this->settings >> 16) & 0xFF;
	int triggeringEventID = this->settings & 0xFFFF;

	if (!A_Map::instance->isEventTriggered(triggeringEventID))
		return 2;

	switch (type) {
		case 0:
			this->parentMgr->createActorByID(13, &this->tilePos, this->settings & 0xFF00FFFF);
			break;
		case 1:
			this->parentMgr->createActorByID(5, &this->tilePos, this->settings & 0xFF00FFFF);
			break;
		case 2:
			this->parentMgr->createActorByID(15, &this->tilePos, this->settings & 0xFF00FFFF);
			break;
		case 3:
			this->parentMgr->createActorByID(16, &this->tilePos, this->settings & 0xFF00FFFF);
			break;
		case 4:
			this->parentMgr->createActorByID(14, &this->tilePos, this->settings & 0xFF00FFFF);
			break;
		default:
			break;
	}

	return 2;
}