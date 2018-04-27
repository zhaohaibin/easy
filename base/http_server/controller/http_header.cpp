#include "http_header.h"

void http_header::add_header(const string& name, const string& value)
{
	m_headers.push_back(tuple<string, string>(name, value));
}

const std::vector<tuple<std::string, std::string>>& http_header::get_headers()
{
	return m_headers;
}

