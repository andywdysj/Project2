//
//  TCPclient.cpp
//  CS118_Project2_Xcode
//
//  Created by Jeannie Chiem on 2/25/18.
//  Copyright © 2018 Andrew. All rights reserved.
//

#include "TCPclient.hpp"


TCPclient::TCPclient(string host, int portno, string filename)
{
    m_portno = portno;
    
    m_sockfd = socket(AF_INET, SOCK_STREAM, 0);  // create new socket
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
    
    memset((char *)&m_serv_addr, 0, sizeof(m_serv_addr));
    m_serv_addr.sin_port = htons(m_portno);
    m_serv_addr.sin_family = AF_INET;
    m_serv_addr.sin_addr.s_addr = INADDR_ANY;
    
    memcpy((void *)&m_serv_addr.sin_addr, m_server->h_addr_list[0], m_server->h_length);

    
    memset((char *) &m_serv_addr, 0, sizeof(m_serv_addr));
    m_serv_addr.sin_family = AF_INET;
    bcopy((char *)m_server->h_addr, (char *)&m_serv_addr.sin_addr.s_addr, m_server->h_length);
    
    if (connect(m_sockfd,(struct sockaddr *)&m_serv_addr,sizeof(m_serv_addr)) < 0)
    {
        cerr << "" << endl;
        exit(0);
    }
}







