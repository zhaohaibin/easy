#include "controller.h"
#include "response.h"
#include "http_header.h"
#include "easy\base\misc\string_helper\string_helper.h"
#include "civetweb.h"
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <vector>

controller::controller(const string& uri, http_method method) : m_uri(uri)
, m_http_method(method)
{
}

controller::~controller()
{
}

int controller::handler(struct mg_connection *conn)
{
	response rp(conn, "content-type application/json");
	rp.set_data(string_helper::wstring_to_utf8(L"{\"name\":\"赵海彬\"}"));
	rp.perform();
	return 0;
}

std::string controller::get_uri()
{
	return m_uri;
}

http_method controller::get_http_method()
{
	return m_http_method;
}

std::string controller::get_query_string(struct mg_connection *conn)
{
	const mg_request_info* p_mg_request_info = mg_get_request_info(conn);
	const char *p_query_string = p_mg_request_info->query_string;
	if(p_query_string != nullptr) return string(p_query_string);
	return "";
}

std::map<std::string, string> controller::get_query_params(struct mg_connection *conn)
{
	map<string, string> query_params;
	const mg_request_info* p_mg_request_info = mg_get_request_info(conn);
	const char *p_query_string = p_mg_request_info->query_string;
	if (p_query_string != nullptr)
	{
		string str;
		str.append(p_query_string);
		vector<string> name_value_pair;
		boost::split(name_value_pair, str, boost::is_any_of("&"));
		for (size_t i = 0; i < name_value_pair.size(); ++i)
		{
			vector<string> name_value;
			boost::split(name_value, name_value_pair[i], boost::is_any_of("="));

			if (name_value.size() == 2)
				query_params.insert(std::make_pair(name_value[0], name_value[1]));
			else if (name_value.size() == 1)
				query_params.insert(std::make_pair(name_value[0], ""));
		}
	}
	return query_params;
}

std::map<std::string, std::string> controller::get_form_data(struct mg_connection *conn)
{
	std::map<std::string, std::string> form_data;
	const mg_request_info* p_mg_request_info = mg_get_request_info(conn);
	char*  p_buffer = new char[(size_t)p_mg_request_info->content_length];
	int ret = mg_read(conn, p_buffer, (size_t)p_mg_request_info->content_length);
	string data(p_buffer);
	delete []p_buffer;
	int64_t index = 0;
	while (true)
	{
		string key, val;
		index = get_next_form_data(data, (size_t)index, key, val);
		if (index > 0)
			form_data.insert(std::make_pair(key, val));
		else
			break;
	}
	return form_data;
}

std::map<std::string, std::string> controller::get_urlencoded_form_data(struct mg_connection *conn)
{
	std::map<std::string, std::string> form_data;
	const mg_request_info* p_mg_request_info = mg_get_request_info(conn);
	char*  p_buffer = new char[(size_t)p_mg_request_info->content_length];
	int ret = mg_read(conn, p_buffer, (size_t)p_mg_request_info->content_length);
	char* p_decode_buffer = new char[(size_t)p_mg_request_info->content_length];
	ret = mg_url_decode(p_buffer, ret, p_decode_buffer, (size_t)p_mg_request_info->content_length, 1);
	string data(p_decode_buffer);
	delete []p_buffer;
	delete []p_decode_buffer;
	int64_t index = 0;
	while (true)
	{
		string key, val;
		index = get_next_urlencoded_form_data(data, (size_t)index, key, val);
		if (index > 0)
			form_data.insert(std::make_pair(key, val));
		else
			break;
	}
	return form_data;
}

int64_t controller::get_next_form_data(
	const string& data, size_t pos,
	std::string& key, std::string& val)
{
	size_t index = pos;

	// step = 0 查找key
	// step = 1 查找value
	// step = 2 结束
	int step = 0;
	while (index < data.size())
	{
		if (step == 0)
		{
			if (index < data.size() - 1 && data[index] == '=' && data[index+1] == '\"')
			{
				++index;
				while (++index < data.size())
				{
					if (data[index] == '\"' && data[index + 1] == '\r' && data[index + 2] == '\n')
					{
						step = 1;
						index += 4;
						break;
					}
					key.push_back(data[index]);
				}
			}
			else
			{
				++index;
			}
		}else if (step == 1)
		{
			while (++index < data.size())
			{
				if (data[index] == '\r' && data[index+1] == '\n')
				{
					step = 2;
					index += 2;
					break;
				}
				val.push_back(data[index]);
			}
		}
		else if(step == 2)
		{
			break;
		}
	}
	if (step == 2) return index;
	return -1;
}

int64_t controller::get_next_urlencoded_form_data(
	const string& data, size_t pos,
	std::string& key, std::string& val)
{
	size_t index = pos;

	// step = 0 查找key
	// step = 1 查找value
	// step = 2 结束
	int step = 0;

	while (index < data.size())
	{
		if (step == 0)
		{
			if(index < data.size() && data[index] == '=')
			{
				step = 1;
			}
			else
			{
				key.push_back(data[index]);
			}
			++index;
		}
		else if (step == 1)
		{
			if ( (index < data.size() && data[index] == '&') || (index+1 == data.size()))
			{
				step = 2;
			}else
			{
				val.push_back(data[index]);
			}
			++index;
		}
		else if (step == 2)
		{
			break;
		}
	}
	if (step == 2) return index;
	return -1;
}