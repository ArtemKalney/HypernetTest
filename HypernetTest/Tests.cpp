#include "Globals.h"
#include "Funcs.h"
#include <gtest/gtest.h>

// 14-й знак может отличаться на 1-9, но на деле только 1 (из-за округления)
const double Eps = 0.0000000000001;

class TestCriteria : public ::testing::Test {
public:
    TestCriteria() { /* init protected members here */ }

    ~TestCriteria() { /* free protected members here */ }
    /* called before every test */
    void SetUp() {
        ReliableHypernets = 0;
        UnconnectedHypernets = 0;
        TwoNodesHypernets = 0;
        ChainsReduced = 0;
        UnconnectedNodesReduced = 0;
        PairConnectivityCalls = 0;
        EdgesReduced = 0;
        ComplexChains = 0;
    }

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
    if (IS_TEST_CHECK_SPECIFICATIONS == 1) {
        ASSERT_EQ(PairConnectivityCalls, 927);
        ASSERT_EQ(UnconnectedNodesReduced, 4626);
        ASSERT_EQ(EdgesReduced, 6071);
        ASSERT_EQ(ChainsReduced, 613);
        ASSERT_EQ(ComplexChains, 27);
        ASSERT_EQ(ReliableHypernets, 231);
        ASSERT_EQ(UnconnectedHypernets, 852);
        ASSERT_EQ(TwoNodesHypernets, 59);
    }
    input.close();
}

TEST_F(TestCriteria, MENCTreeInGrid5x5) {
    input.open("inputs/TreeInGrid(25, 40, 24).txt");
    double expectedValue = 13.797192208223;
    double value = GetMENCValueForTest();
    ASSERT_TRUE(std::abs(value - expectedValue) < Eps);
    if (IS_TEST_CHECK_SPECIFICATIONS == 1) {
        ASSERT_EQ(PairConnectivityCalls, 25);
        ASSERT_EQ(UnconnectedNodesReduced, 435);
        ASSERT_EQ(EdgesReduced, 522);
        ASSERT_EQ(ChainsReduced, 23);
        ASSERT_EQ(ComplexChains, 1);
        ASSERT_EQ(ReliableHypernets, 0);
        ASSERT_EQ(UnconnectedHypernets, 1);
        ASSERT_EQ(TwoNodesHypernets, 23);
    }
    input.close();
}

TEST_F(TestCriteria, APCTreeInGrid4x4) {
    input.open("inputs/TreeInGrid(16, 24, 15).txt");
    double expectedValue = 0.54275080059203;
    double value = GetAPCValueForTest();
    ASSERT_TRUE(std::abs(value - expectedValue) < Eps);
    if (IS_TEST_CHECK_SPECIFICATIONS == 1) {
        ASSERT_EQ(PairConnectivityCalls, 309);
        ASSERT_EQ(UnconnectedNodesReduced, 1073);
        ASSERT_EQ(EdgesReduced, 1391);
        ASSERT_EQ(ChainsReduced, 184);
        ASSERT_EQ(ComplexChains, 0);
        ASSERT_EQ(ReliableHypernets, 87);
        ASSERT_EQ(UnconnectedHypernets, 276);
        ASSERT_EQ(TwoNodesHypernets, 33);
    }
    input.close();
}

TEST_F(TestCriteria, MENCTreeInGrid4x4) {
    input.open("inputs/TreeInGrid(16, 24, 15).txt");
    double expectedValue = 10.65012301;
    double value = GetMENCValueForTest();
    ASSERT_TRUE(std::abs(value - expectedValue) < Eps);
    if (IS_TEST_CHECK_SPECIFICATIONS == 1) {
        ASSERT_EQ(PairConnectivityCalls, 15);
        ASSERT_EQ(UnconnectedNodesReduced, 159);
        ASSERT_EQ(EdgesReduced, 192);
        ASSERT_EQ(ChainsReduced, 14);
        ASSERT_EQ(ComplexChains, 0);
        ASSERT_EQ(ReliableHypernets, 0);
        ASSERT_EQ(UnconnectedHypernets, 0);
        ASSERT_EQ(TwoNodesHypernets, 15);
    }
    input.close();
}

TEST_F(TestCriteria, APCTreeInGrid3x3) {
    input.open("inputs/TreeInGrid(9, 12, 8).txt");
    double expectedValue = 0.66381732525;
    double value = GetAPCValueForTest();
    ASSERT_TRUE(std::abs(value - expectedValue) < Eps);
    if (IS_TEST_CHECK_SPECIFICATIONS == 1) {
        ASSERT_EQ(PairConnectivityCalls, 74);
        ASSERT_EQ(UnconnectedNodesReduced, 143);
        ASSERT_EQ(EdgesReduced, 190);
        ASSERT_EQ(ChainsReduced, 38);
        ASSERT_EQ(ComplexChains, 0);
        ASSERT_EQ(ReliableHypernets, 20);
        ASSERT_EQ(UnconnectedHypernets, 58);
        ASSERT_EQ(TwoNodesHypernets, 16);
    }
    input.close();
}

TEST_F(TestCriteria, MENCTreeInGrid3x3) {
    input.open("inputs/TreeInGrid(9, 12, 8).txt");
    double expectedValue = 6.756031;
    double value = GetMENCValueForTest();
    ASSERT_TRUE(std::abs(value - expectedValue) < Eps);
    if (IS_TEST_CHECK_SPECIFICATIONS == 1) {
        ASSERT_EQ(PairConnectivityCalls, 8);
        ASSERT_EQ(UnconnectedNodesReduced, 38);
        ASSERT_EQ(EdgesReduced, 48);
        ASSERT_EQ(ChainsReduced, 7);
        ASSERT_EQ(ComplexChains, 0);
        ASSERT_EQ(ReliableHypernets, 0);
        ASSERT_EQ(UnconnectedHypernets, 0);
        ASSERT_EQ(TwoNodesHypernets, 8);
    }
    input.close();
}

TEST_F(TestCriteria, APCCycleInGrid) {
    input.open("inputs/CycleInGrid(9, 12, 9).txt");
    double expectedValue = 0.73213512;
    double value = GetAPCValueForTest();
    ASSERT_TRUE(std::abs(value - expectedValue) < Eps);
    if (IS_TEST_CHECK_SPECIFICATIONS == 1) {
        ASSERT_EQ(PairConnectivityCalls, 191);
        ASSERT_EQ(UnconnectedNodesReduced, 96);
        ASSERT_EQ(EdgesReduced, 51);
        ASSERT_EQ(ChainsReduced, 107);
        ASSERT_EQ(ComplexChains, 0);
        ASSERT_EQ(ReliableHypernets, 74);
        ASSERT_EQ(UnconnectedHypernets, 123);
        ASSERT_EQ(TwoNodesHypernets, 15);
    }
    input.close();
}

TEST_F(TestCriteria, MENCCycleInGrid) {
    input.open("inputs/CycleInGrid(9, 12, 9).txt");
    double expectedValue = 6.40830448;
    double value = GetMENCValueForTest();
    ASSERT_TRUE(std::abs(value - expectedValue) < Eps);
    if (IS_TEST_CHECK_SPECIFICATIONS == 1) {
        ASSERT_EQ(PairConnectivityCalls, 27);
        ASSERT_EQ(UnconnectedNodesReduced, 10);
        ASSERT_EQ(EdgesReduced, 5);
        ASSERT_EQ(ChainsReduced, 24);
        ASSERT_EQ(ComplexChains, 0);
        ASSERT_EQ(ReliableHypernets, 12);
        ASSERT_EQ(UnconnectedHypernets, 19);
        ASSERT_EQ(TwoNodesHypernets, 2);
    }
    input.close();
}

TEST_F(TestCriteria, APCWheelInGrid) {
    input.open("inputs/WheelInGrid(9, 12, 16).txt");
    double expectedValue = 0.923865667535;
    double value = GetAPCValueForTest();
    ASSERT_TRUE(std::abs(value - expectedValue) < Eps);
    if (IS_TEST_CHECK_SPECIFICATIONS == 1) {
        ASSERT_EQ(PairConnectivityCalls, 3310);
        ASSERT_EQ(UnconnectedNodesReduced, 1246);
        ASSERT_EQ(EdgesReduced, 1040);
        ASSERT_EQ(ChainsReduced, 344);
        ASSERT_EQ(ComplexChains, 1116);
        ASSERT_EQ(ReliableHypernets, 1647);
        ASSERT_EQ(UnconnectedHypernets, 1581);
        ASSERT_EQ(TwoNodesHypernets, 59);
    }
    input.close();
}

TEST_F(TestCriteria, MENCWheelInGrid) {
    input.open("inputs/WheelInGrid(9, 12, 16).txt");
    double expectedValue = 7.9021629775;
    double value = GetMENCValueForTest();
    ASSERT_TRUE(std::abs(value - expectedValue) < Eps);
    if (IS_TEST_CHECK_SPECIFICATIONS == 1) {
        ASSERT_EQ(PairConnectivityCalls, 415);
        ASSERT_EQ(UnconnectedNodesReduced, 129);
        ASSERT_EQ(EdgesReduced, 138);
        ASSERT_EQ(ChainsReduced, 59);
        ASSERT_EQ(ComplexChains, 202);
        ASSERT_EQ(ReliableHypernets, 205);
        ASSERT_EQ(UnconnectedHypernets, 218);
        ASSERT_EQ(TwoNodesHypernets, 0);
    }
    input.close();
}

TEST_F(TestCriteria, APCRandomHypernet1) {
    input.open("inputs/RandomHypernet(9, 12, 16).txt");
    double expectedValue = 0.6790755043845;
    double value = GetAPCValueForTest();
    ASSERT_TRUE(std::abs(value - expectedValue) < Eps);
    if (IS_TEST_CHECK_SPECIFICATIONS == 1) {
        ASSERT_EQ(PairConnectivityCalls, 835);
        ASSERT_EQ(UnconnectedNodesReduced, 281);
        ASSERT_EQ(EdgesReduced, 529);
        ASSERT_EQ(ChainsReduced, 147);
        ASSERT_EQ(ComplexChains, 657);
        ASSERT_EQ(ReliableHypernets, 344);
        ASSERT_EQ(UnconnectedHypernets, 517);
        ASSERT_EQ(TwoNodesHypernets, 5);
    }
    input.close();
}

TEST_F(TestCriteria, MENCRandomHypernet1) {
    input.open("inputs/RandomHypernet(9, 12, 16).txt");
    double expectedValue = 6.108812602038;
    double value = GetMENCValueForTest();
    ASSERT_TRUE(std::abs(value - expectedValue) < Eps);
    if (IS_TEST_CHECK_SPECIFICATIONS == 1) {
        ASSERT_EQ(PairConnectivityCalls, 141);
        ASSERT_EQ(UnconnectedNodesReduced, 58);
        ASSERT_EQ(EdgesReduced, 104);
        ASSERT_EQ(ChainsReduced, 27);
        ASSERT_EQ(ComplexChains, 140);
        ASSERT_EQ(ReliableHypernets, 57);
        ASSERT_EQ(UnconnectedHypernets, 90);
        ASSERT_EQ(TwoNodesHypernets, 1);
    }
    input.close();
}

TEST_F(TestCriteria, APCRandomHypernet2) {
    input.open("inputs/RandomHypernet(9, 12, 12).txt");
    double expectedValue = 0.62946030614;
    double value = GetAPCValueForTest();
    ASSERT_TRUE(std::abs(value - expectedValue) < Eps);
    if (IS_TEST_CHECK_SPECIFICATIONS == 1) {
        ASSERT_EQ(PairConnectivityCalls, 845);
        ASSERT_EQ(UnconnectedNodesReduced, 288);
        ASSERT_EQ(EdgesReduced, 302);
        ASSERT_EQ(ChainsReduced, 65);
        ASSERT_EQ(ComplexChains, 11);
        ASSERT_EQ(ReliableHypernets, 247);
        ASSERT_EQ(UnconnectedHypernets, 622);
        ASSERT_EQ(TwoNodesHypernets, 6);
    }
    input.close();
}

TEST_F(TestCriteria, MENCRandomHypernet2) {
    input.open("inputs/RandomHypernet(9, 12, 12).txt");
    double expectedValue = 5.43600127504;
    double value = GetMENCValueForTest();
    ASSERT_TRUE(std::abs(value - expectedValue) < Eps);
    if (IS_TEST_CHECK_SPECIFICATIONS == 1) {
        ASSERT_EQ(PairConnectivityCalls, 201);
        ASSERT_EQ(UnconnectedNodesReduced, 35);
        ASSERT_EQ(EdgesReduced, 55);
        ASSERT_EQ(ChainsReduced, 22);
        ASSERT_EQ(ComplexChains, 0);
        ASSERT_EQ(ReliableHypernets, 53);
        ASSERT_EQ(UnconnectedHypernets, 152);
        ASSERT_EQ(TwoNodesHypernets, 2);
    }
    input.close();
}

TEST_F(TestCriteria, APCRandomHypernet3) {
    input.open("inputs/RandomHypernet(6, 7, 8).txt");
    double expectedValue = 0.7370817;
    double value = GetAPCValueForTest();
    ASSERT_TRUE(std::abs(value - expectedValue) < Eps);
    if (IS_TEST_CHECK_SPECIFICATIONS == 1) {
        ASSERT_EQ(PairConnectivityCalls, 84);
        ASSERT_EQ(UnconnectedNodesReduced, 7);
        ASSERT_EQ(EdgesReduced, 21);
        ASSERT_EQ(ChainsReduced, 25);
        ASSERT_EQ(ComplexChains, 50);
        ASSERT_EQ(ReliableHypernets, 31);
        ASSERT_EQ(UnconnectedHypernets, 66);
        ASSERT_EQ(TwoNodesHypernets, 1);
    }
    input.close();
}

TEST_F(TestCriteria, MENCRandomHypernet3) {
    input.open("inputs/RandomHypernet(6, 7, 8).txt");
    double expectedValue = 4.407273;
    double value = GetMENCValueForTest();
    ASSERT_TRUE(std::abs(value - expectedValue) < Eps);
    if (IS_TEST_CHECK_SPECIFICATIONS == 1) {
        ASSERT_EQ(PairConnectivityCalls, 21);
        ASSERT_EQ(UnconnectedNodesReduced, 0);
        ASSERT_EQ(EdgesReduced, 4);
        ASSERT_EQ(ChainsReduced, 8);
        ASSERT_EQ(ComplexChains, 10);
        ASSERT_EQ(ReliableHypernets, 8);
        ASSERT_EQ(UnconnectedHypernets, 18);
        ASSERT_EQ(TwoNodesHypernets, 0);
    }
    input.close();
}

