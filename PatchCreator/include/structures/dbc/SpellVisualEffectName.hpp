#ifndef SpellVisualEffectName_HPP
#define SpellVisualEffectName_HPP

#include "../DBCHeader.hpp"
	
struct SpellVisualEffectNameRecord{
	uint32_t m_ID;
	stringref name;
	stringref m_fileName;
	float m_areaEffectSize;
	float scale;
	float minAllowedScale;
	float maxAllowedScale;
};
struct SpellVisualEffectNameFile{
	DBCHeader header;
	// static_assert (header.record_size == sizeof (record_type));
	SpellVisualEffectNameRecord* records;
	char* stringBlock;
	bool changeRecord_name(uint32_t recordId, const char* newString, std::map<std::string, stringref>* stringHandler){
		if (recordId > header.recordCount) return false;
		//const char* newstr = newString;
		size_t new_strlen = strlen(newString);
		if ((*stringHandler).count(newString)){
			SpellVisualEffectNameRecord& testrecord = records[recordId];
			testrecord.name = (*stringHandler)[newString];
		}else {
			char* newStringBlock = new char[header.stringBlockSize + new_strlen + 1];
			memcpy(newStringBlock, stringBlock, header.stringBlockSize);
			#pragma warning(disable : 4996)
			strcat(newStringBlock + header.stringBlockSize, newString);
			header.stringBlockSize += new_strlen + 1;
			newStringBlock[header.stringBlockSize + new_strlen + 1] = '\0';
			SpellVisualEffectNameRecord& testrecord = records[recordId];
			stringBlock = newStringBlock;
			testrecord.name = header.stringBlockSize - new_strlen - 1;
			(*stringHandler)[newString] = header.stringBlockSize - new_strlen - 1;
		}
		return true;
	};
	bool changeRecord_m_fileName(uint32_t recordId, const char* newString, std::map<std::string, stringref>* stringHandler){
		if (recordId > header.recordCount) return false;

		size_t new_strlen = strlen(newString);
		if ((*stringHandler).count(newString)) {
			SpellVisualEffectNameRecord& testrecord = records[recordId];
			testrecord.m_fileName = (*stringHandler)[newString];
		}else {
			char* newStringBlock = new char[header.stringBlockSize + new_strlen + 1];
			memcpy(newStringBlock, stringBlock, header.stringBlockSize);

			strcat(newStringBlock + header.stringBlockSize, newString);
			header.stringBlockSize += new_strlen + 1;
			newStringBlock[header.stringBlockSize + new_strlen + 1] = '\0';
			SpellVisualEffectNameRecord& testrecord = records[recordId];
			stringBlock = newStringBlock;
			testrecord.m_fileName = header.stringBlockSize - new_strlen - 1;
			(*stringHandler)[newString] = header.stringBlockSize - new_strlen - 1;
		}
		return true;
	}
	bool changeRecord_m_areaEffectSize(uint32_t recordId,float newValue){
		if (recordId > header.recordCount) return false;
		SpellVisualEffectNameRecord& neededRecord = records[recordId];
		neededRecord.m_areaEffectSize = newValue;
		return true;
	};
	bool changeRecord_scale(uint32_t recordId,float newValue){
		if (recordId > header.recordCount) return false;
		SpellVisualEffectNameRecord& neededRecord = records[recordId];
		neededRecord.scale = newValue;
		return true;
	};
	bool changeRecord_minAllowedScale(uint32_t recordId,float newValue){
		if (newValue < 0.1 || newValue > 100.0) return false;
		if (recordId > header.recordCount) return false;
		SpellVisualEffectNameRecord& neededRecord = records[recordId];
		neededRecord.minAllowedScale = newValue;
		return true;
	};
	bool changeRecord_maxAllowedScale(uint32_t recordId,float newValue){
		if (newValue < 0.1 || newValue > 100.0) return false;
		if (recordId > header.recordCount) return false;
		SpellVisualEffectNameRecord& neededRecord = records[recordId];
		neededRecord.maxAllowedScale = newValue;
		return true;
	};
	bool addNewRecord(uint32_t& m_ID, stringref& name, stringref& m_fileName, float& m_areaEffectSize, float& scale, float& minAllowedScale, float& maxAllowedScale){
		SpellVisualEffectNameRecord newRecord{m_ID+1, name, m_fileName, m_areaEffectSize, scale, minAllowedScale, maxAllowedScale};
		SpellVisualEffectNameRecord* newRecords = new SpellVisualEffectNameRecord[header.recordCount+1];
		for (uint32_t i = 0; i < header.recordCount; i++) {
			newRecords[i] = records[i];
		};
		newRecords[header.recordCount] = newRecord;
		records = newRecords;
		header.recordCount++;
		return true;
	};
	bool addNewRecord(SpellVisualEffectNameRecord newRecord){
		// SpellVisualEffectNameRecord newRecord{m_ID+1, name, m_fileName, m_areaEffectSize, scale, minAllowedScale, maxAllowedScale};
		SpellVisualEffectNameRecord* newRecords = new SpellVisualEffectNameRecord[header.recordCount+1];
		for (uint32_t i = 0; i < header.recordCount; i++) {
			newRecords[i] = records[i];
		};
		newRecords[header.recordCount] = newRecord;
		records = newRecords;
		header.recordCount++;
		return true;
	}
};


#endif
