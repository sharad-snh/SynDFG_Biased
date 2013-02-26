/* 
 * File:   opcode.h
 * Author: Sharad Sinha
 * Email: sharad_sinha@pmail.ntu.edu.sg
 * Rev: 1.0
 * Release Date: July 31,2012 (Rev 1.0)
 * © Center for High Performance Embedded Systems(CHiPES),
 * N4-02A-32,School of Computer Engineering, Nanyang Avenue
 * Nanyang Technological University
 * Singapore, 639798
 * This software is released without any warranty and CHiPES and
 * the author disclaims all liabilities arising out of its usage
 */

#ifndef OPCODE_H
#define  OPCODE_H
const char* operators[]={"XOR","ADD","SHR","SHL","AND","OR","MUL","SUB"};   // To be updated based on allowed operators in a DFG to be generated
int NumOpcode = sizeof(operators)/ sizeof(const char*);              
int VAR_LIFETIME_EXTEND = 0;    //To be set to 1 when extension of variable usage beyond one execution step is allowed else set to 0.
int VAR_MAX_DISTANCE = 3;       //The max distance between execution steps for which extension of variable usage is allowed
char loadedOperator[] = "MUL";  // The operator which will bias the DFG i.e. the operator which wil be present a large number of times in a generated DFG
#endif	/* OPCODE_H */


