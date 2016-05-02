#include "rmVCPU.h"

int main(){
	unsigned char memory[CPU_MEM];
	unsigned int bytesRead;
	unsigned length, offset, memModAddress;
	char option;
	int loopRun = 1;

        printf("\n\n--Name: Rakul Mahenthiran--\n--Date: Mar 23, 2016--\n--Student#: 822240982--\n\n");

	while(loopRun == 1){
        	printf("\nSELECT AN OPTION FROM MENU: ");
        	printf("\nd\tMemory dump");
        	printf("\ng\tGo -run the entire program");
        	printf("\nl\tLoad the file into memory");
        	printf("\nm\tMemory modify");
        	printf("\nq\tQuit");
        	printf("\nr\tDispaly registers");
        	printf("\nt\tTrace -execute one instruction");
        	printf("\nw\tWrite file");
        	printf("\nz\tReset all registers to zero");
        	printf("\n?, h\tDisplay list of commands");
		printf("\n\nEnter selection(options are non-case senesitive): ");

		scanf("%s", &option);

		switch(option){
			case 'd':
			case 'D':
				printf("Enter Length(in HEX): 0x");
				scanf("%X", &length);
				printf("Enter Offset(in HEX): 0x");
				scanf("%X", &offset);
				if((offset + length) >= CPU_MEM)
					printf("\nThe length and offset specified is out of memory boudary.\n");
				else
					memDump(memory, offset, length);
				break;

			case 'g':
			case 'G':
				printf("\n\nGo function yet to be implemented.");
                                break;

			case 'l':
			case 'L':
				bytesRead = loadFile(memory, CPU_MEM);
				if(bytesRead != -1) {	//checks file was opened
					printf("Read %u bytes (0x%04x in hex)\n", bytesRead, bytesRead);
                                }
				break;

			case 'm':
			case 'M':
				printf("\n\nEnter starting address to modify(in HEX): 0x");
				scanf("%X", &memModAddress);
				if(memModAddress >= CPU_MEM)
        	printf("\nThe offset specified is out of memory boudary.\n");
				else
					memModify(memory, memModAddress);
        break;

			case 'q':
			case 'Q':
				printf("\nGoodbye!\n");
				loopRun = 0;
        break;

			case 'r':
			case 'R':
				displayRegisters();
        break;

			case 't':
			case 'T':
				instructionCycle(memory);
				displayRegisters();
        break;

			case 'w':
			case 'W':
				writeFile(memory);
        break;

			case 'z':
			case 'Z':
				zeroRegisters();
        break;

			case '?':
			case 'h':
			case 'H':
				printf("\nHelp Menu ->");
				printf("\nEnter 'd' or 'D' for memory dump");
		    printf("\nEnter 'g' or 'G' to go -run the entire program");
		    printf("\nEnter 'l' or 'L' to load the file into memory");
        printf("\nEnter 'm' or 'M' for memory modify");
		    printf("\nEnter 'q' or 'Q' to quit");
		    printf("\nEnter 'r' or 'R' to dispaly registers");
		    printf("\nEnter 't' or 'T' to trace -execute one instruction");
        printf("\nEnter 'w' or 'W' to write file");
        printf("\nEnter 'z' or 'Z' to reset all registers to zero");
        printf("\nEnter '?', 'h' or 'H' to display list of commands");
        printf("\n\nEnter selection: ");
        break;

			default:
				printf("\nError, invalid option entered!\n");
		}

		while(option != '\n') {
    	scanf("%c", &option);
    }
	}
  return(0);
}


//Load file function
int loadFile(void *buffer, unsigned int max){
	FILE *fp;
	char filename[24];
	unsigned int readCount, fileSize;

	printf("Enter Filename to load: ");
	scanf("%s", filename);

	fp = fopen(filename, "r");
   	if (fp != NULL) {
		//checking file original size read to see if truncated
		fseek(fp, 0L, SEEK_END);
		fileSize = ftell(fp);
		fseek(fp, 0L, SEEK_SET);

		if(fileSize > max){
			printf("File has been truncated.\n");
		}
		//bytes read
        	readCount = fread(buffer, 1, max, fp);
        	fclose(fp);
    	}

	else{
		printf("\n\tERROR, file does not exist.\n");
		return(-1);
	}

	return (readCount);
}

//Wite file
void writeFile(void *memory) {
	char filename[24];
 	unsigned int fileSize;
	int i;

 	printf("Enter Filename to write: ");
	scanf("%s", filename);

 	printf("Enter File Size: ");
 	scanf("%d", &fileSize);

	FILE *fp;
 	fp = fopen(filename, "w+");
  	fwrite(memory, 1, fileSize, fp);
 	fclose(fp);
}

//Hex Dump
void memDump(void *memptr,unsigned offset,  unsigned length)
{
	char* p = memptr;
	int rows;
	int x,y,z;
	unsigned int decOffset = ("%d", offset);
	unsigned int decLength = ("%d", length);
	int remainder;

	//determine how many rows need to printed in hex dump
	remainder = decLength % 16;
	if(remainder == 0)
		rows = decLength / 16;
	else
		rows = (decLength / 16) + 1;

	//looping through each row
	for(x = 0; x < rows; x++){
		//print offset at start of row
		printf("%04X\t", decOffset + (x * 16));

		//print all 16 ascii columns for row
		for(y = 0; y < 16; y++){
			if((y + (x * 16)) < decLength){
				if(isprint(((char*)p)[decOffset + y + (x * 16)])){ //printable char
					printf("%c\t",p[decOffset + y + (x * 16)]);
				}
				else{ // other char
        	printf(".\t");
        }
			}
		}

		printf("\n\t");

		//print all 16 hex columns for row
		for(z = 0; z < 16; z++){
			if((z + (x * 16)) < decLength){
				if((z + (x * 16)) < decLength){
        	printf("%02X\t",0xFF & ((char*)p)[decOffset + z + (x * 16)]);
	      }
			}
		}
		printf("\n\n");
	}
}


//Memory Modify
void memModify(void *memptr,unsigned location)
{
	char* p2 = memptr;
	unsigned char value[256];
	unsigned char exit = '.';
	unsigned hexVal = 0x00;

	while(1){
		getchar(); //get rid of \n from scanf
		//print offset and value at offset
		printf("Address location 0x%04x contains hex value: 0x%02X", location, p2[location]);
		printf("\nEnter value to replace with (in hex): 0x");

		fgets(value,sizeof(value)-1,stdin);

		//end loop if '.' is detected
		if(value[0] == exit)
			break;

		sscanf(value," %X",&hexVal);

		//change values at mem location
		(((char*)memptr)[location]) = hexVal;

		//end loop if address full
		if(++location == 16384)
			break;
	}
}


//Dispaly Registers
void displayRegisters()
{
	int i;
	printf("\n");

	for(i =0; i < TOTAL_REG_NUM; i++){
		if(i < 13){
			printf("R[%02d]: %08X\t", i, registers[i]);
		}

		else if(i == 13){
			printf("\n\nSP: %08X\t", registers[i]);
		}

		else if(i == 14){
			printf("LR: %08X\t", registers[i]);
		}

		else{
			printf("PC: %08X\t", registers[i]);
		}

		if(((i+1)%4) == 0){
                        printf("\n");
                }
	}

	printf("\nSign Flag: %d\tZero Flag: %d\tCarry Flag: %d\tStop Flag: %d\tIR Flag: %d", sign_flag, zero_flag, carry_flag, stop_flag, ir_flag);

	printf("\n\nMAR: %08X\tMBR: %08X\tIR0: %04X\t IR1: %04X\t\n", mar, mbr, IR0, IR1);
}

//Zero all Registers
void zeroRegisters(){
	int i;

	for(i = 0; i < TOTAL_REG_NUM; i++){
		registers[i] = 0;
	}

	sign_flag = 0;
	zero_flag = 0;
	carry_flag = 0;
	stop_flag = 0;
	ir_flag = 0;

	mar = 0;
	mbr = 0;
	ir = 0;
	cur_inst = 0;
}

void instructionCycle(void *memptr){
	if(ir_flag == 0){
		fetch(memptr);
		cur_inst = IR0;
		execute(memptr);
		ir_flag = 1;
	}
	if(ir_flag == 1){
		cur_inst = IR1;
		execute(memptr);
		ir_flag = 0;
	}
}

void fetch(void *memptr){
	int i;
	mar = registers[PC_REG];

	for(i = 0; i < 4; i++, mar++){
		mbr = mbr << 8;
		mbr += *((unsigned char*)memptr + mar);
	}

	ir = mbr;

	registers[PC_REG] += 4;
}


void execute(void *memptr){

	//Load Store Instruction
	if(((unsigned)cur_inst >> 13) == LOAD_STORE){
		//Load
		if((cur_inst >> 11) & 0x1){
			//Load a Byte
			if((cur_inst >> 10) & 0x1){
				registers[cur_inst & 0xF] = loadRegisters(registers[(cur_inst >> 4) & 0xF], memptr);
				registers[cur_inst & 0xF] = registers[cur_inst & 0xF] & MASK_BYTE;
			}
			//Load a Word
			else{
				registers[cur_inst & 0xF] = loadRegisters(registers[(cur_inst >> 4) & 0xF], memptr);
			}
		}

		//Store
		else{
			mbr = registers[cur_inst & 0xFF];

			//Store a Byte to register
			if((cur_inst >> 10) & 0x1){
				mar = registers[(cur_inst >> 4) & 0xF];
				mbr = registers[cur_inst & 0xF];
				*((unsigned char*)memptr + mar) = (unsigned char)mbr & MASK_BYTE;
			}
			//Store a Word to Register
			else{
				storeRegisters(registers[(cur_inst >> 4) & 0xF], registers[cur_inst & 0xF], memptr);
			}
		}
	}

	//Imediate Instruction
	else if((((unsigned)cur_inst >> 13) == IMEDIATE1) | (((unsigned)cur_inst >> 13) == IMEDIATE2)) {
			//Move Imediate Value
			if(((cur_inst >> 12) & 0x3) == MOV){
				registers[cur_inst & 0xF] = ((cur_inst >> 4) & 0xFF);
				flagHandler(registers[cur_inst & 0xF]);
			}

			//Compare Imediate Value
			else if(((cur_inst >> 12) & 0x3) == CMP){
				alu = registers[cur_inst & 0xF] + ~((cur_inst >> 4) & 0xFF) + 1;
        flagHandler(alu);
        carry_flag = isCarry(registers[(cur_inst >> 12) & 0x3], ~((cur_inst >> 4) & 0xFF), 0);
			}

			//Add Imediate Value
			else if(((cur_inst >> 12) & 0x3) == ADD){
				alu = registers[cur_inst & 0xF] + ((cur_inst >> 4) & 0xFF);
        flagHandler(alu);
        carry_flag = isCarry(registers[cur_inst & 0xF], ((cur_inst >> 4) & 0xFF), 0);
        registers[cur_inst & 0xF] = alu;
			}

			//Subtract Imediate Value
			else if(((cur_inst >> 12) & 0x3) == SUB){
				alu = registers[cur_inst & 0xF] + ~((cur_inst >> 4) & 0xFF) + 1;
        flagHandler(alu);
        carry_flag = isCarry(registers[cur_inst & 0xF], ~((cur_inst >> 4) & 0xFF), 1);
        registers[cur_inst & 0xF] = alu;
			}
		}

		//Data Process Instruction
		if(((unsigned)cur_inst >> 13) == PROCESS_DATA){

			if(((cur_inst >> 8) & 0xF) == DP_ADD){
        alu = registers[cur_inst & 0xF] + registers[(cur_inst >> 4) & 0xF];
        flagHandler(alu);
        carry_flag = isCarry(registers[cur_inst & 0xF], ~registers[(cur_inst >> 4) & 0xF], 0);
        registers[cur_inst & 0xF] = alu;
			}

			else if(((cur_inst >> 8) & 0xF) == DP_SUB){
      	alu = registers[cur_inst & 0xF] + ~registers[(cur_inst >> 4) & 0xF] + 1;
        flagHandler(alu);
        carry_flag = isCarry(registers[cur_inst & 0xF], ~registers[(cur_inst >> 4) & 0xF], 1);
        registers[cur_inst & 0xF] = alu;
			}

			else if(((cur_inst >> 8) & 0xF) == DP_ADC){
				alu = registers[cur_inst & 0xF] + registers[(cur_inst >> 4) & 0xF] + carry_flag;
				flagHandler(alu);
				carry_flag = isCarry(registers[cur_inst & 0xF], registers[(cur_inst >> 4) & 0xF], carry_flag);
				registers[cur_inst & 0xF] = alu;
			}
		}

		//Unonditional Branch Instruction
    else if(((unsigned)cur_inst >> 13) == UNCONDITONAL_BRANCH){
        if((cur_inst >> 12) & 0x1){
            registers[LR_REG] = registers[PC_REG];
        }
        registers[PC_REG] = cur_inst & 0xFFF;
        // Make sure the IR flag is not still HI after the PC has changed.
        ir_flag = 0;
    }

		//Conditional Branch Instruction
		else if(((unsigned)cur_inst >> 13) == CONDITIONAL_BRANCH){
    		// Check condition codes and flags
      	if(checkBranch()){
					// Add relative address making sure its a signed 8-bit value
        	alu = registers[PC_REG] + (int8_t)(cur_inst & 0xFF);
	    		if(ir_flag != 0){
          	ir_flag = 0;
          	alu = alu + ~THUMB + 1;
        	}
        	registers[PC_REG] = alu;
      	}
		}

		//Stop
		else if(cur_inst == 0xE000){
			stop_flag = 1;
		}
}

//Load Registers
uint32_t loadRegisters(uint32_t marVal, void *memptr){
	int i;

	mar = marVal;

	for(i = 0; i < 4; i++, mar++){
		mbr = mbr << 8;
                mbr += *((unsigned char*)memptr + mar);
	}

	return mbr;
}

//Store Registers
void storeRegisters(uint32_t marVal, uint32_t mbrVal, void *memptr){
	mar = marVal;
	mbr = mbrVal;

	*((unsigned char*)memptr + mar++) = (unsigned char)(mbr >> 24 & 0xFF);
  *((unsigned char*)memptr + mar++) = (unsigned char)(mbr >> 16 & 0xFF);
  *((unsigned char*)memptr + mar++) = (unsigned char)(mbr >> 8 & 0xFF);
  *((unsigned char*)memptr + mar) = (unsigned char)mbr & 0xFF;
}

//Flag Handler
void flagHandler(uint32_t alu){
    if(alu == 0){
        zero_flag = 1;
    }else{
        zero_flag = 0;
    }

    sign_flag = (alu & 0x80000000) >> 31;
}

//Check Carry
int isCarry(uint32_t op1,uint32_t op2, uint8_t c){
    if ((op2== MAX32)&&(c==1))
        return(1);
    return((op1 > (MAX32 - op2 - c))?1:0);
}

//Check condition code and flags to determine if branch needed
int checkBranch(){
    if(((cur_inst >> 8) & 0xF) == EQ){
        if(zero_flag){
            return 1;
        }
    }else if(((cur_inst >> 8) & 0xF) == NE){
        if(zero_flag == 0){
            return 1;
        }
    }else if(((cur_inst >> 8) & 0xF) == CS){
        if(carry_flag){
            return 1;
        }
    }else if(((cur_inst >> 8) & 0xF) == CC){
        if(!carry_flag){
            return 1;
        }
    }else if(((cur_inst >> 8) & 0xF) == MI){
        if(sign_flag){
            return 1;
        }
    }else if(((cur_inst >> 8) & 0xF) == PL){
        if(!sign_flag){
            return 1;
        }
    }else if(((cur_inst >> 8) & 0xF) == HI){
        if(carry_flag && zero_flag == 0){
            return 1;
        }
    }else if(((cur_inst >> 8) & 0xF) == LS){
        if(carry_flag == 0 || zero_flag){
            return 1;
        }
    }else if(((cur_inst >> 8) & 0xF) == AL){
        return 1;
    }

    return 0;
}
