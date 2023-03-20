#include <iostream>
#include <cstring>
#include <stdio.h>

#include "common/common.h"
#include "dbc/dbc.h"
#include "structs/spell.h"
#include <map>
#include <nlohmann/json.hpp>
#include <fstream>

#include <string>
#include <stdexcept>
inline unsigned int stringToUInt(const std::string& s)
{
    unsigned long lresult = stoul(s, 0, 10);
    unsigned int result = lresult;
    if (result != lresult) throw std::out_of_range("result not lresult");
    return result;
}

using json = nlohmann::json;

#define SQL_INSERTS_PER_QUERY 300
#define SLASH_BUFFER2000

DBCFileLoader DBCSpell;

std::map<uint32,uint32> SpellChange {};
std::map<uint32,uint32> CreatureDusplayInfoChange {};
std::map<uint32,uint32> ItemDusplayInfoChange {};
std::map<uint32,uint32> SpellUtemEnchantmentChange {};

bool ChangeSpellDBC();
bool ChangeCreatureDusplayInfoDBC();
bool ChangeItemDusplayInfoDBC();
bool ChangeSpellUtemEnchantmentDBC();
bool parseJsons();

int main(){

    parseJsons();
    
    ChangeSpellDBC();


    #ifdef WIN32
        std::cout << "\n\nPlease press Enter to exit...";
        getchar();
    #endif

    return 0;
}

bool parseJsons(){
    /////////////////////
    std::ifstream f1("./jsons/Spell.dbc.json");
    json data1 = json::parse(f1);
    for (json::iterator it = data1.begin(); it != data1.end(); ++it) {
        // std::cout << "Spell.dbc add" << it.key() << " " <<it.value() << '\n';
        SpellChange[stringToUInt(it.key())] = it.value();
    }
    /////////////////////
    std::ifstream f2("./jsons/CreatureDusplayInfo.dbc.json");
    json data2 = json::parse(f2);
    for (json::iterator it = data2.begin(); it != data2.end(); ++it) {
        // std::cout << "CreatureDusplayInfo.dbc add" << it.key() << " " <<it.value() << '\n';
        CreatureDusplayInfoChange[stringToUInt(it.key())] = it.value();
    }
    /////////////////////
    std::ifstream f3("./jsons/ItemDusplayInfo.dbc.json");
    json data3 = json::parse(f3);
    for (json::iterator it = data3.begin(); it != data3.end(); ++it) {
        // std::cout << "ItemDusplayInfo.dbc add" << it.key() << " " <<it.value() << '\n';
        ItemDusplayInfoChange[stringToUInt(it.key())] = it.value();
    }
    /////////////////////
    std::ifstream f4("./jsons/SpellUtemEnchantment.dbc.json");
    json data4 = json::parse(f4);
    for (json::iterator it = data4.begin(); it != data4.end(); ++it) {
        // std::cout << "Spell.dbc add" << it.key() << " " <<it.value() << '\n';
        SpellUtemEnchantmentChange[stringToUInt(it.key())] = it.value();
    }
    /////////////////////
    return true;
};

bool ChangeSpellDBC(){
    std::cout << "*****************************************************************************\n";
    std::cout << "Spell.dbc format:\n";
    std::cout << "DBC columns numbers:" << SPELL_DBC_COLUMN_NUMS << std::endl;
    std::cout << "DBC supported client build: " << SPELL_DBC_CLIENT_BUILD << "\n";

    DBCSpell.Load(SPELL_DBC);
    if(!DBCSpell.getNumFields()){
        std::cout << "ERROR: Can not open file: " << SPELL_DBC << std::endl;
        return false;
    }else{
        std::cout << SPELL_DBC << " - Opened successful." << std::endl << SPELL_DBC << " - fields: "
        << DBCSpell.getNumFields() << ", rows: " << DBCSpell.getNumRows() << std::endl;
    }
    if(DBCSpell.getNumFields() != SPELL_DBC_COLUMN_NUMS){
        std::cout << SPELL_DBC << " - ERROR: Column numbers do not match with the supported DBC format." << std::endl;
        return false;
    }
    std::cout << SPELL_DBC << " - DBC format: OK." << "\n";

    FILE* npf = fopen("Spell.dbc","wb");
    fwrite(&DBCSpell.header,4,1,npf);

    fwrite(&DBCSpell.recordCount,4,1,npf);

    fwrite(&DBCSpell.fieldCount,4,1,npf);

    fwrite(&DBCSpell.recordSize,4,1,npf);

    fwrite(&DBCSpell.stringSize,4,1,npf);


    for (int i = 0; i < DBCSpell.recordCount; i++){
        auto record = DBCSpell.getRecord(i);
        int spellid = record.getInt32(0);
        const char* spellNameGetet = record.getString(136);
        std::string spellName(spellNameGetet);
        uint32 spellVisualID = record.getUInt32(131);

        if (SpellChange.count(spellVisualID)){
            // std::cout<< "spellid->("<< spellid <<") spellname->("<< spellName <<") SpellChange->("<< spellVisualID<<") changed SpellChange to " << SpellChange[spellVisualID]<< std::endl;
            // std::cout<< "{"<<SpellChange << "," << SpellChange[SpellChange] << "} changed to 0" << std::endl;
            record.setUInt32(131,SpellChange[spellVisualID]);
        }
    }   


    fwrite(DBCSpell.data,DBCSpell.recordSize * DBCSpell.recordCount + DBCSpell.stringSize, 1,npf);

    fwrite(DBCSpell.stringTable,DBCSpell.stringSize,0,npf);


    fclose(npf);
    std::cout << SPELL_DBC << " -> Spell.dbc: OK." << "\n\n";
    std::cout << "*****************************************************************************\n\n\n";
    return true;
}

