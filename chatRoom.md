# 项目一： 图形化聊天室的实现
## 基本信息
### 涉及的知识
- python3 Tkinter
  - [python3.Tkinter学习之Tk概念](http://today2tmr.com/2017/06/28/python3-tkinter学习之tk概念/)
  - [python3.Tkinter学习之基本Widgets](http://today2tmr.com/2017/06/28/python3-tkinter学习之基本widgets/)
  - [python3.Tkinter学习之网格图形管理器](http://today2tmr.com/2017/06/28/python3-tkinter学习之网格图形管理器/)
  - [python3.Tkinter学习之更多Widgets](http://today2tmr.com/2017/06/28/python3-tkinter学习之更多widgets/)
- C++ socket编程
  - [网络编程之socket学习笔记](http://today2tmr.com/2017/06/29/网络编程之socket学习笔记/)
  - [[学习笔记] Windows和Linux下Socket编程中select的应用](http://today2tmr.com/2017/06/30/学习笔记-windows和linux下socket编程中select的应用/)
- [C++ TinyXML](http://www.grinninglizard.com/tinyxmldocs/)
- [python3 Beautiful](http://today2tmr.com/2017/07/15/python-爬虫学习笔记-第二周单元4beautiful-soup库入门/)
- [MySQL Connector for C++](https://dev.mysql.com/doc/connector-cpp/en/)

### 功能
**测试环境： 局域网\Linux**
- 支持多个具有唯一标识的客户端同时访问。
- 默认开放全体用户可进入的公共聊天室。
- 支持用户自定义创建房间。
- 单个用户可同时进入多个房间。

### 参考
- [http://www.cnblogs.com/hazir/p/python_chat_room.html](http://www.cnblogs.com/hazir/p/python_chat_room.html)
- [http://www.cnblogs.com/roger9567/p/4696953.html](http://www.cnblogs.com/roger9567/p/4696953.html)

## 类、变量与方法
### CHelloGUI Class
#### 信息
- 聊天室欢迎窗口，在此窗口设置昵称后进入大厅。
- 应用于客户端。

#### 方法
由于GUI类仅在python3中定义，所以方法前没有类型。
- `m_createWidgets()`

### CLobbyGUI Class
#### 信息
- 聊天室大厅窗口。
- 应用于客户端。

#### 变量
#### 方法
- `m_createWidgets()`
- `void m_accessRoomInfo(int sock)`
- `CRoom* m_returnRoomInfo(int sock)`
  - 返回CRoom数组

### CRoom Class
#### 信息
- 聊天室类。
- 应用于服务端和客户端。

#### 变量
- `int m_roomID`
- `int m_capacity`
- `int m_numberNow`
- `char m_roomName[36]`//用于对齐，仅用32 bytes

#### 方法
- `CRoom()`
- `void m_setRoomID(int id)`
- `int m_getRoomID()`
- `void m_setRoomName(string name)`
- `string m_getRoomName()`
- `void m_setRoomCap(int cap)`
- `int m_getRoomCap()`
- `void m_increaseNumNow()`
- `int m_getNumNow()`


### CRoomGUI(CRoom) Class
#### 信息
- 聊天室窗口。
- 应用于客户端。

#### 变量
- `m_roomID`
- `m_roomName`
- `m_capacity`
- `m_numberNow`

#### 方法
- `m_createWidgets()`
- `CRoomGUI()`
- `void m_setRoomID(int id)`
- `int m_getRoomID()`
- `void m_setRoomName(string name)`
- `string m_getRoomName()`
- `void m_setRoomCap()`
- `int m_getRoomCap()`
- `void m_increaseNumNow()`
- `int m_getNumNow()`
- `void m_chatContentSend(int sock, CRoom room, CUser user, string message)`
- `string m_chatContentRecv(int sock, CRoom room, CUser user, string message)`
  - 返回“user-time:message”字符串

### CUser Class
#### 信息
- 储存用户信息。
- 应用于服务端与客户端。

#### 变量
- `string m_nickname`
- `string m_IP`
- `int m_port`
- `int m_sock`

#### 方法
- `void m_setNickname(string nn)`
- `string m_getNickname()`
- `void m_setIP(string ip)`
- `string m_getIP()`
- `void m_setPort(int port)`
- `int m_getPort()`
- `void m_setSock(int sock)`
- `int m_getSock()`

以下仅客户端需要：
- `m_speak(sock)`
- `m_listening(sock)`
- `m_enterLobby(sock)`
- `m_quitLobby(sock)`
- `m_enterRoom(sock, room)`
- `m_leaveRoom(sock, room)`

### 信息封装
由于两种语言的数据类型大小不统一，我应用XML格式传递数据。
共三个标签，每个标签具有自己的属性。
- content标签
  - type
    - chat, 聊天内容，客户端到服务端+服务端到客户端
    - access, 房间信息访问，客户端到服务端+服务端到客户端
    - create, 创建房间，客户端到服务端
    - enter, 进入房间，客户端到服务端
    - leave, 退出房间，客户端到服务端
    - join, 进入大厅，客户端到服务端+服务端到客户端(返回sock)
    - quit, 离开大厅
  - message，当且仅当type=chat时内容不为空
- room标签
  - id，非空：
    - 客户端到服务端 content.type=chat/enter/leave
    - 服务端到客户端 content.type=chat/access
  - name，非空：
    - 客户端到服务端 content.type=create
    - 服务端到客户端 content.type=access
  - cap，非空：
    - 客户端到服务端 content.type=create
    - 服务端到客户端 content.type=access
  - mem_num，非空：
    - 客户端到服务端 none
    - 服务端到客户端 content.type=access
  - 若type=access，同类属性含多个，以后缀编号区分，如id1,id2,id3...
- user标签
  - sock，非空：
    - 客户端到服务端 content.type!=join
    - 服务端到客户端 content.type=join
  - name，非空：
    - 客户端到服务端 content.type=join
    - 服务端到客户端 content.type=chat

#### 变量
- `char m_type`
- `char m_name[63]`
- `CRoom m_room`
- `char m_content[912]`

#### 方法
- `CDataWrapped(int type, string nn, CRoom room, string content)`
- `int m_getType()`
- `string m_getName()`
- `CRoom m_getRoom()`
- `string m_getContent()`

### 公共方法
应用于客户端：
- `void m_enterRoomSend(int sock, CRoom room, CUser user)`
- `void m_leaveRoomSend(int sock, CRoom room, CUser user)`

应用于服务端和客户端：
- `void m_sendData(int sock, CDataWrapped data)`
- `CDataWrapped m_recvData(int sock)`
- `void handleData(CDataWrapped data, int sock)`

### 全局变量
- `CRoom* ROOM_LIST;`
以下仅用于服务端:
- `int* CONNECTION_LIST;`
- `CUser* USER_LIST`

## 一步步实现
### 服务器框架搭建(c++)
- 数据解析函数
- 数据封装函数

### 客户端框架搭建(python3)
- 用户界面构造函数
- 数据解析函数
- 数据封装函数
