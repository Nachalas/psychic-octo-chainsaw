#include <iostream>
#include <cmath>
#include <iomanip>
#include <vector>
#include <string>
#include <array>
#include <Windows.h>

//struct Point2d
//{
//	double x;
//	double y;
//};

//struct Point3d
//{
//	double x;
//	double y;
//	double z;
//};

class Shape
{
public: 
	virtual void printData() = 0;
	virtual void area() = 0;
	virtual void volume() = 0;
	virtual ~Shape() {};
};

class TwoDShape : public Shape
{
	void volume() override {}
};

class ThreeDShape : public Shape
{
};

class Circle : public TwoDShape
{
public:
	Circle()
	{
		radius_ = 0;
	}

	Circle(double radius)
	{
		radius_ = radius;
	}

	Circle(const Circle& other)
	{
		radius_ = other.radius_;
	}

	double GetRadius()
	{
		return radius_;
	}

	void SetRadius(double radius)
	{
		if(radius > 0)
		{
			radius_ = radius;
		}
	}

	void printData() override
	{
		std::cout << "���������� � �������� " << std::setprecision(2) << std::fixed << radius_ << std::endl;
	}

	void area() override
	{
		std::cout << "������� ����������: " << 3.14 * radius_ * radius_ <<  std::endl;
	}
private:
	double radius_;
};

class RectangleCl : public TwoDShape
{
public:
	RectangleCl(double width, double height)
	{
		width_ = width;
		height_ = height;
	}

	RectangleCl()
	{
		width_ = 0;
		height_ = 0;
	}

	RectangleCl(const RectangleCl& other)
	{
		width_ = other.width_;
		height_ = other.height_;
	}

	double GetWidth()
	{
		return width_;
	}

	void SetWidth(double new_width)
	{
		if(new_width > 0)
		{
			width_ = new_width;
		}
	}

	double GetHeight()
	{
		return height_;
	}

	void SetHeight(double new_height)
	{
		if(new_height > 0)
		{
			height_ = new_height;
		}
	}

	void printData() override
	{
		std::cout << "������������� �� ��������� " << std::setprecision(2) << std::fixed << height_ << ", " << width_ << std::endl;
	}

	void area() override
	{
		std::cout << "������� ��������������: " << height_ * width_ << std::endl;
	}
private:
	double width_;
	double height_;
};

class Ring : public TwoDShape
{
public:
	Ring(double r1, double r2)
	{
		if (r1 > r2) std::swap(r1, r2);
		inner_radius_ = r1;
		outer_radius_ = r2;
	}

	Ring()
	{
		inner_radius_ = 0;
		outer_radius_ = 0;
	}

	Ring(const Ring& other)
	{
		inner_radius_ = other.inner_radius_;
		outer_radius_ = other.outer_radius_;
	}

	double GetOuterRadius()
	{
		return outer_radius_;
	}

	void SetOuterRadius(double r)
	{
		if(r > outer_radius_)
		{
			outer_radius_ = r;
		}
	}

	double GetInnerRadius()
	{
		return inner_radius_;
	}

	void SetInnerRadius(double r)
	{
		if(r > 0 && r < inner_radius_)
		{
			inner_radius_ = r;
		}
	}

	void printData() override
	{
		std::cout << "������ � ���������� �������� " << std::setprecision(2) << std::fixed << inner_radius_ << " � ������� �������� " << outer_radius_ << std::endl;
	}

	void area() override
	{
		std::cout << "������� ������: " << (3.14 * outer_radius_ * outer_radius_) - (3.14 * inner_radius_ * inner_radius_) << std::endl;
	}
private:
	double outer_radius_;
	double inner_radius_;
};

class Sphere : public ThreeDShape
{
public:
	Sphere(double radius)
	{
		radius_ = radius;
	}

	Sphere()
	{
		radius_ = 0;
	}

	Sphere(const Sphere& other)
	{
		radius_ = other.radius_;
	}

	double GetRadius()
	{
		return radius_;
	}

	void SetRadius(double radius)
	{
		if (radius > 0)
		{
			radius_ = radius;
		}
	}

	void printData() override
	{
		std::cout << "����� c �������� " << std::setprecision(2) << std::fixed << radius_ << std::endl;
	}

	void area() override
	{
		std::cout << "������� ����������� �����: " << 4 * 3.14 * radius_ * radius_ << std::endl;
	}

	void volume() override
	{
		std::cout << "����� �����: " << (4. / 3) * 3.14 * radius_ * radius_ * radius_ << std::endl;
	}
private:
	double radius_;
};

class Cylinder : public ThreeDShape
{
public:
	Cylinder(double radius, double height)
	{
		radius_ = radius;
		height_ = height;
	}

	Cylinder()
	{
		radius_ = 0;
		height_ = 0;
	}

	Cylinder(const Cylinder& other)
	{
		radius_ = other.radius_;
		height_ = other.height_;
	}

	double GetRadius()
	{
		return radius_;
	}

	void SetRadius(double radius)
	{
		if(radius > 0)
		{
			radius_ = radius;
		}
	}

	double GetHeight()
	{
		return height_;
	}

	void SetHeight(double height)
	{
		if (height > 0)
		{
			height_ = height;
		}
	}

	void printData() override
	{
		std::cout << "������� � �������� ��������� " << std::setprecision(2) << std::fixed << radius_ << " � ������� " << height_ << std::endl;
	}

	void area() override
	{
		std::cout << "������� ����������� ��������: " << 2 * 3.14 * radius_ * height_ << std::endl;
	}

	void volume() override
	{
		std::cout << "����� ��������: " << 3.14 * radius_ * radius_ * height_ << std::endl;
	}
private:
	double radius_;
	double height_;
};

class Cone : public ThreeDShape
{
public:
	Cone(double radius, double height)
	{
		radius_ = radius;
		height_ = height;
	}

	Cone()
	{
		radius_ = 0;
		height_ = 0;
	}

	Cone(const Cone& other)
	{
		radius_ = other.radius_;
		height_ = other.height_;
	}

	double GetRadius()
	{
		return radius_;
	}

	void SetRadius(double radius)
	{
		if (radius > 0)
		{
			radius_ = radius;
		}
	}

	double GetHeight()
	{
		return height_;
	}

	void SetHeight(double height)
	{
		if (height > 0)
		{
			height_ = height;
		}
	}

	void printData() override
	{
		std::cout << "����� � �������� ��������� " << std::setprecision(2) << std::fixed << radius_ << " � ������� " << height_ << std::endl;
	}

	void area() override
	{
		double generatrix_length = std::sqrt(radius_ * radius_ + height_ * height_);
		double side_area = 3.14 * radius_ * generatrix_length;
		double circle_area = 3.14 * radius_ * radius_;
		std::cout << "������� ����������� ������: " << side_area + circle_area << std::endl;
	}

	void volume() override
	{
		double circle_area = 3.14 * radius_ * radius_;
		std::cout << "����� ��������: " << (1. / 3) * height_ * circle_area << std::endl;
	}
private:
	double radius_;
	double height_;
};

int GetIntOptionFromConsole()
{
	int option = 0;
	while (!(std::cin >> option)) {
		std::cin.clear();
		std::cin.ignore(32767, '\n');
		std::cout << "�������� ����������. ������� �����: ";
	}
	return option;
}

double GetDoubleFromConsole()
{
	double val = 0.;
	while (!(std::cin >> val)) {
		std::cin.clear();
		std::cin.ignore(32767, '\n');
		std::cout << "�������� ����������. ������� ������� �����: ";
	}
	return val;
}

void OutputFigureTypesSelection()
{
	// circle rectangle ring sphere cylinder cone
	std::cout << "�������� ��� ������, ���: \n";
	std::cout << "1. ����.\n";
	std::cout << "2. �������������.\n";
	std::cout << "3. ������.\n";
	std::cout << "4. �����.\n";
	std::cout << "5. �������.\n";
	std::cout << "6. �����.\n";
}

Shape* GetDefaultConstructedShape(int option)
{
	switch (option)
	{
	case 1:
		return new Circle();
	case 2:
		return new RectangleCl();
	case 3:
		return new Ring();
	case 4:
		return new Sphere();
	case 5:
		return new Cylinder();
	case 6:
		return new Cone();
	default:
		return nullptr;
	}
}

Shape* GetShape(int option)
{
	switch (option)
	{
	case 1:
	{
		std::cout << "������� ������ �����: ";
		double radius = GetDoubleFromConsole();
		return new Circle(radius);
	}
	case 2:
	{
		std::cout << "������� ������ ��������������: ";
		double width = GetDoubleFromConsole();
		std::cout << "������� ������ ��������������: ";
		double height = GetDoubleFromConsole();
		return new RectangleCl(width, height);
	}
	case 3:
	{
		std::cout << "������� ���������� ������ ������: ";
		double r1 = GetDoubleFromConsole();
		std::cout << "������� ������� ������ ������: ";
		double r2 = GetDoubleFromConsole();
		return new Ring(r1, r2);
	}
		
	case 4:
	{
		std::cout << "������� ������ �����: ";
		double radius = GetDoubleFromConsole();
		return new Sphere(radius);
	}
	case 5:
	{
		std::cout << "������� ������ ��������� ��������: ";
		double radius = GetDoubleFromConsole();
		std::cout << "������� ������ ��������: ";
		double height = GetDoubleFromConsole();
		return new Cylinder(radius, height);
	}
	case 6:
	{
		std::cout << "������� ������ ��������� ������: ";
		double radius = GetDoubleFromConsole();
		std::cout << "������� ������ ������: ";
		double height = GetDoubleFromConsole();
		return new Cone(radius, height);
	}
	default:
		return nullptr;
	}
}

void showMenu()
{
	int option = -1;
	// circle rectangle ring sphere cylinder cone
	std::vector<Shape*> shapes = {
		//new Circle(),
		//new RectangleCl(),
		//new Ring(),
		//new Sphere(),
		//new Cylinder(),
		//new Cone(),
		new Circle(5.0),
		new RectangleCl(5.0, 4.0),
		new Ring(5.0, 7.0),
		new Sphere(5.0),
		new Cylinder(5.0, 7.0),
		new Cone(5.0, 7.0),
	};
	std::string filename;
	do
	{
		std::cout << "����:\n";
		std::cout << "1. ������� ������ � ������� � �������.\n";
		std::cout << "2. ������� ������ � ������� � ����.\n";
		std::cout << "3. �������� ����� ������, ������������������ �� ���������.\n";
		std::cout << "4. �������� ����� ������.\n";
		std::cout << "5. ������� ��������� ������.\n";
		std::cout << "6. �����.\n";
		std::cout << "�������� �����: ";
		option = GetIntOptionFromConsole();
		switch (option)
		{
		case 1:
			system("cls");
			for(int i = 0; i < shapes.size(); i++)
			{
				shapes[i]->printData();
				shapes[i]->area();
				shapes[i]->volume();
				std::cout << "---------" << std::endl;
			}
			continue;
		case 2:
			system("cls");
			std::cout << "������� �������� ��� �����: ";
			std::cin >> filename;
			// TODO: enter to file logic
			continue;
		case 3:
		{
			system("cls");
			OutputFigureTypesSelection();
			std::cout << "�������� �����: ";
			int figure_option = GetIntOptionFromConsole();
			Shape* new_shape = GetDefaultConstructedShape(figure_option);
			if (new_shape != nullptr)
			{
				shapes.push_back(new_shape);
			}
			else
			{
				std::cout << "������ �������������� �����." << std::endl;
			}
			continue;
		}
		case 4:
		{
			system("cls");
			OutputFigureTypesSelection();
			std::cout << "�������� �����: ";
			int figure_option = GetIntOptionFromConsole();
			Shape* new_shape = GetShape(figure_option);
			if (new_shape != nullptr)
			{
				shapes.push_back(new_shape);
			}
			else
			{
				std::cout << "������ �������������� �����." << std::endl;
			}
			continue;
		}
		case 5:
			system("cls");
			shapes.pop_back();
			continue;
		}
	} while (option != 6);
	for(int i = 0; i < shapes.size(); i++)
	{
		delete shapes[i];
	}
}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	showMenu();
	return 0;
}