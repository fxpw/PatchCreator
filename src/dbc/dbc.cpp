/*
 * DBC to SQL Converter
 * Copyright (C) 2009  David Vas, Anubisss
 * <http://code.google.com/p/dbctosql/>

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <cstdio>
#include "dbc/dbc.hpp"
#include <iostream>

DBCFileLoader::DBCFileLoader(){
    data = NULL;
    stringTable = NULL;
}

DBCFileLoader::~DBCFileLoader(){
    if(data)
       delete [] data;
}

bool DBCFileLoader::Load(char const *filename){
    FILE* pf = fopen(filename, "rb");

    if(!pf)
        return false;
    // uint32 header;

    if(fread(&header, 4, 1, pf) != 1) // первые 4 это хедер
        return false;
    if(header != WDBC_HEADER)
        return false;

    if(fread(&recordCount, 4, 1, pf) != 1) // 2 Number of records
        return false;
    std::cout<<recordCount<<" recordCount \n";
    if(fread(&fieldCount, 4, 1, pf) != 1) // 3 Number of fields
        return false;
    std::cout<<fieldCount<<" fieldCount \n";
    if(fread(&recordSize, 4, 1, pf) != 1) // 4 Size of a record
        return false;
    std::cout<<recordSize<<" recordSize \n";
    if(fread(&stringSize, 4, 1, pf) != 1) // 5 String size это сколько выделяется на стрингу
        return false;
    std::cout<<stringSize<<" stringSize \n";
    std::cout<<recordSize * recordCount + stringSize<< " data size"<< "\n";
    data = new unsigned char[recordSize * recordCount + stringSize]; // data unsigned char*[]

    stringTable = data + recordSize * recordCount;


    if(!data || !stringTable)
        return false;
    if(fread(data, recordSize * recordCount + stringSize , 1, pf) != 1)
        return false;

    fread(stringTable, stringSize, 1, pf);

    fclose(pf);

    return true;
}

DBCFileLoader::Record DBCFileLoader::getRecord(size_t id){
    assert(data);
    return Record(*this, data + id * recordSize);
}
