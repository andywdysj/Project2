//
//  MyTCP.hpp
//  CS118_Project2_Xcode
//
//  Created by Andrew on 2/17/18.
//  Copyright © 2018 Andrew. All rights reserved.
//

#ifndef MyTCP_hpp
#define MyTCP_hpp

#include <iostream>
#include <stdio.h>
#include <cstring>
#include <string>
#include <dirent.h>
#include <stdlib.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "Packet.hpp"
#include "ServerWindow.hpp"
using namespace std;

class MyTCP
{
public:
    MyTCP();
    MyTCP(int port_num);
    
    void init();
    int handshake();
    void send();
    void expecting_fin();
    
    ServerWindow* get_ServerWindow(){return &send_buffer;}
    
    
    
private:
    int m_port_num;
    int m_sockfd;
    uint16_t m_seq_num;
    uint16_t m_ack_num;
    const char* m_server_path = ".";
    DIR* m_server_dir_ptr;
    
    struct sockaddr_in myaddr;
    struct sockaddr_in client_addr;
    socklen_t client_addlen;
    
    char rev_buffer[MAX_PKT_LEN];
    ServerWindow send_buffer;
    
    uint16_t cwnd;
    uint16_t ssthresh;
    
};

#endif /* MyTCP_hpp */
