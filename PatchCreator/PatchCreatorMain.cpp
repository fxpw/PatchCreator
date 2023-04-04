// PatchCreatorDll.cpp : Defines functions for a static library.
//

#include "pch.h"
#include "PatchCreatorMain.hpp"
#include <StormLib.h>
#include "common.hpp"
#include "dbc.hpp"
#include <iostream>
#include <fstream>

DBCFileLoader DBCSpell;
DBCFileLoader DBCItemDisplayInfo;
DBCFileLoader DBCSpellItemEnchantment;
//DBCFileLoader DBCSpellVisual;
DBCFileLoader DBCSpellVisualKit;
//DBCFileLoader DBCSpellVisualEffectName;

std::map<uint32, uint32> SpellMap{};
std::map<uint32, uint32> ItemDisplayInfoMap{};
std::map<uint32, uint32> SpellItemEnchantmentMap{};
//std::map<uint32, uint32> SpellVisualMap{};
std::map<uint32, uint32> SpellVisualKitMap{};
//std::map<uint32, uint32> SpellVisualEffectNameMap{};


__declspec(dllexport) bool PatchCreate(JsonMap* spellMap, int spellMapCount, JsonMap* spellVusualKitMap, int spellVusualKitCount, const char* path)
{
	//std::string spath = std::string(path);
	return MainFunction(spellMap, spellMapCount, spellVusualKitMap, spellVusualKitCount, std::string(path));

}

bool MainFunction(JsonMap* spellMap, int spellMapCount, JsonMap* spellVusualKitMap, int spellVusualKitCount, std::string path)
{
	ParseJsons(spellMap, spellMapCount, spellVusualKitMap, spellVusualKitCount);
	bool isSuccess = ExtractMPQ(path);
	if (!isSuccess) return false;

	ChangeSpellDBC();
	ChangeItemDisplayInfoDBC();
	ChangeSpellItemEnchantmentDBC();
	ChangeSpellVisualKitDBC();
	return CreateMPQ(path);
}

bool ParseJsons(JsonMap* spellMap, int spellMapCount, JsonMap* spellVusualKitMap, int spellVusualKitCount) {
	#pragma region SpellMap
		for (int i = 0; i < spellMapCount; i++) {
			SpellMap[spellMap[i].Key] = spellMap[i].Value;
		}
	#pragma endregion

	#pragma region SpellVisualKitMap
		for (int i = 0; i < spellVusualKitCount; i++) {
			SpellVisualKitMap[spellVusualKitMap[i].Key] = spellVusualKitMap[i].Value;
		}
		//SpellVisualKitMap[28] = 0;
		//SpellVisualKitMap[34] = 0;
		//SpellVisualKitMap[35] = 0;
		//SpellVisualKitMap[58] = 0;
		//SpellVisualKitMap[97] = 0;
		//SpellVisualKitMap[98] = 0;
		//SpellVisualKitMap[116] = 0;
		//SpellVisualKitMap[117] = 0;
		//SpellVisualKitMap[122] = 0;
		//SpellVisualKitMap[163] = 0;
		//SpellVisualKitMap[179] = 0;
		//SpellVisualKitMap[181] = 0;
		//SpellVisualKitMap[187] = 0;
		//SpellVisualKitMap[191] = 0;
		//SpellVisualKitMap[192] = 0;
		//SpellVisualKitMap[193] = 0;
		//SpellVisualKitMap[194] = 0;
		//SpellVisualKitMap[200] = 0;
		//SpellVisualKitMap[214] = 0;
		//SpellVisualKitMap[215] = 0;
		//SpellVisualKitMap[220] = 0;
		//SpellVisualKitMap[261] = 0;
		//SpellVisualKitMap[262] = 0;
		//SpellVisualKitMap[265] = 0;
		//SpellVisualKitMap[340] = 0;
		//SpellVisualKitMap[624] = 0;
		//SpellVisualKitMap[1091] = 0;
		//SpellVisualKitMap[1092] = 0;
		//SpellVisualKitMap[2575] = 0;
		//SpellVisualKitMap[6173] = 0;
	#pragma endregion
	return true;
};

TCHAR* ConverterToTCHAR(const char* orig) {
	TCHAR* tchar = 0;
	std::string str = std::string(orig);
	tchar = new TCHAR[str.size() + 1];
	copy(str.begin(), str.end(), tchar);
	tchar[str.size()] = 0;
	return tchar;
};


bool ExtractMPQ(std::string path = "error") {

	HANDLE mpq;

	bool isSuccess = SFileOpenArchive(ConverterToTCHAR((path + std::string("/Data/ruRU/patch-ruRU-4.mpq")).c_str()), 0, STREAM_FLAG_WRITE_SHARE, &mpq);
	if (!isSuccess) return false;

	if (!mpq) {
		return false;
	}

	SFileExtractFile(mpq, "DBFilesClient\\Spell.dbc", ConverterToTCHAR("./Spell.dbc"), SFILE_OPEN_FROM_MPQ);

	SFileExtractFile(mpq, "DBFilesClient\\ItemDisplayInfo.dbc", ConverterToTCHAR("./ItemDisplayInfo.dbc"), SFILE_OPEN_FROM_MPQ);

	SFileExtractFile(mpq, "DBFilesClient\\SpellItemEnchantment.dbc", ConverterToTCHAR("./SpellItemEnchantment.dbc"), SFILE_OPEN_FROM_MPQ);

	SFileExtractFile(mpq, "DBFilesClient\\SpellVisualKit.dbc", ConverterToTCHAR("./SpellVisualKit.dbc"), SFILE_OPEN_FROM_MPQ);

	SFileCloseArchive(mpq);
	return true;
	
};



bool ChangeSpellDBC() {

	DBCSpell.Load("./Spell.dbc");
	if (!DBCSpell.getNumFields()) {
		return false;
	}

	FILE* npf;
	errno_t err = fopen_s(&npf, "./Spell.dbc", "wb");
	if (err == 0) {
		fwrite(&DBCSpell.header, 4, 1, npf);

		fwrite(&DBCSpell.recordCount, 4, 1, npf);

		fwrite(&DBCSpell.fieldCount, 4, 1, npf);

		fwrite(&DBCSpell.recordSize, 4, 1, npf);

		fwrite(&DBCSpell.stringSize, 4, 1, npf);

		for (uint32 i = 0; i < DBCSpell.recordCount; i++) {
			auto record = DBCSpell.getRecord(i);
			int spellid = record.getInt32(0);
			const char* spellNameGetet = record.getString(136);
			std::string spellName(spellNameGetet);
			uint32 spellVisualID = record.getUInt32(131);

			if (SpellMap.count(spellid)) {
				record.setUInt32(131, SpellMap[spellid]);
			}
		}

		fwrite(DBCSpell.data, DBCSpell.recordSize * DBCSpell.recordCount + DBCSpell.stringSize, 1, npf);
		fwrite(DBCSpell.stringTable, DBCSpell.stringSize, 0, npf);

		fclose(npf);

		return true;
	}else{
		return false;
	}

}


bool ChangeItemDisplayInfoDBC() {

	DBCItemDisplayInfo.Load("./ItemDisplayInfo.dbc");
	if (!DBCItemDisplayInfo.getNumFields()) {

		return false;
	}
	for (uint32 i = 0; i < DBCItemDisplayInfo.recordCount; i++) {
		auto record = DBCItemDisplayInfo.getRecord(i);
		record.setUInt32(23, 0);
		record.setUInt32(11, 0);

	}
	FILE* npf;
	errno_t err = fopen_s(&npf, "./ItemDisplayInfo.dbc", "wb");
	if (err == 0) {
		fwrite(&DBCItemDisplayInfo.header, 4, 1, npf);
		fwrite(&DBCItemDisplayInfo.recordCount, 4, 1, npf);
		fwrite(&DBCItemDisplayInfo.fieldCount, 4, 1, npf);
		fwrite(&DBCItemDisplayInfo.recordSize, 4, 1, npf);
		fwrite(&DBCItemDisplayInfo.stringSize, 4, 1, npf);
		fwrite(DBCItemDisplayInfo.data, DBCItemDisplayInfo.recordSize * DBCItemDisplayInfo.recordCount + DBCItemDisplayInfo.stringSize, 1, npf);
		fwrite(DBCItemDisplayInfo.stringTable, DBCItemDisplayInfo.stringSize, 0, npf);
		fclose(npf);
		return true;
	}else{
		return false;
	}
}

bool ChangeSpellItemEnchantmentDBC() {

	DBCSpellItemEnchantment.Load("./SpellItemEnchantment.dbc");
	if (!DBCSpellItemEnchantment.getNumFields()) {
		return false;
	}
	for (uint32 i = 0; i < DBCSpellItemEnchantment.recordCount; i++) {
		auto record = DBCSpellItemEnchantment.getRecord(i);
		record.setUInt32(31, 0);
	}

	FILE* npf;
	errno_t err = fopen_s(&npf, "./SpellItemEnchantment.dbc", "wb");
	if (err == 0) {
		fwrite(&DBCSpellItemEnchantment.header, 4, 1, npf);
		fwrite(&DBCSpellItemEnchantment.recordCount, 4, 1, npf);
		fwrite(&DBCSpellItemEnchantment.fieldCount, 4, 1, npf);
		fwrite(&DBCSpellItemEnchantment.recordSize, 4, 1, npf);
		fwrite(&DBCSpellItemEnchantment.stringSize, 4, 1, npf);
		fwrite(DBCSpellItemEnchantment.data, DBCSpellItemEnchantment.recordSize * DBCSpellItemEnchantment.recordCount + DBCSpellItemEnchantment.stringSize, 1, npf);
		fwrite(DBCSpellItemEnchantment.stringTable, DBCSpellItemEnchantment.stringSize, 0, npf);
		fclose(npf);
		return true;
	}else{
		return false;
	}
}

bool ChangeSpellVisualKitDBC() {
	DBCSpellVisualKit.Load("./SpellVisualKit.dbc");
	if (!DBCSpellVisualKit.getNumFields()) { return false; };
	for (uint32 i = 0; i < DBCSpellVisualKit.recordCount; i++) {

		auto record = DBCSpellVisualKit.getRecord(i);
		int spellid = record.getInt32(0);

		if (SpellVisualKitMap.count(spellid)) {

			record.setInt32(6, 0);
			record.setInt32(7, 0);
		}
	}
	FILE* npf;
	errno_t err = fopen_s(&npf, "./SpellVisualKit.dbc", "wb");
	if (err == 0) {
		fwrite(&DBCSpellVisualKit.header, 4, 1, npf);
		fwrite(&DBCSpellVisualKit.recordCount, 4, 1, npf);
		fwrite(&DBCSpellVisualKit.fieldCount, 4, 1, npf);
		fwrite(&DBCSpellVisualKit.recordSize, 4, 1, npf);
		fwrite(&DBCSpellVisualKit.stringSize, 4, 1, npf);
		fwrite(DBCSpellVisualKit.data, DBCSpellVisualKit.recordSize * DBCSpellVisualKit.recordCount + DBCSpellVisualKit.stringSize, 1, npf);
		fwrite(DBCSpellVisualKit.stringTable, DBCSpellVisualKit.stringSize, 0, npf);
		fclose(npf);
		return true;
	}
	else {
		return false;
	}
	return true;
}

bool CreateMPQ(std::string path = "error") {
	try {
		HANDLE mpq;
		remove((path + std::string("/Data/ruRU/patch-ruRU-[.mpq")).c_str());
		bool isSucsess = SFileCreateArchive(ConverterToTCHAR((path + std::string("/Data/ruRU/patch-ruRU-[.mpq")).c_str()), MPQ_CREATE_ATTRIBUTES + MPQ_CREATE_ARCHIVE_V2, 0x000000010, &mpq);
		if (!isSucsess){
			remove("./Spell.dbc");
			remove("./ItemDisplayInfo.dbc");
			remove("./SpellItemEnchantment.dbc");
			remove("./SpellVisualKit.dbc");
			return false;
		}

		SFileAddFileEx(mpq, ConverterToTCHAR("./Spell.dbc"), "DBFilesClient\\Spell.dbc", MPQ_FILE_COMPRESS + MPQ_FILE_REPLACEEXISTING, MPQ_COMPRESSION_ZLIB, MPQ_COMPRESSION_NEXT_SAME);
		remove("./Spell.dbc");

		SFileAddFileEx(mpq, ConverterToTCHAR("./ItemDisplayInfo.dbc"), "DBFilesClient\\ItemDisplayInfo.dbc", MPQ_FILE_COMPRESS + MPQ_FILE_REPLACEEXISTING, MPQ_COMPRESSION_ZLIB, MPQ_COMPRESSION_NEXT_SAME);
		remove("./ItemDisplayInfo.dbc");
		
		SFileAddFileEx(mpq, ConverterToTCHAR("./SpellItemEnchantment.dbc"), "DBFilesClient\\SpellItemEnchantment.dbc", MPQ_FILE_COMPRESS + MPQ_FILE_REPLACEEXISTING, MPQ_COMPRESSION_ZLIB, MPQ_COMPRESSION_NEXT_SAME);
		remove("./SpellItemEnchantment.dbc");

		SFileAddFileEx(mpq, ConverterToTCHAR("./SpellVisualKit.dbc"), "DBFilesClient\\SpellVisualKit.dbc", MPQ_FILE_COMPRESS + MPQ_FILE_REPLACEEXISTING, MPQ_COMPRESSION_ZLIB, MPQ_COMPRESSION_NEXT_SAME);
		remove("./SpellVisualKit.dbc");

		return SFileCloseArchive(mpq);

	}catch (...) {
		return false;
	}
};
