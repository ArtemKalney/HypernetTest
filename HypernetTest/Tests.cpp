#include "Globals.h"
#include "Funcs.h"
#include <gtest/gtest.h>
#include "ComputeMENC.h"
#include "ComputeAPC.h"

// 14-й знак может отличаться на 1-9, но на деле только 1 (из-за округления)
// Вычисленная точность для совместимости со значениями полинома
const double Eps = IS_NUMBER_COMPUTATION == 1 && IS_NODES_RELIABLE == 1 ? 0.00001 : 0.0000000000001;
const std::string TestFolderPath = "inputs/tests/";

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

double GetAPCValueForTest (bool isNodeReliable) {
    H hypernet = GetHypernetForTest();
    Branch branchSum;
    Node nodeSum;
    std::stringstream tmp;
    try {
        if (isNodeReliable) {
            ComputeAPC(branchSum, hypernet);
            tmp << std::setprecision(14) << std::fixed << branchSum.GetPolynomialValue(p);
        } else {
            ComputeAPC(nodeSum, hypernet);
            tmp << std::setprecision(14) << std::fixed << nodeSum.GetPolynomialValue(p);
        }
    } catch (const char *str) {
        std::cout << str << std::endl;
    }

    return stod(tmp.str());
}

double GetMENCValueForTest (bool isNodeReliable) {
    H hypernet = GetHypernetForTest();
    Branch branchSum;
    Node nodeSum;
    std::stringstream tmp;
    try {
        if (isNodeReliable) {
            ComputeMENC(branchSum, hypernet);
            tmp << std::setprecision(14) << std::fixed << branchSum.GetPolynomialValue(p);
        } else {
            ComputeMENC(nodeSum, hypernet);
            tmp << std::setprecision(14) << std::fixed << nodeSum.GetPolynomialValue(p);
        }
    } catch (const char *str) {
        std::cout << str << std::endl;
    }

    return stod(tmp.str());
}

// todo исправить проблему с выделением памяти для _FN
// Большие тесты
TEST_F(TestCriteria, APCTreeInGrid5x5) {
    input.open(TestFolderPath + "TreeInGrid(25, 40, 24).txt");
    double expectedValue = 0.40053398051415;
    double value = GetAPCValueForTest(true);
    ASSERT_TRUE(std::abs(value - expectedValue) < Eps);
    if (IS_TEST_CHECK_SPECIFICATIONS == 1) {
        ASSERT_EQ(PairConnectivityCalls, 959);
        ASSERT_EQ(UnconnectedNodesReduced, 4395);
        ASSERT_EQ(EdgesReduced, 6024);
        ASSERT_EQ(ChainsReduced, 629);
        ASSERT_EQ(ComplexChains, 0);
        ASSERT_EQ(ReliableHypernets, 244);
        ASSERT_EQ(UnconnectedHypernets, 903);
        ASSERT_EQ(TwoNodesHypernets, 56);
    }
    input.close();
}

TEST_F(TestCriteria, MENCTreeInGrid5x5) {
    input.open(TestFolderPath + "TreeInGrid(25, 40, 24).txt");
    double expectedValue = 14.025960132773;
    double value = GetMENCValueForTest(true);
    ASSERT_TRUE(std::abs(value - expectedValue) < Eps);
    if (IS_TEST_CHECK_SPECIFICATIONS == 1) {
        ASSERT_EQ(PairConnectivityCalls, 24);
        ASSERT_EQ(UnconnectedNodesReduced, 422);
        ASSERT_EQ(EdgesReduced, 520);
        ASSERT_EQ(ChainsReduced, 23);
        ASSERT_EQ(ComplexChains, 0);
        ASSERT_EQ(ReliableHypernets, 0);
        ASSERT_EQ(UnconnectedHypernets, 0);
        ASSERT_EQ(TwoNodesHypernets, 24);
    }
    input.close();
}

TEST_F(TestCriteria, APCTreeInGrid5x5Node) {
    input.open(TestFolderPath + "TreeInGrid(25, 40, 24).txt");
    double expectedValue = 0.603046944;
    double value = GetAPCValueForTest(false);
    ASSERT_TRUE(std::abs(value - expectedValue) < Eps);
    if (IS_TEST_CHECK_SPECIFICATIONS == 1) {
        ASSERT_EQ(PairConnectivityCalls, 876);
        ASSERT_EQ(UnconnectedNodesReduced, 3903);
        ASSERT_EQ(EdgesReduced, 5472);
        ASSERT_EQ(ReliableHypernets, 276);
        ASSERT_EQ(UnconnectedHypernets, 876);
        ASSERT_EQ(TwoNodesHypernets, 0);
    }
    input.close();
}

TEST_F(TestCriteria, MENCTreeInGrid5x5Node) {
    input.open(TestFolderPath + "TreeInGrid(25, 40, 24).txt");
    double expectedValue = 17.9452;
    double value = GetMENCValueForTest(false);
    ASSERT_TRUE(std::abs(value - expectedValue) < Eps);
    if (IS_TEST_CHECK_SPECIFICATIONS == 1) {
        ASSERT_EQ(PairConnectivityCalls, 32);
        ASSERT_EQ(UnconnectedNodesReduced, 336);
        ASSERT_EQ(EdgesReduced, 428);
        ASSERT_EQ(ReliableHypernets, 20);
        ASSERT_EQ(UnconnectedHypernets, 32);
        ASSERT_EQ(TwoNodesHypernets, 0);
    }
    input.close();
}
// Маленькие тесты
// --------------------------------------------------->Branch tests
TEST_F(TestCriteria, APCTreeInGrid4x4) {
    input.open(TestFolderPath + "TreeInGrid(16, 24, 15).txt");
    double expectedValue = 0.54275080059203;
    double value = GetAPCValueForTest(true);
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
    input.open(TestFolderPath + "TreeInGrid(16, 24, 15).txt");
    double expectedValue = 10.65012301;
    double value = GetMENCValueForTest(true);
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
    input.open(TestFolderPath + "TreeInGrid(9, 12, 8).txt");
    double expectedValue = 0.66381732525;
    double value = GetAPCValueForTest(true);
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
    input.open(TestFolderPath + "TreeInGrid(9, 12, 8).txt");
    double expectedValue = 6.756031;
    double value = GetMENCValueForTest(true);
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

TEST_F(TestCriteria, APCCycleInGrid1) {
    input.open(TestFolderPath + "CycleInGrid1(9, 12, 9).txt");
    double expectedValue = 0.73213512;
    double value = GetAPCValueForTest(true);
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

TEST_F(TestCriteria, MENCCycleInGrid1) {
    input.open(TestFolderPath + "CycleInGrid1(9, 12, 9).txt");
    double expectedValue = 6.40830448;
    double value = GetMENCValueForTest(true);
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
    input.open(TestFolderPath + "WheelInGrid(9, 12, 16).txt");
    double expectedValue = 0.923865667535;
    double value = GetAPCValueForTest(true);
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
    input.open(TestFolderPath + "WheelInGrid(9, 12, 16).txt");
    double expectedValue = 7.9021629775;
    double value = GetMENCValueForTest(true);
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
    input.open(TestFolderPath + "RandomHypernet(9, 12, 16).txt");
    double expectedValue = 0.6790755043845;
    double value = GetAPCValueForTest(true);
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
    input.open(TestFolderPath + "RandomHypernet(9, 12, 16).txt");
    double expectedValue = 6.108812602038;
    double value = GetMENCValueForTest(true);
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
    input.open(TestFolderPath + "RandomHypernet(9, 12, 12).txt");
    double expectedValue = 0.62946030614;
    double value = GetAPCValueForTest(true);
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
    input.open(TestFolderPath + "RandomHypernet(9, 12, 12).txt");
    double expectedValue = 5.43600127504;
    double value = GetMENCValueForTest(true);
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
    input.open(TestFolderPath + "RandomHypernet(6, 7, 8).txt");
    double expectedValue = 0.7370817;
    double value = GetAPCValueForTest(true);
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
    input.open(TestFolderPath + "RandomHypernet(6, 7, 8).txt");
    double expectedValue = 4.407273;
    double value = GetMENCValueForTest(true);
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
// --------------------------------------------------------------------->Node tests
TEST_F(TestCriteria, APCTreeInGrid4x4Node) {
    input.open(TestFolderPath + "TreeInGrid(16, 24, 15).txt");
    double expectedValue = 0.63600066;
    double value = GetAPCValueForTest(false);
    ASSERT_TRUE(std::abs(value - expectedValue) < Eps);
    if (IS_TEST_CHECK_SPECIFICATIONS == 1) {
        ASSERT_EQ(PairConnectivityCalls, 289);
        ASSERT_EQ(UnconnectedNodesReduced, 886);
        ASSERT_EQ(EdgesReduced, 1181);
        ASSERT_EQ(ReliableHypernets, 105);
        ASSERT_EQ(UnconnectedHypernets, 289);
        ASSERT_EQ(TwoNodesHypernets, 0);
    }
    input.close();
}

TEST_F(TestCriteria, MENCTreeInGrid4x4Node) {
    input.open(TestFolderPath + "TreeInGrid(16, 24, 15).txt");
    double expectedValue = 11.7406;
    double value = GetMENCValueForTest(false);
    ASSERT_TRUE(std::abs(value - expectedValue) < Eps);
    if (IS_TEST_CHECK_SPECIFICATIONS == 1) {
        ASSERT_EQ(PairConnectivityCalls, 18);
        ASSERT_EQ(UnconnectedNodesReduced, 120);
        ASSERT_EQ(EdgesReduced, 150);
        ASSERT_EQ(ReliableHypernets, 12);
        ASSERT_EQ(UnconnectedHypernets, 18);
        ASSERT_EQ(TwoNodesHypernets, 0);
    }
    input.close();
}

TEST_F(TestCriteria, APCTreeInGrid3x3Node) {
    input.open(TestFolderPath + "TreeInGrid(9, 12, 8).txt");
    double expectedValue = 0.6819165;
    double value = GetAPCValueForTest(false);
    ASSERT_TRUE(std::abs(value - expectedValue) < Eps);
    if (IS_TEST_CHECK_SPECIFICATIONS == 1) {
        ASSERT_EQ(PairConnectivityCalls, 62);
        ASSERT_EQ(UnconnectedNodesReduced, 94);
        ASSERT_EQ(EdgesReduced, 134);
        ASSERT_EQ(ReliableHypernets, 28);
        ASSERT_EQ(UnconnectedHypernets, 62);
        ASSERT_EQ(TwoNodesHypernets, 0);
    }
    input.close();
}

TEST_F(TestCriteria, MENCTreeInGrid3x3Node) {
    input.open(TestFolderPath + "TreeInGrid(9, 12, 8).txt");
    double expectedValue = 6.8482;
    double value = GetMENCValueForTest(false);
    ASSERT_TRUE(std::abs(value - expectedValue) < Eps);
    if (IS_TEST_CHECK_SPECIFICATIONS == 1) {
        ASSERT_EQ(PairConnectivityCalls, 8);
        ASSERT_EQ(UnconnectedNodesReduced, 25);
        ASSERT_EQ(EdgesReduced, 34);
        ASSERT_EQ(ReliableHypernets, 6);
        ASSERT_EQ(UnconnectedHypernets, 8);
        ASSERT_EQ(TwoNodesHypernets, 0);
    }
    input.close();
}

TEST_F(TestCriteria, APCCycleInGrid1Node) {
    input.open(TestFolderPath + "CycleInGrid1(9, 12, 9).txt");
    double expectedValue = 0.76588341075;
    double value = GetAPCValueForTest(false);
    ASSERT_TRUE(std::abs(value - expectedValue) < Eps);
    if (IS_TEST_CHECK_SPECIFICATIONS == 1) {
        ASSERT_EQ(PairConnectivityCalls, 364);
        ASSERT_EQ(UnconnectedNodesReduced, 116);
        ASSERT_EQ(EdgesReduced, 350);
        ASSERT_EQ(ReliableHypernets, 139);
        ASSERT_EQ(UnconnectedHypernets, 252);
        ASSERT_EQ(TwoNodesHypernets, 0);
    }
    input.close();
}

TEST_F(TestCriteria, MENCCycleInGrid1Node) {
    input.open(TestFolderPath + "CycleInGrid1(9, 12, 9).txt");
    double expectedValue = 7.127067286;
    double value = GetMENCValueForTest(false);
    ASSERT_TRUE(std::abs(value - expectedValue) < Eps);
    if (IS_TEST_CHECK_SPECIFICATIONS == 1) {
        ASSERT_EQ(PairConnectivityCalls, 77);
        ASSERT_EQ(UnconnectedNodesReduced, 15);
        ASSERT_EQ(EdgesReduced, 70);
        ASSERT_EQ(ReliableHypernets, 27);
        ASSERT_EQ(UnconnectedHypernets, 56);
        ASSERT_EQ(TwoNodesHypernets, 0);
    }
    input.close();
}

TEST_F(TestCriteria, APCWheelInGridNode) {
    input.open(TestFolderPath + "WheelInGrid(9, 12, 16).txt");
    double expectedValue = 0.807425775;
    double value = GetAPCValueForTest(false);
    ASSERT_TRUE(std::abs(value - expectedValue) < Eps);
    if (IS_TEST_CHECK_SPECIFICATIONS == 1) {
        ASSERT_EQ(PairConnectivityCalls, 230);
        ASSERT_EQ(UnconnectedNodesReduced, 0);
        ASSERT_EQ(EdgesReduced, 175);
        ASSERT_EQ(ReliableHypernets, 110);
        ASSERT_EQ(UnconnectedHypernets, 140);
        ASSERT_EQ(TwoNodesHypernets, 0);
    }
    input.close();
}

TEST_F(TestCriteria, MENCWheelInGridNode) {
    input.open(TestFolderPath + "WheelInGrid(9, 12, 16).txt");
    double expectedValue = 7.456831975;
    double value = GetMENCValueForTest(false);
    ASSERT_TRUE(std::abs(value - expectedValue) < Eps);
    if (IS_TEST_CHECK_SPECIFICATIONS == 1) {
        ASSERT_EQ(PairConnectivityCalls, 55);
        ASSERT_EQ(UnconnectedNodesReduced, 0);
        ASSERT_EQ(EdgesReduced, 40);
        ASSERT_EQ(ReliableHypernets, 25);
        ASSERT_EQ(UnconnectedHypernets, 35);
        ASSERT_EQ(TwoNodesHypernets, 0);
    }
    input.close();
}

TEST_F(TestCriteria, APCRandomHypernet3Node) {
    input.open(TestFolderPath + "RandomHypernet(6, 7, 8).txt");
    double expectedValue = 0.770148;
    double value = GetAPCValueForTest(false);
    ASSERT_TRUE(std::abs(value - expectedValue) < Eps);
    if (IS_TEST_CHECK_SPECIFICATIONS == 1) {
        ASSERT_EQ(PairConnectivityCalls, 21);
        ASSERT_EQ(UnconnectedNodesReduced, 0);
        ASSERT_EQ(EdgesReduced, 5);
        ASSERT_EQ(ReliableHypernets, 17);
        ASSERT_EQ(UnconnectedHypernets, 13);
        ASSERT_EQ(TwoNodesHypernets, 0);
    }
    input.close();
}

TEST_F(TestCriteria, MENCRandomHypernet3Node) {
    input.open(TestFolderPath + "RandomHypernet(6, 7, 8).txt");
    double expectedValue = 4.71871;
    double value = GetMENCValueForTest(false);
    ASSERT_TRUE(std::abs(value - expectedValue) < Eps);
    if (IS_TEST_CHECK_SPECIFICATIONS == 1) {
        ASSERT_EQ(PairConnectivityCalls, 6);
        ASSERT_EQ(UnconnectedNodesReduced, 0);
        ASSERT_EQ(EdgesReduced, 3);
        ASSERT_EQ(ReliableHypernets, 5);
        ASSERT_EQ(UnconnectedHypernets, 5);
        ASSERT_EQ(TwoNodesHypernets, 0);
    }
    input.close();
}

TEST_F(TestCriteria, APCRandomHypernet2Node) {
    input.open(TestFolderPath + "RandomHypernet(9, 12, 12).txt");
    double expectedValue = 0.72693495;
    double value = GetAPCValueForTest(false);
    ASSERT_TRUE(std::abs(value - expectedValue) < Eps);
    if (IS_TEST_CHECK_SPECIFICATIONS == 1) {
        ASSERT_EQ(PairConnectivityCalls, 264);
        ASSERT_EQ(UnconnectedNodesReduced, 11);
        ASSERT_EQ(EdgesReduced, 119);
        ASSERT_EQ(ReliableHypernets, 112);
        ASSERT_EQ(UnconnectedHypernets, 177);
        ASSERT_EQ(TwoNodesHypernets, 0);
    }
    input.close();
}

TEST_F(TestCriteria, MENCRandomHypernet2Node) {
    input.open(TestFolderPath + "RandomHypernet(9, 12, 12).txt");
    double expectedValue = 6.9013441;
    double value = GetMENCValueForTest(false);
    ASSERT_TRUE(std::abs(value - expectedValue) < Eps);
    if (IS_TEST_CHECK_SPECIFICATIONS == 1) {
        ASSERT_EQ(PairConnectivityCalls, 50);
        ASSERT_EQ(UnconnectedNodesReduced, 3);
        ASSERT_EQ(EdgesReduced, 23);
        ASSERT_EQ(ReliableHypernets, 22);
        ASSERT_EQ(UnconnectedHypernets, 34);
        ASSERT_EQ(TwoNodesHypernets, 0);
    }
    input.close();
}

TEST_F(TestCriteria, APCRandomHypernet1Node) {
    input.open(TestFolderPath + "RandomHypernet(9, 12, 16).txt");
    double expectedValue = 0.79028205525;
    double value = GetAPCValueForTest(false);
    ASSERT_TRUE(std::abs(value - expectedValue) < Eps);
    if (IS_TEST_CHECK_SPECIFICATIONS == 1) {
        ASSERT_EQ(PairConnectivityCalls, 270);
        ASSERT_EQ(UnconnectedNodesReduced, 0);
        ASSERT_EQ(EdgesReduced, 88);
        ASSERT_EQ(ReliableHypernets, 155);
        ASSERT_EQ(UnconnectedHypernets, 139);
        ASSERT_EQ(TwoNodesHypernets, 0);
    }
    input.close();
}

TEST_F(TestCriteria, MENCRandomHypernet1Node) {
    input.open(TestFolderPath + "RandomHypernet(9, 12, 16).txt");
    double expectedValue = 7.33200814;
    double value = GetMENCValueForTest(false);
    ASSERT_TRUE(std::abs(value - expectedValue) < Eps);
    if (IS_TEST_CHECK_SPECIFICATIONS == 1) {
        ASSERT_EQ(PairConnectivityCalls, 87);
        ASSERT_EQ(UnconnectedNodesReduced, 0);
        ASSERT_EQ(EdgesReduced, 33);
        ASSERT_EQ(ReliableHypernets, 48);
        ASSERT_EQ(UnconnectedHypernets, 45);
        ASSERT_EQ(TwoNodesHypernets, 0);
    }
    input.close();
}