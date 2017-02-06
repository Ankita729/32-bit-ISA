Author : ABHINANDAN MITTAL	 14UCS002
	 AMIT SHUKLA		 14UCS011
	 ANANT NEMA		 14UCS012
	 ANKITA BISHNOI		 14UCS015


Assembler for converting our own Assembly Language program into Machine Language. Written in C

Usage :

	1) ./a.out input_file out_file
		reads the input from the file "input_file" and writes the answer in "output_file".
	
	2) ./a.out help
		Tells all the constraints and syntax needed to be used.
		
Constraints and Syntax:
	Input file must be present in the same directory.
	mneumonic instruction muct start with ' '(space) or '	'(tab).	
	Line containing label must not contain any instruction.
	Line containing label must not begin with space.
	Immediate data must be in decimal.
	'#' must be used before starting a comment.
	Value of decimal input chould not be greater than (2^8-1) i.e. 255
	
Sample Usage :

START
	MOVI B,10
	MOV  A,B
#incrementing 'A' register
	INC A
	MUL C,B
	MOD B,A,C
	HLT

	
OPCODE ADDRESSING :

ADD	:00000000000000000
SUB	:00000000000000001
MUL	:00000000000000010
DIV	:00000000000000011
MOD	:00000000000000100
JMP	:00000000000000101
JLT	:00000000000000110
JGT	:00000000000000111
JEQ	:00000000000001000
CMP	:0000000000000100100000
MOV	:0000000000000100100001
CMPI	:0000000000000101000
MOVI	:0000000000000101001
IN	:0000000000000101010
INC	:000000000000010010001000000
DEC	:000000000000010010001000001
OUT	:000000000000010010001000010
HLT	:11111111111111111111111111111111


