#include "FileReader.h"

#include <jsoncons/json.hpp>
#include "BoidData.h"



FileReader::FileReader()
{

}



FileReader::~FileReader()
{

}



void FileReader::setData(BoidData* _boidData, std::string& _fileName)
{
	using jsoncons::json;
	std::ifstream
		is(_fileName);
	json boidData;
	is >> boidData;
	for (const auto& type : boidData.members())
	{
		const auto& data = type.value();

		_boidData->aliWeight     = data["aliWeight"].as_double();
		_boidData->cohWeight     = data["cohWeight"].as_double();
		_boidData->sepWeight     = data["sepWeight"].as_double();
		_boidData->runWeight     = data["runWeight"].as_double();
		_boidData->ffWeight      = data["ffWeight"].as_double();
		_boidData->pathWeight      = data["pathWeight"].as_double();
		_boidData->boidMaxSpeed  = data["boidMaxSpeed"].as_double();
		_boidData->boidMaxForce  = data["boidMaxForce"].as_double();
		_boidData->neighbourDist = data["neighbourDist"].as_double();
		_boidData->desiredSep    = data["desiredSep"].as_double();
	}
}