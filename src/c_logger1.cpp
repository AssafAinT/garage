/****************************************************
* Clogger                                           *
* Assaf Ainhoren                                    *
* Reviewer: Maor fakliro                            *
* date: 27.11.2022                                  *
****************************************************/
/********************************libraries includes****************************/
#include "c_logger1.hpp"
#include <cstring>
#include <stdarg.h>
namespace ilrd
{
    class WriteTask:public ThreadPool::ITask
    {
        public:
        WriteTask(FILE *stream, const char *msg); // nonexplicit, by value to share ownership of object
        virtual ~WriteTask(){}
        void Run();
        private:
        
        FILE *m_file;
        const char *m_msg;
        
        
    };
    WriteTask::WriteTask(FILE *stream, const char *msg):
    m_file(stream), m_msg(msg)
    {
        //empty kukuriku
    }
    void WriteTask::Run()
    {
        fprintf(m_file, "%s\n",m_msg);
        delete[] m_msg;
    }
    class CloseTask:public ThreadPool::ITask
    {
        public:
        CloseTask(FILE *stream); // nonexplicit, by value to share ownership of object
        virtual ~CloseTask(){}
        void Run();
        private:
        
        FILE *m_file;
    };
    CloseTask::CloseTask(FILE *stream):
    m_file(stream)
    {
        //empty kukuriku
    }
    void CloseTask::Run()
    {
        fclose(m_file);
    
    }
/*---------------------------static initializion--------------------------------*/
const size_t PAGE_SIZE = 4096;
CLogger* CLogger::CLogger_instance = NULL;
std::mutex CLogger::mutex_lock; 
/*----------------------------class defintions--------------------------------*/
    CLogger *CLogger::GetInstance(const char *file_name)
    {
        if ((CLogger_instance) == NULL)
        {
            uni_lock lock(mutex_lock);
            if ((CLogger_instance) == NULL)
            {
                static CLogger local(file_name);
                CLogger_instance = &local;
            }
        }
        return (CLogger_instance);
    }
    void CLogger::Log(const char *format, ...)
    {
        // uni_lock lock(mutex_lock);
        char *buff = new char[PAGE_SIZE];
        va_list args;
        time_t current_time;
        struct tm * time_info;

        //printing thread_id to the file
        sprintf(buff, "thread_id: %d ", gettid());
        time(&current_time);
        time_info = localtime(&current_time);

        strftime(buff + strlen(buff), PAGE_SIZE -1,"time: %H:%M:%S ",time_info);
        va_start(args, format);
        vsnprintf(buff + strlen(buff), PAGE_SIZE -1,format, args);
        va_end(args);

        std::shared_ptr<ThreadPool::ITask > task(new WriteTask(m_my_file, buff));
        m_th_p.AddTask(task, ThreadPool::MID);
    }

    CLogger::CLogger(const char *file_name):m_my_file(fopen(file_name, "a+")), m_th_p(1)
    {
        if (NULL == m_my_file)
        {
            throw(std::bad_alloc());
        }
            
        
    }

    CLogger::~CLogger()
    {
        uni_lock lock(mutex_lock); 
        std::shared_ptr<ThreadPool::ITask>
                        closeit(new CloseTask(m_my_file)); 

        m_th_p.AddTask(closeit, ThreadPool::LOW);
        
        CLogger_instance = reinterpret_cast<CLogger *>(0xDEADBEEF);
    }
    
}
