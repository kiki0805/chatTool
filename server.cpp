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
#include "server.h"
#include "yaml.h"

void m_sendData(int sock, CDataWrapped data);
CDataWrapped m_recvData(int sock);
void handleData(CDataWrapped data, int sock);

const int CHAT = 0;
const int ACCESS = 1;
const int CREATE = 2;
const int ENTER = 3;
const int LEAVE = 4;
const int JOIN = 5;
const int QUIT = 6;
//////////////////////////
const int SERVER_PORT = 5000;
const char* SERVER_IP = "192.168.1.103";
fd_set CONNECTION_LIST;
CRoom ROOM_LIST[10];
CUser USER_LIST[10];
CUser MEMEBER_LIST[10][10];
/*
memset(&ROOM_LIST, 0, sizeof(CRoom)*10);
memset(&USER_LIST, 0, sizeof(CUser)*10);
memset(&MEMEBER_LIST, 0 ,sizeof(CUser)*100);
CRoom nullRoom;
memset(&nullRoom, 0, 48);
CUser nullUser;
memset(&nullUser, 0 ,72);
*/
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
  //cout<<maxsock + 1<<endl;
  while(1){
    fd_set readList = CONNECTION_LIST;
    int error_tv = select(maxsock + 1, &readList, NULL, NULL, NULL);
    //if(error_tv == -1) perror("select()");
    //else if(error_tv) cout<<"available now"<<endl;
    //else cout<<"no data"<<endl;
    //cout<<maxsock<<endl;
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
            /////////////////////////
            //////////////////////
            if(newsock > maxsock) maxsock = newsock;
          }
        }
        else{
            //cout<<"come here"<<endl;
            //char temp[1024] = "you are in\n";
            //write(s, &temp, 1024);
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
              //cout<<"Come socket: "<<i<<endl;
              if(FD_ISSET(i, &CONNECTION_LIST)
                && i != server_sock && i != s){
                  //cout<<"Send to: "<<i<<endl;
                  write(i, &send_data, 1024);
                }
            }
          }
          //CDataWrapped data_recv = m_recvData(s);
          //handleData(data_recv, s, maxsock, server_sock);
        }
      }
    }
    //cout<<"finish"<<endl;
  }
  close(server_sock);
  return 0;
}
/*
void roomInsert(CRoom* l, CRoom* ele){
  CRoom* LIST = l;
  while(LIST->m_getRoomID()){
    LIST++;
  }
  *LIST = *ele;
}

void userInsert(CUser* l, CUser* ele){
  CUser* LIST = l;
  while(LIST->m_getSock()){
    LIST++;
  }
  *LIST = *ele;
}

void insert_2nd(CUser* l, CUser* ele, int index){
  CUser* LIST = l;
  LIST += index * sizeof(CUser) * 10;
  while(LIST->m_getSock()){
    LIST++;
  }
  *LIST = *ele;
}

void m_sendData(int sock, CDataWrapped data){
  write(sock, &data, 1024);
}


CDataWrapped m_recvData(int sock){
  char temp[1024];
  memset(&temp, 0, 1024);
  if(read(sock, &temp, 1024) == 0){
    FD_CLR(sock, &CONNECTION_LIST);
    int type = QUIT;
    CDataWrapped return_data = CDataWrapped(type,
      "", nullRoom, "");
    return return_data;
  }
  else{
    CDataWrapped return_data;
    memmove(return_data, &temp, 1024);
    return return_data;
  }
  //////////////////
  //////////////////
}
void handleData(CDataWrapped data, int sock, int maxsock, int server_sock){
  if(data.m_getType() == CHAT){
    for(int i = 0; i <= maxsock; i++){
      if(FD_ISSET(i, &CONNECTION_LIST)
        && i != server_sock && i != sock){
          write(i, &data, 1024);
        }
    }
  }
  else if(data.m_getType() == ACCESS){
    for(int i = 0; i < 10; i++){
      if(ROOM_LIST[i].m_getRoomID() != 0){
        CDataWrapped return_data =
          CDataWrapped(ACCESS, "", ROOM_LIST[i], "");
        return return_data;
      }
    }
  }
  else if(data.m_getType() == CREATE){
    /*for(int i = 0; i < 10; i++){
      if(!ROOM_LIST[i].m_getRoomID()){
        ROOM_LIST[i] = data.m_getRoom();
        break;
      }
    }
  }
    else if(data.m_getType() == ENTER){
      for(int i = 0; i < 10; i++){
        if(MEMEBER_LIST[data.m_getRoom().m_getRoomID()][i].m_getNickname() == ""){
          for(int j = 0; j < 10; j++){
            if(USER_LIST[j].m_getNickname() == data.m_getName()){
              MEMEBER_LIST[data.m_getRoom().m_getRoomID()][i] = USER_LIST[j];
              break;
            }
          }
          break;
        }
      }
    }
  else if(data.m_getType() == LEAVE){

  }
  else if(data.m_getType() == JOIN){
    for(int i = 0; i < 10; i++){
      if(USER_LIST[i].m_getSock() == sock){
        USER_LIST[i].m_setNickname(data.m_getName());
        break;
      }
    }
    string sock_num;
    stringstream ss;
    ss << sock;
    ss >> sock_num;
    CDataWrapped m = CDataWrapped(JOIN, "", nullRoom, sock_num);
    write(newsock, &m, 1024);
  }
}
*/
