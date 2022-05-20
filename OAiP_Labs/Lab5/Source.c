#include <Windows.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>

#include <iostream>
#include <iomanip>

const int MAX_STRING_LENGTH = 20;

double operand_array[201]; // ����� ����� ���� ����������� �� ���������������� �������� �� �������

int GetOperationPriority(char op)
{
	switch (op)
	{
	case '^': return 4;
	case '*': case '/': return 3;
	case '-': case '+': return 2;
	// case )?
	case '(': return 1;
	default: return 0;
	}
}

struct CharNode
{
	CharNode* next;
	char val;
};

struct CharStack
{
	CharNode* head;
};

bool IsExpressionCharInvalid(char c)
{
	return !(c >= 97 && c <= 101) // a - e
	&& c != '\r' && c != '\b' // enter + backspace
	&& !(c >= 40 && c <= 43) // '(', ')', '*', '+'
	&& c != 45 // '-'
	&& c != 47 // '/'
	&& c != 94; // '^'
}

void HandleExpressionInput(char* str)
{
	char c = ' ';
	int i = 0;

	while (true)
	{
		do
		{
			c = _getch();
		} while (IsExpressionCharInvalid(c));

		switch (c)
		{
		case '\r':
			if (i != 0)
			{
				str[i] = '\0';
				return;
			}
			continue;
		case '\b':
			if (i != 0)
			{
				std::cout << "\b \b";
				i--;
			}
			continue;
		default:
			if (i < MAX_STRING_LENGTH)
			{
				str[i] = c;
				i++;
				std::cout << c;
			}
		}
	}
}

void FreeStack(CharStack stack)
{
	CharNode* it = stack.head;
	while(it != NULL)
	{
		CharNode* temp_it = it->next;
		// TODO: delete - �� C++ 
		delete it;
		it = temp_it;
	}
}

void PushToStack(CharStack* stack, char ch)
{
	// TODO: new - �� C++
	CharNode* new_node = new CharNode;
	new_node->val = ch;
	new_node->next = stack->head;
	stack->head = new_node;
}

char PopStack(CharStack* stack)
{
	char to_return = stack->head->val;

	CharNode* head_node = stack->head;
	stack->head = stack->head->next;
	// TODO: delete
	delete head_node;

	return to_return;
}

bool IsStackEmpty(CharStack stack)
{
	return stack.head == NULL;
}

int GetIntFromConsole()
{
	int value = 0;
	while (!(std::cin >> value)) {
		std::cin.clear();
		std::cin.ignore(32767, '\n');
		std::cout << "������ ������������ ������. ������� �����: ";
	}
	return value;
}

double GetDoubleFromConsole()
{
	double value = 0;
	while (!(std::cin >> value)) {
		std::cin.clear();
		std::cin.ignore(32767, '\n');
		std::cout << "������ ������������ ������. ������� �����: ";
	}
	return value;
}

void GetOPZ(const char* infix_str, char* prefix_str)
{
	CharStack in_stack{};
	int infix_str_len = strlen(infix_str);
	int prefix_str_it = 0;
	for(int i = 0; i < infix_str_len; i++)
	{
		char ch = infix_str[i];
		switch (ch)
		{
		case '(':
			PushToStack(&in_stack, ch);
			break;
		case ')':
			while (/*!IsStackEmpty(in_stack) && */in_stack.head->val != '(')
			{
				char a = PopStack(&in_stack);
				prefix_str[prefix_str_it] = a;
				prefix_str_it++;
			}
			PopStack(&in_stack);
			break;
		case 'a': case 'b': case 'c': case 'd': case 'e':
			prefix_str[prefix_str_it] = ch;
			prefix_str_it++;
			break;
		case '+': case '-': case '*': case '/': case '^':
			while(!IsStackEmpty(in_stack) && GetOperationPriority(in_stack.head->val) >= GetOperationPriority(ch))
			{
				char a = PopStack(&in_stack);
				prefix_str[prefix_str_it] = a;
				prefix_str_it++;
			}
			PushToStack(&in_stack, ch);
			break;
		}
	}
	while(!IsStackEmpty(in_stack))
	{
		prefix_str[prefix_str_it] = PopStack(&in_stack);
		prefix_str_it++;
	}
	FreeStack(in_stack);
	prefix_str[prefix_str_it] = '\0';
}

double Count(const char* prefix_str)
{
	double sum = 0.;
	CharStack in_stack{};
	char oper1, oper2;
	double oper1_d, oper2_d;
	int str_len = strlen(prefix_str);
	char chr = 'z' + 1;
	for(int i = 0; i < str_len; i++)
	{
		char ch = prefix_str[i];
		switch (ch)
		{
		case '+': case '-': case '*': case '/': case '^':
			oper1 = PopStack(&in_stack);
			oper2 = PopStack(&in_stack);
			oper1_d = operand_array[int(oper1)];
			oper2_d = operand_array[int(oper2)];
			switch (ch)
			{
			case '+':
				sum = oper2_d + oper1_d;
				break;
			case '-':
				sum = oper2_d - oper1_d;
				break;
			case '*':
				sum = oper2_d * oper1_d;
				break;
			case '/':
				sum = oper2_d / oper1_d;
				break;
			case '^':
				sum = pow(oper2_d, oper1_d);
				break;
			}
			operand_array[int(chr)] = sum;
			PushToStack(&in_stack, chr);
			chr++;
			break;
		default: // �������
			PushToStack(&in_stack, ch);
			break;
		}
	}
	return sum;
}

void ShowMenu()
{
	int selected_index = -1;
	char expression[MAX_STRING_LENGTH];
	expression[0] = '\0';
	char OPZ_expression[MAX_STRING_LENGTH];
	OPZ_expression[0] = '\0';
	do
	{
		system("cls");
		std::cout << "����: \n";
		std::cout << "1. ������ ���������.\n";
		std::cout << "2. ������� ���������.\n";
		std::cout << "3. ������ ������������.\n";
		std::cout << "4. ������� ������������.\n";
		std::cout << "5. ������� ���.\n";
		std::cout << "6. ������� ��������� ���������� ���������.\n";
		std::cout << "0. �����.\n";
		std::cout << "�������� �����: ";

		selected_index = GetIntFromConsole();

		switch (selected_index)
		{
		case 1:
			std::cout << "������� ���������: ";
			HandleExpressionInput(expression);
			continue;
		case 2:
			if(strlen(expression) != 0)
			{
				std::cout << "������� ��������� � ��������� �����: " << expression << std::endl;
			} else
			{
				std::cout << "��������� �����" << std::endl;
			}
			system("pause");
			continue;
		case 3:
			std::cout << "������� ����������� a: ";
			operand_array[int('a')] = GetDoubleFromConsole();
			std::cout << "������� ����������� b: ";
			operand_array[int('b')] = GetDoubleFromConsole();
			std::cout << "������� ����������� c: ";
			operand_array[int('c')] = GetDoubleFromConsole();
			std::cout << "������� ����������� d: ";
			operand_array[int('d')] = GetDoubleFromConsole();
			std::cout << "������� ����������� e: ";
			operand_array[int('e')] = GetDoubleFromConsole();
			continue;
		case 4:
			std::cout << "����������� a: " << operand_array[int('a')] << std::endl;
			std::cout << "����������� b: " << operand_array[int('b')] << std::endl;
			std::cout << "����������� c: " << operand_array[int('c')] << std::endl;
			std::cout << "����������� d: " << operand_array[int('d')] << std::endl;
			std::cout << "����������� e: " << operand_array[int('e')] << std::endl;
			system("pause");
			continue;
		case 5:
			GetOPZ(expression, OPZ_expression);
			std::cout << "���������� �������� �������� ������: " << OPZ_expression << std::endl;
			system("pause");
			continue;
		case 6:
			GetOPZ(expression, OPZ_expression);
			std::cout << "��������� ���������: " << std::setprecision(2) << std::fixed << Count(OPZ_expression) << std::endl;
			system("pause");
			continue;
		}
		
	} while (selected_index != 0);
}

void main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	ShowMenu();

	return;
}