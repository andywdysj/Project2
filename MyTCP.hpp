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
#include <list>
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
    
    clock_t* get_clock_array(){return clock_array;}
    int get_window_size(){return window;};
    Payload get_ith_payload_window(int i){return payload_window[i];}
    int get_sockfd(){return m_sockfd;}
    struct sockaddr_in* get_client_addr(){return &client_addr;}
    socklen_t  get_client_addlen(){return client_addlen;}
    int* get_acked_array(){return acked_array;}
    
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
    
    int window = 5;
    vector<Payload> payload_window;
    clock_t clock_array[20];
    int acked_array[20];
    
};

#endif /* MyTCP_hpp */
