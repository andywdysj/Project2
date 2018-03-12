# Project2
Using UDP features to implement MyTCP protocol


(make Makefile)
to compile server:
g++ -std=c++0x server.cpp MyTCP.cpp Packet.cpp ServerWindow.cpp -o server

to use server:
./server #portnumber

to compile client:
g++ client.cpp TCPclient.cpp Packet.cpp -o client

to use:
./client hostname #portnumber filename
