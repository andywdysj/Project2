//
//  main.cpp
//  CS118_Project2_Xcode
//
//  Created by Andrew on 2/17/18.
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
#include "MyTCP.hpp"
#include "Packet.hpp"
#include "ServerWindow.hpp"
using namespace std;

int main(int argc, const char * argv[])
{
    //commented for testing
    /*/Users/Andrew/Desktop/CS118_Project2_Xcode/CS118_Project2_Xcode/server.cpp
    if(argc != 2)
    {
        cerr << "Error: number of parameters incorrect." << endl;
        return(1);
    }
     */
    
    //for test only
    /*
    vector<char> test;
    test.assign(1016, 'a');
    cout << test.capacity() << endl;
    cout << sizeof(Header) + test.capacity() << endl;
    cout << sizeof(Packet) << endl;
    cout << sizeof(Packet)-sizeof(Header) << endl;
     */
    /*
    if (argc!=2)
    {
        std::cout<<"Usage: server + Port_number\n";
        exit(1);
    }
     */
    int port = 2222;
    MyTCP server(port);
    server.init();
    server.handshake();
    
    
    
    return 0;
    
    
}
