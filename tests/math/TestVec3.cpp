#include "vec3.hpp"
#include "LogHelper.hpp"

#include <iostream>
using namespace math;

int mathTests(const char* testName, vec3<double>& v1, vec3<double>& v2){
    int failed = 0;

    testName = "RoughEqualsTest";

    v1 = {149.9999999999, 149.9999999999, 149.9999999999};
    v2 = {150, 150, 150};
    if(!math::roughEquals(v1, v2, 1e-5)){
        std::cerr << "Epsilon: 1e-5 ";
        printLog(v1, v2, testName);
        failed++;
    }

    v1 = {-1.0, 0.0, 3.3};
    v2 = {1.0, 3.0, -3.3};

    vec3<double> sum = v1 + v2;
    vec3<double> difference = v1 - v2;
    vec3<double> quotient = v1 / v2;
    vec3<double> product = v1 * v2;

    vec3<double> realSum = {0.0, 3.0, 0.0};
    vec3<double> realDiff = {-2.0, -3.0, 6.6};
    vec3<double> realQuot = {-1.0, 0.0, -1.0};
    vec3<double> realProd = {-1.0, 0.0, -10.89};

    double eps = 1e-7;

    if(!(math::roughEquals(sum, realSum, eps))){
        testName = "SumTest";

        printLog(v1, v2, testName);

        failed++;
    }
    if(!(math::roughEquals(difference, realDiff, eps))){
        testName = "DifferenceTest";

        printLog(v1, v2, testName);

        failed++;
    }
    if(!(math::roughEquals(quotient, realQuot, eps))){
        testName = "DivisionTest";

        printLog(v1, v2, testName);

        failed++;
    }
    if(!(math::roughEquals(product, realProd, eps))){
        testName = "MultiplicationTest";

        printLog(v1, v2, testName);

        failed++;
    }

    return failed;
}
int runTests(){
    int failed = 0;
    // initialization tests
    const char* testName = "EmptyInitTest";

    vec3<int> v0;
    vec3<int> v1(0);

    if(v0 != v1){
        failed++;
        printLog(v0, v1, testName);
    }
    // casting tests
    testName = "CastTest";

    // cast test
    vec3<double> v2 = vec3<double>(v0);
    vec3<double> v3;
    if(v2 != v3){
        failed++;
        printLog(v2, v3, testName);
    }
    
    // Tests that can be done with existing resources are done in other functions

    // math test

    failed += mathTests(testName, v2, v3);
    return failed;
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