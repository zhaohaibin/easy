#ifndef _GET_VALUE_H
#define _GET_VALUE_H
#include "json/value.h"
#include <string>
using namespace std;

template<typename T>
class get_value
{};

template<>
class get_value<string>
{
public:
	string operator()(const Json::Value& jobj, const string& key) 
	{
		return jobj[key].asString();
	}
};

template<>
class get_value<int>
{
public:
	int operator()(const Json::Value& jobj, const string& key)
	{
		return jobj[key].asInt();
	}
};

template<>
class get_value<int64_t>
{
public:
	int64_t operator()(const Json::Value& jobj, const string& key)
	{
		return jobj[key].asInt64();
	}
};

template<>
class get_value<uint64_t>
{
public:
	uint64_t operator()(const Json::Value& jobj, const string& key)
	{
		return jobj[key].asUInt64();
	}
};

#endif //_GET_VALUE_H
