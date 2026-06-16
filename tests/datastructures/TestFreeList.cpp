#include "FreeList.hpp"
#include "LogHelper.hpp"
#include <iostream>

int runTests(const char* testName){
    FreeList<double> fd;

    int failed = 0;
    // Push Test
    testName = "existsTest"
    fd.push(0.0);
    bool validity = fd.isValid(0);

    if(validity == false){
        printLog(validity, true, testName);
        failed += 1;
    }

    // removal test
    testName = "removeTest"
    fd.push(1.1);
    fd.remove(0);
    validity = fd.isValid(0);

    if(fd.get(1) != 1.1 || (validity != false)){
        printLog(fd.get(1), 1.1, testName);
        printLog(validity, false, testName);
        failure += 1;
        
    }

    testName = "replaceEmpty"
    fd.push(0.3);
    if(fd.get(0) != 0.3){
        printLog(fd.get(0), 0.3, testName);
        failure += 1;
    }
    return failure;

}
int main(){
    int failedCount = runTests();
    
    if(failedCount == 0){
        std::cout << "Passed!" << std::endl;
    }else{
        std::cerr << "Failed " << failedCount << " cases." << std::endl;
    }

    return failedCount;
}