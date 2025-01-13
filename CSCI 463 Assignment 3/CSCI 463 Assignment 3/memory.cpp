//***************************************************************************
//  File Name: memory.cpp
//
//  CSCI 463 - Assignment 5 - Fall 2020
//
//  Progammer: Sairamasubash Muppalaneni
//  Z-ID: Z1844762
//  Date Due: Monday, November 18, 2020
//***************************************************************************/

// Here are all the libraries that are required by this file.
#include "memory.h"
#include "hex.h"
#include <fstream>
#include <sstream>
#include <ctype.h>

/**
 * memory: This constructor rounds the length of siz up, and then stores the siz
 *    argument in the size member variable. This constructor also allocates the
 *    siz number of bytes for the mem array and initializes every byte to 0xa5.
 *
 * @param siz An unsigned integer type variale with a width of exactly 32 bits.
 *
 * No return values.
 *
 * @note This is the constructor of the memory class that we have located in
 *    the memory.h header file.
 ********************************************************************************/
memory::memory(uint32_t siz)
{
    siz = (siz+15)&0xfffffff0;   // Rounding the length of siz up.
    size = siz;   // Making the siz argument equal to the size member variale.
    mem = new uint8_t[size];   // Allocating siz number of bytes for the mem array.
    
    // Initializing every single byte to 0xa5.
    for(uint32_t i = 0; i < size; i++)
    {
        mem[i] = 0xa5;
    }
}

/**
 * ~memory: This destructor frees the memory that was allocated in the constructor
 *    to represent the simulated memory (mem).
 *
 * No parameter values.
 *
 * No return values.
 *
 * @note This is the destructor of the memory class that we have located in the
 *    memory.h header file.
 ********************************************************************************/
memory::~memory()
{
    delete [] mem;  // Freeing the memory that was allocated in the constructor.
}

/**
 * check_address: This method returns true if argument i is in the simulated
 *    memory, and prints a warning message to the standard output while
 *    returning false, if argument i is not in the simulated memory.
 *
 * @param i An unsigned integer type variale with a width of exactly 32 bits.
 *
 * @return If the argument i is in the simulated memory, we return true. Also,
 *    if the argument i is not in the simulated memory, we return false.
 *
 * @note This boolean check_address method will return two return values (both
 *    true and false.)
 ********************************************************************************/
bool memory::check_address(uint32_t i) const
{
    // Checking to see if the argument i is less than the size member variable.
    if (i < size)
    {
        return true;   // Returning true if the argument i is in the simulated memory.
    }
    else
    {
        // Printing a warning message while returning false, if the argument i is not in the simulated memory.
        std::cout << "WARNING: Address out of range: " << hex0x32(i) << std::endl;
        return false;
    }
}

/**
 * get_size: This method just returns the size member variable which is the
 *    possibly rounded up siz value (from the constructor).
 *
 * No parameter values.
 *
 * @return A member variable called size which is the possibly rounded up siz value.
 *
 * @note The siz value needs to be rounded up sometimes, and it does
 *    not need to be rounded up sometimes.
 ********************************************************************************/
uint32_t memory::get_size() const
{
    return size;   // Returning the size member variable.
}

/**
 * get8: This method checks to see if the addr argument is in the mem data
 *    member by calling the check_address function.
 *
 * @param addr An unsigned integer type variable with a width of exactly 32 bits.
 *
 * @return If the addr argument is in the valid range, we then return the value
 *    of the byte from the simulated memory (mem) at that address. Also, if addr
 *    argument is not in the valid range, we then return a zero.
 *
 * @note This get8 method will be called twice in both the get16 method and the
 *    get32 method.
 ********************************************************************************/
uint8_t memory::get8(uint32_t addr) const
{
    // Checking to see if the addr argument is in the mem data member (using the check_address function).
    if (check_address(addr))
    {
        return mem[addr];   // Returning the value of the byte from the simulated memory (at that address)
    }
    else
    {
        return 0;   // Returning zero to the caller.
    }
}

/**
 * get16: This method calls the get8 method twice to get two differennt bytes,
 *    and combines them in little-endian order to create a 16-bit return value
 *
 * @param addr An unsigned integer type variable with a width of exactly 32 bits.
 *
 * @return An unsigned integer type variable with a width of exactly 16 bits, and
 *    it holds the sum of the both get8 method calls.
 *
 * @note The get32 method does a process that is very similar to the process that
 *    is beinng done in this method, except it calls this get16 method twice.
 ********************************************************************************/
uint16_t memory::get16(uint32_t addr) const
{
    // Creating three unsigned integer type variables to get the final 16-bit return value.
    uint16_t sixteenBitSum;
    uint16_t firstCall;
    uint16_t secondCall;
    
    // Calling the get8 method twice and adding both of the calls to get the final 16-bit return value
    firstCall = get8(addr);
    secondCall = get8(addr + 1) << 8;
    sixteenBitSum = (firstCall | secondCall);
    
    return sixteenBitSum;   // Returning the final 16-bit value.
}

/**
 * get32: This method calls the get16 method twice to get two differennt bytes,
 *    and combines them in little-endian order to create a 32-bit return value
 *
 * @param addr An unsigned integer type variable with a width of exactly 32 bits.
 *
 * @return An unsigned integer type variable with a width of exactly 32 bits, and
 *    it holds the sum of the both get16 method calls.
 *
 * @note The get16 method does a process that is very similar to the process that
 *    is beinng done in this method, except it calls the get8 method twice.
 ********************************************************************************/
uint32_t memory::get32(uint32_t addr) const
{
    // Creating three unsigned integer type variables to get the final 32-bit return value.
    uint32_t thirtyTwoBitSum;
    uint32_t firstCall;
    uint32_t secondCall;
    
    // Calling the get16 method twice and adding both of the calls to get the final 32-bit return value
    firstCall = get16(addr);
    secondCall = get16(addr + 2) << 16;
    thirtyTwoBitSum = (firstCall | secondCall);
    
    return thirtyTwoBitSum;   // Returning the final 32-bit value.
}

/**
 * set8: This method calls the check_address method to verify that the addr
 *    argument is valid. If the addr argumennt is valid, then we set the byte
 *    in the simulated memory at that address, to the val argument.
 *
 * @param addr An unsigned integer type variable with a width of exactly 32 bits.
 * @param val An unsigned integer type variable with a width of exactly 8 bits.
 *
 * No return values.
 *
 * @note This set8 method will be called twice in both the set16 method and the
 *    set32 method.
 ********************************************************************************/
void memory::set8(uint32_t addr, uint8_t val)
{
    if (check_address(addr))   // Calling the check_address method to verify that the addr argument is valid.
    {
        mem[addr] = val;   // If the addr argument is valid, we are setting that address in memory equal to val.
    }
    else
    {
        return;   // Returning back to the caller (no value after return statement).
    }
}

/**
 * set16: This method calls the set8 method twice to store the val argument in
 *    little-endian order into the simulated memory starting at the address
 *    given in the addr argument.
 *
 * @param addr An unsigned integer type variable with a width of exactly 32 bits.
 * @param val An unsigned integer type variable with a width of exactly 16 bits.
 *
 * No return values.
 *
 * @note The set32 method does a process that is very similar to the process that
 *    is beinng done in this method, except it calls this set16 method twice.
 ********************************************************************************/
void memory::set16(uint32_t addr, uint16_t val)
{
    set8(addr + 1, val >> 8);   // Calling set8 method the first time, adding 1 to addr, and right shifting val by 8.
    
    set8(addr, val);   // Calling set8 method the second time, and not changing addr or val.
    
    mem[addr] = val;   // We are setting that address in the simulated memory equal to val.
}

/**
 * set32: This method calls the set16 method twice to store the val argument
 *    in little-endian order into the simulated memory starting at the address
 *    given in the addr argument.
 *
 * @param addr An unsigned integer type variable with a width of exactly 32 bits.
 * @param val An unsigned integer type variable with a width of exactly 32 bits.
 *
 * No return values.
 *
 * @note The set16 method does a process that is very similar to the process that
 *    is beinng done in this method, except it calls the set8 method twice.
 ********************************************************************************/
void memory::set32(uint32_t addr, uint32_t val)
{
    set16(addr + 1, val >> 8);   // Calling set16 method the first time, adding 1 to addr, and right shifting val by 8.
    
    set16(addr + 2, val >> 16);   // Calling set16 method the seconnd time, adding 2 to addr, and right shifting val by 16.
    
    mem[addr] = val;   // We are setting that address in the simulated memory equal to val.
}

/**
 * dump: This method dumps the entire contents of the simulated memory in hex with
 *    ASCII on the right (exactly space-for-space).
 *
 * No parameter values.
 *
 * No return values.
 *
 * @note In this method, we are to show an ASCII character or a dot for those
 *    bytes that do not have a valid printable value.
 ********************************************************************************/
void memory::dump() const
{
    uint8_t ascii[17] = {};   // Creating a 17-byte array called ascii.
    
    // Using a for loop to print out the dump simulated memory output.
    for (uint32_t i = 0; i < size; i++)
    {
        // If i modulo 16 equals 0, and if i does not equal 0, we are printing out an astrisk.
        if (i % 16 == 0)
        {
            if (i != 0)
                std::cout << "*";
        }
        
        // If i modulo 16 equals 0, we are printing out the ascii values.
        if (i % 16 == 0)
        {
            std::cout << ascii;
            
            // If i does not equal 0, we are printing out an astrisk and ending the line.
            if (i != 0)
            {
                std::cout << "*";
                std::cout << std::endl;
            }
            
            std::cout << hex32(i) << ":";
        }
        
        uint8_t ch = get8(i);   // Making ch equal to the call of the get8 method.
        
        // If i modulo 8 equals 0, we are printing out a space.
        if (i % 8 == 0)
        {
            std::cout << " ";
        }
        
        std::cout << hex8(get8(i)) << " ";
        
        ascii[i % 16] = isprint(ch) ? ch : '.';   // Printing an ascii value that is a character or a period.
        
        // If i equals size minus 1, we are printing out an astrisk and more ascii values.
        if (i == size - 1)
        {
            std::cout << "*" << ascii;
        }
    }
    
    std::cout << "*";
    
    std::cout << std::endl;
}

/**
 * load_file: This method opens the file named fname in binary mode, and read its
 *    contents into the simulated memory (mem).
 *
 * @param fname A constant string value that is passed by reference.
 *
 * @return If the byte address is valid, we keep going, and if the byte address is
 *    not valid, then we print an error message, and return false. Also, if the
 *    file loads perfectly, then we return true.
 *
 * @note This method reads the file one byte at-a-time to make sure that the file
 *    can fit into the memory. This process is done by checking the byte address
 *    before we write to it, by calling the check_address method.
 ********************************************************************************/
bool memory::load_file(const std::string &fname)
{
    std::ifstream infile(fname, std::ios::in|std::ios::binary);   // Opening a file in binary mode.
    
    // If the file cannot open properly, we are printing an error message, and returning false.
    if (!infile.is_open())
    {
        std::cerr << "Can’t open file '" << fname << "' for reading." << std::endl;
        return false;
    }
    
    // Creating two variables that are required to read the file one byte at-a-time.
    char currentByte;
    uint32_t i = 0;
    
    infile >> std::noskipws;
    
    while (infile >> currentByte)
    {
        // If the address is valid, we keep going by setting the address in simulated memory equal to currentByte.
        if (check_address(i))
        {
            mem[i] = currentByte;
        }
        else
        {
            // If the address is not valid, we are printing an error message, closing the file, and returning false.
            std::cerr << "Program too big." << std::endl;
            infile.close();
            return false;
        }
        
        i++;   // Incrementing the i variable by one.
    }
    
    // If the file loads perfectly, then we close the file and return true.
    infile.close();
    return true;
}
