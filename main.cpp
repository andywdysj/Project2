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
#include <list>

int sockfd;
struct sockaddr_in serveraddr;

bool compare_function(const Payload &lhs, const Payload &rhs)
{
    uint8_t first, last;
    first = lhs[0];
    last = lhs[1];
    uint16_t lhs_seq_no = first << 8 | last;
    
    first = rhs[0];
    last = rhs[1];
    uint16_t rhs_seq_no = first << 8 | last;
    
    if(lhs_seq_no < rhs_seq_no)
        return true;
    
    return false;
}

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

bool is_pkt_already_received(int array[], Packet* p)
{
    uint16_t seq = p->get_seq_no();
    if(array[seq-1] == 1)
        return true;
    else
        return false;
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
    int check_array[20];
    for(int i=0; i<20; i++)
        check_array[i] = 0;
    uint16_t total_pkt_num;
    list<Payload> payload_list;
    char buffer[MAX_PKT_LEN];
    const char* hostname = "127.0.0.1";
    int port = 2222;
    char filename[50] = "big_ori.txt"; //can be changed
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
    Payload* ip = SYN.get_payload();
    
    for(int i=0; i<strlen(filename); i++)
        (*ip).push_back(filename[i]);
    
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
            Payload temp = *(pkt.get_payload());
            uint8_t head, tail;
            head = temp[0];
            tail = temp[1];
            total_pkt_num = head << 8 | tail;
            
            cout << "total pkt are: " << total_pkt_num << endl;
            
            
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
    int received = 1;
    uint16_t num_of_pkt = 0;
    while(num_of_pkt != total_pkt_num)
    {
        if(num_of_pkt == 6374)
            cout << "" << endl;
        memset(buffer, 0, MAX_PKT_LEN);
        long rcv_len = -1;
        if((rcv_len = recvfrom(sockfd, buffer, MAX_PKT_LEN, 0, (struct sockaddr*) &serveraddr, &addrlen)) < 0)
        {
            cerr << "Error: error receiving data." << endl;
            continue;
        }
        Payload raw_data(buffer, buffer+rcv_len);
        //examine if the pkt receive is a dup
        Packet exam(raw_data);
        bool in = is_pkt_already_received(check_array, &exam);
        if(in == true)
            continue;
        else
        {
            check_array[exam.get_seq_no()-1] = 1;
            payload_list.push_back(raw_data);
            received++;
            num_of_pkt++;
            if(received == 5+1 || num_of_pkt == total_pkt_num)
            {
                //reorder
                payload_list.sort(compare_function);
                Packet ACK;
                ACK.set_ACK(true);
                Payload ACKdata = ACK.load_data();
                if(sendto(sockfd, ACKdata.data(), ACKdata.size(), 0, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
                {
                    cerr << "Error: Normal ACK sending final ack." << endl;
                    return -1;
                }
                cout << "Normal ack sent!" << endl;
                received = 1;
            }
            else
            {
                continue;
            }

        }
            
        for(list<Payload>::iterator iti = payload_list.begin(); iti!=payload_list.end(); iti++)
        {
            Packet dataPKT(payload_list.front());
            payload_list.pop_front();
            Payload* P = dataPKT.get_payload();
            
            char c;
            pFile=fopen("receive.txt","a+b");
            Payload::iterator it = P->begin();
            for(; it != P->end(); it++)
            {
                c = *it;
                putc(c, pFile);
            }
            fclose(pFile);
        }
        //zero the check array
        for(int i=0; i<20; i++)
            check_array[i] = 0;
        
        //test only
    }
    
    //client terminates the connection
    Packet fin;
    fin.set_FIN(true);
    Payload FIN = fin.load_data();
    //TODO: seq no and so
    int times = 0;
    while(true)
    {
        if(times == 1)
            break;
        if(sendto(sockfd, FIN.data(), FIN.size(), 0, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
        {
            cout << "FIN sent failure" << endl;
            continue;
        }
        ssize_t len_of_receive = 0;
        
        clock_t timer = clock();
        while(((clock()-timer)/CLOCKS_PER_SEC) <= 0.5 && times == 0)
        {
            if((len_of_receive = recvfrom(sockfd, buffer, MAX_PKT_LEN, 0, (struct sockaddr*) &serveraddr, &addrlen)) < 0)
            {
                cout << "error receiving ACK" << endl;
                continue;
            }
            else
            {
                Payload received(buffer, buffer+len_of_receive);
                Packet finack(received);
                if(finack.is_ACK())
                {
                    times = 1;
                    break;
                }
            }
        }
        
    }
    
    //expect fin
    times = 0;
    while(true)
    {
        ssize_t len_of_receive = 0;
        if((len_of_receive = recvfrom(sockfd, buffer, MAX_PKT_LEN, 0, (struct sockaddr*) &serveraddr, &addrlen)) < 0)
        {
            cout << "error receiving FIN" << endl;
            continue;
        }
        else
        {
            Payload received(buffer, buffer+len_of_receive);
            Packet finack(received);
            if(finack.is_FIN())
                break;
        }
    }
    
    //send ACK
    Packet FinalACK;
    FinalACK.set_ACK(true);
    if(sendto(sockfd, FIN.data(), FIN.size(), 0, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
        cout << "FIN sent failure" << endl;
    
    
    
    return 0;
}
