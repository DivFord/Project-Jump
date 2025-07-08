#include "Comparator.h"

bool ComparisonWindow::compare(float value) const
{
	return (value > min && value < max);
}

void init(ComparisonWindow* window, Comparator comparator, float value)
{
	switch (comparator)
	{
	case Comparator::LESS_THAN:
		window->min = FLT_MIN;
		window->max = value;
		break;

	case Comparator::LESS_THAN_EQUALS:
		window->min = FLT_MIN;
		window->max = value + FLT_EPSILON;
		break;

	case Comparator::EQUALS:
		window->min = value - FLT_EPSILON;
		window->max = value + FLT_EPSILON;
		break;

	case Comparator::GREATER_THAN_EQUALS:
		window->min = value - FLT_EPSILON;
		window->max = FLT_MAX;
		break;

	case Comparator::GREATER_THAN:
		window->min = value;
		window->max = FLT_MAX;
		break;
	}
}

ComparisonWindow::ComparisonWindow(Comparator comparator, float value)
{
	init(this, comparator, value);
}

ComparisonWindow::ComparisonWindow(std::string comparator, float value)
{
	if (comparator == "<")
		init(this, Comparator::LESS_THAN, value);
	else if (comparator == "<=")
		init(this, Comparator::LESS_THAN_EQUALS, value);
	else if (comparator == "==")
		init(this, Comparator::EQUALS, value);
	else if (comparator == ">=")
		init(this, Comparator::GREATER_THAN_EQUALS, value);
	else if (comparator == ">")
		init(this, Comparator::GREATER_THAN, value);
	else
		throw;
}