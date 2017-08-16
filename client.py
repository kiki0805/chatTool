#coding=utf-8
CHAT = 0
ACCESS = 1
CREATE = 2
ENTER = 3
LEAVE = 4
JOIN = 5
QUIT = 6
'''
class CDataWrapped():
	data_list = [0]*1024
	def setType(a_type):
		data_list[0] = a_type
	def setName(nick):
		data_list[1:64] = nick.encode(encoding="utf-8")
	def setContent(content):
		data_list[112:1024] = content.encode(encoding="utf-8")
	def getData():
		return bytes(data_list)
'''
import socket,select,sys,threading,time
from tkinter import *
HOST='192.168.1.103'
PORT=5000
s=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
s.connect((HOST,PORT))
s.setblocking(0)
NICKNAME = "ENMPTY"

root = Tk()
container = Frame(root, width = 100, height = 50)
container.grid()
output = Text(root, width = 80, height = 15)
output["state"] = "disabled"
output.insert('end',"welcome")
output.grid(column = 0, row = 0, columnspan = 5, rowspan = 5, sticky="N")
input_msg = Text(root, width = 80, height = 7)
input_msg.grid(column = 0, row = 5, columnspan = 5, rowspan = 2, sticky="S")

##############
def speak():
	#print(event.char)
	global s,show_m,in_m,output,input_msg
	msg = input_msg.get('1.0','end')
	input_msg.delete('1.0','end')
	'''CDataWrapped data_send
	data_send.setType(CHAT)
	data_send.setName(NICKNAME)
	data_send.setContent(msg)'''
	s.send(msg.encode(encoding="utf-8"))
	astr = "You >>>     " + msg
	output["state"] = "normal"
	output.insert('end', astr)
	cut = "----------------\n"
	output.insert('end', cut)
	output["state"] = "disabled"
	input_msg.delete('1.0','end')
	print("come here2")

def speak_key(event):
	print("Enter used")
	global s,show_m,in_m,output,input_msg
	msg = input_msg.get('1.0','end')
	input_msg.delete('1.0','end')
	'''CDataWrapped data_send
	data_send.setType(CHAT)
	data_send.setName(NICKNAME)
	data_send.setContent(msg)'''
	s.send(msg.encode(encoding="utf-8"))
	astr = "You >>>     " + msg
	output["state"] = "normal"
	output.insert('end', astr)
	cut = "---------1-------\n"
	output.insert('end', cut)
	output["state"] = "disabled"
	input_msg.delete('1.0','end')
	print("come here")

def start_read():
	global s,show_m,in_m,output,input_msg
	while 1:
		rlist = [s]
		# Get the list sockets which are readable
		read_list, write_list, error_list = select.select(rlist , [], [])
		for sock in read_list:
		#incoming message from remote server
			if sock == s:
				data = sock.recv(1024)
				if not data :
					astr = 'Disconnected from chat server\n'
					output["state"] = "normal"
					output.insert('end', astr)
					output["state"] = "disabled"
					sys.exit()
				else :
					astr = data.decode()
					print(astr[50])
					real = ""
					for i in astr:
						if i == '\x00':
							break
						real = real + i
					output["state"] = "normal"
					output.insert('end', real)
					cut = "----------------\n"
					output.insert('end', cut)
					output["state"] = "disabled"
		time.sleep(0.1)


t = threading.Thread(target=start_read)
t.start()


def cn_again():
	global s,show_m,in_m,output,input_msg,t
	s.close()
	s=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
	s.connect((HOST,PORT))
	output["state"] = "normal"
	system_m = "Welcome back!\n"
	output.insert('end', system_m)
	output["state"] = "disabled"
	t.stop()
	t.start()
	s.setblocking(0)
###############
scroll = Scrollbar(root, orient=VERTICAL, command=output.yview)
scroll.grid(column=4, row = 0, sticky = "E")
output["yscrollcommand"] = scroll.set

#send_b = Button(root, text = "SEND", command = speak)
send_b = Button(root, text = "SEND", command = speak)
input_msg.bind('<Key-Return>', speak_key)
send_b.grid(column = 4, row = 5, sticky = "S")
#again_b = Button(root, text = "CONNECT", command=cn_again)
#again_b.grid(column = 4, row = 6)


#output.insert(0, "Welcome!")
#root.mainloop()
container.mainloop()

def close_sock(s):
	s.close()
