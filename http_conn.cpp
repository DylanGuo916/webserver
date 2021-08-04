#include "http_conn.h"
#include <sys/fcntl.h>
#include <sys/socket.h>

int http_conn::m_epollfd = -1;
int http_conn::m_user_count = 0;
void setnonblocking(int fd) {
  int old_flag = fcntl(fd, F_GETFL);
  int new_flag = old_flag | O_NONBLOCK;
  fcntl(fd, F_SETFL, new_flag);
}

// 向epoll中添加需要监听的文件描述符
void addfd(int epollfd, int fd, bool one_shot) {
  epoll_event event;
  event.date.fd = fd;
  event.events = EPOLLIN | EPOLLET;
  if (one_shot) {
    event.events | EPOLLONESHOT;
  }
  epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);

  // 设置文件描述符非阻塞
  setnonblocking(fd);
}

// 从epoll中移除监听的文件描述符
void removefd(int epollfd, int fd) {
  epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, 0);
  close(fd);
}

// 修改文件描述符，重置socket上EPOLLONESHOT事件
void modfd(int epollfd, int fd, int ev) {
  epoll_event event;
  event.data.fd = fd;
  event.events = ev | EPOLLONESHOT | EPOLLRDHUP;
  epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &event);
}

http_conn::http_conn() {}

http_conn::~http_conn() {}

// 初始化连接
void http_conn::init(int sockfd, const sockaddr_in &addr) {
  m_sockfd = sockfd;
  m_address = addr;
  // 设置端口复用
  int reuse = 1;
  setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

  addfd(m_epollfd, m_sockfd, true);
  m_user_count++;
}

// 关闭连接
void http_conn::close_conn() {
  if (m_sockfd != -1) {
    removefd(m_epollfd, m_sockfd);
    m_sockfd = -1;
    m_user_count--;
  }
}

bool http_conn::read() {
  printf("一次性读完数据\n");
  return true;
}
bool http_conn::write() {
  printf("一次性写完数\n");
  return true;
}

// 线程池中
void http_conn::process() {
  // 解析HTTP请求
  printf("parse request, create response\n");
  // 生成响应
  
}