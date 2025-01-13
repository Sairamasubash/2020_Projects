/***********************************************************
 File Name: prog1.cpp
 
 CSCI 463 - Assignment 2 - Fall 2020
 
 Progammer: Sairamasubash Muppalaneni
 Z-ID: Z1844762
 Date Due: Friday, September 18, 2020
 
 Purpose: The purpose of this computer assignment is to Read
 32-bit HEX representations of IEEE-754 Floating Point Numbers,
 decode them, and print their componenets.
 ************************************************************/

// Here are all the libraries that are required by this program.
#include <iostream>
#include <iomanip>

// Here are all the using statements that are required by this program.
using std::cin;
using std::cout;
using std::endl;
using std::hex;
using std::dec;
using std::setfill;
using std::setw;
using std::cerr;

/***************************************************************
 Function: printBinFloat
 
 Use: We are simply using this function to read a 32-bit hex value,
 and then we are printing the hex value in binary. In this function,
 we are also getting the sign, exponent, and significand from the
 32-bit hex value.
 
 Parameters:
 (1.) int32_t x - this value represents the signed integer type
 called x, with exactly 32 bits.
 
 Returns: none
 
 Notes: This printBinFloat function will be called in a read loop that
 is located in the main function.
 ***************************************************************/
void printBinFloat(int32_t x)
{
    // Getting the sign bit from the 32-bit binary value which was converted from the 32-bit hex value.
    char signBitValue = (x & 0x80000000 ? '1':'0');
    
    // Creating a signed integer type variable called y, to set it equal to the 32-bit hex value stored in x,
    // so that the 32-bit hex value stored in x would not be changed after the 23-bit shift to the right.
    int32_t y;
    y = x;
    y >>= 23;
    
    int32_t exponentValue = (y & 0x000000ff) - 127;   // Getting the exponent value of the 32-bit hex value.
    
    //Creating a signed integer type variable called z, to set it equal to the 32-bit hex value stored in x.
    int32_t z;
    z = x;

    int32_t significandValue = (z & 0x007fffff);   // Getting the significand value of the 32-bit hex value.
    
    cout << setfill('0');   // Setting 0 as the fill character.
    
    // Printing out the 32-bit hex value before printing out the converted 32-bit binary value.
    cout << "0x" << hex << setw(8) << x << " = ";
    
    int numOfBits = 0;   // Setting the number of bitts equal to 0.
    
    // Using a while loop to convert the 32-bit hex value into a 32-bit binary value (we are also printing out these values).
    while (numOfBits < 32)
    {
        // Using an if statement to print out a space after every 4 bits until the 32nd bit is reached.
        if (numOfBits > 0 && (numOfBits % 4) == 0)
        {
            cout << " ";
        }
        
        cout << (x & 0x80000000 ? '1':'0');   // Printing the binary bit values (which we convert from the 32-bit hex value).
        
        x <<= 1;   // Shifting 1-bit to the left of the 32-bit binary value.
        
        numOfBits++;   //Increasing the number of bits by 1;
    }
    
    cout << endl;
    
    // Printing out the sign bit value of the 32-bit binary value.
    cout << "sign: " << signBitValue << endl;
    
    // Printing out the exponent value of the 32-bit hex value.
    cout << " exp: " << "0x" << setw(8) << exponentValue << " (" << dec << exponentValue << ")" << endl;
    
    // Printing out the significand value of the 32-bit hex value.
    cout << " sig: " << "0x" << setw(8) << hex << significandValue << endl;
    
    if (signBitValue == '0')   // If the sign bit value is equal to 0, we print out a "+" sign.
    {
        cout << "+";
    }
    else if (signBitValue == '1')   // If the sign bit value is equal to 1, we print out a "-" sign.
    {
        cout << "-";
    }
    
    if (exponentValue == 0x00000080 && significandValue == 0x00000000)
    {
        cout << "inf";   // If the exponent value is all ones, and if the significand value is all zeros, we print out "inf".
    }
    else if (exponentValue == (int32_t)0xffffff81 && significandValue == 0x00000000)
    {
        cout << 0;   // If the exponent value is all zeros, and if the significand value is all zeros, we print out "0".
    }
    else if (exponentValue >= 0)   // Printing out the binary floating point number when the exponent is positive.
    {
        // Creating an integer called allNumbers to store the value of the exponent added with 23.
        int allNumbers;
        allNumbers = exponentValue + 23;
        
        cout << '1';   // Starting of the positive binary floating point number with a "1".
        
        significandValue <<= 9;   // Shifting 9 bits to the left of the significand.
        
        int i = 0;
        
        while (i < allNumbers)
        {
            // Using an if statement to print out a decimal when we reach the exponent.
            if (i == exponentValue)
            {
                cout << '.';
            }
            
            cout << (significandValue & 0x80000000 ? '1':'0');   // Printing out either ones or zeros based on the significand.
            
            significandValue <<= 1;   // Shifting 1-bit to the left of the significand.
            
            // Using an if statement to break the loop when a certain amount of numbers is reached.
            if (i >= exponentValue && i > 23)
            {
                break;
            }
            
            i++;
        }
    }
    else if (exponentValue < 0)   // Printing out the binary floating point number when the exponent is negative.
    {
        cout << "0.";   // Starting of the negative binary floating point number with a "0." (a zero and a decimal point).
        
        significandValue <<= 9;   // Shifting 9 bits to the left of the significand.
        
        int i = -1;
        
        // Printing out all of the zeros using a while loop.
        while (i > exponentValue)
        {
            cout << '0';
            
            i--;
        }
        
        cout << '1';   // Printing out a "1" after we print out all of the zeros.
        
        int j = 0;
        
        // Using a while loop to print out either ones or zeros based on the significand, and we are also shifting 1-bit to
        // the left of the significand.
        while (j < 23)
        {
            cout << (significandValue & 0x80000000 ? '1':'0');
            
            significandValue <<= 1;
            
            j++;
        }
    }
    
    cout << endl;
}

/***************************************************************
 Function: main
 
 Use: We are using this main function to read the one or more
 32-bit hex values. We are also using this main function to call
 the printBinFloat function inside of a read loop to do all of
 the decoding and printing.
 
 Parameters: none
 
 Returns: (1.) return 0 - whenever we get the return value of 0 in
 the output, we can then confirm that our program was successful.
 
 Notes: This main function is relatively short because most of the
 work is being done in the printBinFloat function.
 ***************************************************************/
int main()
{
    uint32_t x;   // Creating an unsigned integer type variable of 32 bits called x.
    
    // Creating a while loop to read in the 32-bit hex values from user input.
    while (cin >> hex >> x)
    {
        printBinFloat(x);   // Calling the printBinFloat function in the main function.
    }
    
    //  Checking to see if an input stream error occurred while entering input.
    if (!cin.eof())
    {
        cerr << "Warning: An input stream error has occurred" << endl;
    }
    
    return 0;   // Returning the value 0 to see if the program ran successfully.
}
