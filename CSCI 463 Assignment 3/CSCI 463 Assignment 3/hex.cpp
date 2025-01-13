//***************************************************************************
//  File Name: hex.cpp
//
//  CSCI 463 - Assignment 5 - Fall 2020
//
//  Progammer: Sairamasubash Muppalaneni
//  Z-ID: Z1844762
//  Date Due: Monday, November 18, 2020
//***************************************************************************/

// Here are all the libraries that are required by this file.
#include <iomanip>
#include <sstream>
#include "hex.h"

/**
 * hex8: This function returns a string that has exactly 2 hex digits
 *    representing the 8 bits of the i argument.
 *
 * @param i An unsigned integer type variable with a width of exactly 8 bits.
 *
 * @return A string with exactly 2 hex digits representing the 8 bits of
 *    the parameter i.
 *
 * @note We use static_cast in this function to stop the insertion
 *    operator (<<) from treating the 8-bit integer as a character.
 ********************************************************************************/
std::string hex8(uint8_t i)
{
    // The entire process of returning a string that has exactly 2 hex digits.
    std::ostringstream os;
    os << std::hex << std::setfill('0') << std::setw(2) << static_cast<uint16_t>(i);
    return os.str();
}

/**
 * hex32: This function returns a string that has exactly 8 hex digits
 *    representing the 32 bits of the i argument.
 *
 * @param i An unsigned integer type variable with a width of exactly 32 bits.
 *
 * @return A string with exactly 8 hex digits representing the 32 bits of
 *    the parameter i.
 *
 * @note The use of static_cast is not needed in this function unlike
 *    the hex8 function because of the larger integer size.
 ********************************************************************************/
std::string hex32(uint32_t i)
{
    // The entire process of returning a string that has exactly 8 hex digits.
    std::ostringstream os;
    os << std::hex << std::setfill('0') << std::setw(8) << i;
    return os.str();
}

/**
 * hex0x32: This function returns a string that starts with 0x, followed by
 *    exactly 8 hex digits representing the 32 bits of the i argument.
 *
 * @param i An unsigned integer type variable with a width of exactly 32 bits.
 *
 * @return A string that starts with 0x, followed by exactly 8 hex digits
 *    representing the 32 bits of the parameter i.
 *
 * @note All of this is implemented by concatenating a 0x to the output of my
 *    hex32() function.
 ********************************************************************************/
std::string hex0x32(uint32_t i)
{
    // Concatenating a 0x to the output of the hex32() function, and then returning that value to the caller.
    return std::string("0x")+hex32(i);
}
