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

#ifndef __DBC_H__
#define __DBC_H__


#define WDBC_HEADER     0x43424457

class DBCFileLoader
{
public:
    DBCFileLoader() {
        data = NULL;
        stringTable = NULL;
    };
    ~DBCFileLoader() {
        if (data)
            delete[] data;
        if (stringTable)
            delete[] stringTable;
    }

    bool Load(char const* filename) {
        FILE* pf;
        errno_t err = fopen_s(&pf,filename, "rb");

        if (!pf)
            return false;
        if (err != 0)
            return false;

        if (fread(&header, 4, 1, pf) != 1) // ������ 4 ��� �����
            return false;
        if (header != WDBC_HEADER)
            return false;

        if (fread(&recordCount, 4, 1, pf) != 1) // 2 Number of records
            return false;
        //std::cout << recordCount << " recordCount \n";
        if (fread(&fieldCount, 4, 1, pf) != 1) // 3 Number of fields
            return false;
        //std::cout << fieldCount << " fieldCount \n";
        if (fread(&recordSize, 4, 1, pf) != 1) // 4 Size of a record
            return false;
        //std::cout << recordSize << " recordSize \n";
        if (fread(&stringSize, 4, 1, pf) != 1) // 5 String size ��� ������� ���������� �� �������
            return false;
        //std::cout << stringSize << " stringSize \n";
        //std::cout << recordSize * recordCount + stringSize << " data size" << "\n";
        data = new unsigned char[recordSize * recordCount]; // data unsigned char*[]

        stringTable = new unsigned char[stringSize];


        if (!data || !stringTable)
            return false;
        if (fread(data, recordSize * recordCount, 1, pf) != 1)
            return false;
        if (fread(stringTable, stringSize, 1, pf) != 1)
            return false;

        fclose(pf);

        return true;
    };
    class Record
    {
    public:
        float getFloat(size_t field) const
        {
            assert(field < file.fieldCount);
            return *reinterpret_cast<float*>(offset + field * 4);
        }
        void setFloat(size_t field, float newFloat) const
        {
            assert(field < file.fieldCount);
            *reinterpret_cast<float*>(offset + field * 4) = newFloat;
        }
        //!**********************************************
        uint32 getUInt32(size_t field) const
        {
            assert(field < file.fieldCount);
            return *reinterpret_cast<uint32*>(offset + field * 4);
        }
        void setUInt32(size_t field, uint32 newUInt) const
        {
            assert(field < file.fieldCount);
            *reinterpret_cast<uint32*>(offset + field * 4) = newUInt;
        }
        //!**********************************************
        int32 getInt32(size_t field) const
        {
            assert(field < file.fieldCount);
            return *reinterpret_cast<int32*>(offset + field * 4);
        }
        void setInt32(size_t field, int32 newInt) const
        {
            assert(field < file.fieldCount);
            *reinterpret_cast<int32*>(offset + field * 4) = newInt;
        }
        //!**********************************************
        char const* getString(size_t field) const
        {
            assert(field < file.fieldCount);
            size_t stringOffset = getUInt32(field);
            assert(stringOffset < file.stringSize);
            return reinterpret_cast<char*>(file.stringTable + stringOffset);
        }
        //! if u change pointner then all string at same change too
        void setString(size_t field, const char* newString)
        {
            assert(field < file.fieldCount);
            assert(newString != nullptr); // make sure the new string is not null

            size_t stringOffset = getUInt32(field);
            assert(stringOffset < file.stringSize);

            char* stringTablePtr = reinterpret_cast<char*>(file.stringTable + stringOffset);
            strcpy(stringTablePtr, newString);
        }
        //!**********************************************
        void addString(size_t field, const char* newString)
        {
            size_t startOffset = file.stringSize;
            setUInt32(field, startOffset);

            file.stringSize += strlen(newString) + 1;
            file.stringTable = reinterpret_cast<unsigned char*>(realloc(file.stringTable, file.stringSize));

            char* stringTablePtr = reinterpret_cast<char*>(file.stringTable + startOffset);
            strcpy(stringTablePtr, newString);
        }

    private:
        Record(DBCFileLoader& file, unsigned char* offset) : file(file), offset(offset) {}
        DBCFileLoader& file;
        unsigned char* offset;


        friend class DBCFileLoader;
    };

    Record getRecord(size_t id) {
        assert(data);
        return Record(*this, data + id * recordSize);
    };
    uint32 getNumRows() const { return recordCount; }
    uint32 getNumFields() const { return fieldCount; }

    // private:
    uint32 header;
    uint32 recordSize;
    uint32 recordCount;
    uint32 fieldCount;
    uint32 stringSize;
    unsigned char* data;
    unsigned char* stringTable;
};

#endif
