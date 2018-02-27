//
//  main.cpp
//  CS118_Project2_Xcode
//
//  Created by Andrew on 2/17/18.
//  Copyright © 2018 Andrew. All rights reserved.
//

#include <iostream>
#include "MyTCP.hpp"
#include "Packet.hpp"
using namespace std;

int main(int argc, const char * argv[])
{
    //commented for testing
    /*
    if(argc != 2)
    {
        cerr << "Error: number of parameters incorrect." << endl;
        return(1);
    }
     */
    
    //for test only
    /*
    vector<char> test;
    test.assign(1016, 'a');
    cout << test.capacity() << endl;
    cout << sizeof(Header) + test.capacity() << endl;
    cout << sizeof(Packet) << endl;
    cout << sizeof(Packet)-sizeof(Header) << endl;
     */
    
    vector<int> test;
    vector<int> test2;
    test.assign(5, 1);
    test2.assign(10, 2);
    
    test = test2;
    
    for(int i=0; i<200; i++)
        cout << test[i] << endl;
    
    
}
