//***************************************************************************
//  File Name: main.cpp
//
//  CSCI 463 - Assignment 5 - Fall 2020
//
//  Progammer: Sairamasubash Muppalaneni
//  Z-ID: Z1844762
//  Date Due: Monday, November 18, 2020
//***************************************************************************/

// Here are all the libraries that are required by this file.
#include "hex.h"
#include "memory.h"
#include "rv32i.h"
#include <unistd.h>

/**
 * usage: This function prints a detailed error message and terminates the
 *    program if there are any problems.
 *
 * No parameter values.
 *
 * No return values.
 *
 * @note We use exit(1) to terminate the program instead of exit(0) because
 *    1 means the program has issues and 0 means the program is successful.
 ********************************************************************************/
static void usage ()
{
    // Printing out a detailed error message and terminating the program with exit(1).
    std::cerr << "Usage: rv32i [-m hex-mem-size] infile" << std::endl;
    std::cerr << " -m specify memory size (default = 0x10000)" << std::endl;
    exit(1);
}

/**
 * main: This function calls all of the other methods that we created
 *    for this program to work, and gives these methods the data that
 *    they require to complete their jobs and produce the correct output.
 *    This main function also uses a certain amount of command line
 *    arguments to get user input.
 *
 * @param argc An integer variable that holds the number of command line
 *    arguments that were given, when running this program.
 * @param argv An array of character pointers that list all of the command
 *    line arguments that were given, when running this program.
 *
 * @return A value of 0 that states that our program was successful.
 *
 * @note We are using the command line arguments to get the input data
 *    that is required by this program.
 ********************************************************************************/
int main(int argc, char **argv)
{
    // Creating eight variables that are required by this main function.
    uint32_t memory_limit = 0x10000;
    int opt;
    int caseD;
    int caseI;
    int caseR;
    int caseL;
    int caseZ;
    uint64_t insnLimit;
    
    // Using a while loop to create a switch statement that checks the cases for the characters m, d, i, r, l, and z.
    while ((opt = getopt(argc, argv, "m:dirl:z")) != -1)
    {
        switch (opt)   // Checking to see what argument characters were given.
        {
            case 'm':   // Checking for the character m.
                memory_limit = std::stoul(optarg,nullptr,16);
                break;
            case 'd':   // Checking for the character d.
                caseD = 1;
                break;
            case 'i':   // Checking for the character i.
                caseI = 1;
                break;
            case 'r':   // Checking for the character r.
                caseR = 1;
                break;
            case 'l':   // Checking for the character l.
                caseL = 1;
                break;
            case 'z':   // Checking for the character z.
                caseZ = 1;
                break;
            default:   // Calling the usage function as the default in the switch statement.
                usage();
        }
    }
    
    // Calling the usage function if the file name is missing.
    if (optind >= argc)
        usage();
    
    // Using the mem member variable to check the limit of the memory.
    memory mem(memory_limit);
    
    // Calling the usage function if the given file is not loading properly.
    if (!mem.load_file(argv[optind]))
        usage();
    
    rv32i sim(&mem);   // Creating a rv32i class object that is called sim.
    
    // If the character l was given as an argument.
    if (caseL == 1)
    {
        insnLimit = std::stoul(optarg,nullptr,10);   // Setting the instruction limit.
    }
    
    // If the character i was given as an argument.
    if (caseI == 1)
    {
        sim.set_show_instructions(true);   // Calling the rv32i set_show_instructions method.
    }
    
    // If the character d was given as an argument.
    if (caseD == 1)
    {
        sim.disasm();   // Calling the rv32i disasm method.
        sim.reset();   // Calling the rv32i reset method.
    }
    
    // If the character r was given as an argument.
    if (caseR == 1)
    {
        sim.set_show_registers(true);   // Calling the rv32i set_show_registers method.
    }
    
    sim.run(insnLimit);   // Calling the rv32i run method.
    
    // If the character z was given as an argument.
    if (caseZ== 1)
    {
        sim.dump();   // Calling the rv32i dump method.
        mem.dump();   // Calling the memory dump method.
    }
    
    return 0;   // Returning the value 0 to see if the program ran successfully.
}
