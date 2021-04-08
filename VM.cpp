#include "VM.h"

void VM::run(string filename)
{	
	int size = countLine(filename);
	string* str = getLine(filename, size);
	if (invalidInstructions(str, size)) return;
	if (invalidOperand(str, size)) return;	
	CPU start;
	bool a=start.loadInstruction(str, size);
	if(a==1) start.runCPU();
	else return;
	return;
}

//count line of file name.
int VM::countLine(string filename) 
{
	int count = 0;
	ifstream file(filename);
	if (file.is_open())
	{
		string line;
		while (getline(file, line)) 
		{
			count += 1;
		}
		file.close();
	}
	return count;
}

//read and store each line
string* VM::getLine(string filename, int numLine)
{

	string* str = new string[numLine];
	ifstream file1(filename);
	if (file1.is_open())
	{
		int i = 0;
		string line;
		while (getline(file1, line)) {
			*(str + i) = line;
			i++;
		}
		file1.close();
	}
	return str;
}
//check invalidInstructions error
bool VM::invalidInstructions(string* str, int size)
{
	regex pattern("(Add|Minus|Mul|Div|CmpEQ|CmpNE|CmpLT|CmpLE|CmpGT|CmpGE|Not|And|Or|Move|Load|Store|Jump|JumpIf|Call|Return|Halt|Input|Output)(( [^ ,\a\b\t\r\n\f\v]+, [^ ,\a\b\t\r\n\f\v]+)|( [^ ,\a\b\t\r\n\f\v]+))?");
	for (int i = 0; i < size; i++)
		{
			if(!regex_match(*(str+i),pattern))
			{
				InvalidInstruction error(i);
				try {
					throw error;
				}
				catch (exception& ex)
				{
					cout<<ex.what();
				}
				return 1;
			}
		}
	return 0;
}


//check Operand error
bool VM::invalidOperand(string* str, int size)
{
	regex pattern("(Add|Minus|Mul|Div|CmpEQ|CmpNE|CmpLT|CmpLE|CmpGT|CmpGE|Not|And|Or|Move|Load|Store|Jump|JumpIf|Call|Return|Halt|Input|Output)(( ((R1[0-5]?)|(R[1-9])), (([0-9]+A?)|(-?[0-9]+)|((R1[0-5]?)|(R[1-9]))|(true|false)|((-)?[0-9]+.[0-9]+))|( ((R1[0-5]?)|(R[1-9])|(true|false)|(-?[0-9]+)|(-?[0-9]+.[0-9]+)|([0-9]+A?))))?)");      
	for (int i = 0; i < size; i++)
	{
		if (!regex_match(*(str + i), pattern))
		{
			InvalidOperand error(i);
			try {
				throw error;
			}
			catch (exception& ex)
			{
				cout << ex.what();
			}
			return 1;
		}
	}
	return 0;
}
//push x into stack
bool Stack::push(int x)
{
	if (top >= (999)) {
		return false;
	}
	else {
		a[++top] = x;
		return true;
	}
}
//pop value out of stack
int Stack::pop()
{
	if (top < 0) {
		return -1;
	}
	else {
		int x = a[top--];
		return x;
	}
}
//get top value of Stack
int Stack::peek()
{
	if (top < 0) {
		return 0;
	}
	else {
		int x = a[top];
		return x;
	}
}
//check empty stack
bool Stack::isEmpty()
{
	return (top < 0);
}
//Load Instruction and Store into code memory
bool CPU:: loadInstruction(string* str, int size)
{
	for (int i = 0; i < size; i++)
	{
		string instruction = *(str + i);
		int length = instruction.length();
		string tmp[3];
		int j = 0;
		while (instruction[j] != ' ' && j < length)
		{
			tmp[0] += instruction[j];
			j++;
		}
		j++;
		if (j < length) {
			while (instruction[j] != ',' && j < length)
			{
				tmp[1] += instruction[j];
				j++;
			}
			j += 2;
			while (j < length)
			{
				tmp[2] += instruction[j];
				j++;
			}
		}



		regex pattern1("(Add|Minus|Mul|Div|Move|Load|Store|JumpIf|CmpEQ|CmpNE|CmpLT|CmpLE|CmpGT|CmpGE|And|Or)");
		regex pattern2("Jump|Call|Not|Input|Output");
		if (regex_match(tmp[0],pattern1))
		{
			//check InvalidInstruction
			if (tmp[1] == "" || tmp[2] == "")
			{
				InvalidInstruction error(i);
				try {
					throw error;
				}
				catch (exception& ex)
				{
					cout << ex.what();
				}
				return 0;
			}
			else {
				codeMemory[codeLoadedIndex].code = tmp[0];
				codeMemory[codeLoadedIndex].op1 = tmp[1];
				codeMemory[codeLoadedIndex].op2 = tmp[2];
				codeLoadedIndex++;
			}
		}
		else if (regex_match(tmp[0],pattern2))
		{
			if (tmp[1] == "")
			{
				//check invalid instructions
				InvalidInstruction error(i);
				try {
					throw error;
				}
				catch (exception& ex)
				{
					cout << ex.what();
				}
				return 0;
			}
			else {
				codeMemory[codeLoadedIndex].code = tmp[0];
				codeMemory[codeLoadedIndex].op1 = tmp[1];
				codeLoadedIndex++;
			}
		}
		else // halt return
		{
			codeMemory[codeLoadedIndex].code = tmp[0];
			codeLoadedIndex++;
		}
	}
	return 1;
}


// check dot in source
bool CPU::checkDot(string source)
{
	int length = source.length();
	for (int i = 0; i < length; i++)
	{
		if (source[i] == '.') return 1;
	}
	return 0;
}


// check source is address
bool CPU::checkIsAddress(string source)
{
	int length = source.length();
	if (source[length - 1] == 'A') return 1;
	return 0;
}

//check source is register
bool CPU::checkIsReg(string source)
{
	if (source[0] == 'R') return 1;
	return 0;
}

//check source is bool
bool CPU::checkIsBool(string source)
{
	if (source == "true" || source == "false") return 1;
	return 0;
}

bool CPU::stringToBool(string source)
{
	if (source == "true") return 1;
	return 0;
}


//Move operator
void CPU::Move(string dest, string src)
{
	int idxD = stoi(dest.substr(1)) - 1;
	//check src is register
	if (checkIsReg(src))
	{
		int idxS = stoi(src.substr(1))-1;
		if (reg[idxS].type == INT)
		{
			reg[idxD].intValue = reg[idxS].intValue;
			reg[idxD].type = reg[idxS].type;
		}
		else if (reg[idxS].type == FLOAT)
		{
			reg[idxD].floatValue = reg[idxS].floatValue;
			reg[idxD].type = reg[idxS].type;
		}
		else if (reg[idxS].type == BOOL)
		{
			reg[idxD].boolValue = reg[idxS].boolValue;
			reg[idxD].type = reg[idxS].type;
		}
		else if (reg[idxS].type == ADDRESS)
		{
			reg[idxD].Address = reg[idxS].Address;
			reg[idxD].type = reg[idxS].type;
		}
		IP++;
		return;
	}
	//check src is float
	else if(checkDot(src)){
		reg[idxD].floatValue = stof(src);
		reg[idxD].type = FLOAT;
		IP++;
	}
	//check src is address
	else if (checkIsAddress(src))
	{
		int address = stoi(src.substr(0, src.length() - 1));
		reg[idxD].type = ADDRESS;
		reg[idxD].Address = address;
		IP++;
	}
	else if (checkIsBool(src))
	{
		reg[idxD].type = BOOL;
		reg[idxD].boolValue = stringToBool(src);
		IP++;
	}
	else {
		int source = stoi(src);
		reg[idxD].intValue = source;
		reg[idxD].type = INT;
		IP++;
	}
}



// Add operator
bool CPU::Add(string dest, string src)
{
	int idxD = stoi(dest.substr(1)) - 1;
	if (reg[idxD].type == ADDRESS || reg[idxD].type == BOOL || reg[idxD].type == NONE)
	{
		TypeMismatch error(IP);
		try {
			throw error;
		}
		catch (exception& ex)
		{
			cout << ex.what();
		}
		return 0;
	}
	if (checkIsReg(src))
	{
		int idxS = stoi(src.substr(1)) - 1;
		//check typeMismatch
			if(reg[idxD].type == ADDRESS || reg[idxD].type == BOOL || reg[idxD].type == NONE)
			{
				TypeMismatch error(IP);
				try {
					throw error;
				}
				catch (exception& ex)
				{
					cout << ex.what();
				}
				return 0;
			}
			else if (reg[idxS].type == ADDRESS || reg[idxS].type == BOOL || reg[idxS].type == NONE)
			{
				TypeMismatch error(IP);
				try {
					throw error;
				}
				catch (exception& ex)
				{
					cout << ex.what();
				}
				return 0;
			}
			// dest and source match data type
			else if (reg[idxD].type == reg[idxS].type)
			{
				if (reg[idxD].type == INT)
				{
					reg[idxD].intValue += reg[idxS].intValue;
					IP++;
				}
				else
				{
					reg[idxD].floatValue += reg[idxS].floatValue;
					IP++;
				}
			}
			else //dest and source have different data type
			{
				if (reg[idxD].type == INT)
				{
					reg[idxD].floatValue = float(reg[idxD].intValue)+reg[idxS].floatValue;
					reg[idxD].type = FLOAT;
					IP++;
				}
				else
				{
					reg[idxD].floatValue += float(reg[idxS].intValue);
					IP++;
				}
			}
	}
	// case src is float literal
	else if (checkDot(src))
	{
		if (reg[idxD].type == FLOAT)
		{
			reg[idxD].floatValue += stof(src);
			IP++;
		}
		else if (reg[idxD].type == INT)
		{
			reg[idxD].floatValue = float(reg[idxD].intValue) + stof(src);
			reg[idxD].type = FLOAT;
			IP++;
		}
	}
	else if (checkIsAddress(src))
	{
		TypeMismatch error(IP);
		try {
			throw error;
		}
		catch (exception& ex)
		{
			cout << ex.what();
		}
		return 0;
	}
	else if (checkIsBool(src))
	{
		TypeMismatch error(IP);
		try {
			throw error;
		}
		catch (exception& ex)
		{
			cout << ex.what();
		}
		return 0;
	}
	else 
	{
		if (reg[idxD].type == INT)
		{
			reg[idxD].intValue += stoi(src);
			IP++;
		}
		else
		{
			reg[idxD].floatValue += stof(src);
			IP++;
		}
	}
	return 1;
}

bool CPU::Minus(string dest, string src)
{
	int idxD = stoi(dest.substr(1)) - 1;
	if (reg[idxD].type == ADDRESS || reg[idxD].type == BOOL || reg[idxD].type == NONE)
	{
		TypeMismatch error(IP);
		try {
			throw error;
		}
		catch (exception& ex)
		{
			cout << ex.what();
		}
		return 0;
	}
	if (checkIsReg(src))
	{
		int idxS = stoi(src.substr(1)) - 1;
		//check typeMismatch
		if (reg[idxD].type == ADDRESS || reg[idxD].type == BOOL || reg[idxD].type == NONE)
		{
			TypeMismatch error(IP);
			try {
				throw error;
			}
			catch (exception& ex)
			{
				cout << ex.what();
			}
			return 0;
		}
		else if (reg[idxS].type == ADDRESS || reg[idxS].type == BOOL || reg[idxS].type == NONE)
		{
			TypeMismatch error(IP);
			try {
				throw error;
			}
			catch (exception& ex)
			{
				cout << ex.what();
			}
			return 0;
		}
		// dest and source match data type
		else if (reg[idxD].type == reg[idxS].type)
		{
			if (reg[idxD].type == INT)
			{
				reg[idxD].intValue -= reg[idxS].intValue;
				IP++;
			}
			else
			{
				reg[idxD].floatValue -= reg[idxS].floatValue;
				IP++;
			}
		}
		else //dest and source have different data type
		{
			if (reg[idxD].type == INT)
			{
				reg[idxD].floatValue = float(reg[idxD].intValue) - reg[idxS].floatValue;
				reg[idxD].type = FLOAT;
				IP++;
			}
			else
			{
				reg[idxD].floatValue -= float(reg[idxS].intValue);
				IP++;
			}
		}
	}
	// case src is float literal
	else if (checkDot(src))
	{
		if (reg[idxD].type == FLOAT)
		{
			reg[idxD].floatValue -= stof(src);
			IP++;
		}
		else if (reg[idxD].type == INT)
		{
			reg[idxD].floatValue = float(reg[idxD].intValue) - stof(src);
			reg[idxD].type = FLOAT;
			IP++;
		}
	}
	else if (checkIsAddress(src))
	{
		TypeMismatch error(IP);
		try {
			throw error;
		}
		catch (exception& ex)
		{
			cout << ex.what();
		}
		return 0;
	}
	else if (checkIsBool(src))
	{
		TypeMismatch error(IP);
		try {
			throw error;
		}
		catch (exception& ex)
		{
			cout << ex.what();
		}
		return 0;
	}
	else
	{
		if (reg[idxD].type == INT)  //dest type int, src int literal
		{
			reg[idxD].intValue -= stoi(src);
			IP++;
		}
		else //dest type float, src int literal 
		{
			reg[idxD].floatValue -= stof(src);
			IP++;
		}
	}
	return 1;
}

bool CPU::Mul(string dest, string src)
{
	int idxD = stoi(dest.substr(1)) - 1;
	if (reg[idxD].type == ADDRESS || reg[idxD].type == BOOL || reg[idxD].type == NONE)
	{
		TypeMismatch error(IP);
		try {
			throw error;
		}
		catch (exception& ex)
		{
			cout << ex.what();
		}
		return 0;
	}
	if (checkIsReg(src))
	{
		int idxS = stoi(src.substr(1)) - 1;
		//check typeMismatch
		if (reg[idxD].type == ADDRESS || reg[idxD].type == BOOL || reg[idxD].type == NONE)
		{
			TypeMismatch error(IP);
			try {
				throw error;
			}
			catch (exception& ex)
			{
				cout << ex.what();
			}
			return 0;
		}
		else if (reg[idxS].type == ADDRESS || reg[idxS].type == BOOL || reg[idxS].type == NONE)
		{
			TypeMismatch error(IP);
			try {
				throw error;
			}
			catch (exception& ex)
			{
				cout << ex.what();
			}
			return 0;
		}
		// dest and source match data type
		else if (reg[idxD].type == reg[idxS].type)
		{
			if (reg[idxD].type == INT)
			{
				reg[idxD].intValue *= reg[idxS].intValue;
				IP++;
			}
			else
			{
				reg[idxD].floatValue *= reg[idxS].floatValue;
				IP++;
			}
		}
		else //dest and source have different data type
		{
			if (reg[idxD].type == INT)
			{
				reg[idxD].floatValue = float(reg[idxD].intValue) * reg[idxS].floatValue;
				reg[idxD].type = FLOAT;
				IP++;
			}
			else
			{
				reg[idxD].floatValue *= float(reg[idxS].intValue);
				IP++;
			}
		}
	}
	// case src is float literal
	else if (checkDot(src))
	{
		if (reg[idxD].type == FLOAT)
		{
			reg[idxD].floatValue *= stof(src);
			IP++;
		}
		else if (reg[idxD].type == INT)
		{
			reg[idxD].floatValue = float(reg[idxD].intValue) * stof(src);
			reg[idxD].type = FLOAT;
			IP++;
		}
	}
	else if (checkIsAddress(src))
	{
		TypeMismatch error(IP);
		try {
			throw error;
		}
		catch (exception& ex)
		{
			cout << ex.what();
		}
		return 0;
	}
	else if (checkIsBool(src))
	{
		TypeMismatch error(IP);
		try {
			throw error;
		}
		catch (exception& ex)
		{
			cout << ex.what();
		}
		return 0;
	}
	else
	{
		if (reg[idxD].type == INT)  //dest type int, src int literal
		{
			reg[idxD].intValue *= stoi(src);
			IP++;
		}
		else //dest type float, src int literal 
		{
			reg[idxD].floatValue *= stof(src);
			IP++;
		}
	}
	return 1;
}

bool CPU::Div(string dest, string src)
{
	int idxD = stoi(dest.substr(1)) - 1;
	if (reg[idxD].type == ADDRESS || reg[idxD].type == BOOL || reg[idxD].type == NONE)
	{
		TypeMismatch error(IP);
		try {
			throw error;
		}
		catch (exception& ex)
		{
			cout << ex.what();
		}
		return 0;
	}
	if (checkIsReg(src))
	{
		int idxS = stoi(src.substr(1)) - 1;
		//check typeMismatch
		if (reg[idxD].type == ADDRESS || reg[idxD].type == BOOL || reg[idxD].type == NONE)
		{
			TypeMismatch error(IP);
			try {
				throw error;
			}
			catch (exception& ex)
			{
				cout << ex.what();
			}
			return 0;
		}
		else if (reg[idxS].type == ADDRESS || reg[idxS].type == BOOL || reg[idxS].type == NONE)
		{
			TypeMismatch error(IP);
			try {
				throw error;
			}
			catch (exception& ex)
			{
				cout << ex.what();
			}
			return 0;
		}
		//check devide by zero
		else if (reg[idxS].type==INT) 
		{
			if (reg[idxS].intValue == 0)
			{
				DivideByZero error(IP);
				try {
					throw error;
				}
				catch (exception& ex)
				{
					cout << ex.what();
				}
				return 0;
			}
		}
		else if (reg[idxS].type == FLOAT)
		{
			if (reg[idxS].floatValue == float(0))
			{
				DivideByZero error(IP);
				try {
					throw error;
				}
				catch (exception& ex)
				{
					cout << ex.what();
				}
				return 0;
			}
		}
		// dest and source match data type
		else if (reg[idxD].type == reg[idxS].type)
		{
			if (reg[idxD].type == INT)
			{
				reg[idxD].intValue /= reg[idxS].intValue;
				IP++;
			}
			else
			{
				reg[idxD].floatValue /= reg[idxS].floatValue;
				IP++;
			}
		}
		else //dest and source have different data type
		{
			if (reg[idxD].type == INT)
			{
				reg[idxD].floatValue = float(reg[idxD].intValue) / reg[idxS].floatValue;
				reg[idxD].type = FLOAT;
				IP++;
			}
			else
			{
				reg[idxD].floatValue /= float(reg[idxS].intValue);
				IP++;
			}
		}
	}
	// case src is float literal
	else if (checkDot(src))
	{
		if (stof(src) == float(0))
		{
			DivideByZero error(IP);
			try {
				throw error;
			}
			catch (exception& ex)
			{
				cout << ex.what();
			}
			return 0;
		}
		else {
			if (reg[idxD].type == FLOAT)
			{

				reg[idxD].floatValue /= stof(src);
				IP++;
			}
			else if (reg[idxD].type == INT)
			{
				reg[idxD].floatValue = float(reg[idxD].intValue) / stof(src);
				reg[idxD].type = FLOAT;
				IP++;
			}
		}
	}
	else if (checkIsAddress(src))
	{
		TypeMismatch error(IP);
		try {
			throw error;
		}
		catch (exception& ex)
		{
			cout << ex.what();
		}
		return 0;
	}
	else if (checkIsBool(src))
	{
		TypeMismatch error(IP);
		try {
			throw error;
		}
		catch (exception& ex)
		{
			cout << ex.what();
		}
		return 0;
	}
	else
	{
		if (stoi(src) == 0)
		{
			DivideByZero error(IP);
			try {
				throw error;
			}
			catch (exception& ex)
			{
				cout << ex.what();
			}
			return 0;
		}
		else {
			if (reg[idxD].type == INT)  //dest type int, src int literal
			{
				reg[idxD].intValue /= stoi(src);
				IP++;
			}
			else //dest type float, src int literal 
			{
				reg[idxD].floatValue /= stof(src);
				IP++;
			}
		}
	}
	return 1;
}
//nhom lenh so sanh
bool CPU::CmpEQ(string dest, string src)
{
	int idxD = stoi(dest.substr(1)) - 1;
	//check dest type mis match
	if (reg[idxD].type == ADDRESS || reg[idxD].type == NONE)
	{
		TypeMismatch error(IP);
		try {
			throw error;
		}
		catch (exception& ex)
		{
			cout << ex.what();
		}
		return 0;
	}
	if (checkIsReg(src))
	{
		int idxS = stoi(src.substr(1)) - 1;
		//check typeMismatch
		if (reg[idxS].type == ADDRESS || reg[idxS].type == NONE)
		{
			TypeMismatch error(IP);
			try {
				throw error;
			}
			catch (exception& ex)
			{
				cout << ex.what();
			}
			return 0;
		}
		//dest is bool, src not bool => TypeMismatch
		else if (reg[idxD].type==BOOL && reg[idxS].type!=BOOL)
		{
			TypeMismatch error(IP);
			try {
				throw error;
			}
			catch (exception& ex)
			{
				cout << ex.what();
			}
			return 0;
		}
		 //dest not bool, src is bool=> Typemismatch
		else if(reg[idxD].type!=BOOL && reg[idxS].type==BOOL)
		{
			TypeMismatch error(IP);
			try {
				throw error;
			}
			catch (exception& ex)
			{
				cout << ex.what();
			}
			return 0;
		}
		// dest and source match data type
		else if (reg[idxD].type == reg[idxS].type)
		{
			// type dest INT, type src INT
			if (reg[idxD].type == INT)
			{
				if (reg[idxD].intValue == reg[idxS].intValue)
				{
					reg[idxD].boolValue = true;
					reg[idxD].type = BOOL;
				}
				else
				{
					reg[idxD].boolValue = false;
					reg[idxD].type = BOOL;
				}
				IP++;
			}
			//type dest FLOAT, type src FLOAT
			else if(reg[idxD].type == FLOAT)
			{
					if (reg[idxD].floatValue == reg[idxS].floatValue)
					{
						reg[idxD].boolValue = true;
						reg[idxD].type = BOOL;
					}
					else
					{
						reg[idxD].boolValue = false;
						reg[idxD].type = BOOL;
					}
				
				IP++;
			}
			//type dest,src is BOOL
			else
			{
				if (reg[idxD].boolValue == reg[idxS].boolValue)
				{
					reg[idxD].boolValue = true;
				}
				else reg[idxD].boolValue = false;
				IP++;
			}
		}
		else //dest and source have different data type
		{
			//dest INT, src FLOAT
			if (reg[idxD].type == INT)
			{
				if (float(reg[idxD].intValue) == reg[idxS].floatValue)
				{
					reg[idxD].boolValue = true;
					reg[idxD].type = BOOL;
				}
				else
				{
					reg[idxD].boolValue = false;
					reg[idxD].type = BOOL;
				}
				IP++;
			}
			else //dest FLOAT, src INT
			{
				if (reg[idxD].floatValue == float(reg[idxS].intValue))
				{
					reg[idxD].boolValue = true;
					reg[idxD].type = BOOL;
				}
				else
				{
					reg[idxD].boolValue = false;
					reg[idxD].type = BOOL;
				}
				IP++;
			}
		}
	}
	//check float literal
	else if (checkDot(src))
	{
		// dest is FLOAT
		if (reg[idxD].type == FLOAT)
		{
			if (reg[idxD].floatValue == stof(src))
			{
				reg[idxD].boolValue = true;
				reg[idxD].type = BOOL;
			}
			else
			{
				reg[idxD].boolValue = false;
				reg[idxD].type = BOOL;
			}
			IP++;
		}
		//dest is INT
		else if (reg[idxD].type == INT)
		{
			if (float(reg[idxD].intValue) == stof(src))
			{
				reg[idxD].boolValue = true;
				reg[idxD].type = BOOL;
			}
			else
			{
				reg[idxD].boolValue = false;
				reg[idxD].type = BOOL;
			}
			IP++;
		}
		else //dest is BOOL
		{
			TypeMismatch error(IP);
			try {
				throw error;
			}
			catch (exception& ex)
			{
				cout << ex.what();
			}
			return 0;
		}
	}
	else if (checkIsAddress(src))
	{
		TypeMismatch error(IP);
		try {
			throw error;
		}
		catch (exception& ex)
		{
			cout << ex.what();
		}
		return 0;
	}
	else if (checkIsBool(src))
	{
		if (reg[idxD].type == BOOL)
		{
			if (reg[idxD].boolValue == stringToBool(src))
			{
				reg[idxD].boolValue = true;
			}
			else
			{
				reg[idxD].boolValue = false;
			}
			IP++;
		}
		else
		{
			TypeMismatch error(IP);
			try {
				throw error;
			}
			catch (exception& ex)
			{
				cout << ex.what();
			}
			return 0;
		}
	}
	else //src is int literal
	{
		if (reg[idxD].type == INT)  //dest type int, src int literal
		{
			if (reg[idxD].intValue == stoi(src))
			{
				reg[idxD].boolValue = true;
				reg[idxD].type = BOOL;
			}
			else
			{
				reg[idxD].boolValue = false;
				reg[idxD].type = BOOL;
			}
			IP++;
		}
		else if(reg[idxD].type==FLOAT) //dest type float, src int literal 
		{
			if (reg[idxD].floatValue == stof(src))
			{
				reg[idxD].boolValue = true;
				reg[idxD].type = BOOL;
			}
			else
			{
				reg[idxD].boolValue = false;
				reg[idxD].type = BOOL;
			}
			IP++;
		}
		else //dest bool, src int literal
		{
			TypeMismatch error(IP);
			try {
				throw error;
			}
			catch (exception& ex)
			{
				cout << ex.what();
			}
			return 0;
		}
	}			
	return 1;
}

bool CPU::CmpNE(string dest, string src)
{
	int idxD = stoi(dest.substr(1)) - 1;
	//check dest type mis match
	if (reg[idxD].type == ADDRESS || reg[idxD].type == NONE)
	{
		TypeMismatch error(IP);
		try {
			throw error;
		}
		catch (exception& ex)
		{
			cout << ex.what();
		}
		return 0;
	}
	if (checkIsReg(src))
	{
		int idxS = stoi(src.substr(1)) - 1;
		//check typeMismatch
		if (reg[idxS].type == ADDRESS || reg[idxS].type == NONE)
		{
			TypeMismatch error(IP);
			try {
				throw error;
			}
			catch (exception& ex)
			{
				cout << ex.what();
			}
			return 0;
		}
		//dest is bool, src not bool => TypeMismatch
		else if (reg[idxD].type == BOOL && reg[idxS].type != BOOL)
		{
			TypeMismatch error(IP);
			try {
				throw error;
			}
			catch (exception& ex)
			{
				cout << ex.what();
			}
			return 0;
		}
		//dest not bool, src is bool=> Typemismatch
		else if (reg[idxD].type != BOOL && reg[idxS].type == BOOL)
		{
			TypeMismatch error(IP);
			try {
				throw error;
			}
			catch (exception& ex)
			{
				cout << ex.what();
			}
			return 0;
		}
		// dest and source match data type
		else if (reg[idxD].type == reg[idxS].type)
		{
			// type dest INT, type src INT
			if (reg[idxD].type == INT)
			{
				if (reg[idxD].intValue != reg[idxS].intValue)
				{
					reg[idxD].boolValue = true;
					reg[idxD].type = BOOL;
				}
				else
				{
					reg[idxD].boolValue = false;
					reg[idxD].type = BOOL;
				}
				IP++;
			}
			//type dest FLOAT, type src FLOAT
			else if (reg[idxD].type == FLOAT)
			{
				if (reg[idxD].floatValue != reg[idxS].floatValue)
				{
					reg[idxD].boolValue = true;
					reg[idxD].type = BOOL;
				}
				else
				{
					reg[idxD].boolValue = false;
					reg[idxD].type = BOOL;
				}

				IP++;
			}
			//type dest,src is BOOL
			else
			{
				if (reg[idxD].boolValue != reg[idxS].boolValue)
				{
					reg[idxD].boolValue = true;
				}
				else reg[idxD].boolValue = false;
				IP++;
			}
		}
		else //dest and source have different data type
		{
			//dest INT, src FLOAT
			if (reg[idxD].type == INT)
			{
				if (float(reg[idxD].intValue) != reg[idxS].floatValue)
				{
					reg[idxD].boolValue = true;
					reg[idxD].type = BOOL;
				}
				else
				{
					reg[idxD].boolValue = false;
					reg[idxD].type = BOOL;
				}
				IP++;
			}
			else //dest FLOAT, src INT
			{
				if (reg[idxD].floatValue != float(reg[idxS].intValue))
				{
					reg[idxD].boolValue = true;
					reg[idxD].type = BOOL;
				}
				else
				{
					reg[idxD].boolValue = false;
					reg[idxD].type = BOOL;
				}
				IP++;
			}
		}
	}
	//check float literal
	else if (checkDot(src))
	{
		// dest is FLOAT
		if (reg[idxD].type == FLOAT)
		{
			if (reg[idxD].floatValue != stof(src))
			{
				reg[idxD].boolValue = true;
				reg[idxD].type = BOOL;
			}
			else
			{
				reg[idxD].boolValue = false;
				reg[idxD].type = BOOL;
			}
			IP++;
		}
		//dest is INT
		else if (reg[idxD].type == INT)
		{
			if (float(reg[idxD].intValue) != stof(src))
			{
				reg[idxD].boolValue = true;
				reg[idxD].type = BOOL;
			}
			else
			{
				reg[idxD].boolValue = false;
				reg[idxD].type = BOOL;
			}
			IP++;
		}
		else //dest is BOOL
		{
			TypeMismatch error(IP);
			try {
				throw error;
			}
			catch (exception& ex)
			{
				cout << ex.what();
			}
			return 0;
		}
	}
	else if (checkIsAddress(src))
	{
		TypeMismatch error(IP);
		try {
			throw error;
		}
		catch (exception& ex)
		{
			cout << ex.what();
		}
		return 0;
	}
	else if (checkIsBool(src))
	{
		if (reg[idxD].type == BOOL)
		{
			if (reg[idxD].boolValue != stringToBool(src))
			{
				reg[idxD].boolValue = true;
			}
			else
			{
				reg[idxD].boolValue = false;
			}
			IP++;
		}
		else
		{
			TypeMismatch error(IP);
			try {
				throw error;
			}
			catch (exception& ex)
			{
				cout << ex.what();
			}
			return 0;
		}
	}
	else //src is int literal
	{
		if (reg[idxD].type == INT)  //dest type int, src int literal
		{
			if (reg[idxD].intValue != stoi(src))
			{
				reg[idxD].boolValue = true;
				reg[idxD].type = BOOL;
			}
			else
			{
				reg[idxD].boolValue = false;
				reg[idxD].type = BOOL;
			}
			IP++;
		}
		else if (reg[idxD].type == FLOAT) //dest type float, src int literal 
		{
			if (reg[idxD].floatValue != stof(src))
			{
				reg[idxD].boolValue = true;
				reg[idxD].type = BOOL;
			}
			else
			{
				reg[idxD].boolValue = false;
				reg[idxD].type = BOOL;
			}
			IP++;
		}
		else //dest bool, src int literal
		{
			TypeMismatch error(IP);
			try {
				throw error;
			}
			catch (exception& ex)
			{
				cout << ex.what();
			}
			return 0;
		}
	}
	return 1;
}

bool CPU::CmpLT(string dest, string src)
{
	int idxD = stoi(dest.substr(1)) - 1;
	//check dest type mis match
	if (reg[idxD].type == ADDRESS || reg[idxD].type == NONE)
	{
		TypeMismatch error(IP);
		try {
			throw error;
		}
		catch (exception& ex)
		{
			cout << ex.what();
		}
		return 0;
	}
	if (checkIsReg(src))
	{
		int idxS = stoi(src.substr(1)) - 1;
		//check typeMismatch
		if (reg[idxS].type == ADDRESS || reg[idxS].type == NONE)
		{
			TypeMismatch error(IP);
			try {
				throw error;
			}
			catch (exception& ex)
			{
				cout << ex.what();
			}
			return 0;
		}
		//dest is bool, src not bool => TypeMismatch
		else if (reg[idxD].type == BOOL && reg[idxS].type != BOOL)
		{
			TypeMismatch error(IP);
			try {
				throw error;
			}
			catch (exception& ex)
			{
				cout << ex.what();
			}
			return 0;
		}
		//dest not bool, src is bool=> Typemismatch
		else if (reg[idxD].type != BOOL && reg[idxS].type == BOOL)
		{
			TypeMismatch error(IP);
			try {
				throw error;
			}
			catch (exception& ex)
			{
				cout << ex.what();
			}
			return 0;
		}
		// dest and source match data type
		else if (reg[idxD].type == reg[idxS].type)
		{
			// type dest INT, type src INT
			if (reg[idxD].type == INT)
			{
				if (reg[idxD].intValue < reg[idxS].intValue)
				{
					reg[idxD].boolValue = true;
					reg[idxD].type = BOOL;
				}
				else
				{
					reg[idxD].boolValue = false;
					reg[idxD].type = BOOL;
				}
				IP++;
			}
			//type dest FLOAT, type src FLOAT
			else if (reg[idxD].type == FLOAT)
			{
				if (reg[idxD].floatValue < reg[idxS].floatValue)
				{
					reg[idxD].boolValue = true;
					reg[idxD].type = BOOL;
				}
				else
				{
					reg[idxD].boolValue = false;
					reg[idxD].type = BOOL;
				}

				IP++;
			}
			//type dest,src is BOOL
			else
			{
				if (reg[idxD].boolValue < reg[idxS].boolValue)
				{
					reg[idxD].boolValue = true;
				}
				else reg[idxD].boolValue = false;
				IP++;
			}
		}
		else //dest and source have different data type
		{
			//dest INT, src FLOAT
			if (reg[idxD].type == INT)
			{
				if (float(reg[idxD].intValue) < reg[idxS].floatValue)
				{
					reg[idxD].boolValue = true;
					reg[idxD].type = BOOL;
				}
				else
				{
					reg[idxD].boolValue = false;
					reg[idxD].type = BOOL;
				}
				IP++;
			}
			else //dest FLOAT, src INT
			{
				if (reg[idxD].floatValue < float(reg[idxS].intValue))
				{
					reg[idxD].boolValue = true;
					reg[idxD].type = BOOL;
				}
				else
				{
					reg[idxD].boolValue = false;
					reg[idxD].type = BOOL;
				}
				IP++;
			}
		}
	}
	//check float literal
	else if (checkDot(src))
	{
		// dest is FLOAT
		if (reg[idxD].type == FLOAT)
		{
			if (reg[idxD].floatValue < stof(src))
			{
				reg[idxD].boolValue = true;
				reg[idxD].type = BOOL;
			}
			else
			{
				reg[idxD].boolValue = false;
				reg[idxD].type = BOOL;
			}
			IP++;
		}
		//dest is INT
		else if (reg[idxD].type == INT)
		{
			if (float(reg[idxD].intValue) < stof(src))
			{
				reg[idxD].boolValue = true;
				reg[idxD].type = BOOL;
			}
			else
			{
				reg[idxD].boolValue = false;
				reg[idxD].type = BOOL;
			}
			IP++;
		}
		else //dest is BOOL
		{
			TypeMismatch error(IP);
			try {
				throw error;
			}
			catch (exception& ex)
			{
				cout << ex.what();
			}
			return 0;
		}
	}
	else if (checkIsAddress(src))
	{
		TypeMismatch error(IP);
		try {
			throw error;
		}
		catch (exception& ex)
		{
			cout << ex.what();
		}
		return 0;
	}
	else if (checkIsBool(src))
	{
		if (reg[idxD].type == BOOL)
		{
			if (reg[idxD].boolValue < stringToBool(src))
			{
				reg[idxD].boolValue = true;
			}
			else
			{
				reg[idxD].boolValue = false;
			}
			IP++;
		}
		else
		{
			TypeMismatch error(IP);
			try {
				throw error;
			}
			catch (exception& ex)
			{
				cout << ex.what();
			}
			return 0;
		}
	}
	else //src is int literal
	{
		if (reg[idxD].type == INT)  //dest type int, src int literal
		{
			if (reg[idxD].intValue < stoi(src))
			{
				reg[idxD].boolValue = true;
				reg[idxD].type = BOOL;
			}
			else
			{
				reg[idxD].boolValue = false;
				reg[idxD].type = BOOL;
			}
			IP++;
		}
		else if (reg[idxD].type == FLOAT) //dest type float, src int literal 
		{
			if (reg[idxD].floatValue < stof(src))
			{
				reg[idxD].boolValue = true;
				reg[idxD].type = BOOL;
			}
			else
			{
				reg[idxD].boolValue = false;
				reg[idxD].type = BOOL;
			}
			IP++;
		}
		else //dest bool, src int literal
		{
			TypeMismatch error(IP);
			try {
				throw error;
			}
			catch (exception& ex)
			{
				cout << ex.what();
			}
			return 0;
		}
	}
	return 1;
}

bool CPU::CmpLE(string dest, string src)
{
	int idxD = stoi(dest.substr(1)) - 1;
	//check dest type mis match
	if (reg[idxD].type == ADDRESS || reg[idxD].type == NONE)
	{
		TypeMismatch error(IP);
		try {
			throw error;
		}
		catch (exception& ex)
		{
			cout << ex.what();
		}
		return 0;
	}
	if (checkIsReg(src))
	{
		int idxS = stoi(src.substr(1)) - 1;
		//check typeMismatch
		if (reg[idxS].type == ADDRESS || reg[idxS].type == NONE)
		{
			TypeMismatch error(IP);
			try {
				throw error;
			}
			catch (exception& ex)
			{
				cout << ex.what();
			}
			return 0;
		}
		//dest is bool, src not bool => TypeMismatch
		else if (reg[idxD].type == BOOL && reg[idxS].type != BOOL)
		{
			TypeMismatch error(IP);
			try {
				throw error;
			}
			catch (exception& ex)
			{
				cout << ex.what();
			}
			return 0;
		}
		//dest not bool, src is bool=> Typemismatch
		else if (reg[idxD].type != BOOL && reg[idxS].type == BOOL)
		{
			TypeMismatch error(IP);
			try {
				throw error;
			}
			catch (exception& ex)
			{
				cout << ex.what();
			}
			return 0;
		}
		// dest and source match data type
		else if (reg[idxD].type == reg[idxS].type)
		{
			// type dest INT, type src INT
			if (reg[idxD].type == INT)
			{
				if (reg[idxD].intValue <= reg[idxS].intValue)
				{
					reg[idxD].boolValue = true;
					reg[idxD].type = BOOL;
				}
				else
				{
					reg[idxD].boolValue = false;
					reg[idxD].type = BOOL;
				}
				IP++;
			}
			//type dest FLOAT, type src FLOAT
			else if (reg[idxD].type == FLOAT)
			{
				if (reg[idxD].floatValue <= reg[idxS].floatValue)
				{
					reg[idxD].boolValue = true;
					reg[idxD].type = BOOL;
				}
				else
				{
					reg[idxD].boolValue = false;
					reg[idxD].type = BOOL;
				}

				IP++;
			}
			//type dest,src is BOOL
			else
			{
				if (reg[idxD].boolValue <= reg[idxS].boolValue)
				{
					reg[idxD].boolValue = true;
				}
				else reg[idxD].boolValue = false;
				IP++;
			}
		}
		else //dest and source have different data type
		{
			//dest INT, src FLOAT
			if (reg[idxD].type == INT)
			{
				if (float(reg[idxD].intValue) <= reg[idxS].floatValue)
				{
					reg[idxD].boolValue = true;
					reg[idxD].type = BOOL;
				}
				else
				{
					reg[idxD].boolValue = false;
					reg[idxD].type = BOOL;
				}
				IP++;
			}
			else //dest FLOAT, src INT
			{
				if (reg[idxD].floatValue <= float(reg[idxS].intValue))
				{
					reg[idxD].boolValue = true;
					reg[idxD].type = BOOL;
				}
				else
				{
					reg[idxD].boolValue = false;
					reg[idxD].type = BOOL;
				}
				IP++;
			}
		}
	}
	//check float literal
	else if (checkDot(src))
	{
		// dest is FLOAT
		if (reg[idxD].type == FLOAT)
		{
			if (reg[idxD].floatValue <= stof(src))
			{
				reg[idxD].boolValue = true;
				reg[idxD].type = BOOL;
			}
			else
			{
				reg[idxD].boolValue = false;
				reg[idxD].type = BOOL;
			}
			IP++;
		}
		//dest is INT
		else if (reg[idxD].type == INT)
		{
			if (float(reg[idxD].intValue) <= stof(src))
			{
				reg[idxD].boolValue = true;
				reg[idxD].type = BOOL;
			}
			else
			{
				reg[idxD].boolValue = false;
				reg[idxD].type = BOOL;
			}
			IP++;
		}
		else //dest is BOOL
		{
			TypeMismatch error(IP);
			try {
				throw error;
			}
			catch (exception& ex)
			{
				cout << ex.what();
			}
			return 0;
		}
	}
	else if (checkIsAddress(src))
	{
		TypeMismatch error(IP);
		try {
			throw error;
		}
		catch (exception& ex)
		{
			cout << ex.what();
		}
		return 0;
	}
	else if (checkIsBool(src))
	{
		if (reg[idxD].type == BOOL)
		{
			if (reg[idxD].boolValue <= stringToBool(src))
			{
				reg[idxD].boolValue = true;
			}
			else
			{
				reg[idxD].boolValue = false;
			}
			IP++;
		}
		else
		{
			TypeMismatch error(IP);
			try {
				throw error;
			}
			catch (exception& ex)
			{
				cout << ex.what();
			}
			return 0;
		}
	}
	else //src is int literal
	{
		if (reg[idxD].type == INT)  //dest type int, src int literal
		{
			if (reg[idxD].intValue <= stoi(src))
			{
				reg[idxD].boolValue = true;
				reg[idxD].type = BOOL;
			}
			else
			{
				reg[idxD].boolValue = false;
				reg[idxD].type = BOOL;
			}
			IP++;
		}
		else if (reg[idxD].type == FLOAT) //dest type float, src int literal 
		{
			if (reg[idxD].floatValue <= stof(src))
			{
				reg[idxD].boolValue = true;
				reg[idxD].type = BOOL;
			}
			else
			{
				reg[idxD].boolValue = false;
				reg[idxD].type = BOOL;
			}
			IP++;
		}
		else //dest bool, src int literal
		{
			TypeMismatch error(IP);
			try {
				throw error;
			}
			catch (exception& ex)
			{
				cout << ex.what();
			}
			return 0;
		}
	}
	return 1;
}

bool CPU::CmpGT(string dest, string src)
{
	int idxD = stoi(dest.substr(1)) - 1;
	//check dest type mis match
	if (reg[idxD].type == ADDRESS || reg[idxD].type == NONE)
	{
		TypeMismatch error(IP);
		try {
			throw error;
		}
		catch (exception& ex)
		{
			cout << ex.what();
		}
		return 0;
	}
	if (checkIsReg(src))
	{
		int idxS = stoi(src.substr(1)) - 1;
		//check typeMismatch
		if (reg[idxS].type == ADDRESS || reg[idxS].type == NONE)
		{
			TypeMismatch error(IP);
			try {
				throw error;
			}
			catch (exception& ex)
			{
				cout << ex.what();
			}
			return 0;
		}
		//dest is bool, src not bool => TypeMismatch
		else if (reg[idxD].type == BOOL && reg[idxS].type != BOOL)
		{
			TypeMismatch error(IP);
			try {
				throw error;
			}
			catch (exception& ex)
			{
				cout << ex.what();
			}
			return 0;
		}
		//dest not bool, src is bool=> Typemismatch
		else if (reg[idxD].type != BOOL && reg[idxS].type == BOOL)
		{
			TypeMismatch error(IP);
			try {
				throw error;
			}
			catch (exception& ex)
			{
				cout << ex.what();
			}
			return 0;
		}
		// dest and source match data type
		else if (reg[idxD].type == reg[idxS].type)
		{
			// type dest INT, type src INT
			if (reg[idxD].type == INT)
			{
				if (reg[idxD].intValue > reg[idxS].intValue)
				{
					reg[idxD].boolValue = true;
					reg[idxD].type = BOOL;
				}
				else
				{
					reg[idxD].boolValue = false;
					reg[idxD].type = BOOL;
				}
				IP++;
			}
			//type dest FLOAT, type src FLOAT
			else if (reg[idxD].type == FLOAT)
			{
				if (reg[idxD].floatValue > reg[idxS].floatValue)
				{
					reg[idxD].boolValue = true;
					reg[idxD].type = BOOL;
				}
				else
				{
					reg[idxD].boolValue = false;
					reg[idxD].type = BOOL;
				}

				IP++;
			}
			//type dest,src is BOOL
			else
			{
				if (reg[idxD].boolValue > reg[idxS].boolValue)
				{
					reg[idxD].boolValue = true;
				}
				else reg[idxD].boolValue = false;
				IP++;
			}
		}
		else //dest and source have different data type
		{
			//dest INT, src FLOAT
			if (reg[idxD].type == INT)
			{
				if (float(reg[idxD].intValue) > reg[idxS].floatValue)
				{
					reg[idxD].boolValue = true;
					reg[idxD].type = BOOL;
				}
				else
				{
					reg[idxD].boolValue = false;
					reg[idxD].type = BOOL;
				}
				IP++;
			}
			else //dest FLOAT, src INT
			{
				if (reg[idxD].floatValue > float(reg[idxS].intValue))
				{
					reg[idxD].boolValue = true;
					reg[idxD].type = BOOL;
				}
				else
				{
					reg[idxD].boolValue = false;
					reg[idxD].type = BOOL;
				}
				IP++;
			}
		}
	}
	//check float literal
	else if (checkDot(src))
	{
		// dest is FLOAT
		if (reg[idxD].type == FLOAT)
		{
			if (reg[idxD].floatValue > stof(src))
			{
				reg[idxD].boolValue = true;
				reg[idxD].type = BOOL;
			}
			else
			{
				reg[idxD].boolValue = false;
				reg[idxD].type = BOOL;
			}
			IP++;
		}
		//dest is INT
		else if (reg[idxD].type == INT)
		{
			if (float(reg[idxD].intValue) > stof(src))
			{
				reg[idxD].boolValue = true;
				reg[idxD].type = BOOL;
			}
			else
			{
				reg[idxD].boolValue = false;
				reg[idxD].type = BOOL;
			}
			IP++;
		}
		else //dest is BOOL
		{
			TypeMismatch error(IP);
			try {
				throw error;
			}
			catch (exception& ex)
			{
				cout << ex.what();
			}
			return 0;
		}
	}
	else if (checkIsAddress(src))
	{
		TypeMismatch error(IP);
		try {
			throw error;
		}
		catch (exception& ex)
		{
			cout << ex.what();
		}
		return 0;
	}
	else if (checkIsBool(src))
	{
		if (reg[idxD].type == BOOL)
		{
			if (reg[idxD].boolValue > stringToBool(src))
			{
				reg[idxD].boolValue = true;
			}
			else
			{
				reg[idxD].boolValue = false;
			}
			IP++;
		}
		else
		{
			TypeMismatch error(IP);
			try {
				throw error;
			}
			catch (exception& ex)
			{
				cout << ex.what();
			}
			return 0;
		}
	}
	else //src is int literal
	{
		if (reg[idxD].type == INT)  //dest type int, src int literal
		{
			if (reg[idxD].intValue > stoi(src))
			{
				reg[idxD].boolValue = true;
				reg[idxD].type = BOOL;
			}
			else
			{
				reg[idxD].boolValue = false;
				reg[idxD].type = BOOL;
			}
			IP++;
		}
		else if (reg[idxD].type == FLOAT) //dest type float, src int literal 
		{
			if (reg[idxD].floatValue > stof(src))
			{
				reg[idxD].boolValue = true;
				reg[idxD].type = BOOL;
			}
			else
			{
				reg[idxD].boolValue = false;
				reg[idxD].type = BOOL;
			}
			IP++;
		}
		else //dest bool, src int literal
		{
			TypeMismatch error(IP);
			try {
				throw error;
			}
			catch (exception& ex)
			{
				cout << ex.what();
			}
			return 0;
		}
	}
	return 1;
}

bool CPU::CmpGE(string dest, string src)
{
	int idxD = stoi(dest.substr(1)) - 1;
	//check dest type mis match
	if (reg[idxD].type == ADDRESS || reg[idxD].type == NONE)
	{
		TypeMismatch error(IP);
		try {
			throw error;
		}
		catch (exception& ex)
		{
			cout << ex.what();
		}
		return 0;
	}
	if (checkIsReg(src))
	{
		int idxS = stoi(src.substr(1)) - 1;
		//check typeMismatch
		if (reg[idxS].type == ADDRESS || reg[idxS].type == NONE)
		{
			TypeMismatch error(IP);
			try {
				throw error;
			}
			catch (exception& ex)
			{
				cout << ex.what();
			}
			return 0;
		}
		//dest is bool, src not bool => TypeMismatch
		else if (reg[idxD].type == BOOL && reg[idxS].type != BOOL)
		{
			TypeMismatch error(IP);
			try {
				throw error;
			}
			catch (exception& ex)
			{
				cout << ex.what();
			}
			return 0;
		}
		//dest not bool, src is bool=> Typemismatch
		else if (reg[idxD].type != BOOL && reg[idxS].type == BOOL)
		{
			TypeMismatch error(IP);
			try {
				throw error;
			}
			catch (exception& ex)
			{
				cout << ex.what();
			}
			return 0;
		}
		// dest and source match data type
		else if (reg[idxD].type == reg[idxS].type)
		{
			// type dest INT, type src INT
			if (reg[idxD].type == INT)
			{
				if (reg[idxD].intValue >= reg[idxS].intValue)
				{
					reg[idxD].boolValue = true;
					reg[idxD].type = BOOL;
				}
				else
				{
					reg[idxD].boolValue = false;
					reg[idxD].type = BOOL;
				}
				IP++;
			}
			//type dest FLOAT, type src FLOAT
			else if (reg[idxD].type == FLOAT)
			{
				if (reg[idxD].floatValue >= reg[idxS].floatValue)
				{
					reg[idxD].boolValue = true;
					reg[idxD].type = BOOL;
				}
				else
				{
					reg[idxD].boolValue = false;
					reg[idxD].type = BOOL;
				}

				IP++;
			}
			//type dest,src is BOOL
			else
			{
				if (reg[idxD].boolValue >= reg[idxS].boolValue)
				{
					reg[idxD].boolValue = true;
				}
				else reg[idxD].boolValue = false;
				IP++;
			}
		}
		else //dest and source have different data type
		{
			//dest INT, src FLOAT
			if (reg[idxD].type == INT)
			{
				if (float(reg[idxD].intValue) >= reg[idxS].floatValue)
				{
					reg[idxD].boolValue = true;
					reg[idxD].type = BOOL;
				}
				else
				{
					reg[idxD].boolValue = false;
					reg[idxD].type = BOOL;
				}
				IP++;
			}
			else //dest FLOAT, src INT
			{
				if (reg[idxD].floatValue >= float(reg[idxS].intValue))
				{
					reg[idxD].boolValue = true;
					reg[idxD].type = BOOL;
				}
				else
				{
					reg[idxD].boolValue = false;
					reg[idxD].type = BOOL;
				}
				IP++;
			}
		}
	}
	//check float literal
	else if (checkDot(src))
	{
		// dest is FLOAT
		if (reg[idxD].type == FLOAT)
		{
			if (reg[idxD].floatValue >= stof(src))
			{
				reg[idxD].boolValue = true;
				reg[idxD].type = BOOL;
			}
			else
			{
				reg[idxD].boolValue = false;
				reg[idxD].type = BOOL;
			}
			IP++;
		}
		//dest is INT
		else if (reg[idxD].type == INT)
		{
			if (float(reg[idxD].intValue) >= stof(src))
			{
				reg[idxD].boolValue = true;
				reg[idxD].type = BOOL;
			}
			else
			{
				reg[idxD].boolValue = false;
				reg[idxD].type = BOOL;
			}
			IP++;
		}
		else //dest is BOOL
		{
			TypeMismatch error(IP);
			try {
				throw error;
			}
			catch (exception& ex)
			{
				cout << ex.what();
			}
			return 0;
		}
	}
	else if (checkIsAddress(src))
	{
		TypeMismatch error(IP);
		try {
			throw error;
		}
		catch (exception& ex)
		{
			cout << ex.what();
		}
		return 0;
	}
	else if (checkIsBool(src))
	{
		if (reg[idxD].type == BOOL)
		{
			if (reg[idxD].boolValue >= stringToBool(src))
			{
				reg[idxD].boolValue = true;
			}
			else
			{
				reg[idxD].boolValue = false;
			}
			IP++;
		}
		else
		{
			TypeMismatch error(IP);
			try {
				throw error;
			}
			catch (exception& ex)
			{
				cout << ex.what();
			}
			return 0;
		}
	}
	else //src is int literal
	{
		if (reg[idxD].type == INT)  //dest type int, src int literal
		{
			if (reg[idxD].intValue >= stoi(src))
			{
				reg[idxD].boolValue = true;
				reg[idxD].type = BOOL;
			}
			else
			{
				reg[idxD].boolValue = false;
				reg[idxD].type = BOOL;
			}
			IP++;
		}
		else if (reg[idxD].type == FLOAT) //dest type float, src int literal 
		{
			if (reg[idxD].floatValue >= stof(src))
			{
				reg[idxD].boolValue = true;
				reg[idxD].type = BOOL;
			}
			else
			{
				reg[idxD].boolValue = false;
				reg[idxD].type = BOOL;
			}
			IP++;
		}
		else //dest bool, src int literal
		{
			TypeMismatch error(IP);
			try {
				throw error;
			}
			catch (exception& ex)
			{
				cout << ex.what();
			}
			return 0;
		}
	}
	return 1;
}

//nhom lenh luan ly
bool CPU::Not(string dest)
{
	if (checkIsReg(dest))
	{
		int idxD = stoi(dest.substr(1)) - 1;
		if (reg[idxD].type != BOOL) //check type of reg
		{
			TypeMismatch error(IP);
			try {
				throw error;
			}
			catch (exception& ex)
			{
				cout << ex.what();
			}
			return 0;
		}
		else //type of reg is BOOL
		{
			if (reg[idxD].boolValue == 1)
			{
				reg[idxD].boolValue = 0;
			}
			else reg[idxD].boolValue = 1;
			IP++;
		}
	}
	else
	{
		InvalidOperand error(IP);
		try {
			throw error;
		}
		catch (exception& ex)
		{
			cout << ex.what();
		}
		return 0;
	}
	
	return 1;
}
bool CPU::And(string dest, string src)
{
	int idxD = stoi(dest.substr(1)) - 1;
	if (reg[idxD].type != BOOL)
	{
		TypeMismatch error(IP);
		try {
			throw error;
		}
		catch (exception& ex)
		{
			cout << ex.what();
		}
		return 0;
	}
	if (checkIsReg(src))
	{
		int idxS = stoi(src.substr(1)) - 1;
		// src is not BOOL
		if (reg[idxS].type != BOOL)
		{
			TypeMismatch error(IP);
			try {
				throw error;
			}
			catch (exception& ex)
			{
				cout << ex.what();
			}
			return 0;
		}
		else //src is BOOL, dest BOOL
		{
			if (reg[idxD].boolValue == 1 && reg[idxS].boolValue == 1)
			{
				reg[idxD].boolValue = 1;
			}
			else
			{
				reg[idxD].boolValue = 0;
			}
		}
	}
	else if (checkIsBool(src)) //src is true or false
	{
		if (reg[idxD].boolValue == 1 && stringToBool(src) == 1)
		{
			reg[idxD].boolValue = 1;
		}
		else
		{
			reg[idxD].boolValue = 0;
		}
	}
	else
	{
		TypeMismatch error(IP);
		try {
			throw error;
		}
		catch (exception& ex)
		{
			cout << ex.what();
		}
		return 0;
	}
	IP++;
	return 1;
}
bool CPU::Or(string dest, string src)
{
	int idxD = stoi(dest.substr(1)) - 1;
	if (reg[idxD].type != BOOL)
	{
		TypeMismatch error(IP);
		try {
			throw error;
		}
		catch (exception& ex)
		{
			cout << ex.what();
		}
		return 0;
	}
	if (checkIsReg(src))
	{
		int idxS = stoi(src.substr(1)) - 1;
		// src is not BOOL
		if (reg[idxS].type != BOOL)
		{
			TypeMismatch error(IP);
			try {
				throw error;
			}
			catch (exception& ex)
			{
				cout << ex.what();
			}
			return 0;
		}
		else //src is BOOL, dest BOOL
		{
			if (reg[idxD].boolValue == 0 && reg[idxS].boolValue == 0)
			{
				reg[idxD].boolValue = 0;
			}
			else
			{
				reg[idxD].boolValue = 1;
			}
		}
	}
	else if (checkIsBool(src)) //src is true or false
	{
		if (reg[idxD].boolValue == 0 && stringToBool(src) == 0)
		{
			reg[idxD].boolValue = 0;
		}
		else
		{
			reg[idxD].boolValue = 1;
		}
	}
	else
	{
		TypeMismatch error(IP);
		try {
			throw error;
		}
		catch (exception& ex)
		{
			cout << ex.what();
		}
		return 0;
	}
	IP++;
	return 1;
}

//nhom lenh doc/ghi du lieu
bool CPU::Load(string dest, string src)
{ 
	int idxD = stoi(dest.substr(1)) - 1;
	if (checkIsReg(src))
	{
		int idxS = stoi(src.substr(1)) - 1;
		if (reg[idxS].type != ADDRESS) //src is not ADDRESS
		{
			TypeMismatch error(IP);
			try {
				throw error;
			}
			catch (exception& ex)
			{
				cout << ex.what();
			}
			return 0;
		}
		else //src is address
		{
			int idxA = reg[idxS].Address;
			reg[idxD].type = staticMemory[idxA].type;
			if (staticMemory[idxA].type == BOOL) //static memory is BOOL
			{
				reg[idxD].boolValue = staticMemory[idxA].boolValue;
			}
			else if (staticMemory[idxA].type == INT) //static memory is INT
			{
				reg[idxD].intValue = staticMemory[idxA].intValue;
			}
			else if (staticMemory[idxA].type == FLOAT)
			{
				reg[idxD].floatValue = staticMemory[idxA].floatValue;
			}
		}
	}
	else if (checkIsAddress(src)) //src is not register and src type address, ex: 0A
	{
		int idxA = stoi(src.substr(0, src.length() - 1));
		reg[idxD].type = staticMemory[idxD].type;
		if (staticMemory[idxA].type == BOOL) //static memory is BOOL
		{
			reg[idxD].boolValue = staticMemory[idxA].boolValue;
		}
		else if (staticMemory[idxA].type == INT) //static memory is INT
		{
			reg[idxD].intValue = staticMemory[idxA].intValue;
		}
		else if (staticMemory[idxA].type == FLOAT)
		{
			reg[idxD].floatValue = staticMemory[idxA].floatValue;
		}
	}
	IP++;
	return 1;
}
bool CPU::Store(string dest, string src)
{
	int idxD = stoi(dest.substr(1)) - 1;
	if (reg[idxD].type != ADDRESS)
	{
		TypeMismatch error(IP);
		try {
			throw error;
		}
		catch (exception& ex)
		{
			cout << ex.what();
		}
		return 0;
	}
	int idxA = reg[idxD].Address;
	if (checkIsReg(src))  //dest type is ADDRESS, src is register
	{
		int idxS = stoi(src.substr(1)) - 1;
		staticMemory[idxA].type = reg[idxS].type;
		if (reg[idxS].type == INT)  //src type is INT
		{
			staticMemory[idxA].intValue = reg[idxS].intValue;
		}
		else if (reg[idxS].type == FLOAT) //src type is FLOAT
		{
			staticMemory[idxA].floatValue = reg[idxS].floatValue;
		}
		else if (reg[idxS].type == BOOL) //src type is BOOL
		{
			staticMemory[idxA].boolValue = reg[idxS].boolValue;
		}
	}
	else if (checkDot(src)) //src is float interal
	{
		staticMemory[idxA].type = FLOAT;
		staticMemory[idxA].floatValue = stof(src);
	}
	else if (checkIsBool(src))
	{
		staticMemory[idxA].type = BOOL;
		staticMemory[idxA].boolValue = stringToBool(src);
	}
	else if (checkIsAddress(src))
	{
		TypeMismatch error(IP);
		try {
			throw error;
		}
		catch (exception& ex)
		{
			cout << ex.what();
		}
		return 0;
	}
	else
	{
		staticMemory[idxA].type = INT;
		staticMemory[idxA].intValue = stoi(src);
	}
	IP++;
	return 1;
}

int CPU::Jump(string src)
{
	if (checkIsAddress(src))
	{
		int idx = stoi(src.substr(0, src.length() - 1));
		if (idx >= codeLoadedIndex || idx<0)
		{
			InvalidDestination error(IP);
			try {
				throw error;
			}
			catch (exception& ex)
			{
				cout << ex.what();
			}
			return -1;
		}
		IP++;
		return idx;
	}
	else
	{
		TypeMismatch error(IP);
		try {
			throw error;
		}
		catch (exception& ex)
		{
			cout << ex.what();
		}
		return -1;
	}
}
int CPU::JumpIf(string dest, string src)
{
	int idxD = stoi(dest.substr(1)) - 1;
	if (reg[idxD].type != BOOL)
	{
		TypeMismatch error(IP);
		try {
			throw error;
		}
		catch (exception& ex)
		{
			cout << ex.what();
		}
		return -1;
	}
	else
	{
		if (checkIsAddress(src))
		{
			int address = stoi(src.substr(0, src.length() - 1));
			if (reg[idxD].boolValue == false)
			{
				IP++;
				return -2;
			}
			else
			{
				if (address >= codeLoadedIndex || address < 0)
				{
					InvalidDestination error(IP);
					try {
						throw error;
					}
					catch (exception& ex)
					{
						cout << ex.what();
					}
					return -1;
				}
				else
				{
					IP++;
					return address;
				}
			}
		}
		else
		{
			TypeMismatch error(IP);
			try {
				throw error;
			}
			catch (exception& ex)
			{
				cout << ex.what();
			}
			return -1;
		}
	}
}

int CPU::Call(string src)
{
	if (checkIsAddress(src))
	{
		int address = stoi(src.substr(0, src.length() - 1));
		if (address >= codeLoadedIndex || address < 0)
		{
			InvalidDestination error(IP);
			try {
				throw error;
			}
			catch (exception& ex)
			{
				cout << ex.what();
			}
			return -1;
		}
		IP++;
		if (!stack.push(IP))
		{
			StackFull error(IP);
			try {
				throw error;
			}
			catch (exception& ex)
			{
				cout << ex.what();
			}
			return -1;
		}
		return address;
	}
	else
	{
		TypeMismatch error(IP);
		try {
			throw error;
		}
		catch (exception& ex)
		{
			cout << ex.what();
		}
		return -1;
	}
}

int CPU::Return()
{
	IP++;
	return stack.pop();
}

// Output/input operator
bool CPU::Input(string dest)
{
	if (checkIsReg(dest))
	{
		int idxD = stoi(dest.substr(1)) - 1;
		string input;
		cin >> input;
		regex pattern("((-?[0-9]+)|(-?[0-9]+.[0-9]+)|(true|false)|[0-9]+A?)");
		if (regex_match(input, pattern))
		{
			if (checkDot(input))
			{
				reg[idxD].type = FLOAT;
				reg[idxD].floatValue = stof(input);
			}
			else if(checkIsAddress(input))
			{
				reg[idxD].type = ADDRESS;
				reg[idxD].Address = stoi(input.substr(0, input.length() - 1));

			}
			else if (checkIsBool(input))
			{
				reg[idxD].type = BOOL;
				reg[idxD].boolValue = stringToBool(input);
			}
			else
			{
				reg[idxD].type = INT;
				reg[idxD].intValue = stoi(input);
			}
		}
		else
		{
			InvalidInput error(IP);
			try {
				throw error;
			}
			catch (exception& ex)
			{
				cout << ex.what();
			}
			return 0;
		}
	}
	else
	{
		InvalidOperand error(IP);
		try {
			throw error;
		}
		catch (exception& ex)
		{
			cout << ex.what();
		}
		return 0;
	}
	IP++;
	return 1;
}
void CPU::Output(string dest)
{

	IP++;
	if (checkIsReg(dest))
	{
		int idx = stoi(dest.substr(1)) - 1;
		if (reg[idx].type == INT) cout << reg[idx].intValue;
		else if (reg[idx].type == FLOAT) cout << reg[idx].floatValue;
		else if (reg[idx].type == BOOL) cout << reg[idx].boolValue;
		else if (reg[idx].type == ADDRESS) cout << reg[idx].Address;
	}
	else if (checkIsAddress(dest))
	{
		cout << dest.substr(0, dest.length() - 1);
	}
	else
	{
		cout << dest;
	}
}


void CPU::runCPU()
{
	for (int i = 0; i < codeLoadedIndex; i++)
	{
		if (codeMemory[i].code == "Move")
		{
			Move(codeMemory[i].op1, codeMemory[i].op2);
		}
		else if (codeMemory[i].code == "Output")
		{
			Output(codeMemory[i].op1);
		}
		else if (codeMemory[i].code == "Add")
		{
			if (!Add(codeMemory[i].op1, codeMemory[i].op2)) return;
		}
		else if (codeMemory[i].code == "Minus")
		{
			if (!Minus(codeMemory[i].op1, codeMemory[i].op2)) return;
		}
		else if (codeMemory[i].code == "Mul")
		{
			if (!Mul(codeMemory[i].op1, codeMemory[i].op2)) return;
		}
		else if (codeMemory[i].code == "Div")
		{
			if (!Div(codeMemory[i].op1, codeMemory[i].op2)) return;
		}
		else if (codeMemory[i].code == "CmpEQ")
		{
			if (!CmpEQ(codeMemory[i].op1, codeMemory[i].op2)) return;
		}
		else if (codeMemory[i].code == "CmpNE")
		{
			if (!CmpNE(codeMemory[i].op1, codeMemory[i].op2)) return;
		}
		else if (codeMemory[i].code == "CmpLT")
		{
			if (!CmpLT(codeMemory[i].op1, codeMemory[i].op2)) return;
		}
		else if (codeMemory[i].code == "CmpLE")
		{
			if (!CmpLE(codeMemory[i].op1, codeMemory[i].op2)) return;
		}
		else if (codeMemory[i].code == "CmpGT")
		{
			if (!CmpGT(codeMemory[i].op1, codeMemory[i].op2)) return;
		}
		else if (codeMemory[i].code == "CmpGE")
		{
			if (!CmpGE(codeMemory[i].op1, codeMemory[i].op2)) return;
		}
		else if (codeMemory[i].code == "Not")
		{
			if (!Not(codeMemory[i].op1)) return;
		}
		else if (codeMemory[i].code == "And")
		{
			if (!And(codeMemory[i].op1, codeMemory[i].op2)) return;
		}
		else if (codeMemory[i].code == "Or")
		{
			if (!Or(codeMemory[i].op1, codeMemory[i].op2)) return;
		}
		else if (codeMemory[i].code == "Load")
		{
			if (!Load(codeMemory[i].op1, codeMemory[i].op2)) return;
		}
		else if (codeMemory[i].code == "Store")
		{
			if (!Store(codeMemory[i].op1, codeMemory[i].op2)) return;
		}
		else if (codeMemory[i].code == "Jump")
		{
			int tmp = Jump(codeMemory[i].op1);
			if (tmp == -1) return;
			else
			{
				i = tmp-1;
				IP = i +1;
			}
		}
		else if (codeMemory[i].code == "JumpIf")
		{
			int tmp = JumpIf(codeMemory[i].op1, codeMemory[i].op2);
			if (tmp > -1)
			{
				i = tmp - 1;
				IP = i + 1;
			}
			else if (tmp == -1) return;
		}
		else if (codeMemory[i].code == "Call")
		{
			int tmp = Call(codeMemory[i].op1);
			if (tmp == -1) return;
			else
			{
				i = tmp - 1;
				IP = i + 1;
			}
		}
		else if (codeMemory[i].code == "Return")
		{
			int tmp = Return();
			if (tmp == -1) return;
			else {
				i = tmp - 1;
				IP = i + 1;
			}
			
		}
		else if (codeMemory[i].code == "Input")
		{
			if (!Input(codeMemory[i].op1)) return;
		}
		else if (codeMemory[i].code == "Halt") return;
		
	}
}


