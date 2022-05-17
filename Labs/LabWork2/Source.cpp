
#include <iostream>
#include <sstream>
#include <string>
#include <windows.h>

// TODO: capacity?
class String
{
public:
	// TODO: проходили ли списки инициализации?
	String() 
	{
		size_ = 1;
		str_ = new char[1];
		str_[0] = '\0';
	}

	String(const char* str)
	{
		size_ = strlen(str) + 1; // потому что \0
		str_ = new char[size_];
		for(int i = 0; i < size_; i++)
		{
			str_[i] = str[i];
		}
	}

	String(const String& other)
	{
		size_ = other.size_;
		str_ = new char[size_];
		// TODO: повторение копирования
		for(int i = 0; i < size_; i++)
		{
			str_[i] = other.str_[i];
		}
	}

	String& operator=(const String& other)
	{
		// TODO: self-assignment check
		if(this == &other)
		{
			return *this;
		}

		if(size_ != other.size_)
		{
			delete[] str_;
			size_ = other.size_;
			str_ = new char[size_];
		}

		// TODO: повторение копирования
		for(int i = 0; i < size_; i++)
		{
			str_[i] = other.str_[i];
		}

		return *this;
	}

	String& operator=(const char* str)
	{
		delete[] str_;
		size_ = strlen(str) + 1;
		str_ = new char[size_];
		for(int i = 0; i < size_; i++)
		{
			str_[i] = str[i];
		}
		return *this;
	}

	String& operator+=(const char* str)
	{
		int str_len = strlen(str);
		int old_size = size_;
		size_ = size_ + str_len;
		char* new_str = new char[size_];
		for(int i = 0; i < old_size - 1; i++)
		{
			new_str[i] = str_[i];
		}
		delete[] str_;
		for(int i = 0; i < str_len; i++) 
		{
			new_str[old_size + i - 1] = str[i];
		}

		new_str[size_ - 1] = '\0';
		str_ = new_str;
		return *this;
	}

	String& operator+=(const String& str)
	{
		int old_size = size_;
		size_ = size_ + str.size_ - 1;
		char* new_str = new char[size_];
		for (int i = 0; i < old_size - 1; i++)
		{
			new_str[i] = str_[i];
		}
		delete[] str_;
		for (int i = 0; i < str.size_; i++)
		{
			new_str[old_size + i - 1] = str[i];
		}

		new_str[size_ - 1] = '\0';
		str_ = new_str;
		return *this;
	}

	int Size() const
	{
		return size_ - 1;
	}

	char* Data()
	{
		return str_;
	}

	const char* ConstData() const
	{
		return str_;
	}

	int operator[](int index) const
	{
		// TODO: обработка неправильного индекса?
		return str_[index];
	}

	~String()
	{
		delete[] str_;
	}

	friend bool operator==(const String& lhs, const String& rhs);
	friend std::ostream& operator<<(std::ostream& os, const String& str);
	friend std::istream& operator>>(std::istream& is, String& str);
private:
	char* str_;
	int size_;
};

std::ostream& operator<<(std::ostream& os, const String& str)
{
	os << str.ConstData();
	return os;
}

std::istream& operator>>(std::istream& is, String& str)
{
	std::string line;
	std::getline(std::cin, line);
	str = String(line.c_str());
	return is;
}

bool operator==(const String& lhs, const String& rhs)
{
	if (lhs.size_ != rhs.size_)
	{
		return false;
	}
	for (int i = 0; i < lhs.size_; i++)
	{
		if (lhs[i] != rhs[i])
		{
			return false;
		}
	}
	return true;
}

void showMenu()
{
	int option = 0;
	String a;
	String b;
	String optionalStr;
	do
	{
		std::cout << "Меню:\n";
		std::cout << "1. Ввести строку a.\n";
		std::cout << "2. Ввести строку b.\n";
		std::cout << "3. Проверить строки на равенство.\n";
		std::cout << "4. Присвоить строку a строке b.\n";
		std::cout << "5. Прибавить строку b к строке a.\n";
		std::cout << "6. Вывести размер строки a.\n";
		std::cout << "7. Выйти.\n";
		std::cout << "Строка a: " << a << std::endl;
		std::cout << "Строка b: " << b << std::endl;
		std::cout << "Выберите пункт: ";
		while (!(std::cin >> option)) {
			std::cin.clear();
			std::cin.ignore(32767, '\n');
			std::cout << "Выбрана неверная опция! Выберите пункт: ";
		}
		switch (option)
		{
		case 1:
			system("cls");
			std::cout << "Введите строку: ";
			std::cin.ignore(32767, '\n');
			std::cin >> a;
			continue;
		case 2:
			system("cls");
			std::cout << "Введите строку: ";
			std::cin.ignore(32767, '\n');
			std::cin >> b;
			continue;
		case 3:
			system("cls");
			std::cout << ((a == b) ? "Строки равны" : "Строки не равны") << std::endl;
			continue;
		case 4:
			system("cls");
			a = b;
			continue;
		case 5:
			system("cls");
			a += b;
			continue;
		case 6:
			system("cls");
			std::cout << "Размер строки a: " << a.Size() << std::endl;
			continue;
		}
	} while (option != 7);
}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	showMenu();
	return 0;
	//String a; // default ctor
	//std::cout << a.Size() << std::endl; // empty string size check
	//std::cout << a.Data() << std::endl; // empty string data access check
	//std::cout << a << std::endl; // empty string cout check
	//a = "test string a"; // operator=(const char*) check
	//std::cout << a << std::endl; // string cout check
	//String b("test string b"); // String(const char*) ctor check
	//std::cout << b << std::endl;
	//String c = a; // String(const String&) ctor check
	//std::cout << c << std::endl;
	//a = b; // operator=(const String&) check
	//std::cout << a << std::endl;
	//a += "appended"; // operator+=(const char*) check
	//std::cout << a << std::endl;
	//std::cout << a.Size() << std::endl;
	//a += b; // operator+=(const String&) check
	//std::cout << a << std::endl;
	//std::cout << a.Size() << std::endl;
	//a = "test";
	//b = "test";
	//std::cout << (a == b) << std::endl; // operator== check on equal
	//b = "test1";
	//std::cout << (a == b) << std::endl; // operator== check on inequal
	//String d;
	//std::cin >> d;
	//std::cout << d << std::endl;
}