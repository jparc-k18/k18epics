#include <cstdlib>
#include <cstdio>
#include <cstring>

#include <time.h>
#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>

void print_ascii(const char* , int len);

int main(int argc, char* argv[])
{  
  //connect socket
  //char host[]        = "192.168.30.33"; 
  //char host[]        = "192.153.109.17";
  char host[] = "192.168.30.1";
  //char *host = argv[1];
  //int port           = 80;
  int port           = 8080;
  double timeout_sec = 2.0;
  
  struct timeval tv={(int)timeout_sec, (timeout_sec-(int)timeout_sec)*1000000.};
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  setsockopt( sock, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv) );
  
  struct sockaddr_in addr;
  addr.sin_family       = AF_INET;
  addr.sin_port         = htons(port);
  addr.sin_addr.s_addr  = inet_addr(host);

  if(connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0){
    time_t t;
    t = time(NULL);
    std::string date(ctime(&t));
    date = date.substr(0, date.length()-1);
    printf("%s | HTTP connection failure\n",date.c_str());
    close(sock);
    return 0;
  }
  
  // send HTTP request
  //char cmdline[] = "GET http://www-cont.j-parc.jp/HD/magnet/k18 HTTP/1.0\n\n";
  //char cmdline[] = "GET http://www-cont.j-parc.jp/HD/hduser/k18brpc HTTP/1.0\n\n";
  char cmdline[] = "GET http://hd-temp-2.intra.j-parc.jp/index.wrc HTTP/1.0\n\n";
  //char cmdline[] = "GET http://www-cont.j-parc.jp/HD/magnet/primary HTTP/1.0\n\n";
  //char cmdline[] = "GET http://www-cont.j-parc.jp/HD/separator HTTP/1.0\n\n";
  //char cmdline[] = "GET /cdata.inc HTTP/1.0\r\nUser-Agent: Wget/1.12 (linux-gnu)\r\nAccept: */*\r\nHost: 192.168.30.31\r\nConnection: Keep-Alive\r\n\r\n ";
  //char cmdline[] = "GET http://192.168.30.34/cdata.inc HTTP/1.0\r\n";
  write(sock, cmdline, strlen(cmdline));

  // receive
  std::string buf;
  int total_len=0;
  char c;
  while ( read(sock, &c, 1) > 0 ) {
    if(0x20 < c && c <0x7f){ 
      buf += c;
      total_len++;
    }
    if(total_len > 65535) break;
  }
  
  printf("total len =%d\n",total_len);

  close(sock);
  printf("%s\n",buf.c_str());

  return 0;
  //for(int i=0;i<total_len;i++){
  //  printf("%c(%02x) ",buf[i],buf[i]);
  //}

  //print_ascii(buf.c_str(), total_len);

  if(total_len==271){
    
    int ret=0;
    
    int ok_flag;
    double temp,humi;

    std::string sub1 =  buf.substr(116, 1);
    printf("sub1: %s\n",sub1.c_str());
    
    std::string sub2 =  buf.substr(125, 4);
    printf("sub2: %s\n",sub2.c_str());

    std::string sub3 =  buf.substr(155, 2);
    printf("sub3: %s\n",sub3.c_str());
    

    printf("Temp: %f   Humi: %f\n", atof(sub2.c_str()), atof(sub3.c_str()));

    //ret = sscanf(buf.c_str(),"%sChunked4#1%d8data1=4%f8unit1",&ok_flag, &temp);
    //printf("ret = %d\n",ret);

    // char s1[256];
    // char s2[256];
    // char s3[256];
    
    // int p1;
    // double p2,p3;
    
    // int ret = sscanf(buf.c_str(),"%sChunked4#1%d8data1=4%f8unit1%sdata2=2%f8unit2%s",s1,p1,s2,p2,p3,s3);

    // printf("ret = %d\n",ret);
    
  }

  return 0;

  // analize table
  typedef std::vector<std::string> tb_row;
  std::vector<tb_row> tb_content;
  

  int index1=0;
  while(1){
    int pos1 = buf.find("<tr>", index1);
    if(pos1 == std::string::npos) break;
    int pos2 = buf.find("</tr>", index1);
    if(pos2 == std::string::npos) break;
    
    index1 = pos2 + 5;
    
    std::string sub1 = buf.substr(pos1+4, pos2-pos1-5);
    //printf("%s\n",sub1.c_str());
    
    tb_row row;
    int index2=0;
    while(1){
      int pos3 = sub1.find(">", index2);
      if(pos3 == std::string::npos) break;
      int pos4 = sub1.find("</td>", index2);
      if(pos4 == std::string::npos) break;

      index2 = pos4 + 5;

      std::string sub2 = sub1.substr(pos3+1, pos4-pos3-1);
      row.push_back(sub2);
      //printf("%s\n",sub2.c_str());
    }
    tb_content.push_back(row);
  }


  //int a = tb_content.size();
  //for(int i=0;i<a;i++){
  //  tb_row row=
  //}

  std::cout << tb_content[4][5] <<std::endl;

  return 0;
}


void print_ascii(const char* in, int len)
{
  for(int i=0;i<len;i++){
    char c = in[i];
    
    if(0x20 < c && c <0x7f) printf("%c ",c); 
    //else printf(" ");

  }
  return;
}
