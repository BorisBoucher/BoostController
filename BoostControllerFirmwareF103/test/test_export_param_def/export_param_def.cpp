
#include <limits>
#include <iostream>
#include <ArduinoJson.h>
#include <unity.h>

#include <ParameterGen.hpp>

void setUp(void) 
{
	// set stuff up here
}

void tearDown(void) 
{
	// clean stuff up here
}


int main(int argc, const char* argv[])
{
	UNITY_BEGIN();

	TEST_ASSERT_TRUE(true);

	UNITY_END();

	DynamicJsonDocument doc(20000);

	auto items = doc.createNestedArray("items");

	ParamIndex paramIndex(DeviceID::BOOST_CTRL, nullptr, nullptr);

	uint16_t lastIdx = 0;
	for (uint16_t i = 0; i < std::numeric_limits<uint16_t>::max(); ++i)
	{
		const ParamDef* paramDef = paramIndex.getParamDef(i);

		if (paramDef == nullptr or not paramDef->mIsConf)
			continue;
		
		auto item = items.createNestedObject();

		item["index"] = paramDef->mID;
		item["deviceId"] = paramDef->mDeviceID;
		item["signed"] = paramDef->mSigned ? true : false;
		item["wide"] = paramDef->mWide ? true : false;
		item["arraySize"] = paramDef->mArraySize;
		item["ratio"] = paramDef->mRatio;
		item["name"] = paramDef->mName;
		item["unit"] = paramDef->mUnit;

		if (paramDef->mArrayDesc != nullptr)
		{
			const ArrayDesc* arrayDesc = paramDef->mArrayDesc;

			auto rowsInfo = item.createNestedObject("rows");
			rowsInfo["type"] = arrayDesc->mRowType;

			auto rows = item.createNestedArray("rowInfo");
			for (auto j = 0; j<arrayDesc->mArrayRow; ++j)
			{
				auto row = rows.createNestedObject();
				if (arrayDesc->mRowNames_ != nullptr)
				{
					row["name"] = arrayDesc->mRowNames_[j];
				}
				else
				{
					row["name"] = std::to_string(arrayDesc->mRowValues[j]);
				}
				row["value"] = arrayDesc->mRowValues[j];
			}

			if (arrayDesc->mArrayCol > 1)
			{
				auto colsInfo = item.createNestedObject("cols");
				colsInfo["type"] = arrayDesc->mColType;

				auto cols = item.createNestedArray("colInfo");
				for (auto j = 0; j<arrayDesc->mArrayCol; ++j)
				{
					auto col = cols.createNestedObject();
					if (arrayDesc->mColNames_ != nullptr)
					{
						col["name"] = arrayDesc->mColNames_[j];
					}
					else
					{
						col["name"] = std::to_string(arrayDesc->mColValues[j]);
					}
					col["value"] = arrayDesc->mColValues[j];
				}
			}
		}
	}

	std::cout << doc;

	return 0;
}