#include "db_conn_pool.h"

db_conn_pool::db_conn_pool()
{
    m_CurConn = 0;
    m_FreeConn = 0;
}

db_conn_pool::~db_conn_pool()
{
}

db_conn_pool *db_conn_pool::GetInstence()
{
    static db_conn_pool conn_pool;
    return &conn_pool;
}

void db_conn_pool::init(string url, string user, string pwd, string db_name, int port, int max_conn, int close_log)
{
    m_url = url;
    m_user = user;
    m_passwd = pwd;
    m_dbName = db_name;
    m_close_log = close_log;

    for (size_t i = 0; i < max_conn; i++)
    {
        MYSQL *conn = NULL;
        conn = mysql_init(conn);

        if (!conn)
        {
            // LOG
            exit(1);
        }
        conn = mysql_real_connect(conn, url.c_str(), user.c_str(), pwd.c_str(), db_name.c_str(), port, NULL, 0);
        if (!conn)
        {
            // LOG
            exit(1);
        }
        connList.push_back(conn);
        ++m_FreeConn;
    }

    reserve = sem(m_FreeConn);
    m_MaxConn = m_FreeConn;
}

MYSQL *db_conn_pool::getConnection()
{
    MYSQL *conn = NULL;
    if (0 == connList.size())
    {
        return NULL;
    }

    reserve.wait();
    m_lock.lock();

    conn = connList.front();
    connList.pop_front();

    --m_FreeConn;
    ++m_CurConn;

    m_lock.unlock();
    return conn;
}

bool db_conn_pool::releaseConnection(MYSQL * con)
{
    
    
    
    
    

}