Client/Server application for dynamic text communication.

Task:
- client and server has to start communication via port 7300.
- server can request to change communication port in any time.
- client also can request to change communication port in any time.

Ex: Communication established via port 7300.
- Text messages can go in both directions. 
- To change communication port client or server has to send message "NewPort-<PortNumber>"
- Client or server has to replay "success" or "failure"
- In case of success communication will be re-established via New port

WAS NOT IMPLEMENTED:</br>
Bonus option: communication port must be changed each 45 seconds without user disturbing.

How to build project:</br>
server side:
1. ```cd server/```
2. Run ```cmake .```
3. run ```make```

client side:</br>
1. ```cd client/```
2. Run ```cmake .```
3. Run ```make```

Work with server/client:
1. To start server/client run ```./server <port_number>```</br>
E.g: ```./server 7300```</br>
E.g: ```./client 7300```</br>
Note: client always sends messages to server first.
2. To change port enter on either side ```NewPort-<port_number>``` command.</br>
Note: port number must be numberical value in range from 1024 to 49151.
3. To stop either side enter ```Stop``` command.</br>
Note: If client disconnected, server will keep listening to new connections. If server disconnected, client will also stop working.</br>
Note: You cannot send ```Stop``` command on server if it doesn't have any active client connections.</br>
Note: Clinet can recconect with relevant server port executing ```./client <port_number>```
