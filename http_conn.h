#ifndef HTTPCONNECTION_H
#define HTTPCONNECTION_H

#include <netinet/in.h>
#include <sys/epoll.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdarg.h>
#include <errno.h>
#include "locker.h"
#include <sys/uio.h>

class http_conn {
public:
  static int m_epollfd;
  static int m_user_count;
  http_conn();
  ~http_conn();
  void process(); // 处理客户端请求
  void init(int sockfd, const sockaddr_in& addr);
  void close_conn();
  bool read(); //非阻塞读
  bool write(); //非阻塞写
private:
  int m_sockfd; // HTTP连接的socket
  sockaddr_in m_address; // 通信的socket地址
};

#endif