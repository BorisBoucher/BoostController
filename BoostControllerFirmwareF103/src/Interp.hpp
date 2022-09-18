#pragma once

#include <ParameterGen.hpp>

template <typename Iterator, typename U>
Iterator left_bound(Iterator begin, Iterator end, const U value)
{
	auto iter = begin;
	auto ret = begin;

	if (value >= *begin)
	{
		while (iter < end)
		{
			if (*iter > value)
			{
				break;
			}
			else if (*iter == value)
			{
				ret = iter;
				break;
			}

			ret = iter;
			++iter;
		}
	}

	return ret;
}

template <typename Iterator, typename U>
Iterator right_bound(Iterator begin, Iterator end, const U value)
{
	auto iter = begin;
	auto ret = begin;

	if (value >= *begin)
	{
		while (iter < end)
		{
			if (*iter >= value)
			{
				ret = iter;
				break;
			}

			ret = iter;
			++iter;
		}
	}

	return ret;
}

float interp1D(ParamIndex& paramIndex, uint16_t baseParamId, float rowValue);

float interp2D(ParamIndex& paramIndex, uint16_t baseParamId, float rowValue, float colValue);