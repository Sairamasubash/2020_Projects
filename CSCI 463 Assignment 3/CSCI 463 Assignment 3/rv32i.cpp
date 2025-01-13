//***************************************************************************
//  File Name: rv32i.cpp
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
#include <cassert>
#include "memory.h"
#include "rv32i.h"
#include "hex.h"
#include "registerfile.h"

/**
 * rv32i: This constructor saves the m parameter in the mem member variable (and
 *    we use this later when we are disassembling).
 *
 * @param m A pointer to the memory object which we get from the memory class.
 *
 * No return values.
 *
 * @note This is the constructor for this rv32i class, we do not have a destructor
 *    for this rv32i class
 ********************************************************************************/
rv32i::rv32i(memory *m)
{
    mem = m;   // Storing the m variable inside the mem variable.
}

/**
 * disasm: This method is used to disassemble the given instructions in the simulated
 *    memory. Also, this method will continue until every word from the simulated
 *    memory is fetched, decoded, and printed.
 *
 * No parameter values.
 *
 * No return values.
 *
 * @note This disasm method calls the get_size, the hex32, the get32, and the
 *    decode methods.
 ********************************************************************************/
void rv32i::disasm(void)
{
    pc = 0;   // Setting the pc variable equal to zero.
    
    // Calling the get_size method to get the 32-bit simulated memory.
    uint32_t memGetSize = mem->get_size();
    
    // Using a while loop to disassemble the instructions in the simulated memory.
    while (pc < memGetSize)
    {
        // Callig the hex32 function and printing the 32-bit hex address in the pc register.
        std::string hex32string = hex32(pc);
        std::cout << hex32string << ": ";
        
        // Calling the get32 method to get the 32-bit instruction from memory at the address in the pc register.
        uint32_t memGet32= mem->get32(pc);
        
        // Printing the previously described instruction as a 32-bit hex value.
        std::cout << hex32(memGet32);
        
        // Passing the previously described instruction to the decode method and storing the returned value in a string.
        std::string dcodeString = decode(memGet32);
        
        // Printing the decoded instruction string returned from decode method.
        std::cout << dcodeString << std::endl;
        
        pc += 4;   // Incrementing the pc variable by four.
    }
}

/**
 * decode: This method is where all of the decoding is happening in this program.
 *    The method prints out the decoded versions of all the instructions. This
 *    decode method completes this process by using the render functions, and
 *    the constant variables that we created.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 *
 * @return A string value that prints the results of all the render functions that
 *    we created (these render functions also return string values).
 *
 * @note This decode method requires a lot of constant variables to check the
 *    opcode for every single instruction that is given to us.
 ********************************************************************************/
std::string rv32i::decode(uint32_t insn) const
{
    // Calling the get_opcode, the get_funct3, the get_funct7, the get_rs2, and the get_imm_i methods.
    uint32_t opcode = get_opcode(insn);
    uint32_t funct3 = get_funct3(insn);
    uint32_t funct7 = get_funct7(insn);
    uint32_t rs2 = get_rs2(insn);
    int32_t imm_i = get_imm_i(insn);
    
    // Using a switch statement to check for the opcode value of all instructions.
    switch(opcode)
    {
            // Printing out the result of the render_illegal_insn method.
        default:                    return render_illegal_insn();
        case opcode_lui:            return render_lui(insn);   // Printing out the result of the render_lui method.
        case opcode_auipc:          return render_auipc(insn);   // Printing out the result of the render_auipc method.
            // Printing out the result of the render_rtype method.
        case opcode_rtype:
            // Using a switch statement to check for the funct3 value of R-type instructions.
            switch(funct3)
        {
            default:                return render_illegal_insn();
            case funct3_add:
                // Using a switch statement to check for the funct7 value of R-type instructions.
                switch(funct7)
            {
                default:            return render_illegal_insn();
                case funct7_add:    return render_rtype(insn, "  add");
                case funct7_sub:    return render_rtype(insn, "  sub");
            }
                assert(0 && "unhandled funct7");
            case funct3_sll:        return render_rtype(insn, "  sll");
            case funct3_slt:        return render_rtype(insn, "  slt");
            case funct3_sltu:       return render_rtype(insn, "  sltu");
            case funct3_xor:        return render_rtype(insn, "  xor");
            case funct3_srl:
                // Using a switch statement to check for the funct7 value of R-type instructions.
                switch(funct7)
            {
                default:            return render_illegal_insn();
                case funct7_srl:    return render_rtype(insn, "  srl");
                case funct7_sra:    return render_rtype(insn, "  sra");
            }
                assert(0 && "unhandled funct7");
            case funct3_or:         return render_rtype(insn, "  or");
            case funct3_and:        return render_rtype(insn, "  and");
        }
            assert(0 && "unhandled funct3");
            // Printing out the result of the render_itype_alu method.
        case opcode_alu_imm:
            // Using a switch statement to check for the funct3 value of I-type alu instructions.
            switch (funct3)
        {
            default:                return render_illegal_insn();
            case funct3_slli:       return render_itype_alu(insn, "  slli", imm_i%XLEN);
            case funct3_srli:
                // Using a switch statement to check for the funct7 value of I-type alu instructions.
                switch(funct7)
            {
                default:            return render_illegal_insn();
                case funct7_srli:   return render_itype_alu(insn, "  srli", imm_i%XLEN);
                case funct7_srai:   return render_itype_alu(insn, "  srai", imm_i%XLEN);
            }
                assert(0 && "unhandled funct7");
            case funct3_addi:       return render_itype_alu(insn, "  addi", imm_i);
            case funct3_slti:       return render_itype_alu(insn, "  slti", imm_i);
            case funct3_sltiu:      return render_itype_alu(insn, "  sltiu", imm_i);
            case funct3_xori:       return render_itype_alu(insn, "  xori", imm_i);
            case funct3_ori:        return render_itype_alu(insn, "  ori", imm_i);
            case funct3_andi:       return render_itype_alu(insn, "  andi", imm_i);
        }
            assert(0 && "unhandled funct3");
        case opcode_jal:            return render_jal(insn);   // Printing out the result of the render_jal method.
        case opcode_jalr:           return render_jalr(insn);   // Printing out the result of the render_jalr method.
        case opcode_fence:          return render_fence(insn);   // Printing out the result of the render_fence method.
            // Printing out the result of the render_itype_load method.
        case opcode_load_imm:
            // Using a switch statement to check for the funct3 value of I-type load instructions.
            switch (funct3)
        {
            default:                return render_illegal_insn();
            case funct3_lb:         return render_itype_load(insn, "  lb");
            case funct3_lh:         return render_itype_load(insn, "  lh");
            case funct3_lw:         return render_itype_load(insn, "  lw");
            case funct3_lbu:        return render_itype_load(insn, "  lbu");
            case funct3_lhu:        return render_itype_load(insn, "  lhu");
        }
            assert(0 && "unhandled funct3");
            // Printing out the result of the render_stype method.
        case opcode_stype:
            // Using a switch statement to check for the funct3 value of S-type instructions.
            switch (funct3)
        {
            default:                return render_illegal_insn();
            case funct3_sb:         return render_stype(insn, "  sb");
            case funct3_sh:         return render_stype(insn, "  sh");
            case funct3_sw:         return render_stype(insn, "  sw");
        }
            assert(0 && "unhandled funct3");
            // Printing out the result of the render_btype method.
        case opcode_btype:
            // Using a switch statement to check for the funct3 value of B-type instructions.
            switch (funct3)
        {
            default:                return render_illegal_insn();
            case funct3_beq:        return render_btype(insn, "  beq");
            case funct3_bne:        return render_btype(insn, "  bne");
            case funct3_blt:        return render_btype(insn, "  blt");
            case funct3_bge:        return render_btype(insn, "  bge");
            case funct3_bltu:       return render_btype(insn, "  bltu");
            case funct3_bgeu:       return render_btype(insn, "  bgeu");
        }
            assert(0 && "unhandled funct3");
        case opcode_ecall:
            // Using a switch statement to check for the funct7 plus rs2 value of ecall/ebreak instructions.
            switch (funct7 + rs2)
        {
            default:                return render_illegal_insn();
            case funct7_rs2_ecall:  return render_ecall();   // Printing out the result of the render_ecall method.
            case funct7_rs2_ebreak: return render_ebreak();   // Printing out the result of the render_ebreak method.
        }
            assert(0 && "unhandled funct3");
    }
    assert(0 && "unhandled opcode");
}

/**
 * render_illegal_insn: This method just returns a string value that lets the
 *    program know that an unimplemented instruction has been given.
 *
 * No parameter values.
 *
 * @return A string value that only prints the phrase "  ERROR: UNIMPLEMENTED
 *    INSTRUCTION" out to the standard output.
 *
 * @note This render_illegal_insn method is the first render method in this rv32i.cpp
 *    file (also, all of the render methods return a string).
 ********************************************************************************/
std::string rv32i::render_illegal_insn() const
{
    return "  ERROR: UNIMPLEMENTED INSTRUCTION";   // Returning the phrase "  ERROR: UNIMPLEMENTED INSTRUCTION".
}

/**
 * render_lui: This method calls the get_rd, and the get_imm_u methods first,
 *    and then this method prints out a string value cantaining the values that
 *    we get from calling the previously described methods.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 *
 * @return A string value that prints the name of the instruction which is lui,
 *    the rd value of lui, and the imm_u value of lui.
 *
 * @note This render_lui method is the second render method in this rv32i.cpp
 *    file (also, all of the render methods return a string).
 ********************************************************************************/
std::string rv32i::render_lui(uint32_t insn) const
{
    // Calling the get_rd, and the get_imm_u methods.
    uint32_t rd = get_rd(insn);
    int32_t imm_u = get_imm_u(insn);
    
    // Returning a string that prints out the name, the rd value, and the imm_u value of the instruction which is lui.
    std::ostringstream os;
    os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << "  lui"
    << "  x" << std::dec << rd << ",0x" << std::hex << ((imm_u >> 12)&0x0fffff);
    return os.str();
}

/**
 * render_auipc: This method calls the get_rd, and the get_imm_u methods first,
 *    and then this method prints out a string value cantaining the values that
 *    we get from calling the previously described methods.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 *
 * @return A string value that prints the name of the instruction which is auipc,
 *    the rd value of auipc, and the imm_u value of auipc.
 *
 * @note This render_auipc method is the third render method in this rv32i.cpp
 *    file (also, all of the render methods return a string).
 ********************************************************************************/
std::string rv32i::render_auipc(uint32_t insn) const
{
    // Calling the get_rd, and the get_imm_u methods.
    uint32_t rd = get_rd(insn);
    int32_t imm_u = get_imm_u(insn);
    
    // Returning a string that prints out the name, the rd value, and the imm_u value of the instruction which is auipc.
    std::ostringstream os;
    os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << "  auipc"
    << "  x" << std::dec << rd << ",0x" << std::hex << ((imm_u >> 12)&0x0fffff);
    return os.str();
}

/**
 * render_jal: This method calls the get_rd, and the get_imm_j methods first,
 *    and then this method prints out a string value cantaining the values that
 *    we get from calling the previously described methods.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 *
 * @return A string value that prints the name of the instruction which is jal,
 *    the rd value of jal, and the imm_j (plus pc) value of jal.
 *
 * @note This render_jal method is the fourth render method in this rv32i.cpp
 *    file (also, all of the render methods return a string).
 ********************************************************************************/
std::string rv32i::render_jal(uint32_t insn) const
{
    // Calling the get_rd, and the get_imm_j methods.
    uint32_t rd = get_rd(insn);
    int32_t imm_j = get_imm_j(insn);
    
    // Returning a string that prints out the name, the rd value, and the imm_j (plus pc) value of the instruction which is jal.
    std::ostringstream os;
    os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << "  jal"
    << "  x" << std::dec << rd << ",0x" << std::hex << (imm_j + pc);
    return os.str();
}

/**
 * render_jalr: This method calls the get_rd, the get_rs1, and the get_imm_i
 *    methods first, and then this method prints out a string value cantaining
 *    the values that we get from calling the previously described methods.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 *
 * @return A string value that prints the name of the instruction which is jalr,
 *    the rd value of jalr, the imm_i value of jalr, and the rs1 value of jalr.
 *
 * @note This render_jalr method is the fifth render method in this rv32i.cpp
 *    file (also, all of the render methods return a string).
 ********************************************************************************/
std::string rv32i::render_jalr(uint32_t insn) const
{
    // Calling the get_rd, the get_rs1, and the get_imm_i methods.
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    int32_t imm_i = get_imm_i(insn);
    
    // Returning a string that prints out the name, the rd value, the imm_i value, and the rs1 value of the instruction which is jalr.
    std::ostringstream os;
    os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << "  jalr"
    << "  x" << std::dec << rd << "," << imm_i << "(x" << rs1 << ")";
    return os.str();
}

/**
 * render_btype: This method calls the get_rs1, the get_rs2, and the get_imm_b
 *    methods first, and then this method prints out a string value cantaining
 *    the values that we get from calling the previously described methods.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 * @param mnemonic A character variable that stores the name of a given instruction.
 *
 * @return A string value that prints the name of the instruction which is stored
 *    in a character variable called mnemonic, the rs1 value of the mnemonic, the rs2
 *    value of the mnemonic, and the imm_b (plus pc) value of the mnemonic.
 *
 * @note This render_btype method is the sixth render method in this rv32i.cpp
 *    file (also, all of the render methods return a string).
 ********************************************************************************/
std::string rv32i::render_btype(uint32_t insn, const char *mnemonic) const
{
    // Calling the get_rs1, the get_rs2, and the get_imm_b methods.
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    int32_t imm_b = get_imm_b(insn);
    
    // Returning a string that prints out the mnemonic, the rs1 value, the rs2 value, and the imm_b (plus pc) value of the instruction.
    std::ostringstream os;
    os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << mnemonic
    << "  x" << std::dec << rs1 << ",x" << rs2 << ",0x" <<  std::hex << (imm_b + pc);
    return os.str();
}

/**
 * render_itype_load: This method calls the get_rd, the get_rs1, and the get_imm_i
 *    methods first, and then this method prints out a string value cantaining
 *    the values that we get from calling the previously described methods.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 * @param mnemonic A character variable that stores the name of a given instruction.
 *
 * @return A string value that prints the name of the instruction which is stored
 *    in a character variable called mnemonic, the rd value of the mnemonic, the
 *    imm_i value of the mnemonic, and the rs1 value of the mnemonic.
 *
 * @note This render_itype_load method is the seventh render method in this rv32i.cpp
 *    file (also, all of the render methods return a string).
 ********************************************************************************/
std::string rv32i::render_itype_load(uint32_t insn, const char *mnemonic) const
{
    // Calling the get_rd, the get_rs1, and the get_imm_i methods.
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    int32_t imm_i = get_imm_i(insn);
    
    // Returning a string that prints out the mnemonic, the rd value, the imm_i value, and the rs1 value of the instruction.
    std::ostringstream os;
    os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << mnemonic
    << "  x" << std::dec << rd << "," << imm_i << "(x" << rs1 << ")";
    return os.str();
}

/**
 * render_stype: This method calls the get_rs1, the get_rs2, and the get_imm_s
 *    methods first, and then this method prints out a string value cantaining
 *    the values that we get from calling the previously described methods.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 * @param mnemonic A character variable that stores the name of a given instruction.
 *
 * @return A string value that prints the name of the instruction which is stored
 *    in a character variable called mnemonic, the rs2 value of the mnemonic, the
 *    imm_s value of the mnemonic, and the rs1 value of the mnemonic.
 *
 * @note This render_stype method is the eighth render method in this rv32i.cpp
 *    file (also, all of the render methods return a string).
 ********************************************************************************/
std::string rv32i::render_stype(uint32_t insn, const char *mnemonic) const
{
    // Calling the get_rs1, the get_rs2, and the get_imm_s methods.
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    int32_t imm_s = get_imm_s(insn);
    
    // Returning a string that prints out the mnemonic, the rs2 value, the imm_s value, and the rs1 value of the instruction.
    std::ostringstream os;
    os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << mnemonic
    << "  x" << std::dec << rs2 << "," << imm_s << "(x" << rs1 << ")";
    return os.str();
}

/**
 * render_itype_alu: This method calls the get_rd, and the get_rs1 methods first,
 *    and then this method prints out a string value cantaining the values that
 *    we get from calling the previously described methods.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 * @param mnemonic A character variable that stores the name of a given instruction.
 * @param imm_i An integer type variable with a width of exactly 32 bits.
 *
 * @return A string value that prints the name of the instruction which is stored
 *    in a character variable called mnemonic, the rd value of the mnemonic, the
 *    rs1 value of the mnemonic, and the imm_i value of the mnemonic.
 *
 * @note This render_itype_alu method is the ninth render method in this rv32i.cpp
 *    file (also, all of the render methods return a string).
 ********************************************************************************/
std::string rv32i::render_itype_alu(uint32_t insn, const char *mnemonic, int32_t imm_i) const
{
    // Calling the get_rd, and the get_rs1 methods.
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    
    // Returning a string that prints out the mnemonic, the rd value, the rs1 value, and the imm_i value of the instruction.
    std::ostringstream os;
    os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << mnemonic
    << "  x" << std::dec << rd << ",x" << rs1 << "," << imm_i;
    return os.str();
}

/**
 * render_rtype: This method calls the get_rd, the get_rs1, and the get_rs2
 *    methods first, and then this method prints out a string value cantaining
 *    the values that we get from calling the previously described methods.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 * @param mnemonic A character variable that stores the name of a given instruction.
 *
 * @return A string value that prints the name of the instruction which is stored
 *    in a character variable called mnemonic, the rd value of the mnemonic, the
 *    rs1 value of the mnemonic, and the rs2 value of the mnemonic.
 *
 * @note This render_rtype method is the tenth render method in this rv32i.cpp
 *    file (also, all of the render methods return a string).
 ********************************************************************************/
std::string rv32i::render_rtype(uint32_t insn, const char *mnemonic) const
{
    // Calling the get_rd, the get_rs1, and the get_rs2 methods.
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    
    // Returning a string that prints out the mnemonic, the rd value, the rs1 value, and the rs2 value of the instruction.
    std::ostringstream os;
    os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << mnemonic
    << "  x" << std::dec << rd << ",x" << rs1 << ",x" << rs2;
    return os.str();
}

/**
 * render_fence: This method calls the get_rd, the get_rs1, and the get_rs2
 *    methods first, and then this method prints out a string value cantaining
 *    the values that we get from calling the previously described methods.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 *
 * @return A string value that prints the name of the instruction which is stored
 *    in a character variable called mnemonic, the rd value of the mnemonic, the
 *    rs1 value of the mnemonic, and the rs2 value of the mnemonic.
 *
 * @note This render_fence method is the eleventh render method in this rv32i.cpp
 *    file (also, all of the render methods return a string).
 ********************************************************************************/
std::string rv32i::render_fence(uint32_t insn) const
{
    // Here are all of the predecessor and successor variables.
    std::string predI;
    std::string predO;
    std::string predR;
    std::string predW;
    std::string succI;
    std::string succO;
    std::string succR;
    std::string succW;
    
    // Checking to see is the the 27th bit of the insn variable is one.
    if (insn & 0x08000000)
    {
        predI = "i";   // Setting predI equal to i.
    }
    
    // Checking to see is the the 26th bit of the insn variable is one.
    if (insn & 0x04000000)
    {
        predO = "o";   // Setting predO equal to o.
    }
    
    // Checking to see is the the 25th bit of the insn variable is one.
    if (insn & 0x02000000)
    {
        predR = "r";   // Setting predR equal to r.
    }
    
    // Checking to see is the the 24th bit of the insn variable is one.
    if (insn & 0x01000000)
    {
        predW = "w";   // Setting predW equal to w.
    }
    
    // Checking to see is the the 23th bit of the insn variable is one.
    if (insn & 0x00800000)
    {
        succI = "i";   // Setting succI equal to i.
    }
    
    // Checking to see is the the 22th bit of the insn variable is one.
    if (insn & 0x00400000)
    {
        succO = "o";   // Setting succO equal to o.
    }
    
    // Checking to see is the the 21th bit of the insn variable is one.
    if (insn & 0x00200000)
    {
        succR = "r";   // Setting succR equal to r.
    }
    
    // Checking to see is the the 20th bit of the insn variable is one.
    if (insn & 0x00100000)
    {
        succW = "w";   // Setting succW equal to w.
    }
    
    // Returning a string that prints out the predecessor, and the successor variables.
    std::ostringstream os;
    os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << "  fence   "
    << predI << predO << predR << predW << "," << succI << succO << succR << succW;
    return os.str();
}

/**
 * render_ecall: This method just returns a string value that lets the program
 *    know that an ecall has occured.
 *
 * No parameter values.
 *
 * @return A string value that only prints the phrase "  ecall" out to the standard
 *    output.
 *
 * @note This render_ecall method is the twelfth render method in this rv32i.cpp
 *    file (also, all of the render methods return a string).
 ********************************************************************************/
std::string rv32i::render_ecall() const
{
    return "  ecall";   // Returning the phrase "  ecall".
}

/**
 * render_ebreak: This method just returns a string value that lets the program
 *    know that an ebreak has occured.
 *
 * No parameter values.
 *
 * @return A string value that only prints the phrase "  ebreak" out to the standard
 *    output.
 *
 * @note This render_ebreak method is the thirteenth render method in this rv32i.cpp
 *    file (also, all of the render methods return a string).
 ********************************************************************************/
std::string rv32i::render_ebreak() const
{
    return "  ebreak";   // Returning the phrase "  ebreak".
}

/**
 * get_opcode: This method returns a value which we get from anding the insn
 *    parameter with the opcode value.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 *
 * @return We return the opcode value of the given insn unsigned integer type
 *    variable with a width of exactly 32 bits.
 *
 * @note This is the get_opcode method, we also have get methods for rd, funct3
 *    rs1, rs2, and funct7.
 ********************************************************************************/
uint32_t rv32i::get_opcode(uint32_t insn)
{
    return (insn & 0x0000007f);   // Returning the insn opcode value.
}

/**
 * get_rd: This method returns a value which we get from anding the insn
 *    parameter with the rd value.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 *
 * @return We return the rd value of the given insn unsigned integer type
 *    variable with a width of exactly 32 bits.
 *
 * @note This is the get_rd method, we also have get methods for opcode, funct3
 *    rs1, rs2, and funct7.
 ********************************************************************************/
uint32_t rv32i::get_rd(uint32_t insn)
{
    return (insn & 0x00000f80) >> (7 - 0);   // Returning the insn rd value.
}

/**
 * get_funct3: This method returns a value which we get from anding the insn
 *    parameter with the funct3 value.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 *
 * @return We return the funct3 value of the given insn unsigned integer type
 *    variable with a width of exactly 32 bits.
 *
 * @note This is the get_funct3 method, we also have get methods for opcode, rd
 *    rs1, rs2, and funct7.
 ********************************************************************************/
uint32_t rv32i::get_funct3(uint32_t insn)
{
    return (insn & 0x00007000) >> (12 - 0);   // Returning the insn funct3 value.
}

/**
 * get_rs1: This method returns a value which we get from anding the insn
 *    parameter with the rs1 value.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 *
 * @return We return the rs1 value of the given insn unsigned integer type
 *    variable with a width of exactly 32 bits.
 *
 * @note This is the get_rs1 method, we also have get methods for opcode, rd
 *    funct3, rs2, and funct7.
 ********************************************************************************/
uint32_t rv32i::get_rs1(uint32_t insn)
{
    return (insn & 0x000f8000) >> (15 - 0);   // Returning the insn rs1 value.
}

/**
 * get_rs2: This method returns a value which we get from anding the insn
 *    parameter with the rs2 value.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 *
 * @return We return the rs2 value of the given insn unsigned integer type
 *    variable with a width of exactly 32 bits.
 *
 * @note This is the get_rs2 method, we also have get methods for opcode, rd
 *    funct3, rs1, and funct7.
 ********************************************************************************/
uint32_t rv32i::get_rs2(uint32_t insn)
{
    return (insn & 0x01f00000) >> (20 - 0);   // Returning the insn rs2 value.
}

/**
 * get_funct7: This method returns a value which we get from anding the insn
 *    parameter with the funct7 value.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 *
 * @return We return the funct7 value of the given insn unsigned integer type
 *    variable with a width of exactly 32 bits.
 *
 * @note This is the get_funct7 method, we also have get methods for opcode, rd
 *    funct3, rs1, and rs2.
 ********************************************************************************/
uint32_t rv32i::get_funct7(uint32_t insn)
{
    return (insn & 0xfe000000) >> (25 - 0);   // Returning the insn funct7 value.
}

/**
 * get_imm_i: This method returns a value which we get from anding the insn
 *    parameter with the imm_i value, and shifting the value that we get (we
 *    also sign-extend the imm_i value before returning it).
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 *
 * @return We return the imm_i value of the given insn unsigned integer type
 *    variable with a width of exactly 32 bits
 *
 * @note This is the get_imm_i method, we also have get methods for get_imm_u,
 *    get_imm_b, get_imm_s, and get_imm_j.
 ********************************************************************************/
int32_t rv32i::get_imm_i(uint32_t insn)
{
    // Getting the imm_i value by anding the insn parameter with the imm_i value, and shifting (we do this one time).
    int32_t imm_i = (insn & 0xfff00000) >> (20 - 0);
    
    // Using an if statement to sign-extend the imm_i value.
    if (insn & 0x80000000)
        imm_i |= 0xfffff000;
    
    return imm_i;   // Returning the imm_i value.
}

/**
 * get_imm_u: This method returns a value which we get from anding the insn
 *    parameter with the imm_u value, and shifting the value that we get (we
 *    do not need to sign-extend the imm_u value before returning it).
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 *
 * @return We return the imm_u value of the given insn unsigned integer type
 *    variable with a width of exactly 32 bits
 *
 * @note This is the get_imm_u method, we also have get methods for get_imm_i,
 *    get_imm_b, get_imm_s, and get_imm_j.
 ********************************************************************************/
int32_t rv32i::get_imm_u(uint32_t insn)
{
    // Getting the imm_u value by anding the insn parameter with the imm_u value, and shifting (we do this one time).
    int32_t imm_u = (insn & 0xfffff000);
    
    return imm_u;   // Returning the imm_u value.
}

/**
 * get_imm_b: This method returns a value which we get from anding the insn
 *    parameter with the imm_b value, and shifting the value that we get (we
 *    also sign-extend the imm_b value before returning it).
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 *
 * @return We return the imm_b value of the given insn unsigned integer type
 *    variable with a width of exactly 32 bits
 *
 * @note This is the get_imm_b method, we also have get methods for get_imm_i,
 *    get_imm_u, get_imm_s, and get_imm_j.
 ********************************************************************************/
int32_t rv32i::get_imm_b(uint32_t insn)
{
    // Getting the imm_b value by anding the insn parameter with the imm_b value, and shifting (we do this four times).
    int32_t imm_b = (insn & 0x80000000) >> (31 - 12);
    imm_b |= (insn & 0x00000080) << (11 - 7);
    imm_b |= (insn & 0x7e000000) >> (25 - 5);
    imm_b |= (insn & 0x00000f00) >> (8 - 1);
    
    // Using an if statement to sign-extend the imm_b value.
    if (insn & 0x80000000)
        imm_b |= 0xffffe000;
    
    return imm_b;   // Returning the imm_b value.
    
}

/**
 * get_imm_s: This method returns a value which we get from anding the insn
 *    parameter with the imm_s value, and shifting the value that we get (we
 *    also sign-extend the imm_s value before returning it).
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 *
 * @return We return the imm_s value of the given insn unsigned integer type
 *    variable with a width of exactly 32 bits
 *
 * @note This is the get_imm_s method, we also have get methods for get_imm_i,
 *    get_imm_u, get_imm_b, and get_imm_j.
 ********************************************************************************/
int32_t rv32i::get_imm_s(uint32_t insn)
{
    // Getting the imm_s value by anding the insn parameter with the imm_s value, and shifting (we do this two times).
    int32_t imm_s = (insn & 0xfe000000) >> (25 - 5);
    imm_s |= (insn & 0x00000f80) >> (7 - 0);
    
    // Using an if statement to sign-extend the imm_s value.
    if (insn & 0x80000000)
        imm_s |= 0xfffff000;
    
    return imm_s;   // Returning the imm_s value.
}

/**
 * get_imm_j: This method returns a value which we get from anding the insn
 *    parameter with the imm_j value, and shifting the value that we get (we
 *    also sign-extend the imm_j value before returning it).
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 *
 * @return We return the imm_j value of the given insn unsigned integer type
 *    variable with a width of exactly 32 bits
 *
 * @note This is the get_imm_j method, we also have get methods for get_imm_i,
 *    get_imm_u, get_imm_b, and get_imm_s.
 ********************************************************************************/
int32_t rv32i::get_imm_j(uint32_t insn)
{
    // Getting the imm_j value by anding the insn parameter with the imm_j value, and shifting (we do this four times).
    int32_t imm_j = (insn & 0x80000000) >> (31 - 20);
    imm_j |= (insn & 0x000ff000);
    imm_j |= (insn & 0x00100000) >> (20 - 11);
    imm_j |= (insn & 0x7fe00000) >> (21 - 1);
    
    // Using an if statement to sign-extend the imm_j value.
    if (insn & 0x80000000)
        imm_j |= 0xffe00000;
    
    return imm_j;   // Returning the imm_j value.
}

/**
 * set_show_instructions: This method is the mutator for the show_instructions.
 *    variable. Which means that this method sets the show_instructions variable
 *    equal to b.
 *
 * @param b A boolean variable that is set to the show_instructions variable.
 *
 * No return values.
 *
 * @note This is the set_show_instructions method, there is also a similar method
 *    that is called set_show_registers.
 ********************************************************************************/
void rv32i::set_show_instructions(bool b)
{
    show_instructions = b;   // Setting the show_instructions variable equal to b.
}

/**
 * set_show_registers: This method is the mutator for show_registers variable.
 *    Which means that this method sets the show_registers variable equal to b.
 *
 * @param b A boolean variable that is set to the show_registers variable.
 *
 * No return values.
 *
 * @note This is the set_show_registers method, there is also a similar method
 *    that is called set_show_instructions.
 ********************************************************************************/
void rv32i::set_show_registers(bool b)
{
    show_registers = b;   // Setting the show_registers variable equal to b.
}

/**
 * is_halted: This method returns the halt variable which will be set to true later on
 *    in this program. This halt variable is originally set to false in the rv32i class
 *
 * No parameter values.
 *
 * @return We return the halt variable that we created in the rv32i class.
 *
 * @note This is the is_halted method, we can use this is_halted method in other
 *    methods to work with the halt variable, or we can just use the halt variable
 *    by itself.
 ********************************************************************************/
bool rv32i::is_halted() const
{
    return halt;   // Returning the halt variable.
}

/**
 * reset: This method resets the rv32i object and the registerfile by calling the
 *    registerfile reset method, setting the pc variable equal to 0, setting the
 *    insn_counter variable equal to 0, and setting the halt variable equal to false.
 *
 * No parameter values.
 *
 * No return values.
 *
 * @note This is the set method for the registerfile class, there is also a get
 *    method for the registerfile class.
 ********************************************************************************/
void rv32i::reset()
{
    regs.reset();   // Calling the registerfile reset method.
    pc = 0;   // Setting the pc variable equal to 0.
    insn_counter = 0;   // Setting the insn_counter variable equal to 0.
    halt = false;   // Setting the halt variable equal to false.
}

/**
 * dump: This method calls the registerfile dump method, and adds on the address
 *    of the instruction (which is the pc variable located in the rv32i class).
 *
 * No parameter values.
 *
 * No return values.
 *
 * @note This is the second dump method in this entire project, we have the first
 *    dump method in the memory.cpp file, and the third dump method is in the
 *    registerfile.cpp file.
 ********************************************************************************/
void rv32i::dump() const
{
    regs.dump();   // Calling the registerfile dump method.
    std::cout << std::endl;   // Printing out a line break.
    std::cout << " pc " << hex32(pc) << std::endl;   // Printing out the address of the instruction.
}

/**
 * dcex: This method will execute all of the rv32i instructions by calling the
 *    exec methods. These exec method call the render methods that we created in
 *    assignnment 4. This entire process is being done inside of a switch statement
 *    which checks for the opcode of all the rv32i instructions.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 * @param pos An ostream variable that is used as a parameter in the exec methods.
 *
 * No return values.
 *
 * @note This dcex method is the same as the decode method, but insted of calling
 *    render methods, we are now calling the exec methods (which also call the
 *    render methods inside of them).
 ********************************************************************************/
void rv32i::dcex(uint32_t insn, std::ostream* pos)
{
    // Calling the get_opcode, the get_funct3, the get_funct7, and the get_rs2 methods.
    uint32_t opcode = get_opcode(insn);
    uint32_t funct3 = get_funct3(insn);
    uint32_t funct7 = get_funct7(insn);
    uint32_t rs2 = get_rs2(insn);
    
    // Using a switch statement to check for the opcode value of all the rv32i instructions.
    switch(opcode)
    {
        default:                    exec_illegal_insn(insn, pos); return; // Printing out the result of the exec_illegal_insn method.
        case opcode_lui:            exec_lui(insn, pos); return; // Printing out the result of the exec_lui method.
        case opcode_auipc:          exec_auipc(insn, pos); return; // Printing out the result of the exec_auipc method.
        // Printing out the result of the exec_rtype method.
        case opcode_rtype:
            // Using a switch statement to check for the funct3 value of R-type instructions.
            switch(funct3)
        {
            default:                exec_illegal_insn(insn, pos); return; // Printing out the result of the exec_illegal_insn method.
            case funct3_add:
                // Using a switch statement to check for the funct7 value of R-type instructions.
                switch(funct7)
            {
                default:            exec_illegal_insn(insn, pos); return; // Printing out the result of the exec_illegal_insn method.
                case funct7_add:    exec_add(insn, pos); return; // Printing out the result of the exec_add method.
                case funct7_sub:    exec_sub(insn, pos); return; // Printing out the result of the exec_sub method.
            }
                assert(0 && "unhandled funct7");
            case funct3_sll:        exec_sll(insn, pos); return; // Printing out the result of the exec_sll method.
            case funct3_slt:        exec_slt(insn, pos); return; // Printing out the result of the exec_slt method.
            case funct3_sltu:       exec_sltu(insn, pos); return; // Printing out the result of the exec_sltu method.
            case funct3_xor:        exec_xor(insn, pos); return; // Printing out the result of the exec_xor method.
            case funct3_srl:
                // Using a switch statement to check for the funct7 value of R-type instructions.
                switch(funct7)
            {
                default:            exec_illegal_insn(insn, pos); return; // Printing out the result of the exec_illegal_insn method.
                case funct7_srl:    exec_srl(insn, pos); return; // Printing out the result of the exec_srl method.
                case funct7_sra:    exec_sra(insn, pos); return; // Printing out the result of the exec_sra method.
            }
                assert(0 && "unhandled funct7");
            case funct3_or:         exec_or(insn, pos); return; // Printing out the result of the exec_or method.
            case funct3_and:        exec_and(insn, pos); return; // Printing out the result of the exec_and method.
        }
            assert(0 && "unhandled funct3");
        // Printing out the result of the exec_itype_alu method.
        case opcode_alu_imm:
            // Using a switch statement to check for the funct3 value of I-type alu instructions.
            switch (funct3)
        {
            default:                exec_illegal_insn(insn, pos); return; // Printing out the result of the exec_illegal_insn method.
            case funct3_slli:       exec_slli(insn, pos); return; // Printing out the result of the exec_slli method.
            case funct3_srli:
                // Using a switch statement to check for the funct7 value of I-type alu instructions.
                switch(funct7)
            {
                default:            exec_illegal_insn(insn, pos); return; // Printing out the result of the exec_illegal_insn method.
                case funct7_srli:   exec_srli(insn, pos); return; // Printing out the result of the exec_srli method.
                case funct7_srai:   exec_srai(insn, pos); return; // Printing out the result of the exec_srai method.
            }
                assert(0 && "unhandled funct7");
            case funct3_addi:       exec_addi(insn, pos); return; // Printing out the result of the exec_addi method.
            case funct3_slti:       exec_slti(insn, pos); return; // Printing out the result of the exec_slti method.
            case funct3_sltiu:      exec_sltiu(insn, pos); return; // Printing out the result of the exec_sltiu method.
            case funct3_xori:       exec_xori(insn, pos); return; // Printing out the result of the exec_xori method.
            case funct3_ori:        exec_ori(insn, pos); return; // Printing out the result of the exec_ori method.
            case funct3_andi:       exec_andi(insn, pos); return; // Printing out the result of the exec_andi method.
        }
            assert(0 && "unhandled funct3");
        case opcode_jal:            exec_jal(insn, pos); return; // Printing out the result of the exec_jal method.
        case opcode_jalr:           exec_jalr(insn, pos); return; // Printing out the result of the exec_jalr method.
        case opcode_fence:          exec_fence(insn, pos); return; // Printing out the result of the exec_fence method.
        // Printing out the result of the exec_itype_load method.
        case opcode_load_imm:
            // Using a switch statement to check for the funct3 value of I-type load instructions.
            switch (funct3)
        {
            default:                exec_illegal_insn(insn, pos); return; // Printing out the result of the exec_illegal_insn method.
            case funct3_lb:         exec_lb(insn, pos); return; // Printing out the result of the exec_lb method.
            case funct3_lh:         exec_lh(insn, pos); return; // Printing out the result of the exec_lh method.
            case funct3_lw:         exec_lw(insn, pos); return; // Printing out the result of the exec_lw method.
            case funct3_lbu:        exec_lbu(insn, pos); return; // Printing out the result of the exec_lbu method.
            case funct3_lhu:        exec_lhu(insn, pos); return; // Printing out the result of the exec_lhu method.
        }
            assert(0 && "unhandled funct3");
        // Printing out the result of the exec_stype method.
        case opcode_stype:
            // Using a switch statement to check for the funct3 value of S-type instructions.
            switch (funct3)
        {
            default:                exec_illegal_insn(insn, pos); return; // Printing out the result of the exec_illegal_insn method.
            case funct3_sb:         exec_sb(insn, pos); return; // Printing out the result of the exec_sb method.
            case funct3_sh:         exec_sh(insn, pos); return; // Printing out the result of the exec_sh method.
            case funct3_sw:         exec_sw(insn, pos); return; // Printing out the result of the exec_sw method.
        }
            assert(0 && "unhandled funct3");
        // Printing out the result of the exec_btype method.
        case opcode_btype:
            // Using a switch statement to check for the funct3 value of B-type instructions.
            switch (funct3)
        {
            default:                exec_illegal_insn(insn, pos); return; // Printing out the result of the exec_illegal_insn method.
            case funct3_beq:        exec_beq(insn, pos); return; // Printing out the result of the exec_beq method.
            case funct3_bne:        exec_bne(insn, pos); return; // Printing out the result of the exec_bne method.
            case funct3_blt:        exec_blt(insn, pos); return; // Printing out the result of the exec_blt method.
            case funct3_bge:        exec_bge(insn, pos); return; // Printing out the result of the exec_bge method.
            case funct3_bltu:       exec_bltu(insn, pos); return; // Printing out the result of the exec_bltu method.
            case funct3_bgeu:       exec_bgeu(insn, pos); return; // Printing out the result of the exec_bgeu method.
        }
            assert(0 && "unhandled funct3");
        case opcode_ecall:
             // Using a switch statement to check for the funct7 plus rs2 value of ecall/ebreak instructions.
            switch (funct7 + rs2)
        {
            default:                exec_illegal_insn(insn, pos); return; // Printing out the result of the exec_illegal_insn method.
            case funct7_rs2_ecall:  exec_illegal_insn(insn, pos); return; // Printing out the result of the exec_illegal_insn method.
            case funct7_rs2_ebreak: exec_ebreak(insn, pos); return; // Printing out the result of the exec_ebreak method.
        }
            assert(0 && "unhandled funct3");
    }
    assert(0 && "unhandled opcode");
}

/**
 * exec_illegal_insn: This method will use an if statement to call the render_illegal_insn
 *    method, and prints an exec_illegal_insn string out to the standard output.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 * @param pos An ostream variable that is used to print a string to standard output.
 *
 * No return values.
 *
 * @note This is the exec_illegal_insn method, and this method is the 1st out of the 40
 *    exec methods that we create in this program.
 ********************************************************************************/
void rv32i::exec_illegal_insn(uint32_t insn, std::ostream* pos)
{
    // If pos, we call that render_illegal_insn method, and we print an exec_illegal_insn string to standard output.
    if (pos)
    {
        std::string s = render_illegal_insn();
        s.resize(instruction_width , ' ');
        *pos << s;
    }
}

/**
 * exec_lui: This method will use an if statement to call the render_lui method,
 *    and prints an exec_lui string out to the standard output. Then this method sets
 *    the imm_u variable to the rd variable, and increments the pc variable by four.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 * @param pos An ostream variable that is used to print a string to standard output.
 *
 * No return values.
 *
 * @note This is the exec_lui method, and this method is the 2nd out of the 40
 *    exec methods that we create in this program.
 ********************************************************************************/
void rv32i::exec_lui(uint32_t insn, std::ostream* pos)
{
    // Creating the two variables that we need for this exec_lui method.
    uint32_t rd = get_rd(insn);
    uint32_t imm_u = get_imm_u(insn);
    
    // If pos, we call that render_lui method, and we print an exec_lui string to standard output.
    if (pos)
    {
        std::string s = render_lui(insn);
        s.resize(instruction_width , ' ');
        *pos << s << "  // x" << rd << " = " << hex0x32(imm_u);
        std::cout << std::endl;   // Printing out a line break.
    }
    
    regs.set(rd, imm_u);   // Setting the imm_u variable to the rd variable.
    
    pc += 4;   // Incrementing the pc variable by four.
}

/**
 * exec_auipc: This method will use an if statement to call the render_auipc method,
 *    and prints an exec_auipc string out to the standard output. Then this method sets
 *    the val variable to the rd variable, and increments the pc variable by four.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 * @param pos An ostream variable that is used to print a string to standard output.
 *
 * No return values.
 *
 * @note This is the exec_auipc method, and this method is the 3rd out of the 40
 *    exec methods that we create in this program.
 ********************************************************************************/
void rv32i::exec_auipc(uint32_t insn, std::ostream* pos)
{
    // Creating the three variables that we need for this exec_auipc method.
    uint32_t rd = get_rd(insn);
    uint32_t imm_u = get_imm_u(insn);
    uint32_t val = pc + imm_u;
    
    // If pos, we call that render_auipc method, and we print an exec_auipc string to standard output.
    if (pos)
    {
        std::string s = render_auipc(insn);
        s.resize(instruction_width , ' ');
        *pos << s << "  // x" << rd << " = " << hex0x32(pc)
        << " + " << hex0x32(imm_u) << " = " << hex0x32(val);
        std::cout << std::endl;   // Printing out a line break.
    }
    
    regs.set(rd, val);   // Setting the val variable to the rd variable.
    
    pc += 4;   // Incrementing the pc variable by four.
}

/**
 * exec_jal: This method will use an if statement to call the render_jal method,
 *    and prints an exec_jal string out to the standard output. Then this method sets
 *    the val variable to the rd variable, and makes the pc variable equal to the
 *    val2 variable.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 * @param pos An ostream variable that is used to print a string to standard output.
 *
 * No return values.
 *
 * @note This is the exec_jal method, and this method is the 4th out of the 40
 *    exec methods that we create in this program.
 ********************************************************************************/
void rv32i::exec_jal(uint32_t insn, std::ostream* pos)
{
    // Creating the four variables that we need for this exec_jal method.
    uint32_t rd = get_rd(insn);
    uint32_t imm_j = get_imm_j(insn);
    uint32_t val = pc + 4;
    uint32_t val2 = pc + imm_j;
    
    // If pos, we call that render_jal method, and we print an exec_jal string to standard output.
    if (pos)
    {
        std::string s = render_jal(insn);
        s.resize(instruction_width , ' ');
        *pos << s << "  // x" << rd << " = " << hex0x32(val) << ",  pc = "
        << hex0x32(pc) << " + " << hex0x32(imm_j) << " = " << hex0x32(val2);
        std::cout << std::endl;   // Printing out a line break.
    }
    
    regs.set(rd, val);   // Setting the val variable to the rd variable.
    
    pc = val2;   // Making the pc variable equal to the val2 variable.
}

/**
 * exec_jalr: This method will use an if statement to call the render_jalr method,
 *    and prints an exec_jalr string out to the standard output. Then this method sets
 *    the val variable to the rd variable, and makes the pc variable equal to the
 *    val2 variable.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 * @param pos An ostream variable that is used to print a string to standard output.
 *
 * No return values.
 *
 * @note This is the exec_jal method, and this method is the 5th out of the 40
 *    exec methods that we create in this program.
 ********************************************************************************/
void rv32i::exec_jalr(uint32_t insn, std::ostream* pos)
{
    // Creating the five variables that we need for this exec_jalr method.
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);;
    int32_t imm_i = get_imm_i(insn);
    int32_t val = pc + 4;
    int32_t val2 = (imm_i + regs.get(rs1)) & 0xfffffffe;
    
    // If pos, we call that render_jalr method, and we print an exec_jalr string to standard output.
    if (pos)
    {
        std::string s = render_jalr(insn);
        s.resize(instruction_width , ' ');
        *pos << s << "  // x" << rd << " = " << hex0x32(val) << ",  pc = (" << hex0x32(imm_i)
        << " + " << hex0x32(regs.get(rs1)) << ") & 0xfffffffe" << " = " << hex0x32(val2);
        std::cout << std::endl;   // Printing out a line break.
    }
    
    regs.set(rd, val);   // Setting the val variable to the rd variable.
    
    pc = val2;   // Making the pc variable equal to the val2 variable.
}

/**
 * exec_bne: This method will use an if statement to call the render_btype method,
 *    and prints an exec_bne string out to the standard output. Then this method
 *    makes the pc variable equal to the val3 variable.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 * @param pos An ostream variable that is used to print a string to standard output.
 *
 * No return values.
 *
 * @note This is the exec_bne method, and this method is the 6th out of the 40
 *    exec methods that we create in this program.
 ********************************************************************************/
void rv32i::exec_bne(uint32_t insn, std::ostream* pos)
{
    // Creating the six variables that we need for this exec_bne method.
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    uint32_t imm_b = get_imm_b(insn);
    uint32_t val = pc + imm_b;
    uint32_t val2 = pc + 4;
    uint32_t val3 = (regs.get(rs1) != regs.get(rs2) ? val : val2);
    
    // If pos, we call that render_btype method, and we print an exec_bne string to standard output.
    if (pos)
    {
        std::string s = render_btype(insn, "  bne");
        s.resize(instruction_width , ' ');
        *pos << s << "  // pc += (" << hex0x32(regs.get(rs1)) << " != " << hex0x32(regs.get(rs2))
        << " ? " << hex0x32(imm_b) << " : 4) = " << hex0x32(val3);
        std::cout << std::endl;   // Printing out a line break.
    }
    
    pc = val3;   // Making the pc variable equal to the val3 variable.
}

/**
 * exec_blt: This method will use an if statement to call the render_btype method,
 *    and prints an exec_blt string out to the standard output. Then this method
 *    makes the pc variable equal to the val3 variable.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 * @param pos An ostream variable that is used to print a string to standard output.
 *
 * No return values.
 *
 * @note This is the exec_blt method, and this method is the 7th out of the 40
 *    exec methods that we create in this program.
 ********************************************************************************/
void rv32i::exec_blt(uint32_t insn, std::ostream* pos)
{
    // Creating the six variables that we need for this exec_blt method.
    int32_t rs1 = get_rs1(insn);
    int32_t rs2 = get_rs2(insn);
    int32_t imm_b = get_imm_b(insn);
    int32_t val = pc + imm_b;
    int32_t val2 = pc + 4;
    int32_t val3 = (regs.get(rs1) < regs.get(rs2) ? val : val2);
    
    // If pos, we call that render_btype method, and we print an exec_blt string to standard output.
    if (pos)
    {
        std::string s = render_btype(insn, "  blt");
        s.resize(instruction_width , ' ');
        *pos << s << "  // pc += (" << hex0x32(regs.get(rs1)) << " < " << hex0x32(regs.get(rs2))
        << " ? " << hex0x32(imm_b) << " : 4) = " << hex0x32(val3);
        std::cout << std::endl;   // Printing out a line break.
    }
    
    pc = val3;   // Making the pc variable equal to the val3 variable.
}

/**
 * exec_bge: This method will use an if statement to call the render_btype method,
 *    and prints an exec_bge string out to the standard output. Then this method
 *    makes the pc variable equal to the val3 variable.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 * @param pos An ostream variable that is used to print a string to standard output.
 *
 * No return values.
 *
 * @note This is the exec_bge method, and this method is the 8th out of the 40
 *    exec methods that we create in this program.
 ********************************************************************************/
void rv32i::exec_bge(uint32_t insn, std::ostream* pos)
{
    // Creating the six variables that we need for this exec_bge method.
    int32_t rs1 = get_rs1(insn);
    int32_t rs2 = get_rs2(insn);
    int32_t imm_b = get_imm_b(insn);
    int32_t val = pc + imm_b;
    int32_t val2 = pc + 4;
    int32_t val3 = (regs.get(rs1) >= regs.get(rs2) ? val : val2);
    
    // If pos, we call that render_btype method, and we print an exec_bge string to standard output.
    if (pos)
    {
        std::string s = render_btype(insn, "  bge");
        s.resize(instruction_width , ' ');
        *pos << s << "  // pc += (" << hex0x32(regs.get(rs1)) << " >= " << hex0x32(regs.get(rs2))
        << " ? " << hex0x32(imm_b) << " : 4) = " << hex0x32(val3);
        std::cout << std::endl;   // Printing out a line break.
    }
    
    pc = val3;   // Making the pc variable equal to the val3 variable.
}

/**
 * exec_bltu: This method will use an if statement to call the render_btype method,
 *    and prints an exec_bltu string out to the standard output. Then this method
 *    makes the pc variable equal to the val3 variable.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 * @param pos An ostream variable that is used to print a string to standard output.
 *
 * No return values.
 *
 * @note This is the exec_bltu method, and this method is the 9th out of the 40
 *    exec methods that we create in this program.
 ********************************************************************************/
void rv32i::exec_bltu(uint32_t insn, std::ostream* pos)
{
    // Creating the six variables that we need for this exec_bltu method.
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    int32_t imm_b = get_imm_b(insn);
    uint32_t val = pc + imm_b;
    uint32_t val2 = pc + 4;
    uint32_t val3 = ((uint32_t)regs.get(rs1) < (uint32_t)regs.get(rs2) ? val : val2);
    
    // If pos, we call that render_btype method, and we print an exec_bltu string to standard output.
    if (pos)
    {
        std::string s = render_btype(insn, "  bltu");
        s.resize(instruction_width , ' ');
        *pos << s << "  // pc += (" << hex0x32(regs.get(rs1)) << " <U " << hex0x32(regs.get(rs2))
        << " ? " << hex0x32(imm_b) << " : 4) = " << hex0x32(val3);
        std::cout << std::endl;   // Printing out a line break.
    }
    
    pc = val3;   // Making the pc variable equal to the val3 variable.
}

/**
 * exec_bgeu: This method will use an if statement to call the render_btype method,
 *    and prints an exec_bgeu string out to the standard output. Then this method
 *    makes the pc variable equal to the val3 variable.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 * @param pos An ostream variable that is used to print a string to standard output.
 *
 * No return values.
 *
 * @note This is the exec_bgeu method, and this method is the 10th out of the 40
 *    exec methods that we create in this program.
 ********************************************************************************/
void rv32i::exec_bgeu(uint32_t insn, std::ostream* pos)
{
    // Creating the six variables that we need for this exec_bgeu method.
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    int32_t imm_b = get_imm_b(insn);
    uint32_t val = pc + imm_b;
    uint32_t val2 = pc + 4;
    uint32_t val3 = ((uint32_t)regs.get(rs1) >= (uint32_t)regs.get(rs2) ? val : val2);
    
    // If pos, we call that render_btype method, and we print an exec_bgeu string to standard output.
    if (pos)
    {
        std::string s = render_btype(insn, "  bgeu");
        s.resize(instruction_width , ' ');
        *pos << s << "  // pc += (" << hex0x32(regs.get(rs1)) << " >=U " << hex0x32(regs.get(rs2))
        << " ? " << hex0x32(imm_b) << " : 4) = " << hex0x32(val3);
        std::cout << std::endl;   // Printing out a line break.
    }
    
    pc = val3;   // Making the pc variable equal to the val3 variable.
}

/**
 * exec_beq: This method will use an if statement to call the render_btype method,
 *    and prints an exec_beq string out to the standard output. Then this method
 *    makes the pc variable equal to the val3 variable.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 * @param pos An ostream variable that is used to print a string to standard output.
 *
 * No return values.
 *
 * @note This is the exec_beq method, and this method is the 11th out of the 40
 *    exec methods that we create in this program.
 ********************************************************************************/
void rv32i::exec_beq(uint32_t insn, std::ostream* pos)
{
    // Creating the six variables that we need for this exec_beq method.
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    int32_t imm_b = get_imm_b(insn);
    uint32_t val = pc + imm_b;
    uint32_t val2 = pc + 4;
    uint32_t val3 = (regs.get(rs1) == regs.get(rs2) ? val : val2);
    
    // If pos, we call that render_btype method, and we print an exec_beq string to standard output.
    if (pos)
    {
        std::string s = render_btype(insn, "  beq");
        s.resize(instruction_width , ' ');
        *pos << s << "  // pc += (" << hex0x32(regs.get(rs1)) << " == " << hex0x32(regs.get(rs2))
        << " ? " << hex0x32(imm_b) << " : 4) = " << hex0x32(val3);
        std::cout << std::endl;   // Printing out a line break.
    }
    
    pc = val3;   // Making the pc variable equal to the val3 variable.
}

/**
 * exec_addi: This method will use an if statement to call the render_itype_alu method,
 *    and prints an exec_addi string out to the standard output. Then this method sets
 *    the val variable to the rd variable, and increments the pc variable by four.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 * @param pos An ostream variable that is used to print a string to standard output.
 *
 * No return values.
 *
 * @note This is the exec_addi method, and this method is the 12th out of the 40
 *    exec methods that we create in this program.
 ********************************************************************************/
void rv32i::exec_addi(uint32_t insn, std::ostream* pos)
{
    // Creating the four variables that we need for this exec_addi method.
    int32_t rd = get_rd(insn);
    int32_t rs1 = get_rs1(insn);
    int32_t imm_i = get_imm_i(insn);
    uint32_t val = (regs.get(rs1) + imm_i);
    
    // If pos, we call that render_itype_alu method, and we print an exec_addi string to standard output.
    if (pos)
    {
        std::string s = render_itype_alu(insn, "  addi", imm_i);
        s.resize(instruction_width , ' ');
        *pos << s << "  // x" << rd << " = " << hex0x32(regs.get(rs1))
        << " + " << hex0x32(imm_i) << " = "<< hex0x32(val);
        std::cout << std::endl;   // Printing out a line break.
    }
    
    regs.set(rd, val);   // Setting the val variable to the rd variable.
    
    pc += 4;   // Incrementing the pc variable by four.
}

/**
 * exec_lbu: This method will use an if statement to call the render_itype_load method,
 *    and prints an exec_lbu string out to the standard output. Then this method sets
 *    the val2 variable to the rd variable, and increments the pc variable by four.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 * @param pos An ostream variable that is used to print a string to standard output.
 *
 * No return values.
 *
 * @note This is the exec_lbu method, and this method is the 13th out of the 40
 *    exec methods that we create in this program.
 ********************************************************************************/
void rv32i::exec_lbu(uint32_t insn, std::ostream* pos)
{
    // Creating the five variables that we need for this exec_lbu method.
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    int32_t imm_i = get_imm_i(insn);
    uint32_t val = (regs.get(rs1) + imm_i);
    uint32_t val2 = (mem->get8(val) & 0x000000ff);
    
    // If pos, we call that render_itype_load method, and we print an exec_lbu string to standard output.
    if (pos)
    {
        std::string s = render_itype_load(insn, "  lbu");
        s.resize(instruction_width , ' ');
        *pos << s << "  // x" << rd << " = zx(m8(" << hex0x32(regs.get(rs1))
        << " + " << hex0x32(imm_i) << ")) = " << hex0x32(val2);
        std::cout << std::endl;   // Printing out a line break.
    }
    
    regs.set(rd, val2);   // Setting the val2 variable to the rd variable.
    
    pc += 4;   // Incrementing the pc variable by four.
}

/**
 * exec_lhu: This method will use an if statement to call the render_itype_load method,
 *    and prints an exec_lhu string out to the standard output. Then this method sets
 *    the val2 variable to the rd variable, and increments the pc variable by four.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 * @param pos An ostream variable that is used to print a string to standard output.
 *
 * No return values.
 *
 * @note This is the exec_lhu method, and this method is the 14th out of the 40
 *    exec methods that we create in this program.
 ********************************************************************************/
void rv32i::exec_lhu(uint32_t insn, std::ostream* pos)
{
    // Creating the five variables that we need for this exec_lhu method.
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    int32_t imm_i = get_imm_i(insn);
    uint32_t val = (regs.get(rs1) + imm_i);
    uint32_t val2 = (mem->get16(val) & 0x0000ffff);
    
    // If pos, we call that render_itype_load method, and we print an exec_lhu string to standard output.
    if (pos)
    {
        std::string s = render_itype_load(insn, "  lhu");
        s.resize(instruction_width , ' ');
        *pos << s << "  // x" << rd << " = zx(m16(" << hex0x32(regs.get(rs1))
        << " + " << hex0x32(imm_i) << ")) = " << hex0x32(val2);
        std::cout << std::endl;   // Printing out a line break.
    }
    
    regs.set(rd, val2);   // Setting the val2 variable to the rd variable.
    
    pc += 4;   // Incrementing the pc variable by four.
}

/**
 * exec_lb: This method will use an if statement to call the render_itype_load method,
 *    and prints an exec_lb string out to the standard output. Then this method sets
 *    the val2 variable to the rd variable, and increments the pc variable by four.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 * @param pos An ostream variable that is used to print a string to standard output.
 *
 * No return values.
 *
 * @note This is the exec_lb method, and this method is the 15th out of the 40
 *    exec methods that we create in this program.
 ********************************************************************************/
void rv32i::exec_lb(uint32_t insn, std::ostream* pos)
{
    // Creating the five variables that we need for this exec_lb method.
    int32_t rd = get_rd(insn);
    int32_t rs1 = get_rs1(insn);
    int32_t imm_i = get_imm_i(insn);
    uint32_t val = (regs.get(rs1) + imm_i);
    uint32_t val2 = mem->get8(val);
    
    
    // Using an if statement to sign extend the val2 variable.
    if (val2 & 0x00000080)
    {
        val2 = mem->get8(val) | 0xffffff00;   // Sign extending the val2 variable.
    }
    
    // If pos, we call that render_itype_load method, and we print an exec_lb string to standard output.
    if (pos)
    {
        std::string s = render_itype_load(insn, "  lb");
        s.resize(instruction_width , ' ');
        *pos << s << "  // x" << rd << " = sx(m8(" << hex0x32(regs.get(rs1))
        << " + " << hex0x32(imm_i) << ")) = " << hex0x32(val2);
        std::cout << std::endl;   // Printing out a line break.
    }
    
    regs.set(rd, val2);   // Setting the val2 variable to the rd variable.
    
    pc += 4;   // Incrementing the pc variable by four.
}

/**
 * exec_lh: This method will use an if statement to call the render_itype_load method,
 *    and prints an exec_lh string out to the standard output. Then this method sets
 *    the val2 variable to the rd variable, and increments the pc variable by four.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 * @param pos An ostream variable that is used to print a string to standard output.
 *
 * No return values.
 *
 * @note This is the exec_lh method, and this method is the 16th out of the 40
 *    exec methods that we create in this program.
 ********************************************************************************/
void rv32i::exec_lh(uint32_t insn, std::ostream* pos)
{
    // Creating the five variables that we need for this exec_lh method.
    int32_t rd = get_rd(insn);
    int32_t rs1 = get_rs1(insn);
    int32_t imm_i = get_imm_i(insn);
    uint32_t val = (regs.get(rs1) + imm_i);
    uint32_t val2 = mem->get16(val);
    
    // Using an if statement to sign extend the val2 variable.
    if (val2 & 0x00008000)
    {
        val2 = mem->get16(val) | 0xffff0000;   // Sign extending the val2 variable.
    }
    
    // If pos, we call that render_itype_load method, and we print an exec_lh string to standard output.
    if (pos)
    {
        std::string s = render_itype_load(insn, "  lh");
        s.resize(instruction_width , ' ');
        *pos << s << "  // x" << rd << " = sx(m16(" << hex0x32(regs.get(rs1))
        << " + " << hex0x32(imm_i) << ")) = " << hex0x32(val2);
        std::cout << std::endl;   // Printing out a line break.
    }
    
    regs.set(rd, val2);   // Setting the val2 variable to the rd variable.
    
    pc += 4;   // Incrementing the pc variable by four.
}

/**
 * exec_lw: This method will use an if statement to call the render_itype_load method,
 *    and prints an exec_lw string out to the standard output. Then this method sets
 *    the val2 variable to the rd variable, and increments the pc variable by four.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 * @param pos An ostream variable that is used to print a string to standard output.
 *
 * No return values.
 *
 * @note This is the exec_lw method, and this method is the 17th out of the 40
 *    exec methods that we create in this program.
 ********************************************************************************/
void rv32i::exec_lw(uint32_t insn, std::ostream* pos)
{
    // Creating the five variables that we need for this exec_lw method.
    int32_t rd = get_rd(insn);
    int32_t rs1 = get_rs1(insn);
    int32_t imm_i = get_imm_i(insn);
    uint32_t val = (regs.get(rs1) + imm_i);
    uint32_t val2 = mem->get32(val);
    
    // If pos, we call that render_itype_load method, and we print an exec_lw string to standard output.
    if (pos)
    {
        std::string s = render_itype_load(insn, "  lw");
        s.resize(instruction_width , ' ');
        *pos << s << "  // x" << rd << " = sx(m32(" << hex0x32(regs.get(rs1))
        << " + " << hex0x32(imm_i) << ")) = " << hex0x32(val2);
        std::cout << std::endl;   // Printing out a line break.
    }
    
    regs.set(rd, val2);   // Setting the val2 variable to the rd variable.
    
    pc += 4;   // Incrementing the pc variable by four.
}

/**
 * exec_sb: This method will use an if statement to call the render_stype method,
 *    and prints an exec_sb string out to the standard output. Then this method sets
 *    the val2 variable to the val variable, and increments the pc variable by four.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 * @param pos An ostream variable that is used to print a string to standard output.
 *
 * No return values.
 *
 * @note This is the exec_sb method, and this method is the 18th out of the 40
 *    exec methods that we create in this program.
 ********************************************************************************/
void rv32i::exec_sb(uint32_t insn, std::ostream* pos)
{
    // Creating the five variables that we need for this exec_sb method.
    int32_t rs1 = get_rs1(insn);
    int32_t rs2 = get_rs2(insn);
    int32_t imm_s = get_imm_s(insn);
    uint32_t val = (regs.get(rs1) + imm_s);
    uint32_t val2 = (regs.get(rs2) & 0x000000ff);
    
    // If pos, we call that render_stype method, and we print an exec_sb string to standard output.
    if (pos)
    {
        std::string s = render_stype(insn, "  sb");
        s.resize(instruction_width , ' ');
        *pos << s << "  // m8(" << hex0x32(regs.get(rs1))
        << " + " << hex0x32(imm_s) << ") = " << hex0x32(val2);
        std::cout << std::endl;   // Printing out a line break.
    }
    
    mem->set8(val, val2);   // Setting the val2 variable to the val variable.
    
    pc += 4;   // Incrementing the pc variable by four.
}

/**
 * exec_sh: This method will use an if statement to call the render_stype method,
 *    and prints an exec_sh string out to the standard output. Then this method sets
 *    the val2 variable to the val variable, and increments the pc variable by four.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 * @param pos An ostream variable that is used to print a string to standard output.
 *
 * No return values.
 *
 * @note This is the exec_sh method, and this method is the 19th out of the 40
 *    exec methods that we create in this program.
 ********************************************************************************/
void rv32i::exec_sh(uint32_t insn, std::ostream* pos)
{
    // Creating the five variables that we need for this exec_sh method.
    int32_t rs1 = get_rs1(insn);
    int32_t rs2 = get_rs2(insn);
    int32_t imm_s = get_imm_s(insn);
    uint32_t val = (regs.get(rs1) + imm_s);
    uint32_t val2 = (regs.get(rs2) & 0x0000ffff);
    
    // If pos, we call that render_stype method, and we print an exec_sh string to standard output.
    if (pos)
    {
        std::string s = render_stype(insn, "  sh");
        s.resize(instruction_width , ' ');
        *pos << s << "  // m16(" << hex0x32(regs.get(rs1))
        << " + " << hex0x32(imm_s) << ") = " << hex0x32(val2);
        std::cout << std::endl;   // Printing out a line break.
    }
    
    mem->set16(val, val2);   // Setting the val2 variable to the val variable.
    
    pc += 4;   // Incrementing the pc variable by four.
}

/**
 * exec_sw: This method will use an if statement to call the render_stype method,
 *    and prints an exec_sw string out to the standard output. Then this method sets
 *    the val2 variable to the val variable, and increments the pc variable by four.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 * @param pos An ostream variable that is used to print a string to standard output.
 *
 * No return values.
 *
 * @note This is the exec_sw method, and this method is the 20th out of the 40
 *    exec methods that we create in this program.
 ********************************************************************************/
void rv32i::exec_sw(uint32_t insn, std::ostream* pos)
{
    // Creating the five variables that we need for this exec_sw method.
    int32_t rs1 = get_rs1(insn);
    int32_t rs2 = get_rs2(insn);
    int32_t imm_s = get_imm_s(insn);
    uint32_t val = (regs.get(rs1) + imm_s);
    uint32_t val2 = (regs.get(rs2) & 0xffffffff);
    
    // If pos, we call that render_stype method, and we print an exec_sw string to standard output.
    if (pos)
    {
        std::string s = render_stype(insn, "  sw");
        s.resize(instruction_width , ' ');
        *pos << s << "  // m32(" << hex0x32(regs.get(rs1))
        << " + " << hex0x32(imm_s) << ") = " << hex0x32(val2);
        std::cout << std::endl;   // Printing out a line break.
    }
    
    mem->set32(val, val2);   // Setting the val2 variable to the val variable.
    
    pc += 4;   // Incrementing the pc variable by four.
}

/**
 * exec_slti: This method will use an if statement to call the render_itype_alu method,
 *    and prints an exec_slti string out to the standard output. Then this method sets
 *    the val variable to the rd variable, and increments the pc variable by four.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 * @param pos An ostream variable that is used to print a string to standard output.
 *
 * No return values.
 *
 * @note This is the exec_slti method, and this method is the 21th out of the 40
 *    exec methods that we create in this program.
 ********************************************************************************/
void rv32i::exec_slti(uint32_t insn, std::ostream* pos)
{
    // Creating the four variables that we need for this exec_slti method.
    int32_t rd = get_rd(insn);
    int32_t rs1 = get_rs1(insn);
    int32_t imm_i = get_imm_i(insn);
    int32_t val = (regs.get(rs1) < imm_i) ? 1 : 0;
    
    // If pos, we call that render_itype_alu method, and we print an exec_slti string to standard output.
    if (pos)
    {
        std::string s = render_itype_alu(insn, "  slti", imm_i);
        s.resize(instruction_width , ' ');
        *pos << s << "  // x" << rd << " = (" << hex0x32(regs.get(rs1))
        << " < " << imm_i << ") ? 1 : 0 = "<< hex0x32(val);
        std::cout << std::endl;   // Printing out a line break.
    }
    
    regs.set(rd, val);   // Setting the val variable to the rd variable.
    
    pc += 4;   // Incrementing the pc variable by four.
}

/**
 * exec_sltiu: This method will use an if statement to call the render_itype_alu method,
 *    and prints an exec_sltiu string out to the standard output. Then this method sets
 *    the val variable to the rd variable, and increments the pc variable by four.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 * @param pos An ostream variable that is used to print a string to standard output.
 *
 * No return values.
 *
 * @note This is the exec_sltiu method, and this method is the 22th out of the 40
 *    exec methods that we create in this program.
 ********************************************************************************/
void rv32i::exec_sltiu(uint32_t insn, std::ostream* pos)
{
    // Creating the four variables that we need for this exec_sltiu method.
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t imm_i = get_imm_i(insn);
    uint32_t val = ((uint32_t)regs.get(rs1) < (uint32_t)imm_i) ? 1 : 0;
    
    // If pos, we call that render_itype_alu method, and we print an exec_sltiu string to standard output.
    if (pos)
    {
        std::string s = render_itype_alu(insn, "  sltiu", imm_i);
        s.resize(instruction_width , ' ');
        *pos << s << "  // x" << rd << " = (" << hex0x32(regs.get(rs1))
        << " <U " << imm_i << ") ? 1 : 0 = "<< hex0x32(val);
        std::cout << std::endl;   // Printing out a line break.
    }
    
    regs.set(rd, val);   // Setting the val variable to the rd variable.
    
    pc += 4;   // Incrementing the pc variable by four.
}

/**
 * exec_xori: This method will use an if statement to call the render_itype_alu method,
 *    and prints an exec_xori string out to the standard output. Then this method sets
 *    the val variable to the rd variable, and increments the pc variable by four.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 * @param pos An ostream variable that is used to print a string to standard output.
 *
 * No return values.
 *
 * @note This is the exec_xori method, and this method is the 23th out of the 40
 *    exec methods that we create in this program.
 ********************************************************************************/
void rv32i::exec_xori(uint32_t insn, std::ostream* pos)
{
    // Creating the four variables that we need for this exec_xori method.
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    int32_t imm_i = get_imm_i(insn);
    uint32_t val = (regs.get(rs1) ^ imm_i);
    
    // If pos, we call that render_itype_alu method, and we print an exec_xori string to standard output.
    if (pos)
    {
        std::string s = render_itype_alu(insn, "  xori", imm_i);
        s.resize(instruction_width , ' ');
        *pos << s << "  // x" << rd << " = " << hex0x32(regs.get(rs1))
        << " ^ " << hex0x32(imm_i) << " = "<< hex0x32(val);
        std::cout << std::endl;   // Printing out a line break.
    }
    
    regs.set(rd, val);   // Setting the val variable to the rd variable.
    
    pc += 4;   // Incrementing the pc variable by four.
}

/**
 * exec_ori: This method will use an if statement to call the render_itype_alu method,
 *    and prints an exec_ori string out to the standard output. Then this method sets
 *    the val variable to the rd variable, and increments the pc variable by four.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 * @param pos An ostream variable that is used to print a string to standard output.
 *
 * No return values.
 *
 * @note This is the exec_ori method, and this method is the 24th out of the 40
 *    exec methods that we create in this program.
 ********************************************************************************/
void rv32i::exec_ori(uint32_t insn, std::ostream* pos)
{
    // Creating the four variables that we need for this exec_ori method.
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    int32_t imm_i = get_imm_i(insn);
    uint32_t val = (regs.get(rs1) | imm_i);
    
    // If pos, we call that render_itype_alu method, and we print an exec_ori string to standard output.
    if (pos)
    {
        std::string s = render_itype_alu(insn, "  ori", imm_i);
        s.resize(instruction_width , ' ');
        *pos << s << "  // x" << rd << " = " << hex0x32(regs.get(rs1))
        << " | " << hex0x32(imm_i) << " = "<< hex0x32(val);
        std::cout << std::endl;   // Printing out a line break.
    }
    
    regs.set(rd, val);   // Setting the val variable to the rd variable.
    
    pc += 4;   // Incrementing the pc variable by four.
}

/**
 * exec_andi: This method will use an if statement to call the render_itype_alu method,
 *    and prints an exec_andi string out to the standard output. Then this method sets
 *    the val variable to the rd variable, and increments the pc variable by four.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 * @param pos An ostream variable that is used to print a string to standard output.
 *
 * No return values.
 *
 * @note This is the exec_andi method, and this method is the 25th out of the 40
 *    exec methods that we create in this program.
 ********************************************************************************/
void rv32i::exec_andi(uint32_t insn, std::ostream* pos)
{
    // Creating the four variables that we need for this exec_andi method.
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    int32_t imm_i = get_imm_i(insn);
    uint32_t val = (regs.get(rs1) & imm_i);
    
    // If pos, we call that render_itype_alu method, and we print an exec_andi string to standard output.
    if (pos)
    {
        std::string s = render_itype_alu(insn, "  andi", imm_i);
        s.resize(instruction_width , ' ');
        *pos << s << "  // x" << rd << " = " << hex0x32(regs.get(rs1))
        << " & " << hex0x32(imm_i) << " = "<< hex0x32(val);
        std::cout << std::endl;   // Printing out a line break.
    }
    
    regs.set(rd, val);   // Setting the val variable to the rd variable.
    
    pc += 4;   // Incrementing the pc variable by four.
}

/**
 * exec_slli: This method will use an if statement to call the render_itype_alu method,
 *    and prints an exec_slli string out to the standard output. Then this method sets
 *    the val variable to the rd variable, and increments the pc variable by four.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 * @param pos An ostream variable that is used to print a string to standard output.
 *
 * No return values.
 *
 * @note This is the exec_slli method, and this method is the 26th out of the 40
 *    exec methods that we create in this program.
 ********************************************************************************/
void rv32i::exec_slli(uint32_t insn, std::ostream* pos)
{
    // Creating the five variables that we need for this exec_slli method.
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    int32_t imm_i = get_imm_i(insn);
    int32_t shamt_i = imm_i % XLEN;
    uint32_t val = (regs.get(rs1) << shamt_i);
    
    // If pos, we call that render_itype_alu method, and we print an exec_andi string to standard output.
    if (pos)
    {
        std::string s = render_itype_alu(insn, "  slli", shamt_i);
        s.resize(instruction_width , ' ');
        *pos << s << "  // x" << rd << " = " << hex0x32(regs.get(rs1))
        << " << " << shamt_i << " = "<< hex0x32(val);
        std::cout << std::endl;   // Printing out a line break.
    }
    
    regs.set(rd, val);   // Setting the val variable to the rd variable.
    
    pc += 4;   // Incrementing the pc variable by four.
}

/**
 * exec_srli: This method will use an if statement to call the render_itype_alu method,
 *    and prints an exec_srli string out to the standard output. Then this method sets
 *    the val variable to the rd variable, and increments the pc variable by four.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 * @param pos An ostream variable that is used to print a string to standard output.
 *
 * No return values.
 *
 * @note This is the exec_srli method, and this method is the 27th out of the 40
 *    exec methods that we create in this program.
 ********************************************************************************/
void rv32i::exec_srli(uint32_t insn, std::ostream* pos)
{
    // Creating the five variables that we need for this exec_srli method.
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    int32_t imm_i = get_imm_i(insn);
    int32_t shamt_i = imm_i % XLEN;
    uint32_t val = ((uint32_t)regs.get(rs1) >> shamt_i);
    
    // If pos, we call that render_itype_alu method, and we print an exec_srli string to standard output.
    if (pos)
    {
        std::string s = render_itype_alu(insn, "  srli", shamt_i);
        s.resize(instruction_width , ' ');
        *pos << s << "  // x" << rd << " = " << hex0x32(regs.get(rs1))
        << " >> " << shamt_i << " = "<< hex0x32(val);
        std::cout << std::endl;   // Printing out a line break.
    }
    
    regs.set(rd, val);   // Setting the val variable to the rd variable.
    
    pc += 4;   // Incrementing the pc variable by four.
}

/**
 * exec_srai: This method will use an if statement to call the render_itype_alu method,
 *    and prints an exec_srai string out to the standard output. Then this method sets
 *    the val variable to the rd variable, and increments the pc variable by four.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 * @param pos An ostream variable that is used to print a string to standard output.
 *
 * No return values.
 *
 * @note This is the exec_srai method, and this method is the 28th out of the 40
 *    exec methods that we create in this program.
 ********************************************************************************/
void rv32i::exec_srai(uint32_t insn, std::ostream* pos)
{
    // Creating the five variables that we need for this exec_srai method.
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    int32_t imm_i = get_imm_i(insn);
    int32_t shamt_i = imm_i % XLEN;
    uint32_t val = (regs.get(rs1) >> shamt_i);
    
    // If pos, we call that render_itype_alu method, and we print an exec_srai string to standard output.
    if (pos)
    {
        std::string s = render_itype_alu(insn, "  srai", shamt_i);
        s.resize(instruction_width , ' ');
        *pos << s << "  // x" << rd << " = " << hex0x32(regs.get(rs1))
        << " >> " << shamt_i << " = "<< hex0x32(val);
        std::cout << std::endl;   // Printing out a line break.
    }
    
    regs.set(rd, val);   // Setting the val variable to the rd variable.
    
    pc += 4;   // Incrementing the pc variable by four.
}

/**
 * exec_add: This method will use an if statement to call the render_rtype method,
 *    and prints an exec_add string out to the standard output. Then this method sets
 *    the val variable to the rd variable, and increments the pc variable by four.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 * @param pos An ostream variable that is used to print a string to standard output.
 *
 * No return values.
 *
 * @note This is the exec_add method, and this method is the 28th out of the 40
 *    exec methods that we create in this program.
 ********************************************************************************/
void rv32i::exec_add(uint32_t insn, std::ostream* pos)
{
    // Creating the five variables that we need for this exec_add method.
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    uint32_t val = (regs.get(rs1) + regs.get(rs2));
    
    // If pos, we call that render_rtype method, and we print an exec_add string to standard output.
    if (pos)
    {
        std::string s = render_rtype(insn, "  add");
        s.resize(instruction_width , ' ');
        *pos << s << "  // x" << rd << " = " << hex0x32(regs.get(rs1))
        << " + " << hex0x32(regs.get(rs2)) << " = "<< hex0x32(val);
        std::cout << std::endl;   // Printing out a line break.
    }
    
    regs.set(rd, val);   // Setting the val variable to the rd variable.
    
    pc += 4;   // Incrementing the pc variable by four.
}

/**
 * exec_sub: This method will use an if statement to call the render_rtype method,
 *    and prints an exec_sub string out to the standard output. Then this method sets
 *    the val variable to the rd variable, and increments the pc variable by four.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 * @param pos An ostream variable that is used to print a string to standard output.
 *
 * No return values.
 *
 * @note This is the exec_sub method, and this method is the 29th out of the 40
 *    exec methods that we create in this program.
 ********************************************************************************/
void rv32i::exec_sub(uint32_t insn, std::ostream* pos)
{
    // Creating the five variables that we need for this exec_sub method.
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    uint32_t val = (regs.get(rs1) - regs.get(rs2));
    
    // If pos, we call that render_rtype method, and we print an exec_sub string to standard output.
    if (pos)
    {
        std::string s = render_rtype(insn, "  sub");
        s.resize(instruction_width , ' ');
        *pos << s << "  // x" << rd << " = " << hex0x32(regs.get(rs1))
        << " - " << hex0x32(regs.get(rs2)) << " = "<< hex0x32(val);
        std::cout << std::endl;   // Printing out a line break.
    }
    
    regs.set(rd, val);   // Setting the val variable to the rd variable.
    
    pc += 4;   // Incrementing the pc variable by four.
}

/**
 * exec_sll: This method will use an if statement to call the render_rtype method,
 *    and prints an exec_sll string out to the standard output. Then this method sets
 *    the val variable to the rd variable, and increments the pc variable by four.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 * @param pos An ostream variable that is used to print a string to standard output.
 *
 * No return values.
 *
 * @note This is the exec_sll method, and this method is the 30th out of the 40
 *    exec methods that we create in this program.
 ********************************************************************************/
void rv32i::exec_sll(uint32_t insn, std::ostream* pos)
{
    // Creating the five variables that we need for this exec_sll method.
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    uint32_t rs2LS = regs.get(rs2) & 0x0000001f;
    uint32_t val = (regs.get(rs1) << rs2LS);
    
    // If pos, we call that render_rtype method, and we print an exec_sll string to standard output.
    if (pos)
    {
        std::string s = render_rtype(insn, "  sll");
        s.resize(instruction_width , ' ');
        *pos << s << "  // x" << rd << " = " << hex0x32(regs.get(rs1))
        << " << " << rs2LS << " = "<< hex0x32(val);
        std::cout << std::endl;   // Printing out a line break.
    }
    
    regs.set(rd, val);   // Setting the val variable to the rd variable.
    
    pc += 4;   // Incrementing the pc variable by four.
}

/**
 * exec_slt: This method will use an if statement to call the render_rtype method,
 *    and prints an exec_slt string out to the standard output. Then this method sets
 *    the val variable to the rd variable, and increments the pc variable by four.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 * @param pos An ostream variable that is used to print a string to standard output.
 *
 * No return values.
 *
 * @note This is the exec_slt method, and this method is the 31th out of the 40
 *    exec methods that we create in this program.
 ********************************************************************************/
void rv32i::exec_slt(uint32_t insn, std::ostream* pos)
{
    // Creating the four variables that we need for this exec_slt method.
    int32_t rd = get_rd(insn);
    int32_t rs1 = get_rs1(insn);
    int32_t rs2 = get_rs2(insn);
    int32_t val = (regs.get(rs1) < regs.get(rs2)) ? 1 : 0;
    
    // If pos, we call that render_rtype method, and we print an exec_slt string to standard output.
    if (pos)
    {
        std::string s = render_rtype(insn, "  slt");
        s.resize(instruction_width , ' ');
        *pos << s << "  // x" << rd << " = " << "(" << hex0x32(regs.get(rs1))
        << " < " << hex0x32(regs.get(rs2)) << ") ? 1 : 0 = "<< hex0x32(val);
        std::cout << std::endl;   // Printing out a line break.
    }
    
    regs.set(rd, val);   // Setting the val variable to the rd variable.
    
    pc += 4;  // Incrementing the pc variable by four.
}

/**
 * exec_sltu: This method will use an if statement to call the render_rtype method,
 *    and prints an exec_sltu string out to the standard output. Then this method sets
 *    the val variable to the rd variable, and increments the pc variable by four.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 * @param pos An ostream variable that is used to print a string to standard output.
 *
 * No return values.
 *
 * @note This is the exec_sltu method, and this method is the 32th out of the 40
 *    exec methods that we create in this program.
 ********************************************************************************/
void rv32i::exec_sltu(uint32_t insn, std::ostream* pos)
{
    // Creating the four variables that we need for this exec_sltu method.
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    uint32_t val = ((uint32_t)regs.get(rs1) < (uint32_t)regs.get(rs2)) ? 1 : 0;
    
    // If pos, we call that render_rtype method, and we print an exec_sltu string to standard output.
    if (pos)
    {
        std::string s = render_rtype(insn, "  sltu");
        s.resize(instruction_width , ' ');
        *pos << s << "  // x" << rd << " = " << "(" << hex0x32(regs.get(rs1))
        << " <U " << hex0x32(regs.get(rs2)) << ") ? 1 : 0 = "<< hex0x32(val);
        std::cout << std::endl;   // Printing out a line break.
    }
    
    regs.set(rd, val);   // Setting the val variable to the rd variable.
    
    pc += 4;   // Incrementing the pc variable by four.
}

/**
 * exec_xor: This method will use an if statement to call the render_rtype method,
 *    and prints an exec_xor string out to the standard output. Then this method sets
 *    the val variable to the rd variable, and increments the pc variable by four.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 * @param pos An ostream variable that is used to print a string to standard output.
 *
 * No return values.
 *
 * @note This is the exec_xor method, and this method is the 33th out of the 40
 *    exec methods that we create in this program.
 ********************************************************************************/
void rv32i::exec_xor(uint32_t insn, std::ostream* pos)
{
    // Creating the four variables that we need for this exec_xor method.
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    uint32_t val = (regs.get(rs1) ^ regs.get(rs2));
    
    // If pos, we call that render_rtype method, and we print an exec_xor string to standard output.
    if (pos)
    {
        std::string s = render_rtype(insn, "  xor");
        s.resize(instruction_width , ' ');
        *pos << s << "  // x" << rd << " = " << hex0x32(regs.get(rs1))
        << " ^ " << hex0x32(regs.get(rs2)) << " = " << hex0x32(val);
        std::cout << std::endl;   // Printing out a line break.
    }
    
    regs.set(rd, val);   // Setting the val variable to the rd variable.
    
    pc += 4;   // Incrementing the pc variable by four.
}

/**
 * exec_srl: This method will use an if statement to call the render_rtype method,
 *    and prints an exec_srl string out to the standard output. Then this method sets
 *    the val variable to the rd variable, and increments the pc variable by four.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 * @param pos An ostream variable that is used to print a string to standard output.
 *
 * No return values.
 *
 * @note This is the exec_srl method, and this method is the 34th out of the 40
 *    exec methods that we create in this program.
 ********************************************************************************/
void rv32i::exec_srl(uint32_t insn, std::ostream* pos)
{
    // Creating the five variables that we need for this exec_srl method.
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    uint32_t rs2RS = regs.get(rs2) & 0x0000001f;
    uint32_t val = ((uint32_t)regs.get(rs1) >> rs2RS);
    
    // If pos, we call that render_rtype method, and we print an exec_srl string to standard output.
    if (pos)
    {
        std::string s = render_rtype(insn, "  srl");
        s.resize(instruction_width , ' ');
        *pos << s << "  // x" << rd << " = " << hex0x32(regs.get(rs1))
        << " >> " << rs2RS << " = "<< hex0x32(val);
        std::cout << std::endl;   // Printing out a line break.
    }
    
    regs.set(rd, val);   // Setting the val variable to the rd variable.
    
    pc += 4;   // Incrementing the pc variable by four.
}

/**
 * exec_sra: This method will use an if statement to call the render_rtype method,
 *    and prints an exec_sra string out to the standard output. Then this method sets
 *    the val variable to the rd variable, and increments the pc variable by four.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 * @param pos An ostream variable that is used to print a string to standard output.
 *
 * No return values.
 *
 * @note This is the exec_sra method, and this method is the 35th out of the 40
 *    exec methods that we create in this program.
 ********************************************************************************/
void rv32i::exec_sra(uint32_t insn, std::ostream* pos)
{
    // Creating the five variables that we need for this exec_sra method.
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    uint32_t rs2RS = regs.get(rs2) & 0x0000001f;
    uint32_t val = (regs.get(rs1) >> rs2RS);
    
    // If pos, we call that render_rtype method, and we print an exec_sra string to standard output.
    if (pos)
    {
        std::string s = render_rtype(insn, "  sra");
        s.resize(instruction_width , ' ');
        *pos << s << "  // x" << rd << " = " << hex0x32(regs.get(rs1))
        << " >> " << rs2RS << " = "<< hex0x32(val);
        std::cout << std::endl;   // Printing out a line break.
    }
    
    regs.set(rd, val);   // Setting the val variable to the rd variable.
    
    pc += 4;   // Incrementing the pc variable by four.
}

/**
 * exec_or: This method will use an if statement to call the render_rtype method,
 *    and prints an exec_or string out to the standard output. Then this method sets
 *    the val variable to the rd variable, and increments the pc variable by four.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 * @param pos An ostream variable that is used to print a string to standard output.
 *
 * No return values.
 *
 * @note This is the exec_or method, and this method is the 36th out of the 40
 *    exec methods that we create in this program.
 ********************************************************************************/
void rv32i::exec_or(uint32_t insn, std::ostream* pos)
{
    // Creating the four variables that we need for this exec_or method.
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    uint32_t val = (regs.get(rs1) | regs.get(rs2));
    
    // If pos, we call that render_rtype method, and we print an exec_or string to standard output.
    if (pos)
    {
        std::string s = render_rtype(insn, "  or");
        s.resize(instruction_width , ' ');
        *pos << s << "  // x" << rd << " = " << hex0x32(regs.get(rs1))
        << " | " << hex0x32(regs.get(rs2)) << " = " << hex0x32(val);
        std::cout << std::endl;   // Printing out a line break.
    }
    
    regs.set(rd, val);   // Setting the val variable to the rd variable.
    
    pc += 4;   // Incrementing the pc variable by four.
}

/**
 * exec_and: This method will use an if statement to call the render_rtype method,
 *    and prints an exec_and string out to the standard output. Then this method sets
 *    the val variable to the rd variable, and increments the pc variable by four.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 * @param pos An ostream variable that is used to print a string to standard output.
 *
 * No return values.
 *
 * @note This is the exec_and method, and this method is the 37th out of the 40
 *    exec methods that we create in this program.
 ********************************************************************************/
void rv32i::exec_and(uint32_t insn, std::ostream* pos)
{
    // Creating the four variables that we need for this exec_and method.
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    uint32_t val = (regs.get(rs1) & regs.get(rs2));
    
    // If pos, we call that render_rtype method, and we print an exec_and string to standard output.
    if (pos)
    {
        std::string s = render_rtype(insn, "  and");
        s.resize(instruction_width , ' ');
        *pos << s << "  // x" << rd << " = " << hex0x32(regs.get(rs1))
        << " & " << hex0x32(regs.get(rs2)) << " = " << hex0x32(val);
        std::cout << std::endl;   // Printing out a line break.
    }
    
    regs.set(rd, val);   // Setting the val variable to the rd variable.
    
    pc += 4;   // Incrementing the pc variable by four.
}

/**
 * exec_fence: This method will use an if statement to call the render_fence method,
 *    and prints an exec_fence string out to the standard output. Then this method
 *    increments the pc variable by four.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 * @param pos An ostream variable that is used to print a string to standard output.
 *
 * No return values.
 *
 * @note This is the exec_fence method, and this method is the 38th out of the 40
 *    exec methods that we create in this program.
 ********************************************************************************/
void rv32i::exec_fence(uint32_t insn, std::ostream* pos)
{
    // If pos, we call that render_fence method, and we print an exec_fence string to standard output.
    if (pos)
    {
        std::string s = render_fence(insn);
        s.resize(instruction_width , ' ');
        *pos << s << "  // fence";
        std::cout << std::endl;   // Printing out a line break.
    }
    
    pc += 4;   // Incrementing the pc variable by four.
}

/**
 * exec_ebreak: This method will use an if statement to call the render_ebreak method,
 *    and prints an exec_ebreak string out to the standard output. Then this method
 *    sets halt equal to true.
 *
 * @param insn An unsigned integer type variable with a width of exactly 32 bits.
 * @param pos An ostream variable that is used to print a string to standard output.
 *
 * No return values.
 *
 * @note This is the exec_ebreak method, and this method is the 39th out of the 40
 *    exec methods that we create in this program.
 ********************************************************************************/
void rv32i::exec_ebreak(uint32_t insn, std::ostream* pos)
{
    // If pos, we call that render_ebreak method, and we print an exec_ebreak string to standard output.
    if (pos)
    {
        std::string s = render_ebreak();
        s.resize(instruction_width , ' ');
        *pos << s << "  // HALT";
        std::cout << std::endl;   // Printing out a line break.
    }
    
    halt = true;   // Setting halt equal to true.
}

/**
 * tick: This method starts by checking to see if the halt flag is set, if the halt
 *    flag is set, then this method returns immediately without doing anything. Then
 *    this method increments the insn_counter variable, dumps the state of the hart
 *    if show_registers is true, fetches an instruction from the memory at the address
 *    in the pc register, and finally checks to see if show_instructions is true (this
 *    function also calls the dcex function twice).
 *
 * No parameter values.
 *
 * No return values.
 *
 * @note This is the tick method, and this method will be called in the run method
 *    if the insn_counter is less than the limit, and if halt is not equal to true.
 ********************************************************************************/
void rv32i::tick()
{
    if (halt == true)   // Checking to see if halt equals true.
    {
        return;   // Using the return statement to do nothing.
    }
    else
    {
        insn_counter++;   // Incrementing the insn_counter variable by one.
        
        if (show_registers == true) // Checking to see if show_registers equals true.
        {
            dump();   // Calling the dump method.
        }
        
        uint32_t insn = mem->get32(pc);   // Calling the memory get32 method to get all the rv32i instructions.
        
        if (show_instructions == true)   // Checking to see if show_instructions equals true.
        {
            std::cout << hex32(pc) << ": " << hex32(insn);   // Printing out the address and the instruction at that address.
            
            dcex(insn, &std::cout);   // Calling the dcex method for the first time.
        }
        else
        {
            dcex(insn, nullptr);   // Calling the dcex method for the second time.
        }
    }
}

/**
 * run: This method first sets register 2 to the memory size by calling the
 *     memory get_size method. Then this method uses a while loop to call
 *     the tick method, and to also print out a string. Finally this method
 *     prints out the number of instructions executed, and returns nothing.
 *
 * @param limit An unsigned integer type variable with a width of exactly 64 bits.
 *
 * No return values.
 *
 * @note This is the run method, and this method will only be called in the main
 *    function to bring all of the other methods together.
 ********************************************************************************/
void rv32i::run(uint64_t limit)
{
    uint32_t memGetSize = mem->get_size();   // Calling the memory get_size method to get the memory size.
    
    regs.set(2, memGetSize);   // Calling the registerfile set method to set register 2 to the memory size.
    
    // Checking to see if the insn_counter is less than the limit, and if halt is not equal to true.
    while (insn_counter < limit && halt != true)
    {
        tick();   // Calling the tick method.
        
        // If halt is equal to true, we are printing out a string that states that the execution of the program terminated.
        if (halt == true)
        {
            std::cout << "Execution terminated by EBREAK instruction" << std::endl;
        }
    }
    
    std::cout << insn_counter << " instructions executed" << std::endl;   // Printing out the number of instructions executed.
    
    return;   // Using the return statement to do nothing.
}
