#coding=utf-8
import socket,select,sys,threading,time
from tkinter import *

HOST='192.168.1.103'
PORT=5000
s=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
s.connect((HOST,PORT))
s.setblocking(0)

root = Tk()
container = Frame(root, width = 100, height = 50)
container.grid()
output = Text(root, width = 80, height = 15)
output["state"] = "disabled"
output.insert('end',"welcome")
output.grid(column = 0, row = 0, columnspan = 5, rowspan = 5, sticky="N")
input_msg = Text(root, width = 80, height = 7)
input_msg.grid(column = 0, row = 5, columnspan = 5, rowspan = 2, sticky="S")


def speak():
	global s,show_m,in_m,output,input_msg
	msg = input_msg.get('1.0','end')
	input_msg.delete('1.0','end')
	s.send(msg.encode(encoding="utf-8"))
	astr = "You >>>     " + msg
	output["state"] = "normal"
	output.insert('end', astr)
	cut = "----------------\n"
	output.insert('end', cut)
	output["state"] = "disabled"
	input_msg.delete('1.0','end')


def speak_key(event):
	global s,show_m,in_m,output,input_msg
	msg = input_msg.get('1.0','end')
	input_msg.delete('1.0','end')
	s.send(msg.encode(encoding="utf-8"))
	astr = "You >>>     " + msg
	output["state"] = "normal"
	output.insert('end', astr)
	cut = "-----------------\n"
	output.insert('end', cut)
	output["state"] = "disabled"
	input_msg.delete('1.0','end')
	

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


scroll = Scrollbar(root, orient=VERTICAL, command=output.yview)
scroll.grid(column=4, row = 0, sticky = "E")
output["yscrollcommand"] = scroll.set

send_b = Button(root, text = "SEND", command = speak)
input_msg.bind('<Key-Return>', speak_key)
send_b.grid(column = 4, row = 5, sticky = "S")

container.mainloop()


def close_sock(s):
	s.close()
