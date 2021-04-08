#pragma once
#ifndef VM_H
#define VM_H

#include "main.h"


class VM
{
public:
	VM() {}
	void run(string filename);
	int countLine(string filename);
	string* getLine(string filename, int numLine);
	bool invalidInstructions(string* str, int size);
	bool invalidOperand(string* str, int size);
};
enum DataType {NONE,INT,FLOAT,BOOL,ADDRESS};

struct GeneralRegister //define general register
{
	int intValue;
	float floatValue;
	bool boolValue;
	int Address;
	DataType type;

};
struct Memory //define static memory
{
	int intValue;
	float floatValue;
	bool boolValue;
	DataType type;
};

struct Instruction //define instruction
{
	string code;
	string op1;
	string op2;
};

class Stack //define stack
{
private:
	int top;
public:
	int a[1000];
	Stack() { top = -1; }
	bool push(int x);
	int pop();
	int peek();
	bool isEmpty();
};

class CPU
{
private:
	int IP;
	GeneralRegister reg[15];           //register
	Memory staticMemory[65536];		   // static memory
	Stack stack;					   // stack
	Instruction codeMemory[65536];	   // instruction memory
	int codeLoadedIndex;
public:
	CPU() 
	{
		for (int k = 0; k < 65536; k++)
		{
			staticMemory[k].type = NONE;
		}
		this->IP = 0;
		for (int i = 0; i < 15; i++)
		{
			reg[i].type = NONE;
		}
		this->codeLoadedIndex = 0;
	}
	bool checkDot(string source);
	bool checkIsAddress(string source);
	bool checkIsBool(string source);
	bool stringToBool(string src);
	bool checkIsReg(string source);
	bool loadInstruction(string* str, int size);
	//input / output
	void Output(string dest);
	bool Input(string dest);
	//Cac lenh tinh toan
	bool Add(string dest, string src);
	bool Minus(string dest, string src);
	bool Mul(string dest, string src);
	bool Div(string dest, string src);

	//cac lenh so sanh
	bool CmpEQ(string dest, string src);
	bool CmpNE(string dest, string src);
	bool CmpLT(string dest, string src);
	bool CmpLE(string dest, string src);
	bool CmpGT(string dest, string src);
	bool CmpGE(string dest, string src);
	// cac lenh luan ly
	bool Not(string dest);
	bool And(string dest, string src);
	bool Or(string dest, string src);

	//cac lenh doc/ghi du lieu
	void Move(string dest, string src);
	bool Load(string dest, string src);
	bool Store(string dest, string src);

	//cac kenh dieu khien trinh tu
	int Jump(string src);
	int JumpIf(string dest, string src);
	int Call(string src);
	int Return();


	//run cpu
	void runCPU();
};


#endif