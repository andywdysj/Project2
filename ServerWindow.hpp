//
//  ServerWindow.hpp
//  CS118_Project2_Xcode
//
//  Created by Andrew on 3/11/18.
//  Copyright © 2018 Andrew. All rights reserved.
//

#ifndef ServerWindow_hpp
#define ServerWindow_hpp

#include <stdio.h>
#include <string>
#include <queue>
#include <deque>
#include "Packet.hpp"
using namespace std;

class ServerWindow
{
public:
    ServerWindow();
    ServerWindow(int num);
    void read_file(string filename);
    //void read_file_from_pos(string filename, streampos pos);
    Payload get_begin_m_queue_pop();
    Payload get_last_m_queue_pop();
    int get_size_m_queue();
    bool is_empty_m_queue();
    
    deque<deque<Payload>>::iterator get_begin_queue_queue_iter(){return m_queue_queue.begin();}
    deque<deque<Payload>>::iterator get_last_queue_queue_iter(){return m_queue_queue.end();}
    int get_size_queue_queue(){return (int)(m_queue_queue.size());}
    bool is_empty_queue_queue(){return m_queue_queue.empty();}
    void feed_m_queue_from_front();
    
    void resize_queue();
    
private:
    deque<Payload> m_queue;
    deque<deque<Payload>> m_queue_queue;
    deque<deque<Payload>>::iterator anchor;
    streampos size;
    //[!]m_queue will not be increased when m_queue.size() = 255
};


#endif /* ServerWindow_hpp */
