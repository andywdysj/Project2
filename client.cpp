//
//  client.cpp
//  CS118_Project2_Xcode
//
//  Created by Jeannie Chiem on 2/26/18.
//  Copyright © 2018 Andrew. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
#include <strings.h>
#include <string.h>
#include <sys/stat.h>
#include <pthread.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "Packet.hpp"
#include "TCPclient.hpp"

using namespace std;

int main(int argc, const char** argv)
{
   
    if (argc != 4)
    {
        cerr << "Error: the number of parameters is incorrect" << endl;
        exit(0);
    }
    
    string host = argv[1];
    int port = atoi(argv[2]);
    string filename = argv[3];
    
    ////////////test

    TCPclient client(host, port, filename);
    client.handshake();
    client.recv();
}
