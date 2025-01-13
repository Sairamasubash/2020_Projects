//***************************************************************************
//  File Name: hex.h
//
//  CSCI 463 - Assignment 5 - Fall 2020
//
//  Progammer: Sairamasubash Muppalaneni
//  Z-ID: Z1844762
//  Date Due: Monday, November 18, 2020
//***************************************************************************/

// Here are the beginning header guards for the hex.h file.
#ifndef hex_H
#define hex_H

// Here are all the libraries that are required by this header file.
#include <string>
#include <stdint.h>

// Declaring the three function prototypes that are needed for the hex.cpp file.
std::string hex8(uint8_t i);
std::string hex32(uint32_t i);
std::string hex0x32(uint32_t i);

#endif     // Here is the ending header guard for the hex.h file.
