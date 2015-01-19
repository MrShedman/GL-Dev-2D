#pragma once

#include "..\System\BinaryStream.hpp"
#include "MetricsData.hpp"

bool loadSDFF(const std::string& filename, MetricsData& metrics, std::vector<unsigned char>& pixels);