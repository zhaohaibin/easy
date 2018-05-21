//
//  http_request.cpp
//  KingFile
//
//  Created by ks on 15/12/28.
//  Copyright (c) 2015年 ks. All rights reserved.
//

#include "http_request.h"
#include "openssl/ssl.h"



http_base_request::http_base_request(const std::string& url)
: m_send_max_speed(0)
, m_recv_max_seppd(0)
, m_time_out(20)
, m_url(url)
, m_p_header(NULL)
, m_enable_https(false)
{
    
}

int http_base_request::perform()
{
    int re = 0;
    re = init_curl();
    if(re != 0)
        return re;
    
    set_curl_progress_callback();
    set_curl_url();
    set_curl_max_speed();
    set_curl_time_out();
    set_curl_recv_data_function();
    set_curl_post_data();
    set_custom_opt();
    set_curl_protocols();
    set_curl_header();
    re = do_perform();
    
    release_curl();
    return re;
}

std::string http_base_request::get_result()
{
    return m_recv_data;
}

void http_base_request::set_header( const char* p_headers )
{
    const char* pos1 = NULL;
    const char* pos2 = NULL;
    pos1 = p_headers;
    while (true)
    {
        pos2 = strchr(pos1, '\n');
        if (!pos2)
        {
            m_p_header = curl_slist_append(m_p_header, pos1);
            break;
        }
        
        m_p_header = curl_slist_append(m_p_header, std::string(pos1, (int)(pos2 - pos1)).c_str());
        
        pos1 = pos2 + 1;
    }
}

void http_base_request::set_send_max_speed(int max_speed)
{
    m_send_max_speed = max_speed;
}

void http_base_request::set_recv_max_speed(int max_speed)
{
    m_recv_max_seppd = max_speed;
}

void http_base_request::set_time_out(int time_out)
{
    m_time_out = time_out;
}
void http_base_request::enable_https(bool b_eanble)
{
    m_enable_https = b_eanble;
}
void http_base_request::set_pem_path(const std::string& cert_path)
{
    m_cert_path = cert_path;
}
void http_base_request::set_progress_cmd(const std::function<void(double, double, double, double, double)>& cmd)
{
    m_progress_cmd = cmd;
}

void http_base_request::set_cancle_check_cmd(const std::function<bool (void)>& cmd)
{
    m_cancle_check_cmd = cmd;
}

int http_base_request::progress_callback(void *clientp,   double dltotal,   double dlnow,   double ultotal,   double ulnow)
{
    http_base_request* p_request = (http_base_request*)clientp;
    if(p_request->is_cancle())
        return -1;
//    curl_easy_getinfo(p_request->m_p_curl, CURLINFO_SPEED_UPLOAD,&up_speed);
//    curl_easy_getinfo(p_request->m_p_curl, CURLINFO_SPEED_DOWNLOAD,&down_speed);
    
    p_request->update_progress(dltotal, dlnow, ultotal,ulnow);
    return 0;
}

size_t http_base_request::write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    std::string* p_str_stream = (std::string*)userdata;
    try
    {
        p_str_stream->append(static_cast<const char*>(ptr), size*nmemb);
    }
    catch (...)
    {
        return 0;
    }
    return size*nmemb;
    
}

void http_base_request::set_custom_opt()
{
    
}
void http_base_request::set_curl_recv_data_function()
{
    ::curl_easy_setopt(m_p_curl, CURLOPT_WRITEFUNCTION, http_base_request::write_callback);
    ::curl_easy_setopt(m_p_curl, CURLOPT_WRITEDATA, &m_recv_data);
}
void http_base_request::set_https_protocol()
{
    //curl_easy_setopt(m_p_curl, CURLOPT_CAINFO, m_cert_path.c_str());
    if (m_cert.empty())
    {
        // 不验证证书
        curl_easy_setopt(m_p_curl, CURLOPT_SSL_VERIFYPEER, 0L);  

		//允许所有协议
        curl_easy_setopt(m_p_curl, CURLOPT_PROTOCOLS, CURLPROTO_ALL); // 此处放开允许的数据流协议。        
    }
    else
    {
        curl_easy_setopt(m_p_curl,CURLOPT_SSLCERTTYPE,"PEM");
        curl_easy_setopt(m_p_curl, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(m_p_curl, CURLOPT_SSL_CTX_DATA, m_cert.c_str());
        curl_easy_setopt(m_p_curl, CURLOPT_SSL_CTX_FUNCTION,http_base_request::sslctx_function);
        ::curl_easy_setopt(m_p_curl, CURLOPT_PROTOCOLS, CURLPROTO_HTTPS);
    }    
}

void http_base_request::set_curl_post_data()
{
    if(m_post_data.empty() == false)
    {
        ::curl_easy_setopt(m_p_curl, CURLOPT_POSTFIELDS, m_post_data.c_str());
    }
}

int http_base_request::init_curl()
{
    m_p_curl = ::curl_easy_init();
    if(m_p_curl == 0)
        return -1;
    return 0;
}
void http_base_request::release_curl()
{
    if(m_p_header)
        ::curl_slist_free_all(m_p_header);
    if(m_p_curl)
        ::curl_easy_cleanup(m_p_curl);
    
    m_p_header = 0;
    m_p_curl = 0;
}

void http_base_request::set_curl_progress_callback()
{
    ::curl_easy_setopt(m_p_curl, CURLOPT_NOPROGRESS, 0L);
    ::curl_easy_setopt(m_p_curl, CURLOPT_PROGRESSFUNCTION, http_base_request::progress_callback);
    ::curl_easy_setopt(m_p_curl, CURLOPT_PROGRESSDATA, this);
}
void http_base_request::set_curl_url()
{
    ::curl_easy_setopt(m_p_curl, CURLOPT_URL, m_url.c_str());
}
void http_base_request::set_curl_header()
{
	m_p_header = curl_slist_append(m_p_header, "Content-Type:application/json");
    if(m_p_header)
    {
        ::curl_easy_setopt(m_p_curl, CURLOPT_HTTPHEADER, m_p_header);
    }

}
void http_base_request::set_curl_max_speed()
{
    if(m_send_max_speed != 0)
        ::curl_easy_setopt(m_p_curl, CURLOPT_MAX_SEND_SPEED_LARGE, (curl_off_t)(m_send_max_speed*1024));
    if(m_recv_max_seppd != 0)
        ::curl_easy_setopt(m_p_curl, CURLOPT_MAX_RECV_SPEED_LARGE, (curl_off_t)(m_recv_max_seppd*1024));
}
void http_base_request::set_curl_time_out()
{
    ::curl_easy_setopt(m_p_curl, CURLOPT_TIMEOUT_MS, m_time_out*1000);
}
void http_base_request::set_curl_protocols()
{
    if(m_enable_https)
        set_https_protocol();
}
bool http_base_request::is_cancle()
{
    if(m_cancle_check_cmd)
        return m_cancle_check_cmd();
    return false;
}

void http_base_request::update_progress(double tf_dtotal, double tf_dnow, double tf_utotal, double tf_unow)
{
    if(m_progress_cmd)
        m_progress_cmd(tf_dtotal, tf_dnow, tf_utotal, tf_unow, 0);
}

int http_base_request::do_perform()
{
    int re = 0;
    CURLcode	curl_re = ::curl_easy_perform(m_p_curl);
	if(curl_re == CURLE_OK)
	{
		long httpCode = 0;
		curl_re = ::curl_easy_getinfo(m_p_curl, CURLINFO_RESPONSE_CODE, &httpCode);
		if(curl_re != CURLE_OK)
		{
			re = curl_re;
		}else if( 200 != httpCode )
		{
			re = curl_re;
		}
	}else
	{
		re = curl_re;
	}
    return curl_re;
}

CURLcode http_base_request::sslctx_function( CURL * curl, void * sslctx, void * parm )
{
	X509_STORE * store;
		X509 * cert=NULL;
		BIO * bio;
		CURLcode ret = CURLE_OK;
		/* get a BIO */ 
		bio=BIO_new_mem_buf((char*)parm, -1);
		/* use it to read the PEM formatted certificate from memory into an X509
		* structure that SSL can use
		*/ 
		if(bio != NULL)
			PEM_read_bio_X509(bio, &cert, 0, NULL);
		else
			ret = CURLE_SSL_CACERT_BADFILE;
		if(cert != NULL)
		{
			/* get a pointer to the X509 certificate store (which may be empty!) */ 
			store=SSL_CTX_get_cert_store((SSL_CTX *)sslctx);
			/* add our certificate to this store */ 
			if(X509_STORE_add_cert(store, cert)==0)
			{
				ret = CURLE_SSL_CACERT_BADFILE;
			}
		}else
		{
			ret = CURLE_SSL_CACERT_BADFILE;
		}

		/* decrease reference counts */ 
		if(cert != NULL)
			X509_free(cert);
		if(bio != NULL)
			BIO_free(bio);

		/* all set to go */ 
		return ret;
}

void http_base_request::set_cert( const std::string& cert )
{
	m_cert = cert;
}

http_get_request::http_get_request(const std::string& url)
: http_base_request(url)
{
}


http_post_request::http_post_request(const std::string& url,
                                     const std::string& post_data )
: http_base_request(url)
{
    set_post_data(post_data);
}

http_post_request::~http_post_request()
{
    
}

void http_post_request::set_post_data(const std::string& post_data)
{
    m_post_data = post_data;
}
