/*************************************************************************************************************************************************
**							GetMAchineCode - version 1.0  					    			**
**************************************************************************************************************************************************	
**					                                                                            				**
**Description GetMAchineCode is a part of an open source assembler written in C for our Instruction Set Architecture which is designed during a	**
**project in COA Course.                                                                 							**
**                                                                                                                  				**
**		authors  =	ABHINANDAN MITTAL        14UCS002		The LNM institute of information technology         		**
**               		AMIT SHUKLA		 14UCS011		The LNM institute of information technology                     **
**               		ANANT NEMA		 14UCS012		The LNM institute of information technology                     **
**               		ANKITA BISHNOI		 14UCS015		The LNM institute of information technology                     **
*************************************************************************************************************************************************/
 	
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define INPUT_WIDTH 50					//Specifies Max length for an instruction
#define INPUT_HEIGHT 1000				//Specifies Max number of instructions
#define SYMBOL_TABLE_SIZE 1000 				//Specifies size of Symbol Table
#define LABEL_SIZE 20 					//Specifies max-size of a Label
#define MNEUMONIC_SIZE 5 				//Specifies max-size of a Mneumonic
#define REGISTER_NUMBER 26					//Specifies total number of Registers
#define	MACHINE_BIT 33					//to save the binary code in the resultant array

// GLOBAL VARIABLES
int Current_Index=0,Current_Line=0,Instruction_Location_Counter=0,Symbol_Table_Count=0;
char Source[INPUT_HEIGHT][INPUT_WIDTH];		//Array to store Source
char Destination[INPUT_HEIGHT][MACHINE_BIT];	//Array to store Result
int Output_Line=0,Output_Index=0;
int Base_Address=0;			//Base Address of the program after loading into memory
int Size_Source = 0;			// rows in the array Source
int Minus = 0;				// Removing comments and labels from ILC count

struct Symbol 
{
	char  label[LABEL_SIZE];
	int ILC;
};

typedef struct Symbol Symbol;

Symbol Symbol_Table[SYMBOL_TABLE_SIZE];			//Global array to store symbol table

/********************************************************************************************************************
FUNCTION TO PASS ANY SPACE IF IT EXIST
********************************************************************************************************************/
void Pass_Space()
{	
	while (Source[Current_Line][Current_Index] == '\t' || Source[Current_Line][Current_Index] == ' ')
	{
		Current_Index++;
	}
}


/********************************************************************************************************************
RETURNS THE NAME OF THE LABLE
********************************************************************************************************************/
char* Get_Label_Name()
{
	return Source[Current_Line];
}

/********************************************************************************************************************
FINDS IF THE LABEL USED AT THE CURRENT MOMENT HAS BEEN USED BEFORE OR NOT
********************************************************************************************************************/
int Search_Symbol_Table(char *name)
{
	int i;
	for(i=0;i<Symbol_Table_Count;i++)
	{
		if(!strcmp(Symbol_Table[i].label,name))
			return Symbol_Table[i].ILC;
	}
	return -1;
}

/********************************************************************************************************************
FUNCTION TO INSERT LABEL INTO THE SYMBOL TABLE
********************************************************************************************************************/
void Insert_In_Symbol_Table(char *name)
{
	int i;

	if(Search_Symbol_Table(name) != -1)  				//If Label already exists in symbol table
	{
		printf("GetMAchineCode : Error at line number %d ; Label of this name Already exist\n",Current_Line+1);
		return ;
	}
	Symbol_Table[Symbol_Table_Count].ILC = Instruction_Location_Counter;	//Using Global ILC
	strcpy(Symbol_Table[Symbol_Table_Count].label,name);
	Symbol_Table_Count++;
}

/********************************************************************************************************************
SENDS THE ADDRESS OF THE REGISTER BEING USED
********************************************************************************************************************/
char* Register_To_Binary(char a)
{
	char Registr[] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZ"};

	char *opcode[] = {
		"00000",		"00001",		"00010",		"00011",
		"00100",		"00101",		"00110",		"00111",
		"01000",		"01001",		"01010",		"01011",
		"01100",		"01101",		"01110",		"01111",
		"10000",		"10001",		"10010",		"10011",
		"10100",		"10101",		"10110",		"10111",
		"11000",		"11001"
	};
	int i=0;
	for (i = 0; i < REGISTER_NUMBER ; ++i)
	{
		if(a == Registr[i])
			return opcode[i];
	}
	//printf("reg = %c\n",a);
	printf("GetMAchineCode : Error at line number %d ; Register with this name does not exist\n",Current_Line+1);
	exit (0);
}

/********************************************************************************************************************
RETURNS BINARY CONVERSION OF ANY INTEGER NUMBER PASSED IN IT
********************************************************************************************************************/
unsigned long long int Decimal_To_Binary(int number)
{
	unsigned long long binary=0,i=1,remainder;
	while(number)
	{
		remainder = number % 2;
		number /= 2;
		binary += remainder*i;
		i *= 10;
	}
	return binary;
}

/********************************************************************************************************************
RETURNS CHARACTER CONVERSION OF AN INTEGER ONE DIGIT NUMBER
********************************************************************************************************************/
char Integer_to_Character(int number)
{
	return number+48;
}

/********************************************************************************************************************
RETURNS ONE DIGIT INTEGER CONVERSION OF CHARACTER PASSED TO THE FUNCTION
********************************************************************************************************************/
int Character_To_Integer(char character)
{
	return character-48;
}

/********************************************************************************************************************
RETURNS THE INTEGER CONVERSION OF THE CHARACTER STRING PASSED TO IT
********************************************************************************************************************/
int String_To_Integer(char *R2)
{
	int length = strlen(R2);
	//printf("length = %d\n",length);
	int i=0;
	int number=0;
	for (i=0;i<length;i++)
	{
		number = number*10 + Character_To_Integer(R2[i]);
		//printf("pre_num = %d\n",number);
	}
	return number;
}

/********************************************************************************************************************
RETURNS THE BINARY CHARACTER STRING OF THE INTEGER PASSED TO IT
********************************************************************************************************************/
void Integer_To_String(int number,char *Binary)
{
	int i=14;
	//printf("number = %d\n",number);
	while(number != 0)
	{
		Binary[i--] = Integer_to_Character(number%2);
		number /= 2;
	}
	Binary[15] = '\0';
}

/********************************************************************************************************************
PERFORMS THE OPERATION OF ADD OPCODE    ADD A,B,C -> A = B+C;
********************************************************************************************************************/
void Call_ADD()
{
	int i=0;
	char R1,R2,R3;
	strcpy(Destination[Output_Line],"00000000000000000");
	
	Pass_Space();		
	R1 = Source[Current_Line][Current_Index++];
	strcat(Destination[Output_Line],Register_To_Binary(R1));
	
	Pass_Space();
	
	if (Source[Current_Line][Current_Index] == ',')
		Current_Index++;
	else
	{
		printf("GetMAchineCode : Error at line number %d ;\n\twrong syntax... Use \"./a.out help\" to get info\n\n",Current_Line+1);
		exit (0);
	}	
	Pass_Space();
	
	R2 = Source[Current_Line][Current_Index++];
	strcat(Destination[Output_Line],Register_To_Binary(R2));
	
	Pass_Space();
	if (Source[Current_Line][Current_Index] == ',')
		Current_Index++;
	else
	{
		printf("GetMAchineCode : Error at line number %d ;\n\twrong syntax... Use \"./a.out help\" to get info\n\n",Current_Line+1);
		exit (0);
	}	
	Pass_Space();
	
	R3 = Source[Current_Line][Current_Index++];
	strcat(Destination[Output_Line],Register_To_Binary(R3));
	
	Pass_Space();
	if (Source[Current_Line][Current_Index] != '\0')
	{
		printf("GetMAchineCode : Error at line number %d ;\n\twrong syntax... Use \"./a.out help\" to get info\n\n",Current_Line+1);
		exit (0);
	}	
	Output_Line++;
}

/********************************************************************************************************************
PERFORMS THE OPERATION TO SUBTRACT OPCODE    SUB A,B,C -> A = B-C;
********************************************************************************************************************/
void Call_SUB()
{
	int i=0;
	char R1,R2,R3;
	strcpy(Destination[Output_Line],"00000000000000001");
	
	Pass_Space();
	
	R1 = Source[Current_Line][Current_Index++];
	strcat(Destination[Output_Line],Register_To_Binary(R1));
	
	Pass_Space();
	if (Source[Current_Line][Current_Index] == ',')
		Current_Index++;
	else
	{
		printf("GetMAchineCode : Error at line number %d ;\n\twrong syntax... Use \"./a.out help\" to get info\n\n",Current_Line+1);
		exit (0);
	}	
	Pass_Space();
	
	R2 = Source[Current_Line][Current_Index++];
	strcat(Destination[Output_Line],Register_To_Binary(R2));
	
	Pass_Space();	
	if (Source[Current_Line][Current_Index] == ',')
		Current_Index++;
	else
	{
		printf("GetMAchineCode : Error at line number %d ;\n\twrong syntax... Use \"./a.out help\" to get info\n\n",Current_Line+1);
		exit (0);
	}	
	Pass_Space();

	R3 = Source[Current_Line][Current_Index++];
	strcat(Destination[Output_Line],Register_To_Binary(R3));

	Pass_Space();
	if (Source[Current_Line][Current_Index] != '\0')
	{
		printf("GetMAchineCode : Error at line number %d ;\n\twrong syntax... Use \"./a.out help\" to get info\n\n",Current_Line+1);
		exit (0);
	}

	Output_Line++;
}

/********************************************************************************************************************
PERFORMS THE OPERATION TO MULTIPLY OPCODE    MUL A,B,C -> A = B*C;
********************************************************************************************************************/
void Call_MUL()
{
	int i=0;
	char R1,R2,R3;
	strcpy(Destination[Output_Line],"00000000000000010");
	
	Pass_Space();
	
	R1 = Source[Current_Line][Current_Index++];
	strcat(Destination[Output_Line],Register_To_Binary(R1));
	
	Pass_Space();
	if (Source[Current_Line][Current_Index] == ',')
		Current_Index++;
	else
	{
		printf("GetMAchineCode : Error at line number %d ;\n\twrong syntax... Use \"./a.out help\" to get info\n\n",Current_Line+1);
		exit (0);
	}	
	Pass_Space();

	R2 = Source[Current_Line][Current_Index++];
	strcat(Destination[Output_Line],Register_To_Binary(R2));
	
	Pass_Space();
	if (Source[Current_Line][Current_Index] == ',')
		Current_Index++;
	else
	{
		printf("GetMAchineCode : Error at line number %d ;\n\twrong syntax... Use \"./a.out help\" to get info\n\n",Current_Line+1);
		exit (0);
	}	
	Pass_Space();

	R3 = Source[Current_Line][Current_Index++];
	strcat(Destination[Output_Line],Register_To_Binary(R3));

	Pass_Space();	
	if (Source[Current_Line][Current_Index] != '\0')
	{
		printf("GetMAchineCode : Error at line number %d ;\n\twrong syntax... Use \"./a.out help\" to get info\n\n",Current_Line+1);
		exit (0);
	}

	Output_Line++;
}

/********************************************************************************************************************
PERFORMS THE OPERATION TO DIVIDE OPCODE    DIV A,B,C -> A = B/C;
********************************************************************************************************************/
void Call_DIV()
{
	int i=0;
	char R1,R2,R3;
	strcpy(Destination[Output_Line],"00000000000000011");
	
	Pass_Space();
	
	R1 = Source[Current_Line][Current_Index++];
	strcat(Destination[Output_Line],Register_To_Binary(R1));
	
	Pass_Space();
	if (Source[Current_Line][Current_Index] == ',')
		Current_Index++;
	else
	{
		printf("GetMAchineCode : Error at line number %d ;\n\twrong syntax... Use \"./a.out help\" to get info\n\n",Current_Line+1);
		exit (0);
	}	
	Pass_Space();
	
	R2 = Source[Current_Line][Current_Index++];
	strcat(Destination[Output_Line],Register_To_Binary(R2));
	
	Pass_Space();
	if (Source[Current_Line][Current_Index] == ',')
		Current_Index++;
	else
	{
		printf("GetMAchineCode : Error at line number %d ;\n\twrong syntax... Use \"./a.out help\" to get info\n\n",Current_Line+1);
		exit (0);
	}	
	Pass_Space();
	
	R3 = Source[Current_Line][Current_Index++];
	strcat(Destination[Output_Line],Register_To_Binary(R3));

	Pass_Space();	
	if (Source[Current_Line][Current_Index] != '\0')
	{
		printf("GetMAchineCode : Error at line number %d ;\n\twrong syntax... Use \"./a.out help\" to get info\n\n",Current_Line+1);
		exit (0);
	}

	Output_Line++;
}

/********************************************************************************************************************
PERFORMS THE OPERATION TO APPLY MOD OPCODE    MOD A,B,C -> A = B%C;
********************************************************************************************************************/
void Call_MOD()
{
	int i=0;
	char R1,R2,R3;
	strcpy(Destination[Output_Line],"00000000000000100");
	
	Pass_Space();
	
	R1 = Source[Current_Line][Current_Index++];
	strcat(Destination[Output_Line],Register_To_Binary(R1));
	
	Pass_Space();
	if (Source[Current_Line][Current_Index] == ',')
		Current_Index++;
	else
	{
		printf("GetMAchineCode : Error at line number %d ;\n\twrong syntax... Use \"./a.out help\" to get info\n\n",Current_Line+1);
		exit (0);
	}	
	Pass_Space();
	
	R2 = Source[Current_Line][Current_Index++];
	strcat(Destination[Output_Line],Register_To_Binary(R2));
	
	Pass_Space();
	if (Source[Current_Line][Current_Index] == ',')
		Current_Index++;
	else
	{
		printf("GetMAchineCode : Error at line number %d ;\n\twrong syntax... Use \"./a.out help\" to get info\n\n",Current_Line+1);
		exit (0);
	}	
	Pass_Space();
	
	R3 = Source[Current_Line][Current_Index++];
	strcat(Destination[Output_Line],Register_To_Binary(R3));

	Pass_Space();	
	if (Source[Current_Line][Current_Index] != '\0')
	{
		printf("GetMAchineCode : Error at line number %d ;\n\twrong syntax... Use \"./a.out help\" to get info\n\n",Current_Line+1);
		exit (0);
	}

	Output_Line++;
}

/********************************************************************************************************************
PERFORMS UNCONDITIONAL JUMP TO SPECIFIED POSITION	JMP LABEL_NAME
********************************************************************************************************************/
void Call_JMP()
{
	char label[LABEL_SIZE];
	strcpy(Destination[Output_Line],"00000000000000101");
	
	Pass_Space();
	
	int i=0;
	while(Source[Current_Line][Current_Index] != '\0')
		label[i++] = Source[Current_Line][Current_Index++];
	label[i] = '\0';
		
	int temp=0;
	char Binary[] = "000000000000000000";
	for (i=0;i<Symbol_Table_Count;i++)
	{
		if (!strcmp(label,Symbol_Table[i].label))
		{
			temp=1;
			Integer_To_String(Symbol_Table[i].ILC,Binary);
			strcat(Destination[Output_Line],Binary);
			break;	
		}
	}
	if (!temp)
	{
		printf("GetMAchineCode : Error at line number %d ; Label with this name does not exist\n",Current_Line+1);
		return ;
	}	
	
	Pass_Space();
	if (Source[Current_Line][Current_Index] != '\0')
	{
		printf("GetMAchineCode : Error at line number %d ;\n\twrong syntax... Use \"./a.out help\" to get info\n\n",Current_Line+1);
		exit (0);
	}

	Output_Line++;
}

/********************************************************************************************************************
PERFORMS CONDITIONAL JUMP (if register 2 is less than register 1) TO SPECIFIED POSITION	-> JMP LABEL_NAME
********************************************************************************************************************/
void Call_JLT()
{
	char label[LABEL_SIZE];
	strcpy(Destination[Output_Line],"00000000000000110");
	
	Pass_Space();
	
	int i=0;
	while(Source[Current_Line][Current_Index] != '\0')
		label[i++] = Source[Current_Line][Current_Index++];
	label[i] = '\0';
		
	int temp=0;
	char Binary[] = "000000000000000000";
	for (i=0;i<Symbol_Table_Count;i++)
	{
		if (!strcmp(label,Symbol_Table[i].label))
		{
			temp=1;
			Integer_To_String(Symbol_Table[i].ILC,Binary);
			strcat(Destination[Output_Line],Binary);
			break;	
		}
	}
	if (!temp)
	{
		printf("GetMAchineCode : Error at line number %d ; Label with this name does not exist\n",Current_Line+1);
		return ;
	}
	
	Pass_Space();
	if (Source[Current_Line][Current_Index] != '\0')
	{
		printf("GetMAchineCode : Error at line number %d ;\n\twrong syntax... Use \"./a.out help\" to get info\n\n",Current_Line+1);
		exit (0);
	}

	Output_Line++;
}

/********************************************************************************************************************
PERFORMS CONDITIONAL JUMP (if register 2 is greater than register 1) TO SPECIFIED POSITION	-> JMP LABEL_NAME
********************************************************************************************************************/
void Call_JGT()
{
	char label[LABEL_SIZE];
	strcpy(Destination[Output_Line],"00000000000000111");
	
	Pass_Space();
	
	int i=0;
	while(Source[Current_Line][Current_Index] != '\0')
		label[i++] = Source[Current_Line][Current_Index++];
	label[i] = '\0';
		
	int temp=0;
	char Binary[] = "000000000000000000";
	for (i=0;i<Symbol_Table_Count;i++)
	{
		if (!strcmp(label,Symbol_Table[i].label))
		{
			temp=1;
			Integer_To_String(Symbol_Table[i].ILC,Binary);
			strcat(Destination[Output_Line],Binary);
			break;	
		}
	}
	if (!temp)
	{
		printf("GetMAchineCode : Error at line number %d ; Label with this name does not exist\n",Current_Line+1);
		exit (0);
	}	

	Pass_Space();
	if (Source[Current_Line][Current_Index] != '\0')
	{
		printf("GetMAchineCode : Error at line number %d ;\n\twrong syntax... Use \"./a.out help\" to get info\n\n",Current_Line+1);
		exit (0);
	}

	Output_Line++;
}

/********************************************************************************************************************
PERFORMS CONDITIONAL JUMP (if register 2 is equal to register 1) TO SPECIFIED POSITION	-> JMP LABEL_NAME
********************************************************************************************************************/
void Call_JEQ()
{
	char label[LABEL_SIZE];
	strcpy(Destination[Output_Line],"00000000000001000");

	Pass_Space();
	
	int i=0;
	while(Source[Current_Line][Current_Index] != '\0')
		label[i++] = Source[Current_Line][Current_Index++];
	label[i] = '\0';
	
	int temp=0;
	char Binary[] = "000000000000000000";
	for (i=0;i<Symbol_Table_Count;i++)
	{
		if (!strcmp(label,Symbol_Table[i].label))
		{
			temp=1;
			Integer_To_String(Symbol_Table[i].ILC,Binary);	
			strcat(Destination[Output_Line],Binary);
			break;
		}
	}
	if (!temp)
	{
		printf("GetMAchineCode : Error at line number %d ; Label with this name does not exist\n",Current_Line+1);
		return ;
	}	

	Pass_Space();
	if (Source[Current_Line][Current_Index] != '\0')
	{
		printf("GetMAchineCode : Error at line number %d ;\n\twrong syntax... Use \"./a.out help\" to get info\n\n",Current_Line+1);
		exit (0);
	}

	Output_Line++;
}

/********************************************************************************************************************
PERFORMS THE COMPARISON OPERATION    CMP A,B -> COMPARES IF 'B' IS GREATER THAN,EQUAL TO OR LESS THAN 'A'
********************************************************************************************************************/
void Call_CMP()
{
	char R1,R2;
	strcpy(Destination[Output_Line],"0000000000000100100000");
	
	Pass_Space();
	
	R1 = Source[Current_Line][Current_Index++];
	strcat(Destination[Output_Line],Register_To_Binary(R1));
	
	Pass_Space();
	if (Source[Current_Line][Current_Index] == ',')
		Current_Index++;
	else
	{
		printf("GetMAchineCode : Error at line number %d ;\n\twrong syntax... Use \"./a.out help\" to get info\n\n",Current_Line+1);
		exit (0);
	}	
	Pass_Space();
	
	R2 = Source[Current_Line][Current_Index++];
	strcat(Destination[Output_Line],Register_To_Binary(R2));
	
	Pass_Space();
	if (Source[Current_Line][Current_Index] != '\0')
	{
		printf("GetMAchineCode : Error at line number %d ;\n\twrong syntax... Use \"./a.out help\" to get info\n\n",Current_Line+1);
		exit (0);
	}
	
	Output_Line++;
}

/********************************************************************************************************************
PERFORMS THE MOVEMENT OPERATION   MOV A,B -> A = B (MOVES VALUE OF REGISTER 'B' IN REGISTER 'A')
********************************************************************************************************************/
void Call_MOV()
{
	char R1,R2;
	strcpy(Destination[Output_Line],"0000000000000100100001");
	
	Pass_Space();
	
	R1 = Source[Current_Line][Current_Index++];
	strcat(Destination[Output_Line],Register_To_Binary(R1));
	
	Pass_Space();
	if (Source[Current_Line][Current_Index] == ',')
		Current_Index++;
	else
	{
		printf("GetMAchineCode : Error at line number %d ;\n\twrong syntax... Use \"./a.out help\" to get info\n\n",Current_Line+1);
		exit (0);
	}	
	Pass_Space();
	
	R2 = Source[Current_Line][Current_Index++];
	strcat(Destination[Output_Line],Register_To_Binary(R2));
	
	Pass_Space();
	//printf("%sabhi %canmol\n",Source[Current_Line],Source[Current_Line][Current_Index]);
	if (Source[Current_Line][Current_Index] != '\0')
	{
		printf("Get123MAchineCode : Error at line number %d ;\n\twrong syntax... Use \"./a.out help\" to get info\n\n",Current_Line+1);
		exit (0);
	}
	
	Output_Line++;
}

/********************************************************************************************************************
PERFORMS THE COMPARISON OPERATION    CMPI A,integer -> COMPARES IF integer IS GREATER THAN,EQUAL TO OR LESS THAN 'A'
********************************************************************************************************************/
void Call_CMPI()
{
	char R1;
	strcpy(Destination[Output_Line],"0000000000000101000");
	
	Pass_Space();
	
	R1 = Source[Current_Line][Current_Index++];
	strcat(Destination[Output_Line],Register_To_Binary(R1));
	
	Pass_Space();
	if (Source[Current_Line][Current_Index] == ',')
		Current_Index++;
	else
	{
		printf("GetMAchineCode : Error at line number %d ;\n\twrong syntax... Use \"./a.out help\" to get info\n\n",Current_Line+1);
		exit (0);
	}	
	Pass_Space();
	
	char R2[5];
	int i=0;
	while(Source[Current_Line][Current_Index] != '\0')
		R2[i++] = Source[Current_Line][Current_Index++];
	R2[i] = '\0';
	int number = String_To_Integer(R2);
	unsigned long long int binary = Decimal_To_Binary(number);
	char constant[8] = "00000000";
	int j=7;
	while(binary != 0)
	{
		constant[j--] = Integer_to_Character(binary%10);
		binary /= 10;
	}
	strcat(Destination[Output_Line],constant);
	
	Pass_Space();
	if (Source[Current_Line][Current_Index] != '\0')
	{
		printf("GetMAchineCode : Error at line number %d ;\n\twrong syntax... Use \"./a.out help\" to get info\n\n",Current_Line+1);
		exit (0);
	}
	
	Output_Line++;
}

/********************************************************************************************************************
PERFORMS THE MOVEMENT OPERATION    MOVI A,integer -> A = integer (MOVES VALUE OF INTEGER IN REGISTER 'A')
********************************************************************************************************************/
void Call_MOVI()
{
	char R1;
	strcpy(Destination[Output_Line],"0000000000000101001");
	
	Pass_Space();
	
	R1 = Source[Current_Line][Current_Index++];
	strcat(Destination[Output_Line],Register_To_Binary(R1));
	
	Pass_Space();
	if (Source[Current_Line][Current_Index] == ',')
		Current_Index++;
	else
	{
		printf("GetMAchineCode : Error at line number %d ;\n\twrong syntax... Use \"./a.out help\" to get info\n\n",Current_Line+1);
		exit (0);
	}	
	Pass_Space();
	
	char R2[5];
	int i=0;
	while(Source[Current_Line][Current_Index] != '\0')
		R2[i++] = Source[Current_Line][Current_Index++];
	R2[i] = '\0';
	//printf("R2 = %s\n",R2);
	int number = String_To_Integer(R2);
	//printf("num = %d\n",number);
	unsigned long long int binary = Decimal_To_Binary(number);
	char constant[8] = "00000000";
	int j=7;
	while(binary != 0)
	{
		constant[j--] = Integer_to_Character(binary%10);
		binary /= 10;
	}
	strcat(Destination[Output_Line],constant);
	
	Pass_Space();
	if (Source[Current_Line][Current_Index] != '\0')
	{
		printf("GetMAchineCode : Error at line number %d ;\n\twrong syntax... Use \"./a.out help\" to get info\n\n",Current_Line+1);
		exit (0);
	}
	
	Output_Line++;
}

/********************************************************************************************************************
TAKES INPUT FROM THE USER  ->  IN A	(SETS VALUE OF REGISTER 'A' EQUAL TO THE VALUE INPUTED BY THE USER)
********************************************************************************************************************/
void Call_IN()
{
	char R1;
	int number;
	strcpy(Destination[Output_Line],"0000000000000101010");
	
	Pass_Space();
	
	R1 = Source[Current_Line][Current_Index++];
	strcat(Destination[Output_Line],Register_To_Binary(R1));

	Pass_Space();
	if (Source[Current_Line][Current_Index] != '\0')
	{
		printf("GetMAc123hineCode : Error at line number %d ;\n\twrong syntax... Use \"./a.out help\" to get info\n\n",Current_Line+1);
		exit (0);
	}

	printf("Enter the value of the register %c : ",Source[Current_Line][Current_Index]);
	scanf("%d",&number);
	if (number > 255)
	{
		printf("GetMAchineCode : Error at line number %d ;\n\twrong syntax... Use \"./a.out help\" to get info\n\n",Current_Line+1);
		exit (0);
	}
	unsigned long long int binary = Decimal_To_Binary(number);
	char constant[8] = "00000000";
	int j=7;
	while(binary != 0)
	{
		constant[j--] = Integer_to_Character(binary%10);
		binary /= 10;
	}
	strcat(Destination[Output_Line],constant);
	
	Output_Line++;
}

/********************************************************************************************************************
PERFORMS THE INCREMENT OPERATION    INC A -> A = A+1
********************************************************************************************************************/
void Call_INC()
{
	char R1;
	strcpy(Destination[Output_Line],"000000000000010010001000000");
	
	Pass_Space();
	
	R1 = Source[Current_Line][Current_Index++];
	strcat(Destination[Output_Line],Register_To_Binary(R1));
	
	Pass_Space();
	if (Source[Current_Line][Current_Index] != '\0')
	{
		printf("GetMAchineCode : Error at line number %d ;\n\twrong syntax... Use \"./a.out help\" to get info\n\n",Current_Line+1);
		exit (0);
	}
	
	Output_Line++;
}

/********************************************************************************************************************
PERFORMS THE DECREMENT OPERATION    INC A -> A = A-1
********************************************************************************************************************/
void Call_DEC()
{
	char R1;
	strcpy(Destination[Output_Line],"000000000000010010001000001");
	
	Pass_Space();
	
	R1 = Source[Current_Line][Current_Index++];
	strcat(Destination[Output_Line],Register_To_Binary(R1));
	
	Pass_Space();
	if (Source[Current_Line][Current_Index] != '\0')
	{
		printf("GetMAchineCode : Error at line number %d ;\n\twrong syntax... Use \"./a.out help\" to get info\n\n",Current_Line+1);
		exit (0);
	}
	
	Output_Line++;
}

/********************************************************************************************************************
PRINTS THE OUPUT THE OF THE DESIRED REGISTER.	-> OUT A 
********************************************************************************************************************/
void Call_OUT()
{
	char R1;
	strcpy(Destination[Output_Line],"000000000000010010001000010");
	
	Pass_Space();
	
	R1 = Source[Current_Line][Current_Index++];
	strcat(Destination[Output_Line],Register_To_Binary(R1));
	
	Pass_Space();
	if (Source[Current_Line][Current_Index] != '\0')
	{
		printf("GetMAchineCode : Error at line number %d ;\n\twrong syntax... Use \"./a.out help\" to get info\n\n",Current_Line+1);
		exit (0);
	}
	
	Output_Line++;
}

/********************************************************************************************************************
PERFORMS THE EXIT OPERATION    
********************************************************************************************************************/
void Call_HLT()
{
	strcpy(Destination[Output_Line],"11111111111111111111111111111111");
	
	Pass_Space();
	if (Source[Current_Line][Current_Index] != '\0')
	{
		printf("GetMAchineCode : Error at line number %d ;\n\twrong syntax... Use \"./a.out help\" to get info\n\n",Current_Line+1);
		exit (0);
	}
	
	Output_Line++;
}

/********************************************************************************************************************
CHECKS WHICH OPCODE FUNCTION IS NEEDED TO BE CALLED NOW
********************************************************************************************************************/
void Compare_Opcode(char *operation)
{
	//printf("operation = %s\n",operation);
	if (!strcmp(operation,"ADD"))
		Call_ADD();
	else if (!strcmp(operation,"SUB"))
		Call_SUB();
	else if (!strcmp(operation,"MUL"))
		Call_MUL();
	else if (!strcmp(operation,"DIV"))
		Call_DIV();
	else if (!strcmp(operation,"MOD"))
		Call_MOD();
	else if (!strcmp(operation,"JMP"))
		Call_JMP();
	else if (!strcmp(operation,"JLT"))
		Call_JLT();
	else if (!strcmp(operation,"JGT"))
		Call_JGT();
	else if (!strcmp(operation,"JEQ"))
		Call_JEQ();	
	else if (!strcmp(operation,"CMP"))
		Call_CMP();
	else if (!strcmp(operation,"MOV"))
		Call_MOV();
	else if (!strcmp(operation,"CMPI"))
		Call_CMPI();
	else if (!strcmp(operation,"MOVI"))
		Call_MOVI();
	else if (!strcmp(operation,"IN"))
		Call_IN();
	else if (!strcmp(operation,"INC"))
		Call_INC();
	else if (!strcmp(operation,"DEC"))
		Call_DEC();
	else if (!strcmp(operation,"OUT"))
		Call_OUT();
	else if (!strcmp(operation,"HLT"))
		Call_HLT();
}

/********************************************************************************************************************
FUNCTION CALL TO CREATE ALL THE LABLES AND DICIDE INSTRUCTION LOCATION COUNTER 
********************************************************************************************************************/
void Pass_1_operation()
{
	Current_Line = 0;
	Current_Index = 0;
	Instruction_Location_Counter = 0;
	int flag=0;
	for (Current_Line=0;Current_Line<Size_Source;Current_Line++)
	{
		if (Source[Current_Line][Current_Index] == '\0')
		{
			continue;
		}
		else if(Source[Current_Line][Current_Index] != '\t' && Source[Current_Line][Current_Index] != ' ' && Source[Current_Line][Current_Index] != '\0')
		{
			Insert_In_Symbol_Table(Get_Label_Name());
		}
		else
		{
			Instruction_Location_Counter +=4;				//needs to be changed later.
		}
		// ILC increments by each instruction's operand length. 
	}
	Instruction_Location_Counter = 0;		// restoring all values for the second pass
	Current_Line = 0;
	Current_Index = 0;
}

/********************************************************************************************************************
FUNCTION CALL TO CREATE ALL THE LABLES AND DICIDE INSTRUCTION LOCATION COUNTER 
********************************************************************************************************************/
void Pass_2_operation()
{
	int i=0;
	for (Current_Line=0;Current_Line<Size_Source;Current_Line++)
	{
		if (Source[Current_Line][Current_Index] != '\t' && Source[Current_Line][Current_Index] != ' ' && Source[Current_Line][Current_Index] != '\0')
		{
			continue;
		}
		else if (Source[Current_Line][Current_Index] == '\0')
		{
			continue;
		}
		else
		{
			Pass_Space();
			char operation[5];
			int i=0;
			while(Source[Current_Line][Current_Index] != ' ' && Source[Current_Line][Current_Index] != '\0' && Source[Current_Line][Current_Index] != '\n')
			{
				operation[i++] = Source[Current_Line][Current_Index++];
			}
			operation[i] = '\0';
			Compare_Opcode(operation);
			Current_Index=0;
		}
	}
}

/********************************************************************************************************************
REMOVES THE LINES WHICH ARE COMMENTED OR ARE NOT NECESARRY FOR THE CODE 
IT ALSO ERROR IN THE CODE IF ANY COMMAND DOES NOT 
********************************************************************************************************************/
void Remove_Extra_Lines(int NoOfLines)
{
	int i,j;
	for (i=0;i<NoOfLines;i++)
	{
		if(Source[i][0] == '#')
		{
			Source[i][0] = '\0';
			break;
		}
	}
}

/********************************************************************************************************************
MAIN FUNCTION OF THIS PROGRAM.... 
********************************************************************************************************************/
int main(int argc, char const *argv[])
{
	int Input_No_Of_Lines,i,j;
	FILE *In_File,*Out_File;
	FILE *ST;
	char In_File_Name[20];
	char Out_File_Name[20];
	
	
	if(argc < 2)
	{
		printf("correct syntax to use : %s input_file_name output_file_name\nFor help use : %s help\n",argv[0],argv[0]);
		return 0;
	}

	if(!strcmp(argv[1],"help"))
	{
		printf("\n\tGetMAchineCode: Usage: %s input_file_name out_file_name\n\tInput file must be present in same directory",argv[0]);
		printf("\n\tNew line character  - \\n \n\tMneumonics must begin with ' ' or '\t' space \n\tLine containing Label should not contain any Mneumonic\n\tLine containing Label should not begin with space\n\t");
		printf("Immediate data must be in Decimal\n\t'#' sign represent the commenting section\n");
		printf("\nSample Usage:\nSTART\n\tLDR A,2048H\n\tMVR B,A\n\tLOP A\n\tMUL C,B\n\tDEC B\n\tHLT\n\n");
		return 0;
	}

	if(argc <3)
	{
		printf("correct syntax to use : %s input_file output_file\nFor help use : %s help\n",argv[0],argv[0]);
		return 0;
	}

	if(!strcmp(argv[1],argv[2]))
	{
		printf("Warning : The input file will be modified.Do you wish to continue Y/N");
		char ch;
		scanf("%c",&ch);
		if (ch == 'N' || ch == 'n')		
			return 0;		
	}
	else
	{
		strcpy(In_File_Name,argv[1]);
		strcpy(Out_File_Name,argv[2]);	
		In_File = fopen(argv[1],"r");
		Out_File = fopen(argv[2],"w");
		ST = fopen("Symbol_Table","w");
	}

	if(!In_File)
	{
		printf("Input file not found !!\n");
		return 0;
	}

	Input_No_Of_Lines=0;
	Size_Source = 0;
	
	while(fgets(Source[Size_Source],INPUT_WIDTH,In_File)!=NULL)
	{
		Input_No_Of_Lines++;
		Source[Size_Source][strlen(Source[Size_Source])-1] = '\0';
		Size_Source++;
	}
	fclose(In_File);
	
	for (i=0;i < Size_Source;i++)
	{
		for (j=0;j<strlen(Source[i]);j++)
		{									//converts small case to large case and writes in temp.txt
			if(Source[i][j]>96 && Source[i][j]<123)
				Source[i][j] -= 32;
		}
	}
	/*printf("\n");
	for (i=0;i<Size_Source;i++)
	{	
		printf("%s \n",Source[i]);
	}
	printf("Input_No_Of_Lines = %d\n\n",Input_No_Of_Lines);*/
	
	Remove_Extra_Lines(Input_No_Of_Lines);		// removes the lines which are commented or are not necessary for the code.

	/*printf("\n");
	for (i=0;i<Size_Source;i++)
	{	
		printf("%s\n",Source[i]);
	}
	printf("Input_No_Of_Lines = %d\n\n",Input_No_Of_Lines);*/

	Pass_1_operation();				// Pass 1 in the conversion process	// check for all the labels.
	
	/*printf("Symbol_Table_Count = %d\n",Symbol_Table_Count);
	for (i=0;i<Symbol_Table_Count;i++)
	{
		printf("%s %d\n",Symbol_Table[i].label,Symbol_Table[i].ILC);
	}
	printf("\n\n");*/
	
	for (i=0;i<Symbol_Table_Count;i++)
	{
		fprintf(ST,"%s %d",Symbol_Table[i].label,Symbol_Table[i].ILC);
		putc('\n',ST);
	}
	
	Pass_2_operation();				// Pass 2 in the conversion process	// write output in the file.
	
	//printf("\nBinary Output : \n");
	for (i=0;i<Output_Line;i++)
	{
		//printf("%s\n",Destination[i]);
		fputs(Destination[i],Out_File);
		if (i<Output_Line-1)
			fputc('\n',Out_File);
	}
	fclose(Out_File);
	//printf("\n");
	
	printf("\nOutput successfully written to file \"%s\" \n\n",Out_File_Name);
	return 0;
}
