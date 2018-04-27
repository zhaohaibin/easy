#include "http_server.h"

struct user_data
{
	char *first_message;
};

char* sdup(const char* str)
{
	if (nullptr == str)
		return nullptr;
	size_t len;
	char *p;

	len = strlen(str) + 1;
	p = (char *)malloc(len);

	memcpy(p, str, len);
	return p;
}

user_data g_user_data;

//日志消息回调函数
int log_message(const struct mg_connection *conn, const char *message)
{
	const struct mg_context *ctx = mg_get_context(conn);
	user_data *ud = (user_data *)mg_get_user_data(ctx);

	fprintf(stderr, "%s\n", message);

	if (ud->first_message == NULL) {
		ud->first_message = sdup(message);
	}

	return 0;
}

//客户端请求处理函数
//conn 当前连接信心
//cbdata 回调参数，由用户在mg_set_request_handler调用时传入
int request_handler(struct mg_connection *conn, void *cbdata)
{
	controller* p_controller = (controller*)cbdata;
	return p_controller->handler(conn);
}

void http_server::start(const string& port)
{
	memset(m_options, 0, sizeof(m_options));
	set_option("document_root", ".");
	set_option("listening_ports", port);

	memset(&g_user_data, 0, sizeof(g_user_data));
	memset(&m_callbacks, 0, sizeof(m_callbacks));

	//添加log回调函数
	m_callbacks.log_message = &log_message;

	//启动服务
	m_p_mg_context = mg_start(&m_callbacks, &g_user_data, (const char **)m_options);

	//注册controll
	controller_ptr sp_controller = std::make_shared<controller>("/hello/word", http_method::GET);
	register_request_handler(sp_controller);
}

void http_server::set_option(const string& name, const string& value)
{
	const mg_option* pDefaultOptions = mg_get_valid_options();
	for (int i = 0; i < MAX_OPTIONS; ++i)
	{
		if (m_options[2 * i] == nullptr)
		{
			m_options[2 * i] = sdup(name.c_str());
			m_options[2 * i + 1] = sdup(value.c_str());
			m_options[2 * i + 2] = sdup(NULL);
			break;
		}
	}
}

bool http_server::check_controller(controller_ptr sp_controller)
{
	map<string, controller_ptr>::iterator it = m_registed_controllers.find(sp_controller->get_uri());
	return it == m_registed_controllers.end();
}

void http_server::do_register_request_handler(controller_ptr sp_controller)
{
	m_registed_controllers.insert(std::make_pair(sp_controller->get_uri(), sp_controller));
	mg_set_request_handler(
		m_p_mg_context, sp_controller->get_uri().c_str(),
		request_handler, sp_controller.get());
}

bool http_server::register_request_handler(controller_ptr sp_controller)
{
	if (check_controller(sp_controller))
	{
		do_register_request_handler(sp_controller);
		return true;
	}
	else
	{
		return false;
	}
}

