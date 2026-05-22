#ifndef LOGHELPER_H
#define LOGHELPER_H
void printLog(const auto& observed, const auto& expected, const char* testNamePtr){
    std::cerr << &testNamePtr << " failed. " << "Observed: " << observed << " Expected: " << expected << std::endl;
}
#endif