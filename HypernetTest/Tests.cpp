#include "Globals.h"
#include "Funcs.h"
#include <gtest/gtest.h>

// 14-й знак может отличаться на 1-9, но на деле только 1 (из-за округления)
const double Eps = 0.0000000000001;

class TestCriteria : public ::testing::Test {
public:
    TestCriteria() { /* init protected members here */ }
    ~TestCriteria() { /* free protected members here */ }
    void SetUp() { /* called before every test */ }
    void TearDown() { /* called after every test */ }
protected:
    /* none yet */
};

H GetHypernetForTest() {
    std::vector<Branch> branches;
    std::vector<Node> nodes;
    std::vector<Route> routes;
    try {
        GetData(branches, nodes, routes);
    } catch (const char *str) {
        std::cout << str << std::endl;
    }
    ComputeBinomialCoefficients();
    H initialHypernet = H(std::move(branches), std::move(nodes), std::move(routes));
    initialHypernet.RemoveEmptyBranches();

    return initialHypernet;
}

double GetAPCValueForTest () {
    H hypernet = GetHypernetForTest();
    Branch sum;
    Branch pseudoBranch = Branch::GetBranch(0);
    try {
        ComputeAPC(sum, hypernet, pseudoBranch);
    } catch (const char *str) {
        std::cout << str << std::endl;
    }

    std::stringstream tmp;
    tmp << std::setprecision(14) << std::fixed << sum.GetValue();
    return stod(tmp.str());
}

double GetMENCValueForTest () {
    H hypernet = GetHypernetForTest();
    Branch sum;
    Branch pseudoBranch = Branch::GetBranch(0);
    try {
        ComputeMENC(sum, hypernet, pseudoBranch);
    } catch (const char *str) {
        std::cout << str << std::endl;
    }

    std::stringstream tmp;
    tmp << std::setprecision(14) << std::fixed << sum.GetValue();
    return stod(tmp.str());
}

// todo исправить проблему с выделением памяти для _FN

TEST_F(TestCriteria, APCTreeInGrid5x5) {
    input.open("inputs/TreeInGrid(25, 40, 24).txt");
    double expectedValue = 0.40453937997986;
    double value = GetAPCValueForTest();
    ASSERT_TRUE(std::abs(value - expectedValue) < Eps);
    input.close();
}

TEST_F(TestCriteria, MENCTreeInGrid5x5) {
    input.open("inputs/TreeInGrid(25, 40, 24).txt");
    double expectedValue = 13.797192208223;
    double value = GetMENCValueForTest();

    ASSERT_TRUE(std::abs(value - expectedValue) < Eps);
    input.close();
}

TEST_F(TestCriteria, APCTreeInGrid4x4) {
    input.open("inputs/TreeInGrid(16, 24, 15).txt");
    double expectedValue = 0.54275080059203;
    double value = GetAPCValueForTest();
    ASSERT_TRUE(std::abs(value - expectedValue) < Eps);
    input.close();
}

TEST_F(TestCriteria, MENCTreeInGrid4x4) {
    input.open("inputs/TreeInGrid(16, 24, 15).txt");
    double expectedValue = 10.65012301;
    double value = GetMENCValueForTest();

    ASSERT_TRUE(std::abs(value - expectedValue) < Eps);
    input.close();
}

TEST_F(TestCriteria, APCTreeInGrid3x3) {
    input.open("inputs/TreeInGrid(9, 12, 8).txt");
    double expectedValue = 0.66381732525;
    double value = GetAPCValueForTest();
    ASSERT_TRUE(std::abs(value - expectedValue) < Eps);
    input.close();
}

TEST_F(TestCriteria, MENCTreeInGrid3x3) {
    input.open("inputs/TreeInGrid(9, 12, 8).txt");
    double expectedValue = 6.756031;
    double value = GetMENCValueForTest();
    ASSERT_TRUE(std::abs(value - expectedValue) < Eps);
    input.close();
}

TEST_F(TestCriteria, APCCycleInGrid) {
    input.open("inputs/CycleInGrid(9, 12, 9).txt");
    double expectedValue = 0.73213512;
    double value = GetAPCValueForTest();
    ASSERT_TRUE(std::abs(value - expectedValue) < Eps);
    input.close();
}

TEST_F(TestCriteria, MENCCycleInGrid) {
    input.open("inputs/CycleInGrid(9, 12, 9).txt");
    double expectedValue = 6.40830448;
    double value = GetMENCValueForTest();
    ASSERT_TRUE(std::abs(value - expectedValue) < Eps);
    input.close();
}

TEST_F(TestCriteria, APCWheelInGrid) {
    input.open("inputs/WheelInGrid(9, 12, 16).txt");
    double expectedValue = 0.923865667535;
    double value = GetAPCValueForTest();
    ASSERT_TRUE(std::abs(value - expectedValue) < Eps);
    input.close();
}

TEST_F(TestCriteria, MENCWheelInGrid) {
    input.open("inputs/WheelInGrid(9, 12, 16).txt");
    double expectedValue = 7.9021629775;
    double value = GetMENCValueForTest();
    ASSERT_TRUE(std::abs(value - expectedValue) < Eps);
    input.close();
}

TEST_F(TestCriteria, APCRandomHypernet1) {
    input.open("inputs/RandomHypernet(9, 12, 16).txt");
    double expectedValue = 0.6790755043845;
    double value = GetAPCValueForTest();
    ASSERT_TRUE(std::abs(value - expectedValue) < Eps);
    input.close();
}

TEST_F(TestCriteria, MENCRandomHypernet1) {
    input.open("inputs/RandomHypernet(9, 12, 16).txt");
    double expectedValue = 6.108812602038;
    double value = GetMENCValueForTest();
    ASSERT_TRUE(std::abs(value - expectedValue) < Eps);
    input.close();
}

TEST_F(TestCriteria, APCRandomHypernet2) {
    input.open("inputs/RandomHypernet(9, 12, 12).txt");
    double expectedValue = 0.62946030614;
    double value = GetAPCValueForTest();
    ASSERT_TRUE(std::abs(value - expectedValue) < Eps);
    input.close();
}

TEST_F(TestCriteria, MENCRandomHypernet2) {
    input.open("inputs/RandomHypernet(9, 12, 12).txt");
    double expectedValue = 5.43600127504;
    double value = GetMENCValueForTest();
    ASSERT_TRUE(std::abs(value - expectedValue) < Eps);
    input.close();
}

TEST_F(TestCriteria, APCRandomHypernet3) {
    input.open("inputs/RandomHypernet(6, 7, 8).txt");
    double expectedValue = 0.7370817;
    double value = GetAPCValueForTest();
    ASSERT_TRUE(std::abs(value - expectedValue) < Eps);
    input.close();
}

TEST_F(TestCriteria, MENCRandomHypernet3) {
    input.open("inputs/RandomHypernet(6, 7, 8).txt");
    double expectedValue = 4.407273;
    double value = GetMENCValueForTest();
    ASSERT_TRUE(std::abs(value - expectedValue) < Eps);
    input.close();
}

