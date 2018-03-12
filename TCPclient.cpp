//
//  TCPclient.cpp
//  CS118_Project2_Xcode
//
//  Created by Jeannie Chiem on 2/25/18.
//  Copyright © 2018 Andrew. All rights reserved.
//

#include "TCPclient.hpp"
#include "Packet.hpp"

using namespace std;

TCPclient::TCPclient(string host, int portno, string filename)
{
    m_portno = portno;
    
    m_sockfd = socket(AF_INET, SOCK_DGRAM, 0);  //create socket
    if (m_sockfd < 0)
    {
        cerr << "Error: failed to create new socket" << endl;
        exit(1);
    }
    
    m_server = gethostbyname(host.data());  //get host
    if (m_server == NULL)
    {
        cerr << "Error: failed to find host" << endl;
        exit(1);
    }
    
    cerr << "Successful init!" << endl;
}

void TCPclient::handshake()
{
    memset((char *)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_port = htons(m_portno);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    memcpy((void *)&serv_addr.sin_addr, m_server->h_addr_list[0], m_server->h_length);
    
    serv_addlen = sizeof(serv_addr);

    //send initial SYN packet
    Packet pkt;
    pkt.set_SYN(true);
    Payload InitPkt = pkt.load_data();
    
    cout << "Sending Packet SYN" << endl;
    
    while(true)
    {
         if (sendto(m_sockfd, InitPkt.data(), InitPkt.size(), 0, (struct sockaddr *)&serv_addr, serv_addlen) < 0)
         {
             cerr << "Error: SYN did not send, retrying..." << endl;
             continue;
         }
         else
         {
             cerr << "SYN sent!" << endl;
             break;
         }
    }
    
    cout << "Receiving Packet " << m_next_pkt << endl;
    
}

void TCPclient::recv()
{
    ssize_t rev_len = -1;
    memset(rev_buffer, 0, 1500);
    
    while(true)
    {
        rev_len = ::recvfrom(this->m_sockfd, rev_buffer, 1500, 0, (struct sockaddr*) &serv_addr, &serv_addlen); //receive packet
        if(rev_len == -1)
        {
            cerr << "Error: unable to receive message from client." << endl;
            exit(1);
        }
        else if(rev_len == 0)
        {
            continue;
        }
        
        /*ignore for now
        Packet rev_packet = Packet((Header*)rev_buffer, (Payload*)rev_buffer+8);
        m_ack_num = rev_packet.get_seq_no();
        m_next_pkt = rev_packet.get_ack_no();
        */
        
        //send ACK
        Packet pkt;
        pkt.set_ACK(true);
        pkt.set_ack_no(m_ack_num); //ACK number = SEQ number of received packet
        //Data_Package dpkt = pkt.get_data_package();
        Payload ACKpkt = pkt.load_data();
        
        
        cout << "Sending Packet " << m_ack_num << endl;
        
        /*
        if(packet is a retransmission)
        {
            cout << " Retransmission";
        }
        else if(rev_packet.is_FIN())
        {
            cout << " FIN";
        }
        cout << "\n";
        */
        
        while(true)
        {
            if (sendto(m_sockfd, ACKpkt.data(), ACKpkt.size(), 0, (struct sockaddr *)&serv_addr, serv_addlen) < 0)
            {
                cerr << "Error: ACK did not send, retrying..." << endl;
                continue;
            }
            else
            {
                cerr << "ACK sent!" << endl;
                break;
            }
        }
        
        /*
        if(rev_packet.is_FIN())
        {
            do FIN-ACK procedure
        }
        */
        
        cout << "Receiving Packet " << m_next_pkt << endl;

    }

}


