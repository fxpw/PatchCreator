#ifndef DBC_HPP
#define DBC_HPP

#include <StormLib.h>
#include "structures/AllStructuresInclude.hpp"

template <typename DBCFileType, typename DBCRecordType>
class DBC {
	public:
		DBCFileType file;
		std::string fileName;
		std::map<std::string, stringref> mapForReUse;
		// template <typename DBCFileType, typename DBCRecordType>
		inline DBC(std::string dirWhereFindDBC, std::string dbcFileName){
			fileName = dbcFileName;
			std::ifstream dbcFile(dirWhereFindDBC + fileName, std::ios::binary);
			if (!dbcFile.is_open())
			{
				//std::cerr << "Failed to open file: " << filename << std::endl;
				return;
			}
			// Read header
			dbcFile.read((char*)&file.header, sizeof(DBCHeader));
			// std::cout<<"----------------------"<<std::endl;
			// std::cout<<"filename:"<<dbcFileName<<std::endl;
			// std::cout<<"head:"<<file.header.head<<std::endl;
			// std::cout<<"recordCount:"<<file.header.recordCount<<std::endl;
			// std::cout<<"fieldCount:"<<file.header.fieldCount<<std::endl;
			// std::cout<<"recordSize:"<<file.header.recordSize<<std::endl;
			// std::cout<<"stringBlockSize:"<<file.header.stringBlockSize<<std::endl;
			file.records = new DBCRecordType[file.header.recordCount];
			// Read records
			for (uint32_t i = 0; i < file.header.recordCount; i++)
			{
				dbcFile.read(reinterpret_cast<char*>(&file.records[i]), sizeof(DBCRecordType));
			}
			// Allocate memory for string block
			file.stringBlock = new char[file.header.stringBlockSize];
			// Read string block
			dbcFile.read(file.stringBlock, file.header.stringBlockSize);
			dbcFile.close();
		};
		// template<typename DBCFileType, typename DBCRecordType>
		inline bool CreateNewFileIn(std::string newDir){
			std::ofstream outputFile(newDir + fileName, std::ios::binary);
			outputFile.write(reinterpret_cast<const char*>(&file.header), sizeof(DBCHeader));
			outputFile.write(reinterpret_cast<const char*>(file.records), file.header.recordCount * sizeof(DBCRecordType));
			outputFile.write(file.stringBlock, file.header.stringBlockSize);
			outputFile.close();
			return true;
		};
		// template <typename DBCFileType, typename DBCRecordType>
		inline ~DBC(){
		};
	private:
};

#endif


