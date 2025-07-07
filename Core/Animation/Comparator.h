#pragma once

#include <string>

enum class Comparator
{
	LESS_THAN,
	LESS_THAN_EQUALS,
	EQUALS,
	GREATER_THAN_EQUALS,
	GREATER_THAN
};

struct ComparisonWindow
{
	float min = FLT_MIN;
	float max = 0.5f;

	bool compare(float value) const;

	ComparisonWindow() {};
	ComparisonWindow(Comparator comparator, float value);
	ComparisonWindow(std::string comparator, float value);
};