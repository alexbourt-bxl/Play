//#include "FileFormat_Data.h"
//
//#include <Types/Basic\Data.h>
//
//FileFormat_Data::FileFormat_Data()
//{
//	Name = L"All Files";
//
//	Extensions.Add(L"*");
//}
//
//Type FileFormat_Data::GetObjectType() const
//{
//	return TYPEOF(Data);
//}
//
//Object* FileFormat_Data::LoadFile(const String& filename, void* /*params*/)
//{
//	FILE* file;
//
//	if ((_wfopen_s(&file, filename, L"rb")) != 0)
//		return null;
//
//	fseek(file, 0, SEEK_END);
//	int bufferSize = ftell(file);
//	rewind(file);
//
//	Data* data = new Data(bufferSize);
//
//	fread(
//		data->Buffer, 
//		data->Size,
//		1, 
//		file);
//
//	fclose(file);
//
//	return data;
//}
//
//bool FileFormat_Data::SaveFile(Object* obj, const String& filename, void* params)
//{
//	assert(typeof(obj) == TYPEOF(Data));
//	Data* data = (Data*)obj;
//
//	FILE* file;
//
//	if ((_wfopen_s(&file, filename, L"wb")) != 0)
//		return false;
//
//	fwrite(
//		data->Buffer,
//		data->Size, 
//		1, 
//		file);
//
//	fclose(file);
//	
//	return true;
//}