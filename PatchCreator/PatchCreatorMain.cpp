// PatchCreatorDll.cpp : Определяет функции для статической библиотеки.
//

#include "pch.h"
#include "connectFunc.h"
#include <StormLib.h>
#include "common.hpp"
#include "dbc.hpp"
#include <iostream>
#include <fstream>

DBCFileLoader DBCSpell;
DBCFileLoader DBCCreatureDusplayInfo;
DBCFileLoader DBCItemDisplayInfo;
DBCFileLoader DBCSpellItemEnchantment;

std::map<uint32, uint32> SpellChange{};

std::map<uint32, uint32> CreatureDusplayInfoChange{};
std::map<uint32, uint32> ItemDisplayInfoChange{};
std::map<uint32, uint32> SpellItemEnchantmentChange{};

bool ChangeSpellDBC(std::string);
bool ChangeCreatureDisplayInfoDBC(std::string);
bool ChangeItemDisplayInfoDBC(std::string);
bool ChangeSpellItemEnchantmentDBC(std::string);
bool CreateMPQ(std::string);
bool ParseJsons(Maap*);
bool ExtractMPQ(std::string);

bool MainFunction(Maap* pMaap, std::string a = "error") {

	std::string path = a;
	ParseJsons(pMaap);
	ExtractMPQ(path);

	ChangeSpellDBC(path);
	ChangeCreatureDisplayInfoDBC(path);
	ChangeItemDisplayInfoDBC(path);
	ChangeSpellItemEnchantmentDBC(path);

	return CreateMPQ(path);

}


bool PatchCreate(Maap* pMaap, const char* a)
{
	std::string news = std::string(a);
	return MainFunction(pMaap, news);

}

bool ParseJsons(Maap* pMaap) {
	for (int i = 0; i < pMaap[0].Count; i++) {
		SpellChange[pMaap[i].Key] = pMaap[i].Value;
	}
	return true;
};

wchar_t* ConverterToWChar(const char* orig) {
	//const char* orig = "./patch-ruRU-4.mpq";
	size_t newsize = strlen(orig) + 1;
	wchar_t* wcstring = new wchar_t[newsize];

	// Convert char* string to a wchar_t* string.
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcstring, newsize, orig, _TRUNCATE);
	// Display the result and indicate the type of string that it is.
	//wcout << wcstring << L" (wchar_t *)" << endl;
	//delete[] wcstring;


	return wcstring;
};


bool ExtractMPQ(std::string path = "error") {

	//std::cout << "*****************************ExtractMPQ********************************\n";
	HANDLE mpq;
	//std::cout << "SFileOpenArchive start" << std::endl;
	SFileOpenArchive(ConverterToWChar((path + std::string("/Data/ruRU/patch-ruRU-4.mpq")).c_str()), 0, STREAM_FLAG_WRITE_SHARE, &mpq);
	// std::cout<< "SFileOpenArchive error " << GetLastError() << std::endl;
	if (mpq) {
		//std::cout << "SFileExtractFile Spell.dbc" << std::endl;
		SFileExtractFile(mpq, "DBFilesClient\\Spell.dbc", ConverterToWChar("./Spell.dbc"), SFILE_OPEN_FROM_MPQ);
		//std::cout << "SFileExtractFile ItemDisplayInfo.dbc" << std::endl;
		SFileExtractFile(mpq, "DBFilesClient\\ItemDisplayInfo.dbc", ConverterToWChar("./ItemDisplayInfo.dbc"), SFILE_OPEN_FROM_MPQ);
		//std::cout << "SFileExtractFile SpellItemEnchantment.dbc" << std::endl;
		SFileExtractFile(mpq, "DBFilesClient\\SpellItemEnchantment.dbc", ConverterToWChar("./SpellItemEnchantment.dbc"), SFILE_OPEN_FROM_MPQ);
		//std::cout << SFileCloseArchive(mpq) << std::endl;
		SFileCloseArchive(mpq);
		return true;
		//std::cout << "*****************************ExtractMPQ********************************\n";
	}
	return false;
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

			if (SpellChange.count(spellid)) {
				// std::cout<< "spellid->("<< spellid <<") spellname->("<< spellName <<") SpellChange->("<< spellVisualID<<") changed SpellChange to " << SpellChange[spellVisualID]<< std::endl;
				// std::cout<< "spell id- > "<<spellid << "  spell visual -> " << spellVisualID << " changed to "<< SpellChange[spellid] << std::endl;
				record.setUInt32(131, SpellChange[spellid]);
			}
		}

		fwrite(DBCSpell.data, DBCSpell.recordSize * DBCSpell.recordCount + DBCSpell.stringSize, 1, npf);
		fwrite(DBCSpell.stringTable, DBCSpell.stringSize, 0, npf);

		fclose(npf);
		//std::cout << "./DBFilesClient/Spell.dbc" << " -> Spell.dbc: OK." << "\n\n";
		//std::cout << "****************************ChangeSpellDBC**********************************\n\n\n";
		return true;
	}
	else
	{
		return false;
	}

}

bool ChangeCreatureDisplayInfoDBC(std::string path = "error") {
	//std::cout << "******************************ChangeCreatureDisplayInfoDBC********************************\n";
		//std::cout << "CreatureDusplayInfo.dbc format:\n";
	DBCCreatureDusplayInfo.Load("./CreatureDisplayInfo.dbc");
	//std::cout << "******************************ChangeCreatureDisplayInfoDBC**************************\n\n\n";
	return true;
};

bool ChangeItemDisplayInfoDBC(std::string path = "error") {
	//std::cout << "*****************************ChangeItemDisplayInfoDBC********************************\n";
	//std::cout << "ItemDisplayInfo.dbc format:\n";
	DBCItemDisplayInfo.Load("./ItemDisplayInfo.dbc");
	if (!DBCItemDisplayInfo.getNumFields()) {
		//std::cout << "ERROR: Can not open file: " << "./DBFilesClient/ItemDisplayInfo.dbc" << std::endl;
		return false;
	}
	else
	{
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
	}
	else
	{
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
	}
	else {
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
	}
	else
	{
		return false;
	}
}


bool CreateMPQ(std::string path = "error") {
	//std::cout << "**********************************CreateMPQ************************************\n\n\n";
	try {
		HANDLE mpq;
		remove((path + std::string("/Data/ruRU/patch-ruRU-x.mpq")).c_str());
		bool isSucsess = SFileCreateArchive(ConverterToWChar((path + std::string("/Data/ruRU/patch-ruRU-x.mpq")).c_str()), MPQ_CREATE_ATTRIBUTES + MPQ_CREATE_ARCHIVE_V2, 0x000000010, &mpq);
		if (!isSucsess) return false;
		SFileAddFileEx(mpq, ConverterToWChar("./Spell.dbc"), "DBFilesClient\\Spell.dbc", MPQ_FILE_COMPRESS + MPQ_FILE_REPLACEEXISTING, MPQ_COMPRESSION_ZLIB, MPQ_COMPRESSION_NEXT_SAME);
		remove("./Spell.dbc");
		//std::cout << "added Spell.dbc" << std::endl;
		SFileAddFileEx(mpq, ConverterToWChar("./ItemDisplayInfo.dbc"), "DBFilesClient\\ItemDisplayInfo.dbc", MPQ_FILE_COMPRESS + MPQ_FILE_REPLACEEXISTING, MPQ_COMPRESSION_ZLIB, MPQ_COMPRESSION_NEXT_SAME);
		//std::cout << "added ItemDisplayInfo.dbc" << std::endl;
		remove("./ItemDisplayInfo.dbc");
		SFileAddFileEx(mpq, ConverterToWChar("./SpellItemEnchantment.dbc"), "DBFilesClient\\SpellItemEnchantment.dbc", MPQ_FILE_COMPRESS + MPQ_FILE_REPLACEEXISTING, MPQ_COMPRESSION_ZLIB, MPQ_COMPRESSION_NEXT_SAME);
		//std::cout << "added SpellItemEnchantment.dbc" << std::endl;
		remove("./SpellItemEnchantment.dbc");
		//std::cout << "End create MPQ" << std::endl;
		return SFileCloseArchive(mpq);
		//std::cout << "*********************************CreateMPQ**********************************\n\n\n";
	}
	catch (...) {
		return false;
	}
};
