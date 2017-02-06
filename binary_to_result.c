/*************************************************************************************************************************************************
**							ResultOfMachineCode - version 1.0  					    		**
**************************************************************************************************************************************************
**					                                                                            				**
**Description ResultOfMachineCode is a part of an open source assembler written in C for our Instruction Set Architecture which is designed     **
**during a project in COA Course.                                                             							**
**                                                                                                                  				**
**		Authors  =	ABHINANDAN MITTAL        14UCS002		The LNM institute of information technology         		**
**               		AMIT SHUKLA		 14UCS011		The LNM institute of information technology                     **
**               		ANANT NEMA		 14UCS012		The LNM institute of information technology                     **
**               		ANKITA BISHNOI		 14UCS015		The LNM institute of information technology                     **
*************************************************************************************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#define BINARY_WIDTH 40			// SIZE OF THE ARRAY BEING USED TO SAVE BINARY INPUT
#define	BINARY_HEIGHT 1000		// HEIGHT OF THE ARRAY BEING USED TO SAVE BINARY INPUT
#define EXTRACT_17 4294934528		// USED TO EXTRACT MOST SIGNIFICANT 17 BITS OF A BINARY INSTRUCTION
#define EXTRACT_18_19 24576		// USED TO EXTRACT MOST SIGNIFICANT 18TH AND 19TH BITS OF BINARY INSTRUCTION
#define EXTRACT_18_22 31744		// USED TO EXTRACT MOST SIGNIFICANT 18TH TO 22ND BITS OF BINARY INSTRUCTION
#define EXTRACT_23_27 992		// USED TO EXTRACT MOST SIGNIFICANT 23RD TO 27TH BITS OF BINARY INSTRUCTION
#define REGISTER_NUMBER 26 		// NUMBER OF REGISTER BEING USED IN ASSEMBLY LANGUAGE
#define EXTRACT 31			// NUMBER TO EXTRACT REGISTER FROM THE INSTRUCTION THROUGH MASKING

char Binary[BINARY_WIDTH][BINARY_HEIGHT];
long long int Integer[BINARY_HEIGHT];
int Register_value[100] = {-1};
int Program_counter = 0;
int Status_counter = 0;
int Flag_counter = 0;	//n___z__o			// o->1 ; z->8 ; n->128 ; n,o -> 129 ; n,z -> 136 ; z,o -> 9 ; n,z,o -> 137
int End_program=0;
char Character[35];
char Registr[] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
int opcode[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25};

/********************************************************************************************************************
FUNCTION TO CONVERT A CHARACTER TO INTEGER
********************************************************************************************************************/
int Char_To_Digit(char p)
{
	return p-48;
}

/********************************************************************************************************************
FUNCTION TO CONVERT A INTEGER TO CHARACTER
********************************************************************************************************************/
char Digit_To_Char(int p)
{
	return p+48;
}

/********************************************************************************************************************
FUNCTION TO CONVERT BINARY STRING TO INTEGER...
********************************************************************************************************************/
long long int Binary_To_Integer(char* t)
{
	long long int ger=0,p=1;
	int i,j;
	for (i=31;i>=0;i--)
	{
		//printf("%lld %lld\n",ger,p);
		ger += Char_To_Digit(t[i])*p;
		p *= 2;
	}
	return ger;
}

/********************************************************************************************************************
FUNCTION TO CONVERT INTEGER TO BINARY STRING...
********************************************************************************************************************/
void Integer_To_Binary(long long int t)
{
	int Integer[35];
	int i=0,j=0;
	while(t != 0)
	{
		Integer[i++] = t%2;
		t = t/2;
	}
	for (j=i-1;j>=0;j--)
	{
		Character[j] = Digit_To_Char(Integer[i-1-j]);
	}
	Character[i]='\0';
	//printf("%s\n",Character);
}

/********************************************************************************************************************
FUNCITON TO CALCULATE THE POWER OF 2
********************************************************************************************************************/
long long int power_2(int p)
{
	if (p == -1)	
		return 0;
		
	long long int power=1,i;
	
	for (i=1;i<=p;i++)
	{
		power *= 2;
	}
	return power;
}

/********************************************************************************************************************
RETURNS THE REGISTER NAME OF THE ADDRESS BEING INPUTED ...
********************************************************************************************************************/
char Binary_To_Register_8(long long int a)
{
	int i;
	for (i = 0; i < REGISTER_NUMBER ; ++i)
	{
		if(a == opcode[i]<<8)
			return Registr[i];
	}
	printf("No register exist of this address,invalid register in line %d\n",Program_counter);
	exit (0);
}

/********************************************************************************************************************
RETURNS THE REGISTER NAME OF THE ADDRESS BEING INPUTED ...
********************************************************************************************************************/
char Binary_To_Register(long long int a)
{
	int i;
	for (i = 0; i < REGISTER_NUMBER ; ++i)
	{
		if(a == opcode[i] || a == opcode[i]<<5 || a == opcode[i]<<10)
			return Registr[i];
	}
	printf("No register exist of this address,invalid register in line %d\n",Program_counter);
	exit (0);
}

/********************************************************************************************************************
PERFORMS ADDITION OPERATION		ADD A,B,C -> A = B+C;
********************************************************************************************************************/
void Call_ADD()
{
	Flag_counter=0;
	int Address_Register_1,Address_Register_2,Address_Register_3;
		
	Address_Register_1 = Integer[Program_counter] & EXTRACT;
	Address_Register_2 = Integer[Program_counter] & (EXTRACT << 5);
	Address_Register_3 = Integer[Program_counter] & (EXTRACT << 10);
	
	Register_value[Binary_To_Register(Address_Register_3)] = Register_value[Binary_To_Register(Address_Register_2)] + Register_value[Binary_To_Register(Address_Register_1)];
	Program_counter++;
	if (Register_value[Binary_To_Register(Address_Register_3)] > 255)
	{
		Register_value[Binary_To_Register(Address_Register_3)] %= 256;
		Flag_counter += 1;
	}
	if (Register_value[Binary_To_Register(Address_Register_3)] == 0)
	{
		Flag_counter += 8;
	}
}

/********************************************************************************************************************
PERFORMS SUBTRACTION OPERATION			SUB A,B,C -> A = B-C;
********************************************************************************************************************/
void Call_SUB()
{
	Flag_counter = 0;
	int Address_Register_1,Address_Register_2,Address_Register_3;
	
	Address_Register_1 = Integer[Program_counter] & EXTRACT;
	Address_Register_2 = Integer[Program_counter] & (EXTRACT << 5);
	Address_Register_3 = Integer[Program_counter] & (EXTRACT << 10);
	
	Register_value[Binary_To_Register(Address_Register_3)] = Register_value[Binary_To_Register(Address_Register_2)] - Register_value[Binary_To_Register(Address_Register_1)];
	Program_counter++;
	if (Register_value[Binary_To_Register(Address_Register_3)] < 0)
	{
		Flag_counter += 128;
	}
	if (Register_value[Binary_To_Register(Address_Register_3)] == 0)
	{
		Flag_counter += 8;
	}
}

/********************************************************************************************************************
PERFORMS MULTIPLY OPERATION			MUL A,B,C -> A = B*C;
********************************************************************************************************************/
void Call_MUL()
{
	Flag_counter = 0;
	int Address_Register_1,Address_Register_2,Address_Register_3;
	
	Address_Register_1 = Integer[Program_counter] & EXTRACT;
	Address_Register_2 = Integer[Program_counter] & (EXTRACT << 5);
	Address_Register_3 = Integer[Program_counter] & (EXTRACT << 10);
	
	Register_value[Binary_To_Register(Address_Register_3)] = Register_value[Binary_To_Register(Address_Register_2)] * Register_value[Binary_To_Register(Address_Register_1)];
	Program_counter++;
	if (Register_value[Binary_To_Register(Address_Register_3)] > 255)
	{
		Register_value[Binary_To_Register(Address_Register_3)] %= 256;
		Flag_counter += 1;
	}
	if (Register_value[Binary_To_Register(Address_Register_3)] == 0)
	{
		Flag_counter += 8;
	}
}


/********************************************************************************************************************
PERFORMS DIVISION OPERATION			DIV A,B,C -> A = B/C;
********************************************************************************************************************/
void Call_DIV()
{
	Flag_counter = 0;
	int Address_Register_1,Address_Register_2,Address_Register_3;
	
	Address_Register_1 = Integer[Program_counter] & EXTRACT;
	Address_Register_2 = Integer[Program_counter] & (EXTRACT << 5);
	Address_Register_3 = Integer[Program_counter] & (EXTRACT << 10);
	
	if (Register_value[Binary_To_Register(Address_Register_1)] == 0)
	{
		printf("Error at line %d ; Division with 0 cannot be done\n",Program_counter);
		exit (0);
	}
	
	Register_value[Binary_To_Register(Address_Register_3)] = Register_value[Binary_To_Register(Address_Register_2)] / Register_value[Binary_To_Register(Address_Register_1)];
	Program_counter++;
	if (Register_value[Binary_To_Register(Address_Register_3)] == 0)
	{
		Flag_counter += 8;
	}
}

/********************************************************************************************************************
PERFORMS MODULUS OPERATION			MOD A,B,C -> A = B%C;
********************************************************************************************************************/
void Call_MOD()
{
	Flag_counter = 0;
	int Address_Register_1,Address_Register_2,Address_Register_3;
	
	Address_Register_1 = Integer[Program_counter] & EXTRACT;
	Address_Register_2 = Integer[Program_counter] & (EXTRACT << 5);
	Address_Register_3 = Integer[Program_counter] & (EXTRACT << 10);
	
	if (Register_value[Binary_To_Register(Address_Register_1)] == 0)
	{
		printf("Error at line %d ; Division with 0 cannot be done\n",Program_counter);
		exit (0);
	}
	
	Register_value[Binary_To_Register(Address_Register_3)] = Register_value[Binary_To_Register(Address_Register_2)] % Register_value[Binary_To_Register(Address_Register_1)];
	Program_counter++;
	if (Register_value[Binary_To_Register(Address_Register_3)] == 0)
	{
		Flag_counter += 8;
	}
}

/********************************************************************************************************************
PERFORMS UNCONDITIONAL JUMP OPERATION
********************************************************************************************************************/
void Call_JMP()
{
	Flag_counter = 0;
	int extract_add=32767,address;
	address = Integer[Program_counter] & extract_add;
	
	Program_counter = address/4;
}

/********************************************************************************************************************
PERFORMS CONDITIONAL OPERATION (HAPPENES IF 2ND NUMBER IS LESS THAN FIRST NUMBER)
********************************************************************************************************************/
void Call_JLT()
{
	Flag_counter = 0;
	if (Status_counter >= 0)
	{
		Program_counter++;
	}
	else
	{
		int extract_add=32767,address;
		address = Integer[Program_counter] & extract_add;
		
		Program_counter = address/4;
	}
}

/********************************************************************************************************************
PERFORMS CONDITIONAL OPERATION (HAPPENES IF 2ND NUMBER IS GREATER THAN FIRST NUMBER)
********************************************************************************************************************/
void Call_JGT()
{
	Flag_counter = 0;
	if (Status_counter <= 0)
	{
		Program_counter++;
	}
	else
	{
		int extract_add=32767,address;
		address = Integer[Program_counter] & extract_add;
		
		Program_counter = address/4;
	}
}

/********************************************************************************************************************
PERFORMS CONDITIONAL OPERATION (HAPPENES IF 2ND NUMBER IS EQUAL TO FIRST NUMBER)
********************************************************************************************************************/
void Call_JEQ()
{
	Flag_counter = 0;
	if (Status_counter != 0)
	{
		Program_counter++;
	}
	else
	{
		int extract_add=32767,address;
		address = Integer[Program_counter] & extract_add;
		
		Program_counter = address/4;
	}
}

/********************************************************************************************************************
PERFORMS COMPARISON BETWEEN 2 REGISTERS OPERATION
********************************************************************************************************************/
void Call_CMP()
{
	Flag_counter = 0;
	int Address_Register_1,Address_Register_2;
	
	Address_Register_1 = Integer[Program_counter] & EXTRACT;
	Address_Register_2 = Integer[Program_counter] & (EXTRACT << 5);

	Status_counter = Register_value[Binary_To_Register(Address_Register_2)] - Register_value[Binary_To_Register(Address_Register_1)];
	Program_counter++;
	//printf("Status_counter = %d\n",Status_counter);
	if (Status_counter < 0)
	{
		Flag_counter += 128;
	}
	if (Status_counter == 0)
	{
		Flag_counter += 8;
	}
}

/********************************************************************************************************************
PERFORMS MOVEMENT OPERATION (MOVES VALUE FROM ONE REGISTER TO ANOTHER REGISTER)
********************************************************************************************************************/
void Call_MOV()
{
	Flag_counter = 0;
	int Address_Register_1,Address_Register_2;
	
	Address_Register_1 = Integer[Program_counter] & EXTRACT;
	Address_Register_2 = Integer[Program_counter] & (EXTRACT << 5);

	Register_value[Binary_To_Register(Address_Register_2)] = Register_value[Binary_To_Register(Address_Register_1)];
	Program_counter++;
}

/********************************************************************************************************************
PERFORMS COMPARISON OPERATION (COMPARES A REGISTER WITH A CONSTANT VALUE)
********************************************************************************************************************/
void Call_CMPI()
{
	Flag_counter = 0;
	int Address_Register,value;
	int extract_8 = 255;
	
	Address_Register = Integer[Program_counter] & (EXTRACT << 5);
	value = Integer[Program_counter] & extract_8;
	
	Status_counter = Register_value[Binary_To_Register_8(Address_Register)] - value;
	Program_counter++;
	//printf("Status_counter = %d\n",Status_counter);
	if (Status_counter < 0)
	{
		Flag_counter += 128;
	}
	if (Status_counter == 0)
	{
		Flag_counter += 8;
	}
}

/********************************************************************************************************************
PERFORMS MOVEMENT OPERATION (MOVES CONSTANT VALUE TO REGISTER)
********************************************************************************************************************/
void Call_MOVI()
{
	Flag_counter = 0;
	int Address_Register,value;
	int extract_8 = 255;
	
	Address_Register = Integer[Program_counter] & (EXTRACT << 8);
	value = Integer[Program_counter] & extract_8;

	Register_value[Binary_To_Register_8(Address_Register)] = value;
	Program_counter++;
	//printf("%c = %d\n",Binary_To_Register(Address_Register),Register_value[Binary_To_Register(Address_Register)]);
}

/********************************************************************************************************************
TAKES INPUT FROM THE USER
********************************************************************************************************************/
void Call_IN()
{	
	Flag_counter = 0;
	int Address_Register,value;
	int extract_8 = 255;
	
	Address_Register = Integer[Program_counter] & (EXTRACT << 8);
	value = Integer[Program_counter] & extract_8;

	Register_value[Binary_To_Register_8(Address_Register)] = value;
	Program_counter++;
	//printf("%c = %d\n",Binary_To_Register(Address_Register),Register_value[Binary_To_Register(Address_Register)]);
}

/********************************************************************************************************************
INCREMENTS THE VALUE OF THE REGISTER BY 1
********************************************************************************************************************/
void Call_INC()
{
	Flag_counter = 0;
	int Address_Register = Integer[Program_counter] & EXTRACT;
	
	Register_value[Binary_To_Register(Address_Register)] += 1;
	Program_counter++;
	
	if (Register_value[Binary_To_Register(Address_Register)] > 255)
	{
		Register_value[Binary_To_Register(Address_Register)] %= 256;
		Flag_counter += 8;
	}
}

/********************************************************************************************************************
DECREMENTS THE VALUE OF THE REGISTER BY 1
********************************************************************************************************************/
void Call_DEC()
{
	Flag_counter = 0;
	int Address_Register = Integer[Program_counter] & EXTRACT;
	
	Register_value[Binary_To_Register(Address_Register)] -= 1;
	Program_counter++;
	
	if (Register_value[Binary_To_Register(Address_Register)] < 0)
	{
		Flag_counter += 128;
	}
	if (Register_value[Binary_To_Register(Address_Register)] == 0)
	{
		Flag_counter += 8;
	}
}

/********************************************************************************************************************
PERFORMS OPERATION TO PRINT THE VALUE OF GIVEN REGISTER
********************************************************************************************************************/
void Call_OUT()
{
	Flag_counter = 0;
	int Address_Register = Integer[Program_counter] & EXTRACT;
	
	printf("Register %c = %d\n",Binary_To_Register(Address_Register),Register_value[Binary_To_Register(Address_Register)]);
	Program_counter++;
}

/********************************************************************************************************************
PERFORMS OPERATION TO STOP THE OPERATION
********************************************************************************************************************/
void Call_HLT()
{	
	Flag_counter = 0;
	End_program = 1;
	Program_counter++;
}

/********************************************************************************************************************
FUNCTION TO DETERMINE THE OPCODE
********************************************************************************************************************/
char* Determine_Opcode(long long int instruction)
{
	if (instruction == 0)									//00000000000000000
	{
		return "ADD";
	}
	if (instruction == power_2(15))								//00000000000000001
	{
		return "SUB";
	}
	if (instruction == power_2(16))								//00000000000000010
	{
		return "MUL";
	}
	if (instruction == power_2(15) + power_2(16))						//00000000000000011
	{
		return "DIV";
	}
	if (instruction == power_2(17))								//00000000000000100
	{
		return "MOD";
	}
	if (instruction == power_2(17) + power_2(15))						//00000000000000101
	{
		return "JMP";
	}
	if (instruction == power_2(17) + power_2(16))						//00000000000000110
	{
		return "JLT";
	}
	if (instruction == power_2(17) + power_2(16) + power_2(15))				//00000000000000111
	{
		return "JGT";
	}
	if (instruction == power_2(18))								//00000000000001000
	{
		return "JEQ";
	}	
	if (instruction == power_2(18) + power_2(15))						//0000000000000100100000
	{
		return "CMP";
	}
	if (instruction == power_2(18) + power_2(15) + power_2(10))				//0000000000000100100001
	{
		return "MOV";
	}
	if (instruction == power_2(18) + power_2(16))						//0000000000000101000
	{
		return "CMPI";
	}
	if (instruction == power_2(18) + power_2(16) + power_2(13))				//0000000000000101001
	{
		return "MOVI";
	}
	if (instruction == power_2(18) + power_2(16) + power_2(14))				//0000000000000101010
	{
		return "IN";
	}
	if (instruction == power_2(18) + power_2(15) + power_2(11))				//000000000000010010001000000
	{
		return "INC";
	}
	if (instruction == power_2(18) + power_2(15) + power_2(11) + power_2(5))		//000000000000010010001000001
	{
		return "DEC";
	}
	if (instruction == power_2(18) + power_2(15) + power_2(11) + power_2(6))		//000000000000010010001000010
	{
		return "OUT";
	}
	if (instruction == EXTRACT_17)								//11111111111111111111111111111111
	{
		return "HLT";
	}
	else
	{
		printf("Provided input in the line %d contains invalid opcode\n",Program_counter);
		exit (0);
	}
}

/********************************************************************************************************************
FUNCTION TO PERFORM A PARTICULAR INSTRUCTION
********************************************************************************************************************/
char* Perform_Instruction(long long int instruction)
{
	//printf("instruction = %lld\n",instruction);
	Integer_To_Binary(instruction);
	char *op;
	if (instruction == EXTRACT_17)
	{
		op = Determine_Opcode(instruction);
	}
	else if (instruction <= power_2(18))
	{
		op = Determine_Opcode(instruction);
	}
	else if (instruction == power_2(18) + power_2(16))
	{
		instruction = instruction + (Integer[Program_counter] & EXTRACT_18_19);
		//printf("instruction = %lld\n",instruction);
		Integer_To_Binary(instruction);
		op = Determine_Opcode(instruction);
	}
	else if (instruction <= power_2(18) + power_2(15))
	{
		instruction = instruction + (Integer[Program_counter] & EXTRACT_18_22);
		//printf("instruction = %lld\n",instruction);
		Integer_To_Binary(instruction);
		
		if (instruction == power_2(18) + power_2(15))
		{
			op = Determine_Opcode(instruction);
		}
		else if (instruction == power_2(18) + power_2(15) + power_2(10))
		{
			op = Determine_Opcode(instruction);
		}
		else if (instruction == power_2(18) + power_2(15) + power_2(11))
		{
			instruction = instruction + (Integer[Program_counter] & EXTRACT_23_27);
			//printf("instruction = %lld\n",instruction);
			Integer_To_Binary(instruction);
			
			if (instruction == power_2(18) + power_2(15) + power_2(11))
			{
				op = Determine_Opcode(instruction);
			}
			else if (instruction == power_2(18) + power_2(15) + power_2(11) + power_2(5))
			{
				op = Determine_Opcode(instruction);
			}
			else if (instruction == power_2(18) + power_2(15) + power_2(11) + power_2(6))
			{
				op = Determine_Opcode(instruction);
			}
			else
			{	
				printf("Provided input in the line %d contains invalid opcode\n",Program_counter);
				exit (0);
			}
		}
	}
	else
	{
		printf("Provided input in the line %d contains invalid opcode\n",Program_counter);
		exit (0);
	}
	
	//printf("operation = %s\n",op);
	
	if (op == "ADD")
		Call_ADD();
	else if (op == "SUB")
		Call_SUB();
	else if (op == "MUL")
		Call_MUL();
	else if (op == "DIV")
		Call_DIV();
	else if (op == "MOD")
		Call_MOD();
	else if (op == "JMP")
		Call_JMP();
	else if (op == "JLT")
		Call_JLT();
	else if (op == "JGT")
		Call_JGT();
	else if (op == "JEQ")
		Call_JEQ();
	else if (op == "CMP")
		Call_CMP();
	else if (op == "MOV")
		Call_MOV();
	else if (op == "CMPI")
		Call_CMPI();
	else if (op == "MOVI")
		Call_MOVI();
	else if (op == "IN")
		Call_IN();
	else if (op == "INC")
		Call_INC();
	else if (op == "DEC")
		Call_DEC();
	else if (op == "OUT")
		Call_OUT();
	else if (op == "HLT")
		Call_HLT();
	else
	{
		printf("Provided input in the line %d contains invalid opcode\n",Program_counter);
		exit (0);
	}
	return op;
}

/********************************************************************************************************************
MAIN FUNCTION OF THIS PROGRAM.... 
********************************************************************************************************************/
int main(int argc,char const *argv[])
{
	FILE *In_File,*Out_File;
	char In_File_Name[20];
	char Out_File_Name[20];
	int Input_No_Of_Lines=0;
	int Size_Source = 0;		// input size of the 2D array
	int i,j,k;
	
	if(argc <= 2)
	{
		printf("correct syntax to use : \"./a.out input_file_name output_file_name\"\n");
		exit (20);
	}
	
	if (!strcmp(argv[1],argv[2]))
	{
		printf("Warning : The input file will be modified.Do you wish to continue Y/N");
		char ch;
		scanf("%c",&ch);
		if (ch == 'N' || ch == 'n')		
			exit (0);
	}
	
	strcpy(In_File_Name,argv[1]);
	strcpy(Out_File_Name,argv[2]);	
	In_File = fopen(argv[1],"r");
	Out_File = fopen(argv[2],"w");
	
	if(!In_File)						//To check if the input exist or not
	{
		printf("Input file not found !!\n");
		return 0;
	}

	while(fgets(Binary[Size_Source],BINARY_WIDTH,In_File)!=NULL)		// writed the input file in a 2 dimensional array 
	{
		Input_No_Of_Lines++;
		Binary[Size_Source][strlen(Binary[Size_Source])-1] = '\0';
		Size_Source++;
	}
	Binary[Size_Source-1][strlen(Binary[Size_Source-1])] = '1';
	Binary[Size_Source-1][strlen(Binary[Size_Source-1])+1] = '\0';
	fclose(In_File);
	
	for (i=0;i<Size_Source;i++)		// saves the integer conversation of a binary instruction
	{
		Integer[i] = Binary_To_Integer(Binary[i]);
	}
	
	/*printf("\n");				// prints the 2d array which has input file saved in it.
	for (i=0;i<Size_Source;i++)
	{	
		printf("%s %lld\n",Binary[i],Integer[i]);
	}
	printf("Input_No_Of_Lines = %d\n\n",Input_No_Of_Lines);*/
	
	Program_counter = 0;
	while(1)
	{
		/*printf("\nValue of all the registers : \n");
		for (i='A';i<='Z';i++)
		{
			printf("%c=%d ",i,Register_value[i]);
		}
		printf("\n\n");
		printf("program counter = %d\n",Program_counter*4);*/
		//printf("program counter = %d; status counter = %d; N = %d; Z = %d; O = %d\n",Program_counter*4,Status_counter,Negative,Zero,Overflow);
		if (End_program == 1)
		{
			break;
		}
		long long int instruction = Integer[Program_counter] & EXTRACT_17;	// extracting first 17 bits of the instruction
		
		fprintf(Out_File,"Program counter %d\n",Program_counter*4);
		char *opcode = Perform_Instruction(instruction);
		fprintf(Out_File,"Opcode = %s\n",opcode);
		fprintf(Out_File,"Status counter = %d\n",Status_counter);
		if (Flag_counter == 0)
			fprintf(Out_File,"N = 0; Z = 0; O = 0;\n");
		else if (Flag_counter == 1)
			fprintf(Out_File,"N = 0; Z = 0; O = 1;\n");
		else if (Flag_counter == 8)
			fprintf(Out_File,"N = 0; Z = 1; O = 0;\n");
		else if (Flag_counter == 128)
			fprintf(Out_File,"N = 1; Z = 0; O = 0;\n");
		else if (Flag_counter == 129)
			fprintf(Out_File,"N = 1; Z = 0; O = 1;\n");
		else if (Flag_counter == 136)
			fprintf(Out_File,"N = 1; Z = 1; O = 0;\n");
		else if (Flag_counter == 9)
			fprintf(Out_File,"N = 0; Z = 1; O = 1;\n");
		else if (Flag_counter == 137)
			fprintf(Out_File,"N = 1; Z = 1; O = 1;\n");
		for (i='A';i<='Z';i++)
		{	
			fprintf(Out_File,"%c=%d ",i,Register_value[i]);
		}
		fprintf(Out_File,"\n\n");
	}
	
	printf("\nValue of all the registers : \n");
	for (i='A';i<='Z';i++)
	{
		printf("%c=%d ",i,Register_value[i]);
	}
	printf("\n\n");
	return 0;
}
