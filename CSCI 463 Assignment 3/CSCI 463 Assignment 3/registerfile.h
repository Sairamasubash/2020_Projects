//***************************************************************************
//  File Name: registerfile.h
//
//  CSCI 463 - Assignment 5 - Fall 2020
//
//  Progammer: Sairamasubash Muppalaneni
//  Z-ID: Z1844762
//  Date Due: Monday, November 18, 2020
//***************************************************************************/

// Here are the beginning header guards for the registerfile.h file.
#ifndef registerfile_H
#define registerfile_H

// Here are all the libraries that are required by this header file.
#include <iostream>

class registerfile     // Start of the registerfile class.
{
    // Declaring the five public function prototypes for the registerfile class.
public:
    registerfile();
    void reset();
    void set(uint32_t r, int32_t val);
    int32_t get(uint32_t r) const;
    void dump() const;
    
    // Declaring the one private data member for the registerfile class.
private:
    int32_t registersArray[32];
};     // End of the registerfile class.

#endif     // Here is the ending header guard for the registerfile.h file.
