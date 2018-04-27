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

	// ����http ����
	void start(const string& port);

	//ע�� controller
	// �ɹ����� true, ʧ�ܷ���false
	// �ظ�ע��ᵼ��ע��ʧ��
	bool register_request_handler(controller_ptr sp_controller);

private:
	void set_option(const string& name, const string& value);

	//����Ƿ���һ���Ϸ���controller
	// return true �Ϸ����򷵻� false
	bool check_controller(controller_ptr sp_controller);

	//��controllerע�ᵽhttp server��
	void do_register_request_handler(controller_ptr sp_controller);
	
private:
	char *m_options[2 * MAX_OPTIONS + 1];
	mg_callbacks m_callbacks;
	mg_context* m_p_mg_context;

	//����ע���controller
	map<string, controller_ptr> m_registed_controllers;
};
#endif //_HTTP_SERVER_H
