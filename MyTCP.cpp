//
//  MyTCP.cpp
//  CS118_Project2_Xcode
//
//  Created by Andrew on 2/17/18.
//  Copyright © 2018 Andrew. All rights reserved.
//

#include "MyTCP.hpp"
#include "Packet.hpp"
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
    string temp = ".";
    m_port_num = port_num;
    m_server_path = temp.data();
    m_seq_num = rand()%MAX_SEQ_NUM;
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
    cout << "Server has been initialized!" << endl;
    //TODO: lots of things...
}

int MyTCP::handshake()
{
    ssize_t rev_len = -1;
    while(true)
    {
        memset(rev_buffer, 0, 1500);
        rev_len = recvfrom(this->m_sockfd, rev_buffer, 1500, 0, (struct sockaddr*) &client_addr, &client_addlen);
        if(rev_len == -1)
        {
            cerr << "Error: unable to receive messege from client." << endl;
            exit(1);
        }
        else if(rev_len == 0)
            continue;
        //construct a rev_packet
        Packet rev_packet = Packet((Header*)rev_buffer, (Payload*)rev_buffer+8);
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
    Data_Package dpkt = pkt.get_data_package();
    
    
    return 0; //TODO: create different return values
}

/*
 TODO: ACK handling; SR needs buffer for out of order ACKs
 */

void MyTCP::send()
{
    
}

void MyTCP::tear_down()
{
    
}

















