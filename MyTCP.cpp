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

using namespace std;

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
    m_seq_num = 100;
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
            cout << "[1]Step 1 of 3-way handshake: SYN from client received: " << rev_packet.get_seq_no() << endl;
            cout << "[2]My ACK number is: " << m_ack_num << endl;
            break;
        }
        else
        {
            cerr << "Not a SYN packet." << endl;
            continue;
        }
    }
    //send back SYNACK
    Packet pkt;
    pkt.set_ACK(true);
    pkt.set_SYN(true);
    pkt.set_ack_no(m_ack_num);
    pkt.set_seq_no(m_seq_num);
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
            cout << "SYNACK sent!" << endl;
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
            send_buffer.read_file("Test.bin");
            cout << "First ACK received, my ACK num is " << m_ack_num << ". Sending file..." << endl;
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



















