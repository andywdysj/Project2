//
//  TCPclient.hpp
//  CS118_Project2_Xcode
//
//  Created by Jeannie Chiem on 2/25/18.
//  Copyright © 2018 Andrew. All rights reserved.
//

#ifndef TCPclient_hpp
#define TCPclient_hpp

#include <stdio.h>
#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/un.h>
#include <cstring>
#include <string>
#include <dirent.h>


using namespace std;

class TCPclient
{
public:
    TCPclient(string host, int portno, string filename);
    void init();
    void recv();
    
private:
    int m_sockfd;
    int m_portno;
    string m_host;
    
    struct hostent *m_server;
    struct sockaddr_in serv_addr;
    socklen_t serv_addlen;
    
    uint16_t m_next_pkt;
    uint16_t m_ack_num;
    char rev_buffer[1500];
    
};

#endif /* TCPclient_hpp */
