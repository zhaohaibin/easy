#pragma once

#include <string>
using namespace std;

class random_uuid
{
public:
	random_uuid();
	~random_uuid();

public:
	static string generate();
	static wstring wgenerate_t();
};

