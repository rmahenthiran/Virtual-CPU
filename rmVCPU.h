//Rakul Mahenthiran Virtual CPU header file
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#include <ctype.h>
#include <stdint.h>

//General Defines
#define CPU_MEM 	      0x4000
#define REG_BIT_SIZE 	  0x20
#define MAX32 		      0xFFFFFFFF
#define TOTAL_REG_NUM 	0x10
#define IR1_MASK	      0xFFFF
#define SHIFT_BYTE      8
#define SHIFT_2_BYTES   16
#define SHIFT_3_BYTES   24
#define THUMB           0x2
#define MASK_BYTE	      0xFF

//Special Rgisters Index
#define SP_REG		0xD
#define LR_REG 		0xE
#define PC_REG 		0xF

//Imediate Instructions Values
#define MOV 0x0
#define CMP 0x1
#define ADD 0x2
#define SUB 0x3

//Data Process Instructions Values
#define DP_ADD 0x4
#define DP_SUB 0x2
#define DP_ADC 0x5

//CPU Registers
static uint32_t registers[TOTAL_REG_NUM];
static uint32_t mar;
static uint32_t mbr;
static uint32_t ir;
static uint32_t alu;
static uint16_t cur_inst;

//Flags
static uint8_t sign_flag;
static uint8_t zero_flag;
static uint8_t carry_flag;
static uint8_t stop_flag;
static uint8_t ir_flag;

#define IR0 (unsigned)ir >> 16
#define IR1 ir & IR1_MASK

//Execute Function Types
#define PROCESS_DATA		      0x0
#define LOAD_STORE		        0x1
#define IMEDIATE1		          0x2
#define IMEDIATE2		          0x3
#define CONDITIONAL_BRANCH	  0x4
#define UNCONDITONAL_BRANCH		0x6
#define STOP			            0xE000

//Branch Conditions
#define EQ 0x0
#define NE 0x1
#define CS 0x2
#define CC 0x3
#define MI 0x4
#define PL 0x5
#define HI 0x8
#define LS 0x9
#define AL 0xE

//CPU Functions
int loadFile(void *buffer, unsigned int max);
void writeFile(void *memory);
void memDump(void *memptr,unsigned offset,  unsigned length);
void memModify(void *memptr,unsigned location);
void displayRegisters();
void zeroRegisters();
void instructionCycle(void *memptr);
void fetch(void *memptr);
void execute(void *memptr);
uint32_t loadRegisters(uint32_t marVal, void *memptr);
void storeRegisters(uint32_t marVal, uint32_t mbrVal, void *memptr);
void flagHandler(uint32_t alu);
int isCarry(uint32_t op1,uint32_t op2, uint8_t c);
int checkBranch();
