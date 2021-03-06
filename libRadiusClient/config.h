#ifndef CONFIG_H
#define CONFIG_H
//#include <sys/epoll.h>

/*Variable public*/
#define ACCOUNTING_NAME ("Socks5")
#define MAX_THREAD (100)
#define BUFFER_SIZE (8 * 1024)	//8 KB

#define MAX_EPOLL_COUNT (60*1000)   //max connection
#define THREAD_WORKERS_COUNT (1)   //max thread
#define MAX_USERS (15000)            //max users
#define MAX_USERS_LIST_COUNT (30000)            //max users list

#define MAX_EPOLL_EVENT_COUNT (1024)
//#define MAX_EPOLL_EVENT_COUNT (MAX_EPOLL_COUNT / THREAD_WORKERS_COUNT)  //max 5000
//#define EPOLL_EVENTS_MULTITHREAD (EPOLLIN | EPOLLHUP | EPOLLPRI | EPOLLERR | EPOLLONESHOT) //EPOLLRDHUP
#define EPOLL_EVENTS_MULTITHREAD (EPOLLIN | EPOLLONESHOT | EPOLLET) //EPOLLRDHUP
#define EPOLL_EVENTS_MULTITHREAD_NONBLOCKING (EPOLL_EVENTS_MULTITHREAD | EPOLLET)
#define EPOLL_MULTI_EVENTS (EPOLLIN | EPOLLONESHOT)
#define ISINVALID (-1)

#endif // CONFIG_H
