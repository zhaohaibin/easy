#define USING_LOG
#include "sqlit_warpper.h"
#include "db_error.h"

#define sql_openmode						(SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_WAL | SQLITE_OPEN_FULLMUTEX)

sqlit_warpper::sqlit_warpper()
	:m_p_sqlite(0)
{

}

sqlit_warpper::~sqlit_warpper()
{
	if(m_p_sqlite)
		sqlite3_close(m_p_sqlite);
}

std::string sqlit_warpper::error_msg()
{
	return std::string(sqlite3_errmsg(m_p_sqlite));
}

//可能导致数据库损的原因
//http://www.sqlite.org/howtocorrupt.html
int sqlit_warpper::initialize_db(const std::string& db_path)
{
    int iRev =  sqlite3_open_v2(db_path.c_str(), &m_p_sqlite, sql_openmode, 0);
	return iRev;
// 	if (iRev!=SQLITE_OK) return iRev;
// 	return exec_sql("PRAGMA synchronous=OFF");// 关闭每个事务即与硬盘同步，否则插入性能会很慢。默认为FULL。
}

int sqlit_warpper::exec_sql( const std::string& sql )
{
	string exec_tag = "exec_sql:";
	exec_tag += sql;
	char * pszErrMsg = NULL;
	int iRev = sqlite3_exec(m_p_sqlite, sql.c_str(), 0, 0,&pszErrMsg);
	if (db_error::is_error(iRev) == true)
	{
		if(pszErrMsg != NULL) sqlite3_free(pszErrMsg);
	}
	return iRev;
}

int sqlit_warpper::exec_sql( const char* sql )
{
	string exec_tag = "exec_sql:";
	exec_tag += string(sql);
	char * pszErrMsg = NULL;
	int iRev = sqlite3_exec(m_p_sqlite, sql, 0, 0,&pszErrMsg);
	if (db_error::is_error(iRev) == true)
	{
		if(pszErrMsg != NULL) sqlite3_free(pszErrMsg);
	}
	return iRev;
}

int sqlit_warpper::sqlit_prepare( const char* sql, sqlit_stmt_wrapper& stmt )
{
	string exec_tag = "sqlit_prepare:";
	exec_tag += string(sql);
	int ret;
	ret = sqlite3_reset(stmt.sqlit_stmt());
	if(db_error::is_error(ret) == true)
		return ret;

	ret = sqlite3_prepare_v2( m_p_sqlite, sql, -1, &stmt.sqlit_stmt(), 0 );
	return ret;
}

int sqlit_warpper::perform( sqlit_stmt_wrapper& stmt )
{
	string exec_tag = "perform";
	int ret = sqlite3_step(stmt.sqlit_stmt());
	return ret;
}

int sqlit_warpper::begin_transaction()
{
	string exec_tag = "begin_transaction";
	int ret = sqlite3_exec ( m_p_sqlite , "begin transaction" , 0 , 0 ,0 );
	return ret;
}

int sqlit_warpper::commit_transaction()
{
	string exec_tag = "commit_transaction";
	int ret = sqlite3_exec ( m_p_sqlite , "commit transaction" , 0 , 0 , 0 );
	return ret;
}

int sqlit_warpper::rollback_transaction()
{
	string exec_tag = "rollback_transaction";
	int ret = sqlite3_exec ( m_p_sqlite , "rollback transaction" , 0 , 0 , 0 );
	return ret;
}

int sqlit_warpper::exec_query( const std::string& sql, sqlit_stmt_wrapper& stmt )
{
	string exec_tag = "exec_query:";
	exec_tag += sql;
	int ret;
	ret = sqlite3_reset(stmt.sqlit_stmt());
	if(db_error::is_error(ret) == true)
		return ret;
	ret = sqlite3_prepare_v2( m_p_sqlite, sql.c_str(), -1, &stmt.sqlit_stmt(), 0 );
	return ret;
}

int sqlit_warpper::exec_query( const char* sql, sqlit_stmt_wrapper& stmt )
{
	string exec_tag = "exec_query:";
	exec_tag += string(sql);
	int ret;
	ret = sqlite3_reset(stmt.sqlit_stmt());
	if(db_error::is_error(ret) == true)
		return ret;
	ret = sqlite3_prepare_v2( m_p_sqlite, sql, -1, &stmt.sqlit_stmt(), 0 );
	return ret;
}
