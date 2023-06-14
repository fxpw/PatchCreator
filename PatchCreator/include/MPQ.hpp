#ifndef MPQ_HPP
#define MPQ_HPP

#include "DBC.hpp"
	class MPQ{
		private:
		static TCHAR* ConverterToTCHAR(const char* orig){
			// TCHAR* tchar = 0;
			std::string str = std::string(orig);
			TCHAR* tchar = new TCHAR[str.size() + 1];
			copy(str.begin(), str.end(), tchar);
			tchar[str.size()] = 0;
			return tchar;
		};
		public:
		static bool ExtractDBCFromMPQ(std::string pathToPatch){
			HANDLE mpq;
			bool isSuccess = SFileOpenArchive(ConverterToTCHAR(pathToPatch.c_str()), 0, STREAM_FLAG_WRITE_SHARE, &mpq);
			if (!isSuccess) return false;
			if (!mpq) {
				return false;
			};
			if (!SFileExtractFile(mpq, "DBFilesClient\\Spell.dbc", ConverterToTCHAR("./Spell.dbc"), SFILE_OPEN_FROM_MPQ)) return false;
			if (!SFileExtractFile(mpq, "DBFilesClient\\ItemDisplayInfo.dbc", ConverterToTCHAR("./ItemDisplayInfo.dbc"), SFILE_OPEN_FROM_MPQ)) return false;
			if (!SFileExtractFile(mpq, "DBFilesClient\\SpellItemEnchantment.dbc", ConverterToTCHAR("./SpellItemEnchantment.dbc"), SFILE_OPEN_FROM_MPQ)) return false;
			if (!SFileExtractFile(mpq, "DBFilesClient\\SpellVisualKit.dbc", ConverterToTCHAR("./SpellVisualKit.dbc"), SFILE_OPEN_FROM_MPQ)) return false;
			if (!SFileExtractFile(mpq, "DBFilesClient\\SpellVisualEffectName.dbc", ConverterToTCHAR("./SpellVisualEffectName.dbc"), SFILE_OPEN_FROM_MPQ)) return false;
			SFileCloseArchive(mpq);
			return true;
		};
		static bool CreateMPQWhitDBC(std::string newPatchDirAndName = "error"){
			try {
				HANDLE mpq;
				remove(newPatchDirAndName.c_str());
				bool isSucsess = SFileCreateArchive(ConverterToTCHAR(newPatchDirAndName.c_str()), MPQ_CREATE_ATTRIBUTES + MPQ_CREATE_ARCHIVE_V2, 0x000000010, &mpq);
				if (!isSucsess){
					remove("./Spell.dbc");
					remove("./ItemDisplayInfo.dbc");
					remove("./SpellItemEnchantment.dbc");
					remove("./SpellVisualKit.dbc");
					remove("./SpellVisualEffectName.dbc");
					return false;
				};
				SFileAddFileEx(mpq, ConverterToTCHAR("./Spell.dbc"), "DBFilesClient\\Spell.dbc", MPQ_FILE_COMPRESS + MPQ_FILE_REPLACEEXISTING, MPQ_COMPRESSION_ZLIB, MPQ_COMPRESSION_NEXT_SAME);
				remove("./Spell.dbc");
				SFileAddFileEx(mpq, ConverterToTCHAR("./ItemDisplayInfo.dbc"), "DBFilesClient\\ItemDisplayInfo.dbc", MPQ_FILE_COMPRESS + MPQ_FILE_REPLACEEXISTING, MPQ_COMPRESSION_ZLIB, MPQ_COMPRESSION_NEXT_SAME);
				remove("./ItemDisplayInfo.dbc");
				SFileAddFileEx(mpq, ConverterToTCHAR("./SpellItemEnchantment.dbc"), "DBFilesClient\\SpellItemEnchantment.dbc", MPQ_FILE_COMPRESS + MPQ_FILE_REPLACEEXISTING, MPQ_COMPRESSION_ZLIB, MPQ_COMPRESSION_NEXT_SAME);
				remove("./SpellItemEnchantment.dbc");
				SFileAddFileEx(mpq, ConverterToTCHAR("./SpellVisualKit.dbc"), "DBFilesClient\\SpellVisualKit.dbc", MPQ_FILE_COMPRESS + MPQ_FILE_REPLACEEXISTING, MPQ_COMPRESSION_ZLIB, MPQ_COMPRESSION_NEXT_SAME);
				remove("./SpellVisualKit.dbc");
				SFileAddFileEx(mpq, ConverterToTCHAR("./SpellVisualEffectName.dbc"), "DBFilesClient\\SpellVisualEffectName.dbc", MPQ_FILE_COMPRESS + MPQ_FILE_REPLACEEXISTING, MPQ_COMPRESSION_ZLIB, MPQ_COMPRESSION_NEXT_SAME);
				remove("./SpellVisualEffectName.dbc");
				return SFileCloseArchive(mpq);
			}catch (...){
				return false;
			};
		};
	};
#endif
