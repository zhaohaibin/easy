#include "random_uuid.h"
#include "boost/uuid/uuid.hpp"
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <sstream>
using namespace boost;

random_uuid::random_uuid()
{
}


random_uuid::~random_uuid()
{
}

std::string random_uuid::generate()
{
	uuids::random_generator rgen;
	uuids::uuid ran_uuid = rgen();
	stringstream ss;
	ss << ran_uuid;
	string struuid;
	ss >> struuid;
	return struuid;
}

std::wstring random_uuid::wgenerate_t()
{
	uuids::random_generator rgen;
	uuids::uuid ran_uuid = rgen();
	wstringstream ss;
	ss << ran_uuid;
	wstring struuid;
	ss >> struuid;
	return struuid;
}
