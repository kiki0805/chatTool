#include <stdio.h>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <sstream>
using namespace std;

class CUser{
private:
  string m_nickname;
  string m_IP;
  int m_port;
  int m_sock;
public:
  void m_setNickname(string nn){
    m_nickname = nn;
  }
  string m_getNickname(){
    return m_nickname;
  }
  void m_setIP(string ip){
    m_IP = ip;
  }
  string m_getIP(){
    return m_IP;
  }
  void m_setPort(int port){
    m_port = port;
  }
  int m_getPort(){
    return m_port;
  }
  void m_setSock(int sock){
    m_sock = sock;
  }
  int m_getSock(){
    return m_sock;
  }
};


const int SERVER_PORT = 5000;
const char* SERVER_IP = "192.168.1.103";
fd_set CONNECTION_LIST;
CUser USER_LIST[10];


int main(){
  int reuseaddr = 1;
  struct timeval tv;
  tv.tv_sec = 5;
  tv.tv_usec = 0;
	int server_sock = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERVER_PORT);
	serv_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
  if(setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR,
    &reuseaddr, sizeof(int)) == -1) perror("setsockopt");
	if(bind(server_sock, (sockaddr*)&serv_addr, sizeof(sockaddr)) == -1)
    perror("bind");
  if(listen(server_sock, 20) == -1) perror("listen");
	int maxsock = server_sock;
  FD_ZERO(&CONNECTION_LIST);
  FD_SET(server_sock, &CONNECTION_LIST);
  while(1){
    fd_set readList = CONNECTION_LIST;
    int error_tv = select(maxsock + 1, &readList, NULL, NULL, NULL);
    for(int s = 0; s <= maxsock; s++){
      if(FD_ISSET(s, &readList)){
        if(s == server_sock){
          int newsock;
          struct sockaddr_in their_addr;
          socklen_t size = sizeof(struct sockaddr_in);
          newsock = accept(server_sock,
            (struct sockaddr*)&their_addr, &size);
          if(newsock != -1){
            cout<<"New connection: "<<inet_ntoa(their_addr.sin_addr)
              <<":"<<htons(their_addr.sin_port)<<endl;
            FD_SET(newsock, &CONNECTION_LIST);
            for(int i = 0; i < 10; i++){
              if(USER_LIST[i].m_getIP() == ""){
                USER_LIST[i].m_setIP(inet_ntoa(their_addr.sin_addr));
                USER_LIST[i].m_setPort(htons(their_addr.sin_port));
                break;
              }
            }
            if(newsock > maxsock) maxsock = newsock;
          }
        }
        else{
          char data[1024];
          memset(&data, 0, 1024);
          int err_code = read(s, &data, 1024);
          if(err_code!=0){
            string msg;
            stringstream ss;
            ss << s;
            ss >> msg;
            msg = "Sock" + msg + " <<<     " + data;
            char send_data[1024];
            memset(&send_data, 0, 1024);
            cout<<msg.length()<<endl;
            for(int i = 0; i < msg.length(); i++){
                send_data[i] = msg[i];
            }
            send_data[msg.length()] = '\0';
            cout<<msg<<endl;
            for(int i = 0; i <= maxsock; i++){
              if(FD_ISSET(i, &CONNECTION_LIST)
                && i != server_sock && i != s){
                  write(i, &send_data, 1024);
                }
            }
          }
        }
      }
    }
  }
  close(server_sock);
  return 0;
}