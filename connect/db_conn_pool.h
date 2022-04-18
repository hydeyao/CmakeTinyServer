#ifndef DB_CONN_POOL_H
#define DB_CONN_POOL_H

#pragma once
#include <mysql/mysql.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <list>
#include <pthread.h>
#include <iostream>
#include "../lock/locker.h"

using namespace std;
class db_conn_pool
{
public:
    //get database connetion
    MYSQL *getConnection();
    //release connection
    bool releaseConnection(MYSQL *conn);
    //get connecton if conn pool free
    int getFreeConnection();
    //destroy all the connections 
    void destroyPool();

    //singleton
    static db_conn_pool * GetInstence();

    void init(string url,string user,string pwd,string db_name,int port,int max_conn,int close_log);
private:
    db_conn_pool();
    ~db_conn_pool();

    //Max Connection Count
    int m_MaxConn;
    //Curent Connection Count
    int m_CurConn;
    //Current Free Connection
    int m_FreeConn;

    locker m_lock;
    list<MYSQL *> connList;
    sem reserve;

public:
    string m_url;
    string m_port;
    string m_user;
    string m_passwd;
    string m_dbName;
    int m_close_log;
};

class connectionRAII
{
public:
    connectionRAII(MYSQL **conn,db_conn_pool *connPool);
    ~connectionRAII();
private:
    MYSQL *connRAII;
    db_conn_pool *poolRAII;
};



#endif