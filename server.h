//#ifndef SERVER_H
//#define SERVER_H
const int maxRoomNum = 3;
#include <string>
#include <string.h>
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

class CRoom{//size = 48
private:
  int m_roomID;
  int m_capacity;
  int m_numberNow;
  char m_roomName[36];
public:
  void m_setRoomID(int id){
    m_roomID = id;
  }
  int m_getRoomID(){
    return m_roomID;
  }
  void m_setRoomName(string name){
    for(int i = 0; i < name.length(); i++){
      m_roomName[i] = name[i];
    }
  }
  string m_getRoomName(){
    string name = m_roomName;
    return name;
  }
  void m_setRoomCap(int cap){
    m_capacity = cap;
  }
  int m_getRoomCap(){
    return m_capacity;
  }
  void m_increaseNumNow(){
    m_numberNow++;
  }
  int m_getNumNow(){
    return m_numberNow;
  }
};

class CDataWrapped{
private:
  char m_type;
  char m_name[63];
  CRoom m_room;//48
  char m_content[912]; // without limitation
public:
  CDataWrapped(int type, string nn, CRoom room, string content){
    m_type = (char)type;
    ////////////////////////
    memset(&m_name, 0, sizeof(char)*63);
    for(int i = 0; i < nn.length(); i++){
      m_name[i] = nn[i];
    }
    ////////////////////////
    m_room = room;
    ////////////////////////
    memset(&m_content, 0, sizeof(char)*912);
    for(int i = 0; i < content.length(); i++){
      m_content[i] = content[i];
    }
  }
  int m_getType(){
    return (int)m_type;
  }
  string m_getName(){
    string name = m_name;
    return name;
  }
  CRoom m_getRoom(){
    return m_room;
  }
  string m_getContent(){
    string content = m_content;
    return content;
  }
};
