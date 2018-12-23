//
// Created by alian on 12/23/18.
//

#ifndef EXCELLENTEAM_ELLA_CPP_SINGLETON_WALL_ET_SAFESINGLETON_H
#define EXCELLENTEAM_ELLA_CPP_SINGLETON_WALL_ET_SAFESINGLETON_H

#include <pthread.h>
#include <iostream>

template<typename T>
class Singleton
{
public:
    static T* GetInstance();
private:
    Singleton();
    ~Singleton();
    Singleton(Singleton const&);
    Singleton& operator=(Singleton const&);

    static void initLocks();
    static void destroyLocks();
    static void destoryMember();

    static T* m_instance;
    pthread_mutex_t m_lockInstance;
};

template<typename T>
T* Singleton<T>::m_instance = NULL;

template<typename T>
T* Singleton<T>::GetInstance()
{
    pthread_mutex_lock( &Singleton<T>::m_lockInstance );
    if (m_instance == NULL) {
        try {
            m_instance = new T;
        }
        catch (std::bad_alloc)
        {
            pthread_mutex_unlock(&Singleton<T>::m_lockInstance);
            throw;
        }
    }
    pthread_mutex_unlock( &Singleton<T>::m_lockInstance );
    return m_instance;
}

template<typename T>
void Singleton<T>::destoryMember()
{
    delete Singleton<T>::m_instance;
    Singleton<T>::m_instance = 0;
}

template <typename T>
Singleton<T>::Singleton()
{
    initLocks();
    m_instance = static_cast <T*> (this);

}

template <typename T>
Singleton<T>::~Singleton()
{
    destroyLocks();
    destoryMember();
}

template <typename T>
void Singleton<T>::initLocks() {
    if ( pthread_mutex_init ( &Singleton<T>::m_lockInstance, NULL ) != 0 )
    {
        std::cout << "mutex init failed" << std::endl;
        destroyLocks();
    }
}

template <typename T>
void Singleton<T>::destroyLocks() {
    pthread_mutex_destroy(&Singleton<T>::m_lockInstance);
}

#endif //EXCELLENTEAM_ELLA_CPP_SINGLETON_WALL_ET_SAFESINGLETON_H
