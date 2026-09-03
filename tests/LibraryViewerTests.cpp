#pragma once

#include <gtest/gtest.h>
#include <fstream>
#include <vector>
#include <string>
#include "engine.h"
#include "jsonTemplates.h"

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}