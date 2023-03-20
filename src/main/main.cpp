#include <iostream>
#include <cstring>
#include <stdio.h>

#include "common/common.hpp"
#include "dbc/dbc.hpp"
#include "structs/spell.hpp"
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
DBCFileLoader DBCCreatureDusplayInfo;
DBCFileLoader DBCItemDisplayInfo;
DBCFileLoader DBCSpellItemEnchantment;

std::map<uint32,uint32> SpellChange {};
std::map<uint32,uint32> CreatureDusplayInfoChange {};
std::map<uint32,uint32> ItemDisplayInfoChange {};
std::map<uint32,uint32> SpellItemEnchantmentChange {};

bool ChangeSpellDBC();
bool ChangeCreatureDisplayInfoDBC();
bool ChangeItemDisplayInfoDBC();
bool ChangeSpellItemEnchantmentDBC();

bool parseJsons();

int main(){

    parseJsons();
    
    ChangeSpellDBC();
    ChangeCreatureDisplayInfoDBC();
    ChangeItemDisplayInfoDBC();
    ChangeSpellItemEnchantmentDBC();
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
    std::ifstream f3("./jsons/ItemDisplayInfo.dbc.json");
    json data3 = json::parse(f3);
    for (json::iterator it = data3.begin(); it != data3.end(); ++it) {
        // std::cout << "ItemDisplayInfo.dbc add" << it.key() << " " <<it.value() << '\n';
       ItemDisplayInfoChange[stringToUInt(it.key())] = it.value();
    }
    /////////////////////
    std::ifstream f4("./jsons/SpellItemEnchantment.dbc.json");
    json data4 = json::parse(f4);
    for (json::iterator it = data4.begin(); it != data4.end(); ++it) {
        // std::cout << "Spell.dbc add" << it.key() << " " <<it.value() << '\n';
        SpellItemEnchantmentChange[stringToUInt(it.key())] = it.value();
    }
    /////////////////////
    return true;
};

bool ChangeSpellDBC(){
    std::cout << "*****************************************************************************\n";
    std::cout << "Spell.dbc format:\n";

    DBCSpell.Load("./original/Spell.dbc");
    if(!DBCSpell.getNumFields()){
        std::cout << "ERROR: Can not open file: " << "./original/Spell.dbc" << std::endl;
        return false;
    }else{
        std::cout << "./original/Spell.dbc" << " - Opened successful." << std::endl << "./original/Spell.dbc" << " - fields: "
        << DBCSpell.getNumFields() << ", rows: " << DBCSpell.getNumRows() << std::endl;
    }

    std::cout << "./original/Spell.dbc" << " - DBC format: OK." << "\n";

    FILE* npf = fopen("./buildedDBC/Spell.dbc","wb");
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
    std::cout << "./original/Spell.dbc" << " -> Spell.dbc: OK." << "\n\n";
    std::cout << "*****************************************************************************\n\n\n";
    return true;
}

bool ChangeCreatureDisplayInfoDBC(){
    std::cout << "*****************************************************************************\n";
    std::cout << "CreatureDusplayInfo.dbc format:\n";

    DBCCreatureDusplayInfo.Load("./original/CreatureDisplayInfo.dbc");
    std::cout << "*****************************************************************************\n\n\n";
    return true;
};

bool ChangeItemDisplayInfoDBC(){
    std::cout << "*****************************************************************************\n";
    std::cout << "ItemDisplayInfo.dbc format:\n";

    DBCItemDisplayInfo.Load("./original/ItemDisplayInfo.dbc");
    if(!DBCItemDisplayInfo.getNumFields()){
        std::cout << "ERROR: Can not open file: " << "./original/ItemDisplayInfo.dbc" << std::endl;
        return false;
    }else{
        std::cout << "./original/ItemDisplayInfo.dbc" << " - Opened successful." << std::endl << "./original/ItemDisplayInfo.dbc" << " - fields: "
        << DBCItemDisplayInfo.getNumFields() << ", rows: " << DBCItemDisplayInfo.getNumRows() << std::endl;
    }
    for (int i = 0; i < DBCItemDisplayInfo.recordCount; i++){
        auto record = DBCItemDisplayInfo.getRecord(i);
        // int spellid = record.getInt32(0);
        // const char* spellNameGetet = record.getString(136);
        // std::string spellName(spellNameGetet);
        // uint32 spellVisualID = record.getUInt32(23);

        // if (ItemDisplayInfoChange.count(spellVisualID)){
            // std::cout<< "spellid->("<< spellid <<") spellname->("<< spellName <<") SpellChange->("<< spellVisualID<<") changed SpellChange to " << SpellChange[spellVisualID]<< std::endl;
            // std::cout<< "{"<<SpellChange << "," << SpellChange[SpellChange] << "} changed to 0" << std::endl;
        record.setUInt32(23,0);
        // }
    }   
    FILE* npf = fopen("./buildedDBC/ItemDisplayInfo.dbc","wb");

    fwrite(&DBCItemDisplayInfo.header,4,1,npf);

    fwrite(&DBCItemDisplayInfo.recordCount,4,1,npf);

    fwrite(&DBCItemDisplayInfo.fieldCount,4,1,npf);

    fwrite(&DBCItemDisplayInfo.recordSize,4,1,npf);

    fwrite(&DBCItemDisplayInfo.stringSize,4,1,npf);
    fwrite(DBCItemDisplayInfo.data, DBCItemDisplayInfo.recordSize * DBCItemDisplayInfo.recordCount + DBCItemDisplayInfo.stringSize, 1,npf);

    fwrite(DBCItemDisplayInfo.stringTable,DBCItemDisplayInfo.stringSize,0,npf);
    fclose(npf);

    std::cout << "./original/ItemDisplayInfo.dbc" << " -> ItemDisplayInfo.dbc: OK." << "\n\n";
    std::cout << "*****************************************************************************\n\n\n";
    return true;
}

bool ChangeSpellItemEnchantmentDBC(){
    std::cout << "*****************************************************************************\n";
    std::cout << "SpellItemEnchantment.dbc format:\n";

    DBCSpellItemEnchantment.Load("./original/SpellItemEnchantment.dbc");
    if(!DBCSpellItemEnchantment.getNumFields()){
        std::cout << "ERROR: Can not open file: " << "./original/SpellItemEnchantment.dbc" << std::endl;
        return false;
    }else{
        std::cout << "./original/SpellItemEnchantment.dbc" << " - Opened successful." << std::endl << "./original/SpellItemEnchantment.dbc" << " - fields: "
        << DBCSpellItemEnchantment.getNumFields() << ", rows: " << DBCSpellItemEnchantment.getNumRows() << std::endl;
    }
    for (int i = 0; i < DBCSpellItemEnchantment.recordCount; i++){
        auto record = DBCSpellItemEnchantment.getRecord(i);
        // int spellid = record.getInt32(0);
        // const char* spellNameGetet = record.getString(136);
        // std::string spellName(spellNameGetet);
        // uint32 spellVisualID = record.getUInt32(30);

        // if (ItemDisplayInfoChange.count(spellVisualID)){
            // std::cout<< "spellid->("<< spellid <<") spellname->("<< spellName <<") SpellChange->("<< spellVisualID<<") changed SpellChange to " << SpellChange[spellVisualID]<< std::endl;
            // std::cout<< "{"<<SpellChange << "," << SpellChange[SpellChange] << "} changed to 0" << std::endl;
        record.setUInt32(31,0);
        // }
    }

    FILE* npf = fopen("./buildedDBC/SpellItemEnchantment.dbc","wb");

    fwrite(&DBCSpellItemEnchantment.header,4,1,npf);

    fwrite(&DBCSpellItemEnchantment.recordCount,4,1,npf);

    fwrite(&DBCSpellItemEnchantment.fieldCount,4,1,npf);

    fwrite(&DBCSpellItemEnchantment.recordSize,4,1,npf);

    fwrite(&DBCSpellItemEnchantment.stringSize,4,1,npf);
    fwrite(DBCSpellItemEnchantment.data, DBCSpellItemEnchantment.recordSize * DBCSpellItemEnchantment.recordCount + DBCSpellItemEnchantment.stringSize, 1,npf);

    fwrite(DBCSpellItemEnchantment.stringTable,DBCSpellItemEnchantment.stringSize,0,npf);
    fclose(npf);

    std::cout << "./original/SpellItemEnchantment.dbc" << " -> SpellItemEnchantment.dbc: OK." << "\n\n";
    std::cout << "*****************************************************************************\n\n\n";
    return true;
}