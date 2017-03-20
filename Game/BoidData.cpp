#include "BoidData.h"

#include "FileReader.h"



BoidData::BoidData(std::unique_ptr<FileReader>& _fileReader, std::string& _fileName)
{
	// passes itself through to the filereader, sets variables from Json
	_fileReader->setData(this, _fileName);
}



BoidData::~BoidData()
{

}