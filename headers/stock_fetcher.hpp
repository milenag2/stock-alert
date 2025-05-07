#pragma once
#include <string>
#include <optional>

std::optional<float> fetchStockPrice(const std::string& ticker, int interval);
