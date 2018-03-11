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
    Payload get_begin();
    Payload get_last();
    int get_size();
    bool is_empty();
    
private:
    vector<Payload> m_buffer;
    deque<Payload> m_queue;
};


#endif /* ServerWindow_hpp */
