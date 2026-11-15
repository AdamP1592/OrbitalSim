#pragma once
#include <iostream>
void printLog(const auto& observed, const auto& expected, const char* testNamePtr){
    std::cerr << testNamePtr << " failed. " << "Observed: " << observed << " Expected: " << expected << std::endl;
}