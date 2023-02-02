/****************************************************
* Clogger                                           *
* Assaf Ainhoren                                    *
* Reviewer: Maor fakliro                            *
* date: 27.11.2022                                  *
****************************************************/
/********************************libraries includes****************************/
#ifndef __ILRD_RD127_128_C_LOGGER__
#define __ILRD_RD127_128_C_LOGGER__
#include "thread_pool.hpp"
#include <mutex>
namespace ilrd
{
typedef typename std::unique_lock<std::mutex> uni_lock;

class CLogger
{
    public:
    void Log(const char *format, ...);
    
    static CLogger *GetInstance(const char *file_name); 
    ~CLogger();
    
    private:
    CLogger(const char *file_name);
    CLogger(const CLogger&);
    CLogger& operator=(const CLogger&);

    static CLogger *CLogger_instance;
    static std::mutex mutex_lock;
    FILE *m_my_file;
    const char *m_file;
    ThreadPool m_th_p;
    
};
 
}
#endif