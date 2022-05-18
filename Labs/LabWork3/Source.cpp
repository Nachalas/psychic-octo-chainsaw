#include <array>
#include <cassert>
#include <vector>
#include <cmath>
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <Windows.h>

#undef max
#undef min

enum class ShapeType
{
	BASE,
	CIRCLE,
	RECTANGLE,
	TRIANGLE,
	PYRAMID,
};

struct Point2d
{
	double x;
	double y;

	Point2d()
	{
		x = 0.;
		y = 0.;
	}

	Point2d(double x_, double y_)
	{
		x = x_;
		y = y_;
	}

	friend std::ostream& operator<<(std::ostream& os, const Point2d&);
};

struct Point3d
{
	double x;
	double y;
	double z;

	Point3d()
	{
		x = 0.;
		y = 0.;
		z = 0.;
	}

	friend std::ostream& operator<<(std::ostream& os, const Point3d&);
};

std::ostream& operator<<(std::ostream& os, const Point2d& point)
{
	os << "(" << point.x << ", " << point.y << ")";
	return os;
}

std::ostream& operator<<(std::ostream& os, const Point3d& point)
{
	os << "(" << point.x << ", " << point.y << ", " << point.z << ")";
	return os;
}

struct Segment
{
	Point2d point_1;
	Point2d point_2;
};

double GetDistanceBetweenPoints(Point2d a, Point2d b)
{
	double x_delta = b.x - a.x;
	double y_delta = b.y - a.y;
	return std::sqrt(x_delta * x_delta + y_delta * y_delta);
}

// Необходимые функции для проверки двух отрезков на пересечение
// https://e-maxx.ru/algo/segments_intersection_checking, первый способ
int area(Point2d a, Point2d b, Point2d c) {
	return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

bool intersect_1(int a, int b, int c, int d) {
	if (a > b)  std::swap(a, b);
	if (c > d)  std::swap(c, d);
	return std::max(a, c) <= std::min(b, d);
}

bool intersect(Segment ab, Segment cd) {
	return intersect_1(ab.point_1.x, ab.point_2.x, cd.point_1.x, cd.point_2.x)
		&& intersect_1(ab.point_1.y, ab.point_2.y, cd.point_1.y, cd.point_2.y)
		&& area(ab.point_1, ab.point_2, cd.point_1) * area(ab.point_1, ab.point_2, cd.point_2) <= 0
		&& area(cd.point_1, cd.point_2, ab.point_1) * area(cd.point_1, cd.point_2, ab.point_2) <= 0;
}

// Функция для проверки пересечения отрезка с окружностью

bool commonSectionCircle(Segment ab,
	Point2d circle_center, double R)
{
	double xC = circle_center.x;
	double yC = circle_center.y;

	ab.point_1.x -= xC;
	ab.point_1.y -= yC;
	ab.point_2.x -= xC;
	ab.point_2.y -= yC;

	double dx = ab.point_2.x - ab.point_1.x;
	double dy = ab.point_2.y - ab.point_1.y;

	//составляем коэффициенты квадратного уравнения на пересечение прямой и окружности.
	//если на отрезке [0..1] есть отрицательные значения, значит отрезок пересекает окружность
	double a = dx * dx + dy * dy;
	double b = 2. * (ab.point_1.x * dx + ab.point_1.y * dy);
	double c = ab.point_1.x * ab.point_1.x + ab.point_1.y * ab.point_1.y - R * R;

	//а теперь проверяем, есть ли на отрезке [0..1] решения
	if (-b < 0)
		return (c < 0);
	if (-b < (2. * a))
		return ((4. * a * c - b * b) < 0);

	return (a + b + c < 0);
}
// ---------------

class Shape
{
public:
	Shape()
	{
		type_ = ShapeType::BASE;
	}

	ShapeType GetShapeType()
	{
		return type_;
	}

	virtual void PrintData(std::ostream&) = 0;
protected:
	ShapeType type_;
};

class Circle : public Shape
{
public:
	Circle(Point2d center, double radius)
	{
		type_ = ShapeType::CIRCLE;
		center_coords_ = center;
		radius_ = radius;
	}

	Point2d GetCenterCoords()
	{
		return center_coords_;
	}

	void SetCenterCoords(const Point2d& coords)
	{
		center_coords_ = coords;
	}

	double GetRadius()
	{
		return radius_;
	}

	void SetRadius(double radius)
	{
		radius_ = radius;
	}

	void PrintData(std::ostream& os) override
	{
		os << "Круг с центром в точке " << center_coords_ << " и радиусом " << radius_ << std::endl;
	}

private:
	Point2d center_coords_;
	double radius_;
};

class Pyramid : public Shape
{
public:

	Pyramid(Point3d first_corner, Point3d second_corner, Point3d edge)
	{
		type_ = ShapeType::PYRAMID;
		base_corner_1 = first_corner;
		base_corner_2 = second_corner;
		edge_point = edge;
	}

	void SetBasePoints(const std::array<Point3d, 2>& points)
	{
		base_corner_1 = points[0];
		base_corner_2 = points[1];
	}

	std::array<Point3d, 2> GetBasePoints()
	{
		return { base_corner_1, base_corner_2 };
	}

	void SetEdgePoint(const Point3d& point)
	{
		edge_point = point;
	}

	Point3d GetEdgePoint()
	{
		return edge_point;
	}

	void PrintData(std::ostream& os) override
	{
		os << "Пирамида с основанием в точках " << base_corner_1 << ", " << base_corner_2 << " и точкой " << edge_point << std::endl;
	}

private:

	Point3d base_corner_1;
	Point3d base_corner_2;
	Point3d edge_point;
};

class RectangleCl : public Shape
{
public:
	RectangleCl(Point2d first_corner, Point2d second_corner)
	{
		type_ = ShapeType::RECTANGLE;
		// TODO: проверка на x1 == x2 || y1 == y2
		assert((first_corner.x != second_corner.x) && first_corner.y != second_corner.y);
		if (first_corner.x < second_corner.x)
		{
			lower_left_corner = first_corner;
			upper_right_corner = second_corner;
		} else
		{
			lower_left_corner = second_corner;
			upper_right_corner = first_corner;
		}
	}

	std::array<Point2d, 2> GetPoints()
	{
		return { lower_left_corner, upper_right_corner };
	}

	void SetPoints(const std::array<Point2d, 2>& points)
	{
		lower_left_corner = points[0];
		upper_right_corner = points[1];
	}

	bool ContainsPoint(const Point2d& point)
	{	
		return (point.x >= lower_left_corner.x && point.x <= upper_right_corner.x)
		&& (point.y >= lower_left_corner.y && point.y <= upper_right_corner.y);
	}

	void PrintData(std::ostream& os) override
	{
		os << "Прямоугольник, заданный точками: " << lower_left_corner << ", " << upper_right_corner << std::endl;
	}
	
private:
	Point2d lower_left_corner;
	Point2d upper_right_corner;
};

class Triangle : public Shape
{
public:
	Triangle(Point2d a, Point2d b, Point2d c)
	{
		type_ = ShapeType::TRIANGLE;
		a_ = a;
		b_ = b;
		c_ = c;
	}

	std::array<Point2d, 3> GetPoints()
	{
		return {a_, b_, c_};
	}

	void SetPoints(const std::array<Point2d, 3>& points)
	{
		a_ = points[0];
		b_ = points[1];
		c_ = points[2];
	}

	void PrintData(std::ostream& os) override
	{
		os << "Треугольник с вершинами " << a_ << ", " << b_ << ", " << c_ << std::endl;
	}
private:
	Point2d a_;
	Point2d b_;
	Point2d c_;
};

std::vector<Segment> GetTriangleSides(const std::array<Point2d, 3>& triangle_points)
{
	return {
		{triangle_points[0], triangle_points[1]},
		{triangle_points[1], triangle_points[2]},
		{triangle_points[0], triangle_points[2]},
	};
}

std::vector<Segment> GetRectangleSides(const std::array<Point2d, 2>& rectangle_corner_points)
{
	return {
		{rectangle_corner_points[0], Point2d(rectangle_corner_points[0].x, rectangle_corner_points[1].y)},
		{Point2d{rectangle_corner_points[0].x, rectangle_corner_points[1].y}, rectangle_corner_points[1]},
		{rectangle_corner_points[1], Point2d(rectangle_corner_points[1].x, rectangle_corner_points[0].y)},
		{Point2d(rectangle_corner_points[1].x, rectangle_corner_points[0].y), rectangle_corner_points[0]},
	};
}

bool intersectionTriangleRectangle(Shape* triangle, Shape* rectangle)
{
	Triangle* triangle_ptr = (Triangle*)triangle;
	RectangleCl* rectangle_ptr = (RectangleCl*)rectangle;
	std::array<Point2d, 3> triangle_points = triangle_ptr->GetPoints();
	std::array<Point2d, 2> rectangle_corner_points = rectangle_ptr->GetPoints();
	std::vector<Segment> triangle_sides = GetTriangleSides(triangle_points);
	std::vector<Segment> rectangle_sides = GetRectangleSides(rectangle_corner_points);

	bool intersection_found = false;
	for(int i = 0; i < triangle_sides.size(); i++)
	{
		for(int j = 0; j < rectangle_sides.size(); j++)
		{
			if(intersect(triangle_sides[i], rectangle_sides[j]))
			{
				intersection_found = true;
				break;
			}
		}
	}
	return intersection_found;
}

bool intersectionTriangleTriangle(Shape* triangle_1, Shape* triangle_2)
{
	Triangle* triangle_1_ptr = (Triangle*)triangle_1;
	Triangle* triangle_2_ptr = (Triangle*)triangle_2;
	std::array<Point2d, 3> triangle_1_points = triangle_1_ptr->GetPoints();
	std::array<Point2d, 3> triangle_2_points = triangle_2_ptr->GetPoints();

	std::vector<Segment> triangle_1_sides = GetTriangleSides(triangle_1_points);
	std::vector<Segment> triangle_2_sides = GetTriangleSides(triangle_2_points);

	bool intersection_found = false;
	for(int i = 0; i < triangle_1_sides.size(); i++)
	{
		for(int j = 0; j < triangle_2_sides.size(); j++)
		{
			if(intersect(triangle_1_sides[i], triangle_2_sides[j]))
			{
				intersection_found = true;
				break;
			}
		}
	}

	return intersection_found;
}

bool intersectionCircleCircle(Shape* circle_1, Shape* circle_2)
{
	Circle* circle_1_ptr = (Circle*) circle_1;
	Circle* circle_2_ptr = (Circle*) circle_2;
	double r1 = circle_1_ptr->GetRadius();
	Point2d center1 = circle_1_ptr->GetCenterCoords();
	double r2 = circle_2_ptr->GetRadius();
	Point2d center2 = circle_2_ptr->GetCenterCoords();

	double distance = GetDistanceBetweenPoints(center1, center2);
	return (distance <= (r1 + r2));
}

bool intersectionRectangleRectangle(Shape* rectangle_1, Shape* rectangle_2) 
{
	RectangleCl* rectangle_1_ptr = (RectangleCl*) rectangle_1;
	RectangleCl* rectangle_2_ptr = (RectangleCl*) rectangle_2;

	std::array<Point2d, 2> rectangle_1_corner_points = rectangle_1_ptr->GetPoints();
	std::vector<Point2d> rectangle_1_points = {
		rectangle_1_corner_points[0],
		Point2d{rectangle_1_corner_points[0].x, rectangle_1_corner_points[1].y},
		rectangle_1_corner_points[1],
		Point2d(rectangle_1_corner_points[1].x, rectangle_1_corner_points[0].y)
	};

	bool intersection_found = false;
	for(int i = 0; i < rectangle_1_corner_points.size(); i++)
	{
		if(rectangle_2_ptr->ContainsPoint(rectangle_1_corner_points[i]))
		{
			intersection_found = true;
			break;
		}
	}

	return intersection_found;
}

bool intersectionTriangleCircle(Shape* triangle, Shape* circle)
{
	Triangle* triangle_ptr = (Triangle*) triangle;
	Circle* circle_ptr = (Circle*) circle;

	std::array<Point2d, 3> triangle_points = triangle_ptr->GetPoints();
	std::vector<Segment> triangle_sides = GetTriangleSides(triangle_points);

	bool intersection_found = false;
	for(int i = 0; i < triangle_sides.size(); i++)
	{
		if(commonSectionCircle(triangle_sides[i], circle_ptr->GetCenterCoords(), circle_ptr->GetRadius()))
		{
			intersection_found = true;
			break;
		}
	}
	return intersection_found;
}

bool intersectionRectangleCircle(Shape* rectangle, Shape* circle)
{
	RectangleCl* rectangle_ptr = (RectangleCl*) rectangle;
	Circle* circle_ptr = (Circle*) circle;

	std::array<Point2d, 2> rectangle_corner_points = rectangle_ptr->GetPoints();
	std::vector<Segment> rectangle_sides = GetRectangleSides(rectangle_corner_points);

	bool intersection_found = false;
	for (int i = 0; i < rectangle_sides.size(); i++)
	{
		if (commonSectionCircle(rectangle_sides[i], circle_ptr->GetCenterCoords(), circle_ptr->GetRadius()))
		{
			intersection_found = true;
			break;
		}
	}
	return intersection_found;
}

bool intersection(Shape* shape1, Shape* shape2)
{
	ShapeType shape1_type = shape1->GetShapeType();
	ShapeType shape2_type = shape2->GetShapeType();
	switch (shape1_type)
	{
	case ShapeType::CIRCLE:
		switch (shape2_type)
		{
		case ShapeType::CIRCLE:
			return intersectionCircleCircle(shape1, shape2);
		case ShapeType::RECTANGLE:
			return intersectionRectangleCircle(shape2, shape1);
		case ShapeType::TRIANGLE:
			return intersectionTriangleCircle(shape2, shape1);
		}
	case ShapeType::RECTANGLE:
		switch (shape2_type)
		{
		case ShapeType::CIRCLE:
			return intersectionRectangleCircle(shape1, shape2);
		case ShapeType::RECTANGLE:
			return intersectionRectangleRectangle(shape1, shape2);
		case ShapeType::TRIANGLE:
			return intersectionTriangleRectangle(shape2, shape1);
		}
	case ShapeType::TRIANGLE:
		switch (shape2_type)
		{
		case ShapeType::CIRCLE:
			return intersectionTriangleCircle(shape1, shape2);
		case ShapeType::RECTANGLE:
			return intersectionTriangleRectangle(shape1, shape2);
		case ShapeType::TRIANGLE:
			return intersectionTriangleTriangle(shape1, shape2);
		}
		break;
	}
}

int GetIntFromConsole()
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

Point2d GetTrianglePointFromConsole(int index)
{
	Point2d point;
	std::cout << "Введите координату x точки " << index << ": ";
	point.x = GetDoubleFromConsole();
	std::cout << "Введите координату y точки " << index << ": ";
	point.y = GetDoubleFromConsole();
	return point;
}

void ShowChangeFigureMenu(Shape* shape)
{
	switch (shape->GetShapeType())
	{
	case ShapeType::CIRCLE:
	{
		Circle* circle_ptr = (Circle*) shape;
		Point2d center{};
		double radius = 0.;
		std::cout << "Введите координату x центра: ";
		center.x = GetDoubleFromConsole();
		std::cout << "Введите координату y центра: ";
		center.y = GetDoubleFromConsole();
		std::cout << "Введите радиус окружности: ";
		radius = GetDoubleFromConsole();
		circle_ptr->SetRadius(radius);
		circle_ptr->SetCenterCoords(center);
		break;
	}
	case ShapeType::TRIANGLE:
	{
		Triangle* triangle_ptr = (Triangle*)shape;
		std::array<Point2d, 3> points{};
		points[0] = GetTrianglePointFromConsole(1);
		points[1] = GetTrianglePointFromConsole(2);
		points[2] = GetTrianglePointFromConsole(3);
		triangle_ptr->SetPoints(points);
		break;
	}
	case ShapeType::RECTANGLE:
	{
		RectangleCl* rectangle_ptr = (RectangleCl*)shape;
		std::array<Point2d, 2> points{};
		std::cout << "Введите координату x угла 1: ";
		points[0].x = GetDoubleFromConsole();
		std::cout << "Введите координату y угла 1: ";
		points[0].y = GetDoubleFromConsole();

		std::cout << "Введите координату x угла 2: ";
		points[1].x = GetDoubleFromConsole();
		std::cout << "Введите координату y угла 2: ";
		points[1].y = GetDoubleFromConsole();

		rectangle_ptr->SetPoints(points);
		break;
	}
	}
}

void showMenu()
{
	int option = -1;

	std::array<Shape*, 6> shapes;

	int shape_1_index = -1;
	int shape_2_index = -1;

	Circle circle1(Point2d(0, 0), 5);
	Circle circle2(Point2d(2, 2), 7);
	RectangleCl rectangle1(Point2d(0,0), Point2d(2,2));
	RectangleCl rectangle2(Point2d(7,2), Point2d(9, 4));
	Triangle triangle1(Point2d(-2, 3), Point2d(4,3), Point2d(1,10));
	Triangle triangle2(Point2d(6, 4), Point2d(4, 4), Point2d(6, 1));

	shapes[0] = &circle1;
	shapes[1] = &circle2;
	shapes[2] = &rectangle1;
	shapes[3] = &rectangle2;
	shapes[4] = &triangle1;
	shapes[5] = &triangle2;

	std::string filename;
	do
	{
		std::cout << "Меню:\n";
		std::cout << "1. Вывести данные о фигурах в консоль.\n";
		std::cout << "2. Вывести данные о фигурах в файл.\n";
		std::cout << "3. Изменить фигуру.\n";
		std::cout << "4. Проверить фигуры на пересечение.\n";
		std::cout << "5. Выйти.\n";
		std::cout << "Выберите пункт: ";
		option = GetIntFromConsole();

		switch (option)
		{
		case 1:
			system("cls");
			for(int i = 0; i < shapes.size(); i++) 
			{
				shapes[i]->PrintData(std::cout);
				std::cout << "-----------" << std::endl;
			}
			continue;

		case 2:
		{
			system("cls");
			std::cout << "Введите название для файла: ";
			std::cin >> filename;
			std::ofstream ofs(filename);
			if (ofs.is_open())
			{
				for (int i = 0; i < shapes.size(); i++)
				{
					shapes[i]->PrintData(ofs);
					ofs << "-----------" << std::endl;
				}
			}
			else
			{
				std::cout << "Не удалось открыть файл. Попробуйте другое название." << std::endl;
			}
			continue;
		}
		case 3:
		{
			system("cls");
			std::cout << "Выберите индекс фигуры: ";
			int selectedFigureIndex = GetIntFromConsole();
			if(selectedFigureIndex >= 0 && selectedFigureIndex <= 6)
			{
				ShowChangeFigureMenu(shapes[selectedFigureIndex]);
			} else
			{
				std::cout << "Выбрана несуществующая фигура" << std::endl;
			}
			continue;
		}		
		case 4:
			system("cls");
			std::cout << "Введите индекс 1: ";
			shape_1_index = GetIntFromConsole();
			std::cout << "Введите индекс 2: ";
			shape_2_index = GetIntFromConsole();
			if(shape_1_index != shape_2_index && shape_1_index >= 0 && shape_2_index >= 0)
			{
				std::cout << (intersection(shapes[shape_1_index], shapes[shape_2_index]) ? "Фигуры пересекаются" : "Фигуры не пересекаются") << std::endl;
			} else
			{
				std::cout << "Индексы фигур должны отличаться." << std::endl;
			}
			continue;
		}

	} while (option != 5);
}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	showMenu();
	return 0;
}