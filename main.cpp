//
//  main.cpp
//  CS118_client
//
//  Created by Andrew on 3/11/18.
//  Copyright © 2018 Andrew. All rights reserved.
//

#include <iostream>
#include "Packet.hpp"
#include <string>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <fstream>

int sockfd;
struct sockaddr_in serveraddr;

void send_fin()
{
    //TODO: update seq and ack num!!!
    Packet FIN;
    FIN.set_seq_no(123);
    FIN.set_ack_no(456);
    FIN.set_FIN(true);
    Payload FINdata = FIN.load_data();
    while(true)
    {
        if(sendto(sockfd, FINdata.data(), FINdata.size(), 0, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
        {
            cerr << "Error: FIN sent failed" << endl;
            continue;
        }
        else
        {
            cout << "FIN sent!" << endl;
            break;
        }
    }
}


int main(int argc, const char * argv[])
{
    /*
    if(argc != 4)
    {
        cout << "Error: wrong input." << endl;
        return 1;
    }
    */
    //construct a client
    char buffer[MAX_PKT_LEN];
    const char* hostname = argv[1];
    int port = atoi(argv[2]);
    const char* filename = "Test.txt";
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0)
    {
        cerr << "Error: can not open socket" << endl;
        return -1;
    }
    
    
    struct hostent* server;
    socklen_t addrlen = sizeof(serveraddr);
    
    server = gethostbyname(hostname);
    if(server == NULL)
    {
        cerr << "Error: can not find server." << endl;
        return -1;
    }
    memset((char *)&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    memcpy((char *)&serveraddr.sin_addr.s_addr, (char *)server->h_addr, server->h_length);
    serveraddr.sin_port = htons(port);
    
    //3-way handshack with designated server
    //initialize SYN packet
    //uint16_t SEQ_NUM = rand() % MAX_SEQ_NUM;
    uint16_t SEQ_NUM = 100;
    uint16_t ACK_NUM = 0; //unkown ack at this time
    cout << "[!]Picked sequence number: " << SEQ_NUM << endl;
    Packet SYN;
    SYN.set_SYN(true);
    SYN.set_seq_no(SEQ_NUM);
    Payload SYN_payload = SYN.load_data();
    
    //test only .
    Packet test(SYN_payload);
    cout << "test SEQ: " << test.get_seq_no() << endl;
    cout << "test ACK: " << test.get_ack_no() << endl;
    //test only
    
    cout << "[!]Sending SYN packet" << endl;
    
    //sending SYN packet
    if(sendto(sockfd, SYN_payload.data(), SYN_payload.size(), 0, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
    {
        cerr << "Error: failed to send to server." << endl;
        return -1;
    }
    
    //waiting for SYNACK
    while(true)
    {
        memset(buffer, 0, MAX_PKT_LEN);
        long rcv_len = -1;
        if((rcv_len = recvfrom(sockfd, buffer, MAX_PKT_LEN, 0, (struct sockaddr*) &serveraddr, &addrlen)) < 0)
        {
            cerr << "Error: error receiving data." << endl;
            continue;
        }
        //construct pkt using data
        Payload receive(buffer, buffer+rcv_len);
        Packet pkt(receive);
        if(pkt.is_SYN() && pkt.is_ACK())
        {
            cout << "[!]Received SYNACK pkt, Seq is: " << pkt.get_seq_no() << endl;
            cout << "[!]Received SYNACK pkt, Ack is: " << pkt.get_ack_no() << endl;
            ACK_NUM = pkt.get_seq_no() + 1;
            SEQ_NUM = pkt.get_ack_no();
            cout << "[!]My SEQ is: " << SEQ_NUM << endl;
            cout << "[!]My ACK is: " << ACK_NUM << endl;
            break;
        }
        else
        {
            cout << "This is not SYNACK" << endl;
            continue;
        }
    }
    
    //send final ACK
    Packet final_ACK;
    final_ACK.set_ACK(true);
    final_ACK.set_ack_no(ACK_NUM);
    final_ACK.set_seq_no(SEQ_NUM);
    Payload final_ack_payload = final_ACK.load_data();
    if(sendto(sockfd, final_ack_payload.data(), final_ack_payload.size(), 0, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
    {
        cerr << "Error: error sending final ack." << endl;
        return -1;
    }
    cout << "Final ACK is sent!" << endl;
    
    //now expecting normal pkts for big file
    //write to a file in dir called received.txt
    FILE * pFile;
    while(true)
    {
        memset(buffer, 0, MAX_PKT_LEN);
        long rcv_len = -1;
        if((rcv_len = recvfrom(sockfd, buffer, MAX_PKT_LEN, 0, (struct sockaddr*) &serveraddr, &addrlen)) < 0)
        {
            cerr << "Error: error receiving data." << endl;
            continue;
        }
        Payload data(buffer, buffer+rcv_len);
        Packet dataPKT(data);
        //test only
        Payload* P = dataPKT.get_payload();
        
        char c;
        pFile=fopen("receive.jpg","a+b");
        Payload::iterator it = P->begin();
        for(; it != P->end(); it++)
        {
            c = *it;
            putc(c, pFile);
        }
        fclose(pFile);
        //test only
    }
    //fclose(pFile);
    
    
    
    return 0;
}
