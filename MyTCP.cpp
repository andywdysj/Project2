//
//  MyTCP.cpp
//  CS118_Project2_Xcode
//
//  Created by Andrew on 2/17/18.
//  Copyright © 2018 Andrew. All rights reserved.
//

#include <string.h>
#include "MyTCP.hpp"
#include "Packet.hpp"
#include "ServerWindow.hpp"
#include <fstream>
#include <sys/socket.h>
#include <unistd.h>

#define WINDOW_SIZE 5

uint16_t total_pkt = 0;

using namespace std;

//wait funciton for testing
void wait ( double seconds )
{
    clock_t endwait;
    endwait = clock () + seconds * CLOCKS_PER_SEC ;
    while (clock() < endwait) {}
}
//wait funciton for testing

MyTCP::MyTCP()
{
    string temp = ".";
    m_port_num = 2222; //by default
    m_server_path = temp.data();
    m_seq_num = rand()%MAX_SEQ_NUM;
}

MyTCP::MyTCP(int port_num)
{
    m_port_num = port_num;
    //m_seq_num = rand()%MAX_SEQ_NUM;
    m_seq_num = 200;
    cout << "my seq no is: " << m_seq_num << endl;
}

void MyTCP::init()
{
    //configure server
    if((m_sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        cerr << "Error: socket creation fails." << endl;
        exit(1);
    }
    memset((char *)&myaddr, 0, sizeof(myaddr));
    myaddr.sin_port = htons(m_port_num);
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = INADDR_ANY;
    //bind
    if((::bind(m_sockfd, (struct sockaddr*) &myaddr, sizeof(myaddr))) < 0)
    {
        cerr << "Error: binding failure." << endl;
        exit(1);
    }
    //check dir validity
    if((m_server_dir_ptr = opendir(m_server_path)) == NULL)
    {
        cerr << "Error: cannot open directory: " << m_server_path << endl;
        exit(1);
    }
    cout << "Server has be initialized!" << endl;
    //TODO: lots of things...
    
    //initiaze client addr addrlen
    client_addlen = sizeof(client_addr);
}

int MyTCP::handshake()
{
    ssize_t rev_len = -1;
    while(true)
    {
        memset(rev_buffer, 0, MAX_PKT_LEN);
        rev_len = recvfrom(this->m_sockfd, rev_buffer, MAX_PKT_LEN, 0, (struct sockaddr*) &client_addr, &client_addlen);
        if(rev_len == -1)
        {
            cerr << "Error: unable to receive messege from client." << endl;
            exit(1);
        }
        else if(rev_len == 0)
            continue;
        //construct a rev_packet
        Payload mydata(rev_buffer, rev_buffer+rev_len);
        Packet rev_packet(mydata);
        if(rev_packet.is_SYN())
        {
            m_ack_num = rev_packet.get_seq_no()+1;
            //cout << "[1]Step 1 of 3-way handshake: SYN from client received: " << rev_packet.get_seq_no() << endl;
            //cout << "[2]My ACK number is: " << m_ack_num << endl;
            cout << "Receive SYN " << rev_packet.get_seq_no() << endl;
            auto i=rev_packet.get_payload()->begin();
            auto end = rev_packet.get_payload()->end();
            for(; i!=end; i++)
                m_requested_file += *i;
            if(m_requested_file == "")
                m_requested_file = "404.html";
            break;
        }
        else
        {
            cerr << "Not a SYN packet." << endl;
            continue;
        }
    }
    //send back SYNACK, read and chuck the file requested
    
    send_buffer.read_file(m_requested_file);
    //send_buffer.read_file("big_ori.txt");
    // load real data client wants
    Header header;
    header.ACK = true;
    header.SYN = true;
    header.ACK_NO = m_ack_num;
    header.SEQ_NO = m_seq_num;
    Payload* temp = new Payload;
    
    for(auto i=send_buffer.get_begin_queue_queue_iter(); i!=send_buffer.get_last_queue_queue_iter(); i++)
        total_pkt += (*i).size();
    //uint16_t total_pkt = 26;
    uint8_t head, tail;
    head = total_pkt >> 8;
    tail = total_pkt;
    (*temp).push_back(head);
    (*temp).push_back(tail);
    Packet pkt(&header, temp);
    Payload SYNACK = pkt.load_data();
    while(true)
    {
        if(sendto(m_sockfd, SYNACK.data(), SYNACK.size(), 0, (struct sockaddr*) &client_addr, client_addlen) < 0)
        {
            cerr << "Error: SYNACK sent failed" << endl;
            continue;
        }
        else
        {
            cout << "Send SYNACK " << pkt.get_seq_no() << " " << pkt.get_ack_no() << endl;
            //cout << "SYNACK sent!" << endl;
            break;
        }
    }
    
    //wait for receive first ack
    rev_len = -1;
    while(true)
    {
        memset(rev_buffer, 0, MAX_PKT_LEN);
        rev_len = ::recvfrom(this->m_sockfd, rev_buffer, MAX_PKT_LEN, 0, (struct sockaddr*) &client_addr, &client_addlen);
        if(rev_len == -1)
        {
            cerr << "Error: unable to receive messege from client." << endl;
            exit(1);
        }
        else if(rev_len == 0)
            continue;
        Payload mydata(rev_buffer, rev_buffer+rev_len);
        Packet rev_packet(mydata);
        if(rev_packet.is_ACK())
        {
            m_ack_num = rev_packet.get_seq_no()+rev_len+1;
            //
            cout << "Receive ACK " << rev_packet.get_seq_no() << " " << rev_packet.get_ack_no() << endl;
            cout << "Dummy test begins: " << endl;
            //send_buffer.read_file("big.txt");
            //cout << "First ACK received, my ACK num is " << m_ack_num << ". Sending file..." << endl;
            break;
        }
        else
        {
            cerr << "Error: This is not the first ACK pkt." << endl;
            continue;
        }
        
        
    }
    
    
    
    return 0; //TODO: create different return values
}

void MyTCP::send()
{
    //TODO: send big file for now, need to adapt to other files too
    //At this time, the big file has been chucked and put into m_queue_queue
    //TODO: update seq and ack no!!!
    int counter = 0;
    int window_counter = 1;
    int switcher = 0;
    send_buffer.feed_m_queue_from_front();
    while(counter != total_pkt) //!send_buffer.is_empty_queue_queue()
    {
        while(!(send_buffer.get_size_m_queue() == 0) && window_counter <= WINDOW_SIZE) //&& window_counter <= WINDOW_SIZE
        {
            int k = send_buffer.get_size_m_queue();
            Payload myPayload = send_buffer.get_begin_m_queue_pop();
            Header* myHeader = new Header;
            myHeader->SEQ_NO = window_counter;
            myHeader->ACK_NO = 456;
            Packet myPKT(myHeader, &myPayload);
            Payload data = myPKT.load_data();
            if(switcher == 0)
            {
                
            }
            while(true)
            {
                if(sendto(m_sockfd, data.data(), data.size(), 0, (struct sockaddr*) &client_addr, client_addlen) < 0)
                {
                    cerr << "Error: Normal data pkt sending failed" << endl;
                    continue;
                }
                else
                {
                    //push into deque and start timer
                    m_payload_window.push_back(data);
                    //m_timer[window_counter-1] = clock();
                    cout << "Normal data pkt send! No." << counter << endl;
                    counter++;
                    //test//
                    if(counter == 6380)
                        cout << " " << endl;
                    window_counter++;
                    break;
                }
            }
        }
        //get out either send buffer is empty, or window counter == 5
        if(send_buffer.is_empty_m_queue())
        {
            if(send_buffer.is_empty_queue_queue())
                continue;
            send_buffer.feed_m_queue_from_front();
            continue;
        }
        //server turns into listening state
        int expected = 0;
        clock_t t = clock();
        while(true)//
        {
            float time = (float)t/CLOCKS_PER_SEC;
            if(time > 600)
            {
                deque<Payload>::iterator it = m_payload_window.begin();
                for(; it!=m_payload_window.end(); it++)
                {
                    Payload data = *it;
                    while(true)
                    {
                        if(sendto(m_sockfd, data.data(), data.size(), 0, (struct sockaddr*) &client_addr, client_addlen) < 0)
                        {
                            cerr << "Error: Normal data pkt sending failed" << endl;
                            continue;
                        }
                        else
                        {
                            cout << "Normal data pkt RESEND!" << endl;
                            break;
                        }
                    }
                }
            }
            ssize_t rev_len = -1;
            memset(rev_buffer, 0, MAX_PKT_LEN);
            //listen to port
            //
            if((rev_len = recvfrom(m_sockfd, rev_buffer, MAX_PKT_LEN, 0, (struct sockaddr*) &client_addr, &client_addlen)) < 0)
            {
                cerr << "Error: error receiving data." << endl;
                continue;
            }
            //construct a pkt
            Payload mydata(rev_buffer, rev_buffer+rev_len);
            Packet rev_packet(mydata);
            if(rev_packet.is_ACK())
            {
                //do stuff...
                window_counter = 1;
                m_payload_window.clear();
                //expected = 1;
                
                break;
                //uint16_t ack_no = rev_packet.get_ack_no();
                //int num_of_payload;
            }
            else
            {
                cout << "Not an ACK." << endl;
                continue;
            }
            
        }
        
    //biggest while loop
    }
}

void MyTCP::expecting_fin()
{
    ssize_t rev_len = 0;
    if((rev_len = recvfrom(m_sockfd, rev_buffer, MAX_PKT_LEN, 0, (struct sockaddr*) &client_addr, &client_addlen)) < 0)
        cerr << "Error: error receiving data." << endl;
    
    
    
}
/*
 while(true)//
 {
 float time = (float)t/CLOCKS_PER_SEC;
 if(time > 0.5)
 {
 deque<Payload>::iterator it = m_payload_window.begin();
 for(; it!=m_payload_window.end(); it++)
 {
 Payload data = *it;
 while(true)
 {
 if(sendto(m_sockfd, data.data(), data.size(), 0, (struct sockaddr*) &client_addr, client_addlen) < 0)
 {
 cerr << "Error: Normal data pkt sending failed" << endl;
 continue;
 }
 else
 {
 cout << "Normal data pkt RESEND!" << endl;
 break;
 }
 }
 }
 }
 ssize_t rev_len = -1;
 memset(rev_buffer, 0, MAX_PKT_LEN);
 //listen to port
 //
 if((rev_len = recvfrom(m_sockfd, rev_buffer, MAX_PKT_LEN, 0, (struct sockaddr*) &client_addr, &client_addlen)) < 0)
 {
 cerr << "Error: error receiving data." << endl;
 continue;
 }
 //construct a pkt
 Payload mydata(rev_buffer, rev_buffer+rev_len);
 Packet rev_packet(mydata);
 if(rev_packet.is_ACK())
 {
 //do stuff...
 window_counter = 1;
 m_payload_window.clear();
 //expected = 1;
 
 break;
 //uint16_t ack_no = rev_packet.get_ack_no();
 //int num_of_payload;
 }
 else
 {
 cout << "Not an ACK." << endl;
 continue;
 }
 
 }
 
 */



















