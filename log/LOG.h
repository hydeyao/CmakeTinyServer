#ifndef LOG_H
#define LOG_H

#pragma once
#include <stdio.h>
#include <iostream>
#include <string>
#include <stdarg.h>
#include <pthread.h>
#include "block_queue.h"

enum LOG_LEVEL{
    LEVEL_DEBUG = 0,
    LEVEL_INFO = 1,
    LEVEL_WARN = 2,
    LEVEL_ERR = 3
};

class LOG
{
public:
    static LOG *getInstance();
    static void* flush_log_thread(void *args);

    bool init(const char* file_name,int close_log,int log_buf_size = 8192,int split_lines = 5000000,int max_queue_size= 0);
    void write_log(int level,const char * format,...);
    void flush(void);
private:
    LOG();
    virtual ~LOG();
    void *async_write_log();
private:
    char dir_name[128];
    char log_name[128];
    int m_split_lines;
    int m_log_buf_size;
    long long m_count;
    int m_today;
    FILE *m_fp;
    char* m_buf;
    block_queue<string> *m_log_queue;
    bool m_is_async;
    locker m_mutex;
    int m_close_log;
};
#endif