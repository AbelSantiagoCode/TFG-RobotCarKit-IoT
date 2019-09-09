#!/usr/bin/python
from multiprocessing import Process
import socket
import select
import sys

# importing the requests library
import requests

class ServerPipe(object):
	"""ServerChat which uses TCP protocol"""
	def __init__(self, HOST, PORT, API_ENDPOINT):
		self.HOST = HOST
		self.PORT = PORT
		self.API_ENDPOINT = API_ENDPOINT
		self.ClientArduino = 0
		self.ArduinoAviable = False
		self.NClient    = 2
		self.SockClient = []
		self.ProClient 	= []
		try:
			self.SERVER  = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
			self.SERVER.bind((HOST,PORT))
			self.SERVER.listen(self.NClient)
		except socket.error, e:
			print("Error creating socket: "+str(e))
			sys.exit(1)
		self.SelectList = [self.SERVER,sys.stdin]


	def __AppendSelect(self,Arg):
		self.SelectList.append(Arg)

	def __RemoveSelect(self,Arg):
		self.SelectList.remove(Arg)

	def __CloseSockClient(self):
		for Client in self.SockClient:
			Client.close()

	def __CloseProClient(self):
		for PChild in self.ProClient:
			PChild.terminate()

	def CloseServer(self):
		self.SERVER.close()

	def __ServiceClient(self,Client):
		"""Prints Data sended by clients"""
		while True:
			DataClient = Client.recv(1024)
			print(DataClient)
			# your source code here
			message = DataClient
			# data to be sent to api
			data = {'message': message}
			# sending post request and saving response as response object
			r = requests.post(url = self.API_ENDPOINT, data = data)
			# extracting response text
			#pastebin_url = r.text
			#print("The pastebin URL is:%s"%pastebin_url)


	def __ClientProcess(self,Client):
		try:
			ClientPro = Process(target = self.__ServiceClient, args=(Client,))  # Delegate a Process to each Client
			ClientPro.daemon = True # Automatically the child process deads before his father terminates
			ClientPro.start()
			self.ProClient.append(ClientPro)    # Append Process-Client to a list

		except Exception as e:
			ClientPro.terminate()
			print("\nProcess can't create\n")



	def Run(self):
		while True:
			rlist, wlist, xlist = select.select(self.SelectList,[],[])

			if rlist[0] == self.SelectList[0]:
				Client,Addr = self.SERVER.accept()   	#Accept Client
				print("Connection from: "+str(Addr))
				if self.ArduinoAviable == False: 		#Accepted Client ARDUINO
					print("Arduino Connected")
					self.ArduinoAviable = True
					self.ClientArduino = Client
					self.__ClientProcess(Client)
					self.SockClient.append(Client)
				else:									#Accepted Client SERVER WEB
					DataClientServerWeb = Client.recv(1024)
					print(DataClientServerWeb)
					self.ClientArduino.sendall(DataClientServerWeb)
					Client.close()



			elif rlist[0] == self.SelectList[1]:		#KEYBOARD-TERMINAL
				DataKeyB = sys.stdin.readline()
				for ClientSock in self.SockClient:
					ClientSock.sendall(DataKeyB)
				if DataKeyB == "exit\n":
					self.__CloseSockClient()
					self.__CloseProClient()
					self.CloseServer()


			else:
				print("\nERROR")
		self.__CloseSockClient()
		self.__CloseProClient()
		self.CloseServer()


if __name__ == '__main__':
	# defining the api-endpoint
	API_ENDPOINT = "http://192.168.0.159:8000/api/messageServerPipe"
	Server = ServerPipe("192.168.0.159",2000,API_ENDPOINT) # ServerChat(IpServer,Port,Nombre de Clients)
	Server.Run()
