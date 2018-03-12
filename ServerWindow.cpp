//
//  ServerWindow.cpp
//  CS118_Project2_Xcode
//
//  Created by Andrew on 3/11/18.
//  Copyright © 2018 Andrew. All rights reserved.
//

#include "ServerWindow.hpp"
#include <fstream>
#include <iostream>
using namespace std;

ServerWindow::ServerWindow()
{
    
}

ServerWindow::ServerWindow(int num)
{
    m_queue = deque<Payload>(num, {'C'});
}

bool ServerWindow::is_empty()
{
    return m_queue.empty();
}

Payload ServerWindow::get_last()
{
    Payload last = m_queue.back();
    m_queue.pop_back();
    return last;
}

Payload ServerWindow::get_begin()
{
    Payload first = m_queue.front();
    m_queue.pop_front();
    return first;
}

int ServerWindow::get_size()
{
    return (int)m_queue.size();
}

void ServerWindow::read_file(string filename)
{
    streampos size;
    char* block;
    ifstream file(filename, std::ios::in|std::ios::ate|std::ios::binary);
    if(file.is_open())
    {
        size = file.tellg();
        block = new char[size];
        file.seekg(0,std::ios::beg);
        file.read(block, size);
        file.close();
        int i = 0;
        while(i+MAX_PKT_LEN-7 < size)
        {
            Payload temp(block+i, block+i+MAX_PKT_LEN-7);
            m_queue.push_back(temp);
            i+=MAX_PKT_LEN-7;
        }
        Payload temp(block+i, block+size);
        m_queue.push_back(temp);
    }
    else
    {
        cerr << "Error: unable to open file." << endl;
    }
    return;
}

