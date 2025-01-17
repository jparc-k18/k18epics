// -*- C++ -*-

#ifndef USER_SOCKET_HH
#define USER_SOCKET_HH

#include <string>
#include <vector>
#include <sys/types.h>

//______________________________________________________________________________
class UserSocket
{
public:
  enum SocketType { NONE, TCP, UDP, NSocketType };

public:
  UserSocket();
  // TCP Socket
  UserSocket(const std::string& host,
	      int                port,
	      double             timeout = 2. /* second */);
  // UDP Socket
  UserSocket(int                port,
	      double             timeout = 2. /* second */);
  ~UserSocket();

private:
  UserSocket(const UserSocket&);
  UserSocket& operator =(const UserSocket&);

private:
  static int  m_n_socket;
  SocketType  m_socket_type;
  std::string m_host;
  int         m_port;
  double      m_timeout;
  int         m_socket;
  bool        m_is_open;
  bool        m_verbose;

public:
  bool       IsOpen() const { return m_is_open; }
  int        Close();
  SocketType GetSocketType() const { return m_socket_type; }
  // TCP Open
  int        Open(const std::string& host,
		   int                port,
		   double             timeout = 2. /* second */);
  // UDP Open
  int        Open(int                port,
		   double             timeout = 0.5 /* second */);
  void       Print() const;
  int        Read(void* buf, std::size_t count);
  ssize_t    Recv(void* buf, std::size_t len, int flags);
  int        Write(const void* buf, std::size_t count);
  void       Verbose(bool verbose=true){ m_verbose = verbose; }
};

#endif
