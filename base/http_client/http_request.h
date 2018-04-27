#ifndef _HTTP_REQUEST_H
#define _HTTP_REQUEST_H

#include <stdio.h>
#include <string>
#include <functional>
#include <curl/curl.h>

class http_base_request
{
public:
	http_base_request(const std::string& url);
	virtual ~http_base_request() {}

	int perform();
	std::string get_result();

	void set_header(const char* p_headers);
	void set_send_max_speed(int max_speed);
	void set_recv_max_speed(int max_speed);
	void set_time_out(int time_out);

	//启用或禁用https协议
	void enable_https(bool b_eanble);

	//设置https证书路径
	void set_pem_path(const std::string& cert_path);

	//设置https证书
	void set_cert(const std::string& cert);

	//设置进度回调
	void set_progress_cmd(const std::function<void(double, double, double, double, double)>& cmd);

	//设置一个function，用于检查请求是否取消
	// cmd()返回true请求取消
	void set_cancle_check_cmd(const std::function<bool(void)>& cmd);

protected:
	static int progress_callback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow);
	static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata);
	static CURLcode sslctx_function(CURL * curl, void * sslctx, void * parm);

	virtual void set_custom_opt();
	virtual void set_curl_recv_data_function();

	// 设置https协议
	// 如果证书为空，则不验证证书
	virtual void set_https_protocol();

	virtual void set_curl_post_data();

private:
	int init_curl();
	void release_curl();

	void set_curl_progress_callback();
	void set_curl_url();
	void set_curl_header();
	void set_curl_max_speed();
	void set_curl_time_out();
	void set_curl_protocols();
	bool is_cancle();
	void update_progress(double tf_dtotal, double tf_dnow, double tf_utotal, double tf_unow);
	int do_perform();
protected:
	std::string		m_recv_data;
	std::string     m_post_data;
	std::string		m_url;
	std::string		m_cert;
	CURL*			m_p_curl;
	curl_slist*		m_p_header;
	int				m_time_out;
	int				m_send_max_speed;
	int				m_recv_max_seppd;
	std::string		m_cert_path;
	bool			m_enable_https;
	std::function<bool(void)>		m_cancle_check_cmd;
	std::function<void(double, double, double, double, double)> m_progress_cmd;
};

////////////////////////////////////////////////////////////////////////////////
class http_get_request : public http_base_request
{
public:
	http_get_request(const std::string& url);

};

/////////////////////////////////////////////////////////////////////////////////
class http_post_request : public http_base_request
{
public:
	http_post_request(const std::string& url, const std::string& post_data);
	virtual ~http_post_request();

	void set_post_data(const std::string& post_data);
};

#endif //_HTTP_REQUEST_H


















