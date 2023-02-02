/****************************************************
* ThreadPool                                        *
* Assaf Ainhoren                                    *
* Reviewer: Ester Shpoker                           *
****************************************************/
/********************************libraries includes****************************/

#include "thread_pool.hpp"

namespace details
{
    typedef ilrd::WaitablePQueue<std::
            pair<int, std::shared_ptr<ilrd::ThreadPool::ITask> > >
            pqueue_ty;

    typedef std::unordered_map<std::thread::id, bool> u_map_ty;
    
    inline void ThreadFunc(pqueue_ty &m_pqueue , u_map_ty &m_enables, 
                           sem_t *m_sem_finish)
    {
        std::thread::id id = std::this_thread::get_id(); 
        m_enables.insert({id, true});
        sem_post(m_sem_finish);
    
        std::pair <int, std::shared_ptr<ilrd::ThreadPool::ITask> >ret;
        while (m_enables[id]) //need to defend
        {
            m_pqueue.Dequeue(ret);
            ret.second->Run();
        }
    
        sem_post(m_sem_finish);
    }
    inline void CreateAndDetachThread(size_t thread_amount,
                                     pqueue_ty &m_pqueue, u_map_ty &m_enables,
                                     sem_t *m_sem_finish)
    {
        for (size_t i = 0; i < thread_amount; ++i)
        {
            std::thread task_executer(details::ThreadFunc,
                                    std::ref(m_pqueue),
                                    std::ref(m_enables), 
                                    m_sem_finish);
            task_executer.detach();
            sem_wait(m_sem_finish);
        }
    }

    
}
namespace ilrd
{
ThreadPool::ThreadPool(size_t thread_amount) : m_threadAmount(0),
                            m_pauseSem(details::CreateSem(&m_pauseSem, 0)),
                            m_sem_finish(details::CreateSem(&m_sem_finish, 0)),
                            is_paused(false)
{
    
    details::CreateAndDetachThread(thread_amount,
                                   std::ref(m_pqueue),
                                   std::ref(m_enables), 
                                   &m_sem_finish);
    m_threadAmount = thread_amount;


}
ThreadPool::~ThreadPool()
{
    std::shared_ptr<ITask > task(new BadApple(m_enables));
    for (size_t i = 0; i < m_threadAmount; ++i)
    {
        AddTask(task, static_cast<priority_ty>(LOW-2));
    }
    for (size_t i = 0; i < m_threadAmount; ++i)
    {
        sem_wait(&m_sem_finish);

    }
    sem_destroy(&m_pauseSem);
    sem_destroy(&m_sem_finish);
    

}

void ThreadPool::AddTask(std::shared_ptr<ITask> task, priority_ty priority)
{
    m_pqueue.Enqueue(std::make_pair(static_cast<int >(priority), task));

}

void ThreadPool::Pause()
{
    if (is_paused)
    {
        return;
    }
    is_paused = true;
    std::shared_ptr<ITask > task(new PauseTask(m_pauseSem));
    for (size_t i = 0; i < m_threadAmount; ++i)
    {
        AddTask(task, static_cast<priority_ty>(HIGH+2));
    }
}


ThreadPool::PauseTask::PauseTask(sem_t &sem): m_pauseSem(sem)
{
    //
}
ThreadPool::PauseTask::~PauseTask()
{
    //
    
}
void ThreadPool::PauseTask::Run()
{
    sem_wait(&m_pauseSem);
}

ThreadPool::FunctionTask::~FunctionTask()
{
    //
}

void ThreadPool::Resume()
{
    is_paused = false;
    for (std::size_t i = 0; i < m_threadAmount ; ++i)
    {
        sem_post(&m_pauseSem);
    }
}

void ThreadPool::FunctionTask::Run()
{
    m_func();
}

ThreadPool::BadApple::BadApple(std::unordered_map<std::thread::id, bool> &u_map) :
                        m_enables(u_map)
{
    //
}

ThreadPool::BadApple::~BadApple()
{

}
void ThreadPool::BadApple::Run()
{
    std::thread::id id = std::this_thread::get_id();
    m_enables[id] = false;
}


void ThreadPool::SetNumberOfThreads(size_t thread_amount)
{
    int diff = thread_amount - m_threadAmount;
    if (is_paused)
    {
        throw std::string("illegal state");
    }
    if (diff > 0)
    {
        details::CreateAndDetachThread(diff,
                                std::ref(m_pqueue),
                                std::ref(m_enables), 
                                &m_sem_finish);
    }
    else
    {
        std::shared_ptr<ITask > task(new BadApple(m_enables));
        for (int i = diff; i != 0; ++i)
        {
            AddTask(task, static_cast<priority_ty>(HIGH+2));
        }
        for (int i = diff; i != 0; ++i)
        {
            sem_wait(&m_sem_finish);

        }
       
    }
    m_threadAmount = thread_amount;
}//class kukuriku

}
