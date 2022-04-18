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
    if (NULL == con)
    {
        return false;
    }

    m_lock.lock();
    connList.push_back(con);
    ++m_FreeConn;
    --m_CurConn;

    m_lock.unlock();
    reserve.post();
    return true;
}

void db_conn_pool::destroyPool()
{
    m_lock.lock();
    if (connList.size()>0)
    {
        list<MYSQL *>::iterator iter;
        for (iter = connList.begin();iter != connList.end(); iter++)
        {
            MYSQL *conn = *iter;
            mysql_close(conn);
        }
        m_CurConn = 0;
        m_FreeConn = 0;        
    }
    m_lock.unlock();
}

int db_conn_pool::getFreeConnection()
{
    return this->m_FreeConn;
}

connectionRAII::connectionRAII(MYSQL **conn,db_conn_pool *connPool)
{
    connRAII = *conn;
    poolRAII = connPool;
}

connectionRAII::~connectionRAII()
{
    poolRAII->releaseConnection(connRAII);
}