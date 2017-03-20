#pragma once

#include <string>

class BoidData;


class FileReader
{
public:
	FileReader();
	~FileReader();

	void setData(BoidData* _boidData, std::string& _fileName);

protected:

private:

	float aw;

};