//
//  Packet.hpp
//  CS118_Project2_Xcode
//
//  Created by Andrew on 2/17/18.
//  Copyright © 2018 Andrew. All rights reserved.
//

#ifndef Packet_hpp
#define Packet_hpp

#include <stdio.h>
#include <random>
#include <vector>
#include <stdlib.h>
#include <cstdint>
#include <sys/socket.h>
#include <bitset>
using namespace std;

#define MAX_PKT_LEN 1024
#define PAYLOAD_SIZE 1017 //1024-2-2-1-1-1 = 1017; 1017-1 = 1016 due to alignment
#define MAX_SEQ_NUM 30720
#define WND_SIZE 5120
#define RESTRANS_TIMEOUT 500 //ms

typedef vector<char> Payload; //construct payload section by deviding payload into 1B block


struct Header
{
    uint16_t SEQ_NO;
    uint16_t ACK_NO;
    bool ACK;
    bool FIN;
    bool SYN;
    Header()
    {
        SEQ_NO = 0;
        ACK_NO = 0;
        ACK = false;
        FIN = false;
        SYN = false;
    }
}; //header constructor, total 8 bytes

struct Data_Package
{
    Header data_header;
    Payload data_payload;
};

class Packet
{
public:
    Packet(uint16_t SEQ_NUM=0, uint16_t ACK_NO=0, bool ACK=false, bool SYN=false, bool FIN=false);
    Packet(Header* head_ptr, Payload* payload_ptr);
    Packet(Payload &rcvData);
    Header* get_header();
    Payload* get_payload();
    uint16_t get_seq_no();
    void set_seq_no(uint16_t new_seq_no);
    uint16_t get_ack_no();
    void set_ack_no(uint16_t new_ack_no);
    bool is_ACK();
    void set_ACK(bool ack);
    bool is_SYN();
    void set_SYN(bool syn);
    bool is_FIN();
    void set_FIN(bool fin);
    Data_Package get_data_package();
    Payload load_data();
private:
    Header m_header;
    Payload m_payload; //max capacity of payload is 1016 bytes
};


//uint16_t get_random_SEQ_NUM();
int get_event_type();


#endif /* Packet_hpp */
