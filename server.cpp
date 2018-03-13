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

#define NUM_THREADS 5

mutex lock1;
mutex lock2;

void* check_clock_and_resend(void* srv)
{
    while(true) //TODO: need to be changed
    {
        MyTCP* server = (MyTCP*)srv;
        lock1.lock();
        clock_t* clocks = server->get_clock_array();
        int window_size = server->get_window_size();
        lock1.unlock();
        for(int i=0; i<window_size; i++)
        {
            lock1.lock();
            clock_t t = clocks[i];
            lock1.unlock();
            t = clock() - t;
            if((float)t >= 500.00)
            {
                //resend i pkt
                //TODO: lock
                lock1.lock();
                Payload p = server->get_ith_payload_window(i);
                lock1.unlock();
                //sendto(m_sockfd, SYNACK.data(), SYNACK.size(), 0, (struct sockaddr*) &client_addr, client_addlen)
                while(true)
                {
                    if(sendto(server->get_sockfd(), p.data(), p.size(), 0, (struct sockaddr*)server->get_client_addr(), server->get_client_addlen()) < 0)
                    {
                        cerr << "Resend failed." << endl;
                        continue;
                    }
                    else
                    {
                        cout << "Resend successed." << endl;
                        break;
                    }
                }
            }
            
        }
    }
}

/*
void* update_ack_list(void* srv)
{
    MyTCP* server = (MyTCP*)srv;
    int* ack_arr = server->get_acked_array();
    
}
*/

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
    pthread_t threads1;
    pthread_t threads2;
    
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
    
    
}
