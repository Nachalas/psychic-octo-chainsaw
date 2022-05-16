#include <array>
#include <cassert>

enum class ShapeType
{
	BASE,
	CIRCLE,
	SQUARE,
	TRIANGLE,
};

struct Point2d
{
	double x;
	double y;
};

class Shape
{
public:
	Shape()
	{
		type_ = ShapeType::BASE;
	}
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
private:
	Point2d center_coords_;
	double radius_;
};

class Square : public Shape
{
	Square(Point2d first_corner, Point2d second_corner)
	{
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

	//bool ContainsPoint(const Point2d& point)
	//{	
	//	return (point.x >= lower_left_corner.x && point.x <= upper_right_corner.x)
	//	&& (point.y >= lower_left_corner.y && point.y <= upper_right_corner.y);
	//}


private:
	Point2d lower_left_corner;
	Point2d upper_right_corner;
};

class Triangle : public Shape
{
	Triangle(Point2d a, Point2d b, Point2d c)
	{
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
private:
	Point2d a_;
	Point2d b_;
	Point2d c_;
};

int main()
{
	return 0;
}