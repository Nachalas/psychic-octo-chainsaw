#include "VectorUtils.h"

#include "Global.h"

std::vector<ProductEntry> GetProductsByDurationAndPrice(int min_months, double min_price)
{
	std::vector<ProductEntry> new_products;
	std::copy_if(
		products.begin(),
		products.end(),
		std::back_inserter(new_products),
		[min_months, min_price](const ProductEntry& product)
		{
			return isNMonthsBeforeNow(product.time, min_months) && product.price > min_price;
		});
	std::sort(new_products.begin(), new_products.end(), [](const ProductEntry& lhs, const ProductEntry& rhs)
		{
			return lhs.name < rhs.name;
		});
	return new_products;
}

std::vector<ProductEntry> GetProductsByName(const std::string& name)
{
	std::vector<ProductEntry> new_products;
	std::copy_if(
		products.begin(),
		products.end(),
		std::back_inserter(new_products),
		[name](const ProductEntry& product) {return product.name == name; });
	return new_products;
}

std::vector<ProductEntry> GetProductsByAdderSurname(const std::string& surname)
{
	std::vector<ProductEntry> new_products;
	std::copy_if(
		products.begin(),
		products.end(),
		std::back_inserter(new_products),
		[surname](const ProductEntry& product) {return product.added_by.surname == surname; });
	return new_products;
}

std::vector<ProductEntry> GetProductsByDate(SYSTEMTIME date)
{
	std::vector<ProductEntry> new_products;
	std::copy_if(
		products.begin(),
		products.end(),
		std::back_inserter(new_products),
		[date](const ProductEntry& product)
		{
			return product.time.wMonth == date.wMonth && product.time.wDay == date.wDay && product.time.wYear == date.wYear;
		});
	return new_products;
}

std::vector<ProductEntry> GetProductsSortedByName()
{
	std::vector<ProductEntry> new_products = products;
	std::sort(new_products.begin(), new_products.end(), [](const ProductEntry& lhs, const ProductEntry& rhs)
		{
			return lhs.name < rhs.name;
		});
	return new_products;
}

std::vector<ProductEntry> GetProductsSortedByPrice()
{
	std::vector<ProductEntry> new_products = products;
	std::sort(new_products.begin(), new_products.end(), [](const ProductEntry& lhs, const ProductEntry& rhs)
		{
			return lhs.price < rhs.price;
		});
	return new_products;
}

std::vector<ProductEntry> GetProductsSortedByDate()
{
	std::vector<ProductEntry> new_products = products;
	std::sort(new_products.begin(), new_products.end(), [](const ProductEntry& lhs, const ProductEntry& rhs)
		{
			return IsTimeLess(lhs.time, rhs.time);
		});
	return new_products;
}