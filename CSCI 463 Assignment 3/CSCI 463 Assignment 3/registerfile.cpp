//***************************************************************************
//  File Name: registerfile.cpp
//
//  CSCI 463 - Assignment 5 - Fall 2020
//
//  Progammer: Sairamasubash Muppalaneni
//  Z-ID: Z1844762
//  Date Due: Monday, November 18, 2020
//***************************************************************************/

// Here are all the libraries that are required by this file.
#include "registerfile.h"
#include "hex.h"

/**
 * registerfile: This constructor simply calls the reset method, and the reset
 *    method initializes register x0 to zero, and all the other registers to
 *    0xf0f0f0f0.
 *
 * No parameter values.
 *
 * No return values.
 *
 * @note This is the constructor of the registerfile class that we have located
 *    in the registerfile.h header file.
 ********************************************************************************/
registerfile::registerfile()
{
    reset();   // Calling the reset method.
}

/**
 * reset: This method initializes register x0 (the first register) to zero, and
 *    this method also initializes all the other registers to 0xf0f0f0f0.
 *
 * No parameter values.
 *
 * No return values.
 *
 * @note This reset method is called in the registerfile constructor because the
 *    registerfile constructor also does the same process.
 ********************************************************************************/
void registerfile::reset()
{
    registersArray[0] = 0x00000000;   // Initializing register x0 to zero using registersArray.
    
    int32_t i = 1;
    
    while (i < 32)   // Creating a while loop to initialize all the other registers to 0xf0f0f0f0 using registersArray.
    {
        registersArray[i] = 0xf0f0f0f0;
        i++;
    }
}

/**
 * set: This method assigns/sets register r equal to the given val, if register r is
 *    not equal to zero. If register r is equal zero, then this method does nothing.
 *
 * @param r An unsigned integer type variable with a width of exactly 32 bits.
 * @param val An integer type variable with a width of exactly 32 bits.
 *
 * No return values.
 *
 * @note This is the set method for the registerfile class, there is also a get
 *    method for the registerfile class.
 ********************************************************************************/
void registerfile::set(uint32_t r, int32_t val)
{
    if (r != 0)   // Checking to see if register r is not equal to 0.
    {
        registersArray[r] = val;   // Setting register r equal to the given val.
    }
}

/**
 * get: This method returns the value of register r, if register r is not equal
 *    to zero. If register r is equal to zero, then this method returns zero.
 *
 * @param r An unsigned integer type variable with a width of exactly 32 bits.
 *
 * @return 0 If register r is equal to zero, then we return a 0.
 * @return r If register r is not equal to zero, then we return register r.
 *
 * @note This is the get method for the registerfile class, there is also a set
 *    method for the registerfile class.
 ********************************************************************************/
int32_t registerfile::get(uint32_t r) const
{
    if (r != 0)   // Checking to see is register r is equal to zero.
    {
        return registersArray[r];   // Returning a 0.
    }
    else
    {
        return 0;   // Returning register r.
    }
}

/**
 * dump: This method dumps out all 32 registers in a specific format that only
 *    prints out eight registers per line, and four lines in total (also, the
 *    the registers start from 0 and they end with 32).
 *
 * No parameter values.
 *
 * No return values.
 *
 * @note This is the Third dump method in this entire project, we have the first
 *    dump method in the memory.cpp file, and the second dump method is in the
 *    rv32i.cpp file.
 ********************************************************************************/
void registerfile::dump() const
{
    int32_t i = 0;
    
    while (i < 32)   // Checking to see if i is less than 32.
    {
        if (i % 8 == 0)   // Checking to see if i modulo 8 equals 0.
        {
            if (i != 0)   // Checking to see if i is not equal to 0.
            {
                std::cout << std::endl;   // Printing out a line break everytime 8 registers are dumped.
            }
            
            if (i == 0)   // Checking to see if i is equal to 0.
            {
                std::cout << " x" << i;   // Printing out the register number.
            }
            else if (i == 8)   // Checking to see if i is equal to 8.
            {
                std::cout << " x" << i;   // Printing out the register number.
            }
            else
            {
                std::cout << "x" << i;   // Printing out the register number.
            }
        }
        
        std::cout << " " << hex32(registersArray[i]);   // Dumping out all 32 registers.
        
        i++;
    }
}
