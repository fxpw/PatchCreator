// PatchCreatorDll.cpp : Определяет функции для статической библиотеки.
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
DBCFileLoader DBCSpellVisual;
DBCFileLoader DBCSpellVisualKit;
DBCFileLoader DBCSpellVisualEffectName;

std::map<uint32, uint32> SpellMap{};
std::map<uint32, uint32> ItemDisplayInfoMap{};
std::map<uint32, uint32> SpellItemEnchantmentMap{};
std::map<uint32, uint32> SpellVisualMap{};
std::map<uint32, uint32> SpellVisualKitMap{};
std::map<uint32, uint32> SpellVisualEffectNameMap{};


__declspec(dllexport) bool PatchCreate(Maap* pMaap, int count, const char* path)
{
	//std::string spath = std::string(path);
	return MainFunction(pMaap, count, std::string(path));

}

bool MainFunction(Maap* pMaap, int count, std::string path)
{
	ParseJsons(pMaap, count);
	ExtractMPQ(path);

	ChangeSpellDBC(path);
	ChangeItemDisplayInfoDBC(path);
	ChangeSpellItemEnchantmentDBC(path);

	CreateMPQ(path);
	return true;
}

bool ParseJsons(Maap* pMaap, int count) {
	for (int i = 0; i < count; i++) {
		SpellMap[pMaap[i].Key] = pMaap[i].Value;
	}
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

	//std::cout << "*****************************ExtractMPQ********************************\n";
	HANDLE mpq;
	//std::cout << "SFileOpenArchive start" << std::endl;
	bool isSuccess = SFileOpenArchive(ConverterToTCHAR((path + std::string("/Data/ruRU/patch-ruRU-4.mpq")).c_str()), 0, STREAM_FLAG_WRITE_SHARE, &mpq);
	if (!isSuccess) return false;
	// std::cout<< "SFileOpenArchive error " << GetLastError() << std::endl;
	if (!mpq) {
		return false;
	}
	//std::cout << "SFileExtractFile Spell.dbc" << std::endl;
	SFileExtractFile(mpq, "DBFilesClient\\Spell.dbc", ConverterToTCHAR("./Spell.dbc"), SFILE_OPEN_FROM_MPQ);
	//std::cout << "SFileExtractFile ItemDisplayInfo.dbc" << std::endl;
	SFileExtractFile(mpq, "DBFilesClient\\ItemDisplayInfo.dbc", ConverterToTCHAR("./ItemDisplayInfo.dbc"), SFILE_OPEN_FROM_MPQ);
	//std::cout << "SFileExtractFile SpellItemEnchantment.dbc" << std::endl;
	SFileExtractFile(mpq, "DBFilesClient\\SpellItemEnchantment.dbc", ConverterToTCHAR("./SpellItemEnchantment.dbc"), SFILE_OPEN_FROM_MPQ);
	//std::cout << SFileCloseArchive(mpq) << std::endl;
	SFileCloseArchive(mpq);
	return true;
		//std::cout << "*****************************ExtractMPQ********************************\n";
	
};





bool ChangeSpellDBC(std::string path = "error") {
	//try {//std::cout << "*****************************ChangeSpellDBC********************************\n";
		//std::cout << "Spell.dbc format:\n";
	DBCSpell.Load("./Spell.dbc");
	if (!DBCSpell.getNumFields()) {
		//std::cout << "ERROR: Can not open file: " << "./DBFilesClient/Spell.dbc" << std::endl;
		return false;
	}
	else {
		//std::cout << "./DBFilesClient/Spell.dbc" << " - Opened successful." << std::endl << "./DBFilesClient/Spell.dbc" << " - fields: "
			//<< DBCSpell.getNumFields() << ", rows: " << DBCSpell.getNumRows() << std::endl;
	}
	//std::cout << "./DBFilesClient/Spell.dbc" << " - DBC format: OK." << "\n";

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
				// std::cout<< "spellid->("<< spellid <<") spellname->("<< spellName <<") SpellMap->("<< spellVisualID<<") changed SpellMap to " << SpellMap[spellVisualID]<< std::endl;
				// std::cout<< "spell id- > "<<spellid << "  spell visual -> " << spellVisualID << " changed to "<< SpellMap[spellid] << std::endl;
				record.setUInt32(131, SpellMap[spellid]);
			}
		}

		fwrite(DBCSpell.data, DBCSpell.recordSize * DBCSpell.recordCount + DBCSpell.stringSize, 1, npf);
		fwrite(DBCSpell.stringTable, DBCSpell.stringSize, 0, npf);

		fclose(npf);
		//std::cout << "./DBFilesClient/Spell.dbc" << " -> Spell.dbc: OK." << "\n\n";
		//std::cout << "****************************ChangeSpellDBC**********************************\n\n\n";
		return true;
	}else{
		return false;
	}

}


bool ChangeItemDisplayInfoDBC(std::string path = "error") {
	//std::cout << "*****************************ChangeItemDisplayInfoDBC********************************\n";
	//std::cout << "ItemDisplayInfo.dbc format:\n";
	DBCItemDisplayInfo.Load("./ItemDisplayInfo.dbc");
	if (!DBCItemDisplayInfo.getNumFields()) {
		//std::cout << "ERROR: Can not open file: " << "./DBFilesClient/ItemDisplayInfo.dbc" << std::endl;
		return false;
	}else{
		//std::cout << "./DBFilesClient/ItemDisplayInfo.dbc" << " - Opened successful." << std::endl << "./DBFilesClient/ItemDisplayInfo.dbc" << " - fields: "
			//<< DBCItemDisplayInfo.getNumFields() << ", rows: " << DBCItemDisplayInfo.getNumRows() << std::endl;
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
		//std::cout << "./DBFilesClient/ItemDisplayInfo.dbc" << " -> ItemDisplayInfo.dbc: OK." << "\n\n";
		//std::cout << "**********************************ChangeItemDisplayInfoDBC*****************************\n\n\n";
		return true;
	}else{
		return false;
	}
}

bool ChangeSpellItemEnchantmentDBC(std::string path = "error") {
	//std::cout << "**************************ChangeSpellItemEnchantmentDBC****************************\n";
	//std::cout << "SpellItemEnchantment.dbc format:\n";
	DBCSpellItemEnchantment.Load("./SpellItemEnchantment.dbc");
	if (!DBCSpellItemEnchantment.getNumFields()) {
		//std::cout << "ERROR: Can not open file: " << "./DBFilesClient/SpellItemEnchantment.dbc" << std::endl;
		return false;
	}else{
		//std::cout << "./DBFilesClient/SpellItemEnchantment.dbc" << " - Opened successful." << std::endl << "./DBFilesClient/SpellItemEnchantment.dbc" << " - fields: "
			//<< DBCSpellItemEnchantment.getNumFields() << ", rows: " << DBCSpellItemEnchantment.getNumRows() << std::endl;
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
		//std::cout << "./DBFilesClient/SpellItemEnchantment.dbc" << " -> SpellItemEnchantment.dbc: OK." << "\n\n";
		//std::cout << "***********************************ChangeSpellItemEnchantmentDBC*******************************\n\n\n";
		return true;
	}else{
		return false;
	}
}


bool CreateMPQ(std::string path = "error") {
	//std::cout << "**********************************CreateMPQ************************************\n\n\n";
	try {
		HANDLE mpq;
		remove((path + std::string("/Data/ruRU/patch-ruRU-x.mpq")).c_str());
		bool isSucsess = SFileCreateArchive(ConverterToTCHAR((path + std::string("/Data/ruRU/patch-ruRU-x.mpq")).c_str()), MPQ_CREATE_ATTRIBUTES + MPQ_CREATE_ARCHIVE_V2, 0x000000010, &mpq);
		if (!isSucsess){
			remove("./Spell.dbc");
			remove("./ItemDisplayInfo.dbc");
			remove("./SpellItemEnchantment.dbc");
			return false;
		}

		SFileAddFileEx(mpq, ConverterToTCHAR("./Spell.dbc"), "DBFilesClient\\Spell.dbc", MPQ_FILE_COMPRESS + MPQ_FILE_REPLACEEXISTING, MPQ_COMPRESSION_ZLIB, MPQ_COMPRESSION_NEXT_SAME);
		remove("./Spell.dbc");
		//std::cout << "added Spell.dbc" << std::endl;
		SFileAddFileEx(mpq, ConverterToTCHAR("./ItemDisplayInfo.dbc"), "DBFilesClient\\ItemDisplayInfo.dbc", MPQ_FILE_COMPRESS + MPQ_FILE_REPLACEEXISTING, MPQ_COMPRESSION_ZLIB, MPQ_COMPRESSION_NEXT_SAME);
		//std::cout << "added ItemDisplayInfo.dbc" << std::endl;
		remove("./ItemDisplayInfo.dbc");
		SFileAddFileEx(mpq, ConverterToTCHAR("./SpellItemEnchantment.dbc"), "DBFilesClient\\SpellItemEnchantment.dbc", MPQ_FILE_COMPRESS + MPQ_FILE_REPLACEEXISTING, MPQ_COMPRESSION_ZLIB, MPQ_COMPRESSION_NEXT_SAME);
		//std::cout << "added SpellItemEnchantment.dbc" << std::endl;
		remove("./SpellItemEnchantment.dbc");
		//std::cout << "End create MPQ" << std::endl;
		return SFileCloseArchive(mpq);
		//std::cout << "*********************************CreateMPQ**********************************\n\n\n";
	}catch (...) {
		return false;
	}
};
