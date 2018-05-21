#ifndef _HTTP_RESPONSE_H
#define _HTTP_RESPONSE_H
#include <string>
#include "civetweb.h"
using namespace std;

class response
{
public:
	response(struct mg_connection *conn, const string& mime_type);
	virtual ~response();

	void set_data(const string& data);

	virtual int perform();

protected:
	string m_mime_type;
	string m_data;
	mg_connection* m_p_mg_connection;
};

#endif //_HTTP_RESPONSE_H
