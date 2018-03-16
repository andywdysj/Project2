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
    server.send();
    server.expecting_fin();
    
    /*
    
    //test file chucking
    deque<deque<Payload>>::iterator it = server.get_ServerWindow()->get_begin_queue_queue_iter();
    deque<deque<Payload>>::iterator end = server.get_ServerWindow()->get_last_queue_queue_iter();
    while(it != end)
    {
        deque<Payload>::iterator DPit = (*it).begin();
        deque<Payload>::iterator DPit_end = (*it).end();
        for(;DPit != DPit_end; DPit++)
        {
            Payload::iterator Pit = (*DPit).begin();
            Payload::iterator Pit_end = (*DPit).end();
            for(; Pit != Pit_end; Pit++)
            {
                cout << *Pit;
            }
            
        }
        it++;
    }
    */
    
    
    
    return 0;
    /*/*
     Payload a = send_buffer.get_begin_m_queue_pop();
     Header* aH = new Header;
     aH->SEQ_NO = 1;
     Packet aP(aH, &a);
     Payload aS = aP.load_data();
     
     Payload b = send_buffer.get_begin_m_queue_pop();
     Header* bH = new Header;
     bH->SEQ_NO = 2;
     Packet bP(bH, &b);
     Payload bS = bP.load_data();
     
     
     Payload c = send_buffer.get_begin_m_queue_pop();
     Header* cH = new Header;
     cH->SEQ_NO = 3;
     Packet cP(cH, &c);
     Payload cS = cP.load_data();
     
     
     Payload d = send_buffer.get_begin_m_queue_pop();
     Header* dH = new Header;
     dH->SEQ_NO = 4;
     Packet dP(dH, &d);
     Payload dS = dP.load_data();
     
     
     Payload e = send_buffer.get_begin_m_queue_pop();
     Header* eH = new Header;
     eH->SEQ_NO = 5;
     Packet eP(eH, &e);
     Payload eS = eP.load_data();
     
     vector<Payload> pv;
     pv.push_back(bS); //2
     pv.push_back(cS); //3
     pv.push_back(eS); //5
     pv.push_back(aS); //1
     pv.push_back(dS); //4
     
     Payload data = pv[0];
     sendto(m_sockfd, data.data(), data.size(), 0, (struct sockaddr*) &client_addr, client_addlen);
     
     data = pv[1];
     sendto(m_sockfd, data.data(), data.size(), 0, (struct sockaddr*) &client_addr, client_addlen);
     
     data = pv[2];
     sendto(m_sockfd, data.data(), data.size(), 0, (struct sockaddr*) &client_addr, client_addlen);
     
     data = pv[3];
     sendto(m_sockfd, data.data(), data.size(), 0, (struct sockaddr*) &client_addr, client_addlen);
     
     data = pv[4];
     sendto(m_sockfd, data.data(), data.size(), 0, (struct sockaddr*) &client_addr, client_addlen);
     
     data = pv[4];
     sendto(m_sockfd, data.data(), data.size(), 0, (struct sockaddr*) &client_addr, client_addlen);
     */
    
    
}
