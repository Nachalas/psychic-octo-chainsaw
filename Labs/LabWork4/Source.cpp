#include <iostream>
#include <cmath>
#include <iomanip>
#include <vector>
#include <string>
#include <array>
#include <fstream>
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
	virtual void printData(std::ostream&) = 0;
	virtual void area(std::ostream&) = 0;
	virtual void volume(std::ostream&) = 0;
	virtual ~Shape() {}
};

class TwoDShape : public Shape
{
	void volume(std::ostream&) override {}
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

	void printData(std::ostream& os) override
	{
		os << "Окружность с радиусом " << std::setprecision(2) << std::fixed << radius_ << std::endl;
	}

	void area(std::ostream& os) override
	{
		os << "Площадь окружности: " << 3.14 * radius_ * radius_ <<  std::endl;
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

	void printData(std::ostream& os) override
	{
		os << "Прямоугольник со сторонами " << std::setprecision(2) << std::fixed << height_ << ", " << width_ << std::endl;
	}

	void area(std::ostream& os) override
	{
		os << "Площадь прямоугольника: " << height_ * width_ << std::endl;
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

	void printData(std::ostream& os) override
	{
		os << "Кольцо с внутренним радиусом " << std::setprecision(2) << std::fixed << inner_radius_ << " и внешним радиусом " << outer_radius_ << std::endl;
	}

	void area(std::ostream& os) override
	{
		os << "Площадь кольца: " << (3.14 * outer_radius_ * outer_radius_) - (3.14 * inner_radius_ * inner_radius_) << std::endl;
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

	void printData(std::ostream& os) override
	{
		os << "Сфера c радиусом " << std::setprecision(2) << std::fixed << radius_ << std::endl;
	}

	void area(std::ostream& os) override
	{
		os << "Площадь поверхности сферы: " << 4 * 3.14 * radius_ * radius_ << std::endl;
	}

	void volume(std::ostream& os) override
	{
		os << "Объём сферы: " << (4. / 3) * 3.14 * radius_ * radius_ * radius_ << std::endl;
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

	void printData(std::ostream& os) override
	{
		os << "Цилиндр с радиусом основания " << std::setprecision(2) << std::fixed << radius_ << " и высотой " << height_ << std::endl;
	}

	void area(std::ostream& os) override
	{
		os << "Площадь поверхности цилиндра: " << 2 * 3.14 * radius_ * height_ << std::endl;
	}

	void volume(std::ostream& os) override
	{
		os << "Объём цилиндра: " << 3.14 * radius_ * radius_ * height_ << std::endl;
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

	void printData(std::ostream& os) override
	{
		os << "Конус с радиусом основания " << std::setprecision(2) << std::fixed << radius_ << " и высотой " << height_ << std::endl;
	}

	void area(std::ostream& os) override
	{
		double generatrix_length = std::sqrt(radius_ * radius_ + height_ * height_);
		double side_area = 3.14 * radius_ * generatrix_length;
		double circle_area = 3.14 * radius_ * radius_;
		os << "Площадь поверхности конуса: " << side_area + circle_area << std::endl;
	}

	void volume(std::ostream& os) override
	{
		double circle_area = 3.14 * radius_ * radius_;
		os << "Объём цилиндра: " << (1. / 3) * height_ * circle_area << std::endl;
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
		std::cout << "Неверная информация. Введите число: ";
	}
	return option;
}

double GetDoubleFromConsole()
{
	double val = 0.;
	while (!(std::cin >> val)) {
		std::cin.clear();
		std::cin.ignore(32767, '\n');
		std::cout << "Неверная информация. Введите дробное число: ";
	}
	return val;
}

void OutputFigureTypesSelection()
{
	// circle rectangle ring sphere cylinder cone
	std::cout << "Выберите тип фигуры, где: \n";
	std::cout << "1. Круг.\n";
	std::cout << "2. Прямоугольник.\n";
	std::cout << "3. Кольцо.\n";
	std::cout << "4. Сфера.\n";
	std::cout << "5. Цилиндр.\n";
	std::cout << "6. Конус.\n";
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
		std::cout << "Введите радиус круга: ";
		double radius = GetDoubleFromConsole();
		return new Circle(radius);
	}
	case 2:
	{
		std::cout << "Введите ширину прямоугольника: ";
		double width = GetDoubleFromConsole();
		std::cout << "Введите высоту прямоугольника: ";
		double height = GetDoubleFromConsole();
		return new RectangleCl(width, height);
	}
	case 3:
	{
		std::cout << "Введите внутренний радиус кольца: ";
		double r1 = GetDoubleFromConsole();
		std::cout << "Введите внешний радиус кольца: ";
		double r2 = GetDoubleFromConsole();
		return new Ring(r1, r2);
	}
		
	case 4:
	{
		std::cout << "Введите радиус сферы: ";
		double radius = GetDoubleFromConsole();
		return new Sphere(radius);
	}
	case 5:
	{
		std::cout << "Введите радиус основания цилиндра: ";
		double radius = GetDoubleFromConsole();
		std::cout << "Введите высоту цилиндра: ";
		double height = GetDoubleFromConsole();
		return new Cylinder(radius, height);
	}
	case 6:
	{
		std::cout << "Введите радиус основания конуса: ";
		double radius = GetDoubleFromConsole();
		std::cout << "Введите высоту конуса: ";
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
		std::cout << "Меню:\n";
		std::cout << "1. Вывести данные о фигурах в консоль.\n";
		std::cout << "2. Вывести данные о фигурах в файл.\n";
		std::cout << "3. Добавить новую фигуру, инициализированную по умолчанию.\n";
		std::cout << "4. Добавить новую фигуру.\n";
		std::cout << "5. Удалить последнюю фигуру.\n";
		std::cout << "6. Выйти.\n";
		std::cout << "Выберите пункт: ";
		option = GetIntOptionFromConsole();
		switch (option)
		{
		case 1:
			system("cls");
			for(int i = 0; i < shapes.size(); i++)
			{
				shapes[i]->printData(std::cout);
				shapes[i]->area(std::cout);
				shapes[i]->volume(std::cout);
				std::cout << "---------" << std::endl;
			}
			continue;
		case 2:
			{
			system("cls");
			std::cout << "Введите название для файла: ";
			std::cin >> filename;
			// TODO: enter to file logic
			std::ofstream ofs(filename);
				if(!ofs.is_open())
				{
					for (int i = 0; i < shapes.size(); i++)
					{
						shapes[i]->printData(ofs);
						shapes[i]->area(ofs);
						shapes[i]->volume(ofs);
						ofs << "---------" << std::endl;
					}
				} else
				{
					std::cout << "Не удалось открыть файл. Попробуйте другое название." << std::endl;
				}
			continue;
			}
		case 3:
		{
			system("cls");
			OutputFigureTypesSelection();
			std::cout << "Выберите пункт: ";
			int figure_option = GetIntOptionFromConsole();
			Shape* new_shape = GetDefaultConstructedShape(figure_option);
			if (new_shape != nullptr)
			{
				shapes.push_back(new_shape);
			}
			else
			{
				std::cout << "Выбран несуществующий пункт." << std::endl;
			}
			continue;
		}
		case 4:
		{
			system("cls");
			OutputFigureTypesSelection();
			std::cout << "Выберите пункт: ";
			int figure_option = GetIntOptionFromConsole();
			Shape* new_shape = GetShape(figure_option);
			if (new_shape != nullptr)
			{
				shapes.push_back(new_shape);
			}
			else
			{
				std::cout << "Выбран несуществующий пункт." << std::endl;
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