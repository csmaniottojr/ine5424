/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   testeID.cc
 * Author: decker
 *
 * Created on June 5, 2017, 3:21 PM
 */

#include <machine.h>
#include <utility/ostream.h>
using namespace EPOS;
OStream cout;
using namespace std;

/*
 * 
 */
int main() {
    for (unsigned int i = 0; i < 8; i++)
        cout << " " << hex << Machine::id()[i];
    cout << endl;
    return 0;
}

