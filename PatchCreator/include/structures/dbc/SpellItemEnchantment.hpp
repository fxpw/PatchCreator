#ifndef SpellItemEnchantment_HPP
#define SpellItemEnchantment_HPP

	#include "../DBCHeader.hpp"
	struct SpellItemEnchantmentRecord {
		uint32_t ID;
		uint32_t charges;
		uint32_t SpellDispelType1;
		uint32_t SpellDispelType2;
		uint32_t SpellDispelType3;
		uint32_t minAmount1;
		uint32_t minAmount2;
		uint32_t minAmount3;
		uint32_t maxAmount1;
		uint32_t maxAmount2;
		uint32_t maxAmount3;
		uint32_t objectId1;
		uint32_t objectId2;
		uint32_t objectId3;
		stringref sRefName1;
		stringref sRefName2;
		stringref sRefName3;
		stringref sRefName4;
		stringref sRefName5;
		stringref sRefName6;
		stringref sRefName7;
		stringref sRefName8;
		stringref sRefName9;
		stringref sRefName10;
		stringref sRefName11;
		stringref sRefName12;
		stringref sRefName13;
		stringref sRefName14;
		stringref sRefName15;
		stringref sRefName16;
		stringref sRefName17;
		uint32_t ItemVisuals;
		uint32_t Flags;
		uint32_t ItemCache;
		uint32_t SpellItemEnchantmentCondition;
		uint32_t SkillLine;
		uint32_t SkillLevel;
		uint32_t requiredLevel;
	};
	struct SpellItemEnchantmentFile {
		DBCHeader header;
		// static_assert (header.record_size == sizeof (record_type));
		SpellItemEnchantmentRecord* records;
		char* stringBlock;

		bool addNewRecord(SpellItemEnchantmentRecord newRecord) {
			SpellItemEnchantmentRecord* newRecords = new SpellItemEnchantmentRecord[header.recordCount + 1];
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
