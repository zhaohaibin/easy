/********************************************************************
created:	2015/09/24
created:	24:9:2015   14:16
filename: 	D:\KSYUN\new_client\pclite\src\include\db\db_base.h
file path:	D:\KSYUN\new_client\pclite\src\include\db
file base:	db_base
file ext:	h
author:		zhaohaibin

purpose:	
*********************************************************************/
#ifndef DB_BASE_H
#define DB_BASE_H

#include <string>
#include <cstdint>
#include <sqlite/sqlite3.h>
#include "sqlit_stmt_warpper.h"

class sqlit_warpper
{
public:
	sqlit_warpper();
	~sqlit_warpper();

public:
	std::string error_msg();
	//db_path need utf8 encode
    int initialize_db(const std::string& db_path);

	//����ִ�� sql���(����������ɾ����)
	int exec_sql(const std::string& sql);
	int exec_sql(const char* sql);

	////////////////////////////////////////////////////////
	//step_1 sqlit_prepare
	//bind---������Ҫѡ��
	//step_2 perform
	int sqlit_prepare(const char* sql, sqlit_stmt_wrapper& stmt);
	int perform(sqlit_stmt_wrapper& stmt);

	//////////////////////////////////////////////////������ؽӿ�////////////////////////////////////////
	//step_1	begin_transaction
	//step_2	�������ݿ����
	//step_3	û�д���commit_transaction����rollback_transaction
	//��ʼһ������
	int begin_transaction();

	//�ύ����
	int commit_transaction();

	//�ع�����
	int rollback_transaction();

	////////////////////////////////////////////////////��ѯ��ؽӿ�
	int exec_query(const std::string& sql, sqlit_stmt_wrapper& stmt);
	int exec_query(const char* sql, sqlit_stmt_wrapper& stmt);
private:
	sqlite3* m_p_sqlite;
};
#endif