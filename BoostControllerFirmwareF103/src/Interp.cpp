#include <stdint.h>

#include <ParameterDef.hpp>
#include "Interp.hpp"

float interp1D(ParamIndex& paramIndex, uint16_t baseParamId, float rowValue)
{
	// Linear interpolation.
	// @see https://en.wikipedia.org/wiki/Linear_interpolation for details
	auto paramDef = paramIndex.getParamDef(baseParamId);
	if (paramDef == nullptr)
	{
		return 0.0;
	}
	auto arrayDef = paramDef->mArrayDesc;
	if (arrayDef == nullptr)
	{
		// not an array!
		return 0.0;
	}
	if (arrayDef->mRowValues == nullptr)
	{
		// No data for interp on row!
		return 0.0;
	}

	// find row indexes
	auto left = left_bound(arrayDef->mRowValues, arrayDef->mRowValues + arrayDef->mArrayRow, rowValue);
	auto right = right_bound(left, arrayDef->mRowValues + arrayDef->mArrayRow, rowValue);
	uint8_t rowIndex1 = left - arrayDef->mRowValues;
	uint8_t rowIndex2 = right - arrayDef->mRowValues;

	// Get the 2 header values
	const float row1 = arrayDef->mRowValues[rowIndex1];
	const float row2 = arrayDef->mRowValues[rowIndex2];

	// compute delta
	const float deltaRow = row2 - row1;

	float ret;

	// Manage edge cases
	if (rowIndex1 == rowIndex2)
	{
		// zero interp, only 1 cell is selected
		// Simply get cell value
		ret = paramIndex.getParam<float>(
			baseParamId + rowIndex1).second;
	}
	else
	{
		// 1D interp on row
		const float cell1 = paramIndex.getParam<float>(
			baseParamId + rowIndex1).second;
		const float cell2 = paramIndex.getParam<float>(
			baseParamId + rowIndex2).second;

		float deltaCell = cell2 - cell1;
		float t = deltaCell / deltaRow;
		ret = cell1 + (rowValue - row1) * t;

//		ret = (cell1 * (row2 - rowValue) + cell2 * (rowValue - row1)) / deltaRow;
	}

	return ret;
}

float interp2D(ParamIndex& paramIndex, uint16_t baseParamId, float rowValue, float colValue)
{
	// Bilinear interpolation using weighted mean
	// @see https://en.wikipedia.org/wiki/Bilinear_interpolation for details
	auto paramDef = paramIndex.getParamDef(baseParamId);
	if (paramDef == nullptr)
	{
		return 0.0;
	}
	auto arrayDef = paramDef->mArrayDesc;
	if (arrayDef == nullptr)
	{
		// not a table!
		return 0.0;
	}
	if (arrayDef->mRowValues == nullptr or arrayDef->mColValues == nullptr)
	{
		// No data for interp on row and col!
		return 0.0;
	}
	// find row indexes
	auto left = left_bound(arrayDef->mRowValues, arrayDef->mRowValues + arrayDef->mArrayRow, rowValue);
	auto right = right_bound(left, arrayDef->mRowValues + arrayDef->mArrayRow, rowValue);
	uint8_t rowIndex1 = left - arrayDef->mRowValues;
	uint8_t rowIndex2 = right - arrayDef->mRowValues;

	// find col indexes
	auto leftCol = left_bound(arrayDef->mColValues, arrayDef->mColValues + arrayDef->mArrayCol, colValue);
	auto rightCol = right_bound(leftCol, arrayDef->mColValues + arrayDef->mArrayCol, colValue);
	uint8_t colIndex1 = leftCol - arrayDef->mColValues;
	uint8_t colIndex2 = rightCol - arrayDef->mColValues;

	// Get the 4 header values
	const float row1 = arrayDef->mRowValues[rowIndex1];
	const float row2 = arrayDef->mRowValues[rowIndex2];
	const float col1 = arrayDef->mColValues[colIndex1];
	const float col2 = arrayDef->mColValues[colIndex2];

	// compute delta
	const float deltaRow = row2 - row1;
	const float deltaCol = col2 - col1;

	float ret;

	if (rowIndex1 == rowIndex2 and colIndex1 == colIndex2)
	{
		// zero interp, only 1 cell is selected
		// Simply get cell value
		ret = paramIndex.getParam<float>(
			baseParamId + rowIndex1 * arrayDef->mArrayCol + colIndex1).second;
	}
	else if (rowIndex1 == rowIndex2)
	{
		// 1D interp on coll only
		// Get the 2 cell values
		const float cell1 = paramIndex.getParam<float>(
			baseParamId + rowIndex1 * arrayDef->mArrayCol + colIndex1).second;
		const float cell2 = paramIndex.getParam<float>(
			baseParamId + rowIndex1 * arrayDef->mArrayCol + colIndex2).second;
		
		float deltaCell = cell2 - cell1;
		float t = deltaCell / deltaCol;
		ret = cell1 + (colValue - col1) * t;

//		ret = (cell1 * (col2 - colValue) + cell2 * (colValue - col1)) / deltaCol;
	}
	else if (colIndex1 == colIndex2)
	{
		// 1D interp on row only
		const float cell1 = paramIndex.getParam<float>(
			baseParamId + rowIndex1 * arrayDef->mArrayCol + colIndex1).second;
		const float cell2 = paramIndex.getParam<float>(
			baseParamId + rowIndex1 * arrayDef->mArrayCol + colIndex2).second;
		
		float deltaCell = cell2 - cell1;
		float t = deltaCell / deltaRow;
		ret = cell1 + (rowValue - row1) * t;
//		ret = (cell1 * (row2 - rowValue) + cell2 * (rowValue - row1)) / deltaRow;
	}
	else
	{
		//  2D interp

		// compute ratio
		const float ratio = deltaRow * deltaCol;

		// compute weights
		const float w11 = (row2 - rowValue) * (col2 - colValue) / ratio;
		const float w12 = (row2 - rowValue) * (colValue - col1) / ratio;
		const float w21 = (rowValue - row1) * (col2 - colValue) / ratio;
		const float w22 = (rowValue - row1) * (colValue - col1) / ratio;

		// Get the 4 cell values
		const float cell11 = paramIndex.getParam<float>(
			baseParamId + rowIndex1 * arrayDef->mArrayCol + colIndex1).second;
		const float cell12 = paramIndex.getParam<float>(
			baseParamId + rowIndex1 * arrayDef->mArrayCol + colIndex2).second;
		const float cell21 = paramIndex.getParam<float>(
			baseParamId + rowIndex2 * arrayDef->mArrayCol + colIndex1).second;
		const float cell22 = paramIndex.getParam<float>(
			baseParamId + rowIndex2 * arrayDef->mArrayCol + colIndex1).second;

		// And finally, compute the output value!
		ret = cell11 * w11 + cell12 * w12 + cell21 * w21 + cell22 * w22;
	}

	return ret;
}
