//
//  Packet.cpp
//  CS118_Project2_Xcode
//
//  Created by Andrew on 2/17/18.
//  Copyright © 2018 Andrew. All rights reserved.
//

#include <iostream>
#include "Packet.hpp"
using namespace std;

/*
uint16_t get_random_SEQ_NUM()
{
    random_device X;
    default_random_engine engine(X);
    uniform_int_distribution<uint16_t> uniform_dist(0, MAX_SEQ_NUM-1);
    return uniform_dist(engine);
}
*/

Packet::Packet(uint16_t SEQ_NUM, uint16_t ACK_NO, bool ACK, bool SYN, bool FIN)
{
    this->m_header.SEQ_NO = SEQ_NUM;
    this->m_header.ACK_NO = ACK_NO;
    this->m_header.ACK = ACK;
    this->m_header.SYN = SYN;
    this->m_header.FIN = FIN;
}

Packet::Packet(Header* head_ptr, Payload* payload_ptr)
{
    m_header = *head_ptr;
    m_payload = *payload_ptr;
    //TODO: not sure if this is right, further tests needed
}

Packet::Packet(Payload &rcvData)
{
    uint8_t first, last;
    if(&rcvData == NULL)
        first = 0;
    first = rcvData[0];
    last = rcvData[1];
    uint16_t temp = first << 8 | last;
    m_header.SEQ_NO = temp;
    
    first = rcvData[2];
    last = rcvData[3];
    temp = first << 8 | last;
    m_header.ACK_NO = temp;
    
    m_header.ACK = rcvData[4];
    m_header.FIN = rcvData[5];
    m_header.SYN = rcvData[6];

    if(rcvData.size() > 7)
        m_payload = vector<char>(rcvData.begin() + 7,rcvData.end());
}

Header* Packet::get_header()
{
    return &(this->m_header);
}

Payload* Packet::get_payload()
{
    return &(this->m_payload);
}

uint16_t Packet::get_seq_no()
{
    return this->m_header.SEQ_NO;
}

void Packet::set_seq_no(uint16_t new_seq_no)
{
    m_header.SEQ_NO = new_seq_no;
}

uint16_t Packet::get_ack_no()
{
    return this->m_header.ACK_NO;
}

void Packet::set_ack_no(uint16_t new_ack_no)
{
    m_header.ACK_NO = new_ack_no;
}

bool Packet::is_ACK()
{
    return this->m_header.ACK;
}

void Packet::set_ACK(bool ack)
{
    m_header.ACK = ack;
}

bool Packet::is_SYN()
{
    return this->m_header.SYN;
}

void Packet::set_SYN(bool syn)
{
    m_header.SYN = syn;
}

bool Packet::is_FIN()
{
    return this->m_header.FIN;
}

void Packet::set_FIN(bool fin)
{
    m_header.FIN = fin;
}

Data_Package Packet::get_data_package()
{
    Data_Package temp;
    temp.data_header = this->m_header;
    temp.data_payload = this->m_payload;
    return temp;
}

Payload Packet::load_data()
{
    Payload result;
    uint8_t first, last;
    first = m_header.SEQ_NO >> 8;
    last = m_header.SEQ_NO;
    result.push_back(first);
    result.push_back(last);
    
    first = m_header.ACK_NO >> 8;
    last = m_header.ACK_NO;
    result.push_back(first);
    result.push_back(last);
    
    result.push_back(m_header.ACK);
    result.push_back(m_header.FIN);
    result.push_back(m_header.SYN);
    
    result.insert(result.end(), this->m_payload.begin(), this->m_payload.end());
    return result;
}

