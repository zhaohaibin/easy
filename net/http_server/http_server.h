#ifndef _HTTP_SERVER_H
#define _HTTP_SERVER_H
#include <string>
#include <civetweb.h>
#include "controller\controller.h"
#include <map>
using namespace std;


#define MAX_OPTIONS (50)

class http_server
{
public:

	// 启动http 服务
	void start(const string& port);

	//注册 controller
	// 成功返回 true, 失败返回false
	// 重复注册会导致注册失败
	bool register_request_handler(controller_ptr sp_controller);

private:
	void set_option(const string& name, const string& value);

	//检查是否是一个合法的controller
	// return true 合法否则返回 false
	bool check_controller(controller_ptr sp_controller);

	//将controller注册到http server中
	void do_register_request_handler(controller_ptr sp_controller);
	
private:
	char *m_options[2 * MAX_OPTIONS + 1];
	mg_callbacks m_callbacks;
	mg_context* m_p_mg_context;

	//保存注册的controller
	map<string, controller_ptr> m_registed_controllers;
};
#endif //_HTTP_SERVER_H
