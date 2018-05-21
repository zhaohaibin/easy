#include "response.h"

response::response(struct mg_connection *conn, const string& mime_type)
	: m_p_mg_connection(conn)
	, m_mime_type(mime_type)
{
}

response::~response()
{
}

void response::set_data(const string& data)
{
	m_data = data;
}

int response::perform()
{
	int ret = mg_send_http_ok(m_p_mg_connection, m_mime_type.c_str(), m_data.size());
	if (ret < 0) return ret;
	ret = mg_write(m_p_mg_connection, m_data.c_str(), m_data.size());
	return ret;
}