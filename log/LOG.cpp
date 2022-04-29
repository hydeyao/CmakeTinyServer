#include "LOG.h"
#include <string.h>

LOG::LOG()
{
    m_count = 0;
    m_is_async = false;
}

LOG::~LOG()
{
    if (m_fp != NULL)
    {
        fclose(m_fp);
    }
}

LOG* LOG::getInstance()
{
    static LOG ins;
    return &ins;
}

void* LOG::flush_log_thread(void *args)
{
    LOG::getInstance()->async_write_log();
}

bool LOG::init(const char* file_name,int close_log,int log_buf_size,int split_lines,int max_queue_size)
{
    //set max_queue_size
    if (max_queue_size >= 1)
    {
        m_is_async = true;
        m_log_queue = new block_queue<string>(max_queue_size);
        pthread_t tid;
        pthread_create(&tid,NULL,flush_log_thread,NULL);
    }
    
    m_close_log = close_log;
    m_log_buf_size = log_buf_size;
    m_buf = new char[m_log_buf_size];
    memset(m_buf,'\0',m_log_buf_size);
    m_split_lines = split_lines;

    time_t t = time(NULL);
    struct tm *sys_tm = localtime(&t);
    struct tm my_tm = *sys_tm;

    const char* p = strrchr(file_name,'/');
    char log_full_name[256] = {0};

    if (NULL == p)
    {
        snprintf(log_full_name, 255, "%d_%02d_%02d_%s",my_tm.tm_year + 1900,my_tm.tm_mon + 1,my_tm.tm_mday,file_name);
    }else
    {
        strcpy(log_name,p + 1);
        strncpy(dir_name,file_name,p - file_name + 1);
        snprintf(log_full_name,255,"%s%d_%02d_%02d_%s",dir_name,my_tm.tm_year + 1900,my_tm.tm_mon + 1,my_tm.tm_mday,log_name);
    }

    m_today = my_tm.tm_mday;
    m_fp = fopen(log_full_name,"a");
    if (!m_fp)
    {
        return false;
    }
    
    return true;
}

void LOG::write_log(int level,const char * format,...)
{
    struct timeval now = {0,0};
    gettimeofday(&now,NULL);
    time_t t = now.tv_sec;
    struct tm *sys_tm = localtime(&t);
    struct tm my_tm = *sys_tm;

    char s[16] = {0};
    switch (level)
    {
    case LEVEL_DEBUG:
        strcpy(s,"[debug]:");
        break;
    case LEVEL_INFO:
        strcpy(s,"[info]:");
        break;
    case LEVEL_WARN:
        strcpy(s,"[warn]:");
        break;
    case LEVEL_ERR:
        strcpy(s,"[error]:");
        break;
    default:
        strcpy(s,"[info]:");
        break;
    }

    

    //write log


}

void* LOG::async_write_log()
{
    string single_log;
    while (m_log_queue->pop(single_log))
    {
        m_mutex.lock();
        fputs(single_log.c_str(),m_fp);
        m_mutex.unlock();
    }   
}