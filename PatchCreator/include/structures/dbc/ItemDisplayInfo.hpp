#ifndef ItemDisplayInfo_HPP
#define ItemDisplayInfo_HPP
#include "../DBCHeader.hpp"
struct ItemDisplayInfoRecord {
	uint32_t ID;
	stringref leftModel;
	stringref rightModel;
	stringref leftModelTexture;
	stringref rightModelTexture;
	stringref Icon1;
	stringref Icon2;
	uint32_t geosetGroup1;
	uint32_t geosetGroup2;
	uint32_t geosetGroup3;
	uint32_t flags;
	uint32_t spellVisualID;
	uint32_t groupSoundIndex;
	uint32_t helmetGeosetVis1;
	uint32_t helmetGeosetVis2;
	stringref upperArmTexture;
	stringref lowerArmTexture;
	stringref handsTexture;
	stringref upperTorsoTexture;
	stringref lowerTorsoTexture;
	stringref upperLegTexture;
	stringref lowerLegTexture;
	stringref footTexture;
	uint32_t itemVisualID;
	uint32_t particleColorID;
};

struct ItemDisplayInfoFile {
	DBCHeader header;
	// static_assert (header.record_size == sizeof (record_type));
	ItemDisplayInfoRecord* records;
	char* stringBlock;

	bool changeRecord_spellVisualID_ByID(uint32_t id, uint32_t newValue) {
		for (uint32_t i = 0; i < header.recordCount; i++) {
			ItemDisplayInfoRecord& record = records[i];
			if (record.ID == id) {
				record.spellVisualID = newValue;
				return true;
			}
		}
		return false;
	};
	bool changeRecord_itemVisualID_ByID(uint32_t id, uint32_t newValue) {
		for (uint32_t i = 0; i < header.recordCount; i++) {
			ItemDisplayInfoRecord& record = records[i];
			if (record.ID == id) {
				record.itemVisualID = newValue;
				return true;
			}
		}
		return false;
	};
	bool addNewRecord(ItemDisplayInfoRecord newRecord) {
		ItemDisplayInfoRecord* newRecords = new ItemDisplayInfoRecord[header.recordCount + 1];
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
