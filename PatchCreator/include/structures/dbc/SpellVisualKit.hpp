#ifndef SpellVisualKit_HPP
#define SpellVisualKit_HPP

#include "../DBCHeader.hpp"
struct SpellVisualKitRecord {
	uint32_t ID;
	uint32_t startAnimID;
	uint32_t animID;
	uint32_t headEffect;        // attached to Head
	uint32_t chestEffect;       // attached to Chest
	uint32_t baseEffect;        // attached to Base
	uint32_t leftHandEffect;    // attached to SpellLeftHand
	uint32_t rightHandEffect;   // attached to SpellRightHand
	uint32_t breathEffect;      // attached to Breath
	uint32_t leftWeaponEffect;
	uint32_t rightWeaponEffect;
	uint32_t specialEffect1;    // attached to Special1…3
	uint32_t specialEffect2;
	uint32_t specialEffect3;
	uint32_t worldEffect;
	uint32_t soundID;
	uint32_t shakeID;
	uint32_t charProc1;
	uint32_t charProc2;
	uint32_t charProc3;
	uint32_t charProc4;
	float charParamZero1;
	float charParamZero2;
	float charParamZero3;
	float charParamZero4;
	float charParamOne1;
	float charParamOne2;
	float charParamOne3;
	float charParamOne4;
	float charParamTwo1;
	float charParamTwo2;
	float charParamTwo3;
	float charParamTwo4;
	float charParamThree1;
	float charParamThree2;
	float charParamThree3;
	float charParamThree4;
	uint32_t flag;
};

struct SpellVisualKitFile {
	DBCHeader header;
	// static_assert (header.record_size == sizeof (record_type));
	SpellVisualKitRecord* records;
	char* stringBlock;
	bool addNewRecord(SpellVisualKitRecord newRecord) {
		SpellVisualKitRecord* newRecords = new SpellVisualKitRecord[header.recordCount + 1];
		for (uint32_t i = 0; i < header.recordCount; i++) {
			newRecords[i] = records[i];
		};
		newRecords[header.recordCount] = newRecord;
		records = newRecords;
		header.recordCount++;
		return true;
	};
};


#endif
