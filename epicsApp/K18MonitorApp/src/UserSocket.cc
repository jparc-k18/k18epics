// -*- C++ -*-

#include "UserSocket.hh"

#include <cstring>
#include <ctime>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int UserSocket::m_n_socket = 0;

//______________________________________________________________________________
UserSocket::UserSocket()
  : m_socket_type(NONE),
    m_host(),
    m_port(-1),
    m_timeout(-1.),
    m_socket(-1),
    m_is_open(false),
    m_verbose(false)
{
  m_n_socket++;
}

//______________________________________________________________________________
// TCP Socket
UserSocket::UserSocket(const std::string& host,
		       int                port,
		       double             timeout /* = 2 second */)
  : m_socket_type(TCP),
    m_host(host),
    m_port(port),
    m_timeout(timeout),
    m_socket(-1),
    m_is_open(false),
    m_verbose(false)
{
  m_n_socket++;
  Open(host, port, timeout);
}

//______________________________________________________________________________
// UDP Socket
UserSocket::UserSocket(int                port,
		       double             timeout /* = 0.5 second */)
  : m_socket_type(UDP),
    m_host(),
    m_port(port),
    m_timeout(timeout),
    m_socket(-1),
    m_is_open(false),
    m_verbose(false)
{
  m_n_socket++;
  Open(port, timeout);
}

//______________________________________________________________________________
UserSocket::~UserSocket()
{
  m_n_socket--;
  if(m_is_open)
    Close();
}

//______________________________________________________________________________
int
UserSocket::Close()
{
  int ret = ::close(m_socket);
  if(ret != 0){
    std::cerr << "#E UserSocket::Close() ERROR : " << ret << std::endl;
  } else {
    m_is_open = false;
  }
  return ret;
}

//______________________________________________________________________________
// TCP Open
int
UserSocket::Open(const std::string& host,
		 int                port,
		 double             timeout /* = 2 second */)
{
  if(m_is_open){
    std::cerr << "#W UserSocket::Open() socket is already open ... "
	      << m_host << ":" << m_port << std::endl;
    return -1;
  }

  timeval tv = { (int)m_timeout,
    (m_timeout-(int)m_timeout)*1000000. };

  int         status;
  std::time_t t = std::time(NULL);
  std::string date(std::ctime(&t));
  date = date.substr(0, date.length()-1);

  m_socket = ::socket(AF_INET, SOCK_STREAM, 0);

  ::setsockopt(m_socket, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));

  sockaddr_in addr;
  std::memset((void*)&addr, 0, sizeof(addr));
  addr.sin_family       = AF_INET;
  addr.sin_port         = htons(port);
  addr.sin_addr.s_addr  = ::inet_addr(host.c_str());

  status = ::connect(m_socket, (struct sockaddr*)&addr, sizeof(addr));

  if(status < 0){
    std::cerr << date << " | " << host
	      << " connection failure" << std::endl;
    Close();
    return status;
  }

  if(m_verbose){
    std::cout << date << " | " << host
	      << " connection done" << std::endl;
  }
  m_is_open     = true;
  m_socket_type = TCP;
  return 0;
}

//______________________________________________________________________________
// UDP Open
int
UserSocket::Open(int                port,
		 double             timeout /* = 2 second */)
{
  if(m_is_open){
    std::cerr << "#W UserSocket::Open() socket is already open ... "
	      << m_host << ":" << m_port << std::endl;
    return -1;
  }

  timeval tv = { (int)m_timeout,
    (m_timeout-(int)m_timeout)*1000000. };

  sockaddr_in addr;
  int         status;
  std::time_t t = std::time(NULL);
  std::string date(std::ctime(&t));
  date = date.substr(0, date.length()-1);

  m_socket = ::socket(AF_INET, SOCK_DGRAM, 0);
  ::setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
  addr.sin_family       = AF_INET;
  addr.sin_port         = htons(port);
  addr.sin_addr.s_addr  = INADDR_ANY;
  status = ::bind(m_socket, (struct sockaddr*)&addr, sizeof(addr));
  if(status < 0){
    std::cerr << date << " | UDP bind failure ("
	      << port << ")" << std::endl;
    Close();
    return status;
  }

  m_is_open     = true;
  m_socket_type = UDP;
  m_host        = "";
  return 0;
}

//______________________________________________________________________________
void
UserSocket::Print() const
{
  std::string socket_type = (m_socket_type == TCP) ? "TCP" : "UDP";

  std::cout << "#D UserSocket::Print() " << std::endl
	    << " n socket    = " << m_n_socket  << std::endl
	    << " socket type = " << socket_type << std::endl
	    << " host        = " << m_host      << std::endl
	    << " port        = " << m_port      << std::endl
	    << " timeout     = " << m_timeout   << std::endl
	    << " socket      = " << m_socket    << " (" << &m_socket << ")"
	    << std::endl
	    << " is open     = " << m_is_open   << std::endl;
}

//______________________________________________________________________________
int
UserSocket::Read(void* buf, std::size_t count)
{
  if(!m_is_open){
    std::cerr << "#E UserSocket::Read() socket is not open" << std::endl;
    return -1;
  }

  int ret = ::read(m_socket, buf, count);
  if(ret < 0){
    std::cerr << "#E UserSocket::Read() ERROR : " << ret << std::endl;
  }
  return ret;
}

//______________________________________________________________________________
ssize_t
UserSocket::Recv(void* buf, std::size_t len, int flags)
{
  if(!m_is_open){
    std::cerr << "#E UserSocket::Recv() socket is not open" << std::endl;
    return -1;
  }

  ssize_t ret = ::recv(m_socket, buf, len, flags);
  if(ret < 0){
    std::cerr << "#E UserSocket::Recv() ERROR : " << ret << std::endl;
  }
  return ret;
}

//______________________________________________________________________________
int
UserSocket::Write(const void* buf, std::size_t count)
{
  if(!m_is_open){
    std::cerr << "#E UserSocket::Write() socket is not open" << std::endl;
    return -1;
  }

  int ret = ::write(m_socket, buf, count);
  if(ret < 0){
    std::cerr << "#E UserSocket::Write() ERROR : " << ret << std::endl;
  }
  return ret;
}
