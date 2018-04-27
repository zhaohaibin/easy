#ifndef _HTTP_CONTROLLER_H
#define _HTTP_CONTROLLER_H
#include <string>
#include <memory>
#include <map>
#include <boost\any.hpp>
#include "..\http_method\http_method.h"

using namespace std;

class controller
{
public:
	controller(const string& uri, http_method method);
	virtual ~controller();

	//响应用户的请求
	virtual int handler(struct mg_connection *conn);
public:
	string get_uri();
	http_method get_http_method();

protected:
	map<string, string> get_query_params(struct mg_connection *conn);
private:
	string m_uri;
	http_method m_http_method;
};

typedef std::shared_ptr<controller> controller_ptr;
#endif //_HTTP_CONTROLLER_H
