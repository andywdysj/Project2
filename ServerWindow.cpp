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
#include <iterator>
using namespace std;

ServerWindow::ServerWindow()
{
    anchor = m_queue_queue.begin();
}

ServerWindow::ServerWindow(int num)
{
    m_queue = deque<Payload>(num, {'C'});
    anchor = m_queue_queue.begin();
}

bool ServerWindow::is_empty_m_queue()
{
    return m_queue.empty();
}

Payload ServerWindow::get_last_m_queue_pop()
{
    Payload last = m_queue.back();
    m_queue.pop_back();
    return last;
}

Payload ServerWindow::get_begin_m_queue_pop()
{
    Payload first = m_queue.front();
    m_queue.pop_front();
    return first;
}

int ServerWindow::get_size_m_queue()
{
    return (int)m_queue.size();
}

void ServerWindow::read_file(string filename)
{
    //streampos size;
    char* block;
    ifstream file(filename, std::ios::in|std::ios::ate|std::ios::binary);
    if(file.is_open())
    {
        size = file.tellg();
        block = new char[size];
        file.seekg(0,std::ios::beg);
        file.read(block, size);
        file.close();
        //test file chucking only
        int count = 1;
        const unsigned int threshhold = 255;
        //test file chucking only
        int i = 0;
        while(i+MAX_PKT_LEN-7 < size)
        {
            Payload temp(block+i, block+i+MAX_PKT_LEN-7);
            m_queue.push_back(temp);
            i+=MAX_PKT_LEN-7;
            //test file chucking only
            if(count == threshhold)
            {
                cout << "count is: " << count << endl;
                cout << "threshhold is: " << threshhold << endl;
                cout << "=====resizeing=====" << endl;
                //reallocate memory by calling resize_queue
                resize_queue();
                
                count = 0;
            }
            count++;
            //test file chucking only
        }
        
        //resize if count == threshhold
        if(count == threshhold)
            resize_queue();
        
        Payload temp(block+i, block+size);
        m_queue.push_back(temp);
        
        m_queue_queue.push_back(m_queue);
    }
    else
    {
        cerr << "Error: unable to open file." << endl;
        return;
    }
    file.close();
    return;
}

void ServerWindow::resize_queue()
{
    m_queue_queue.push_back(m_queue);
    m_queue.clear();
}

void ServerWindow::feed_m_queue_from_front()
{
    m_queue = m_queue_queue.front();
    m_queue_queue.pop_front();
    return;
}

