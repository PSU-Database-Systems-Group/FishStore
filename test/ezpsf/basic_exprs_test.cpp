// A basic test for EzPsf
//
// Created by Max Norfolk on 7/6/23.

#include <gtest/gtest.h>
#include "basic_ezpsf_fixture.h"


TEST_F(BasicJitFixture, BasicArithmetic) {
    std::vector<int> inputs = {0, 1, 3, 5, 20, 50, -40, -100, -1};
    RecBatchInsert("x.y", inputs);

    ASSERT_EQ(records.size(), inputs.size());

    PsfInfo psf = getPsf("BasicArithmetic => (Int) x.y + 2");

    ASSERT_EQ(psf.fields.size(), 1);
    ASSERT_EQ(psf.fields[0], "x.y");
    ASSERT_EQ(psf.type, DataType::INT32_T) << getName(psf.type);

    for (size_t i = 0; i < inputs.size(); ++i) {
        int32_t value = 0;
        bool hasValue = psf.psf(&records[i], &value);
        ASSERT_TRUE(hasValue);
        ASSERT_EQ(inputs[i] + 2, value) << inputs[i] << " + 2 v.s. " << value << ". Iteration: " << i;
    }
}

TEST_F(BasicJitFixture, ComplexArithmetic) {
    std::vector<int> xyInputs = {0, 1, 3, 5, 20, 100, -40, -100, -1, -4};
    std::vector<int> abInputs = {3, 2, 7, 0, -4, -28, 105, 2801, 80, 0};
    RecBatchInsert("x.y", xyInputs);
    RecBatchInsert("a.b", abInputs);
    ASSERT_EQ(abInputs.size(), xyInputs.size());

    ASSERT_EQ(records.size(), xyInputs.size());

    PsfInfo psf = getPsf("ComplexArithmetic => ((Int) x.y + 2) * 4 + (Int) a.b // 5");

    ASSERT_EQ(psf.fields.size(), 2);
    ASSERT_EQ(psf.fields[0], "x.y");
    ASSERT_EQ(psf.fields[1], "a.b");
    ASSERT_EQ(psf.type, DataType::INT32_T) << getName(psf.type);

    for (size_t i = 0; i < xyInputs.size(); ++i) {
        int32_t value = 0;
        bool hasValue = psf.psf(&records[i], &value);
        ASSERT_TRUE(hasValue);
        int32_t real_value = (xyInputs[i] + 2) * 4 + abInputs[i] / 5;
        ASSERT_EQ(real_value, value)
                                    << xyInputs[i] << " as 'x.y', and " << abInputs[i] << " as 'a.b'. Iteration: " << i;
    }
}

TEST_F(BasicJitFixture, AutomaticCastArithmetic) {
    std::vector<int> inputs = {0, 1, 3, 5, 20, 50, -40, -100, -1};
    RecBatchInsert("x.y", inputs);

    ASSERT_EQ(records.size(), inputs.size());

    PsfInfo psf = getPsf("AutomaticCastArithmetic => (Int) x.y / 2.0 * (5.4 // 2)");

    ASSERT_EQ(psf.fields.size(), 1);
    ASSERT_EQ(psf.fields[0], "x.y");
    ASSERT_EQ(psf.type, DataType::DOUBLE_T) << getName(psf.type);

    for (size_t i = 0; i < inputs.size(); ++i) {
        double value = 0;
        bool hasValue = psf.psf(&records[i], &value);
        ASSERT_TRUE(hasValue);
        double real_value = inputs[i] / 2.0 * static_cast<int>(5.4 / 2);
        ASSERT_EQ(real_value, value) << real_value << " v.s. " << value << ". Iteration: " << i;
    }
}

TEST_F(BasicJitFixture, StringLength) {
    std::vector<std::string> inputs = {"", "ab", "hello", "really long", "x", "This is an example!"};
    RecBatchInsert("x.y", inputs);

    ASSERT_EQ(records.size(), inputs.size());

    PsfInfo psf = getPsf("StringLength => |(Str) x.y|");

    ASSERT_EQ(psf.fields.size(), 1);
    ASSERT_EQ(psf.fields[0], "x.y");
    ASSERT_TRUE(isIntType(psf.type)) << getName(psf.type);

    for (size_t i = 0; i < inputs.size(); ++i) {
        uint64_t value = 0;
        bool hasValue = psf.psf(&records[i], &value);
        ASSERT_TRUE(hasValue);
        uint64_t real_value = inputs[i].size();
        ASSERT_EQ(real_value, value) << real_value << " v.s. " << value << ". Iteration: " << i;
    }
}

TEST_F(BasicJitFixture, StringComparison) {
    std::vector<std::string> xyInputs = {"a", "ab", "hello", "z", "wonderful", "LENGTH VARIABLE", "OTHER"};
    std::vector<std::string> abInputs = {"z", "cd", "hello", "a", "Capitals!", "LENGTH VARI", "OTHER WAY"};
    RecBatchInsert("x.y", xyInputs);
    RecBatchInsert("a.b", abInputs);
    ASSERT_EQ(abInputs.size(), xyInputs.size());
    ASSERT_EQ(records.size(), xyInputs.size());

    PsfInfo psf = getPsf("StringComparison => (Str) x.y == (Str) a.b");

    ASSERT_EQ(psf.fields.size(), 2);
    ASSERT_EQ(psf.fields[0], "x.y");
    ASSERT_EQ(psf.fields[1], "a.b");
    ASSERT_EQ(psf.type, DataType::BOOL_T) << getName(psf.type);

    for (size_t i = 0; i < xyInputs.size(); ++i) {
        bool value = false;
        bool hasValue = psf.psf(&records[i], &value);
        ASSERT_TRUE(hasValue);
        bool real_value = xyInputs[i] == abInputs[i];
        ASSERT_EQ(real_value, value)
                                    << xyInputs[i] << " as 'x.y', and " << abInputs[i] << " as 'a.b'. Iteration: " << i;
    }
}

TEST_F(BasicJitFixture, StringComparisonLiterals){
    std::vector<std::string> xyInputs = {"a", "ab", "hello", "z", "wonderful", "abc", "OTHER"};
    RecBatchInsert("x.y", xyInputs);
    PsfInfo psf = getPsf("StringComparisonLiterals => (Str) x.y == \"abc\"");

    ASSERT_EQ(psf.fields.size(), 1);
    ASSERT_EQ(psf.fields[0], "x.y");
    ASSERT_EQ(psf.type, DataType::BOOL_T) << getName(psf.type);
    for (size_t i = 0; i < xyInputs.size(); ++i) {
        bool value = false;
        bool hasValue = psf.psf(&records[i], &value);
        ASSERT_TRUE(hasValue);
        bool real_value = xyInputs[i] == "abc";
        ASSERT_EQ(real_value, value) << xyInputs[i] << " as 'x.y'. Iteration: " << i;
    }
}

TEST_F(BasicJitFixture, IntegerComparison) {
    std::vector<int> xyInputs = {0, 1, 3, 5, 20, 100, -40, -100, -1, -4};
    std::vector<int> abInputs = {3, 2, 7, 0, -4, -28, 105, 2801, 80, 0};
    RecBatchInsert("x.y", xyInputs);
    RecBatchInsert("a.b", abInputs);
    ASSERT_EQ(abInputs.size(), xyInputs.size());

    ASSERT_EQ(records.size(), xyInputs.size());

    PsfInfo psf = getPsf("IntegerComparison => (Int) x.y > (Int) a.b");

    ASSERT_EQ(psf.fields.size(), 2);
    ASSERT_EQ(psf.fields[0], "x.y");
    ASSERT_EQ(psf.fields[1], "a.b");
    ASSERT_EQ(psf.type, DataType::BOOL_T) << getName(psf.type);

    for (size_t i = 0; i < xyInputs.size(); ++i) {
        int32_t value = 0;
        bool hasValue = psf.psf(&records[i], &value);
        ASSERT_TRUE(hasValue);
        int32_t real_value = xyInputs[i] > abInputs[i];
        ASSERT_EQ(real_value, value) << real_value << " v.s. " << value << ". Iteration: " << i;
    }
}

TEST_F(BasicJitFixture, ShortCircuitAndComparison) {
    std::vector<int> xyInputs = {0, 1, 3, 5, 20, 100, -40, -100, -1, -4};
    std::vector<int> abInputs = {3, 2, 7, 0, -4, -28, 105, 2801, 80, 0};
    RecBatchInsert("x.y", xyInputs);
    RecBatchInsert("a.b", abInputs);
    ASSERT_EQ(abInputs.size(), xyInputs.size());

    ASSERT_EQ(records.size(), xyInputs.size());

    PsfInfo psf = getPsf("ShortCircuitAndComparison => (Int) x.y < 5 && (Int) a.b > 0");

    ASSERT_EQ(psf.fields.size(), 2);
    ASSERT_EQ(psf.fields[0], "x.y");
    ASSERT_EQ(psf.fields[1], "a.b");
    ASSERT_EQ(psf.type, DataType::BOOL_T) << getName(psf.type);

    for (size_t i = 0; i < xyInputs.size(); ++i) {
        int32_t value = 0;
        bool hasValue = psf.psf(&records[i], &value);
        ASSERT_TRUE(hasValue);
        int32_t real_value = xyInputs[i] < 5 && abInputs[i] > 0;
        ASSERT_EQ(real_value, value)
                                    << xyInputs[i] << " as 'x.y', and " << abInputs[i] << " as 'a.b'. Iteration: " << i;
    }
}

TEST_F(BasicJitFixture, ShortCircuitOrComparison) {
    std::vector<std::string> xyInputs = {"a", "ab", "hello", "z", "wonderful", "LENGTH VARIABLE", "OTHER",
                                         "A LOT OF THE SAME"};
    std::vector<std::string> abInputs = {"z", "cd", "hello", "a", "Capitals!", "LENGTH VARI", "OTHER WAY",
                                         "A LOT OF THE SAME"};
    std::vector<std::string> mnInputs = {"m", "xy", "hibye", "a", "wonderful", "some garbage", "OTHER WA",
                                         "A LOT OF THE SAME"};
    RecBatchInsert("x.y", xyInputs);
    RecBatchInsert("a.b", abInputs);
    RecBatchInsert("m.n", mnInputs);
    ASSERT_EQ(abInputs.size(), xyInputs.size());
    ASSERT_EQ(records.size(), xyInputs.size());
    ASSERT_EQ(records.size(), mnInputs.size());

    PsfInfo psf = getPsf("StringComparison => (Str) x.y == (Str) a.b || |(Str) m.n| < 6");

    ASSERT_EQ(psf.fields.size(), 3);
    ASSERT_EQ(psf.fields[0], "x.y");
    ASSERT_EQ(psf.fields[1], "a.b");
    ASSERT_EQ(psf.fields[2], "m.n");
    ASSERT_EQ(psf.type, DataType::BOOL_T) << getName(psf.type);

    for (size_t i = 0; i < xyInputs.size(); ++i) {
        bool value = false;
        bool hasValue = psf.psf(&records[i], &value);
        ASSERT_TRUE(hasValue);
        bool real_value = xyInputs[i] == abInputs[i] || mnInputs[i].length() < 6;
        ASSERT_EQ(real_value, value) << xyInputs[i] << " as 'x.y', and " << abInputs[i] << " as 'a.b', and "
                                     << mnInputs[i] << " as 'm.n'. Iteration: " << i;
    }
}

TEST_F(BasicJitFixture, BitwiseArithmetic) {
    std::vector<int> xyInputs = {0, 1, 3, 5, 20, 100, -40, -100, -1, -4};
    std::vector<int> abInputs = {3, 2, 7, 0, -4, -28, 105, 2805, 80, 0};
    RecBatchInsert("x.y", xyInputs);
    RecBatchInsert("a.b", abInputs);
    ASSERT_EQ(abInputs.size(), xyInputs.size());

    ASSERT_EQ(records.size(), xyInputs.size());

    PsfInfo psf = getPsf("BitwiseArithmetic => ((Int) x.y ^ 15) | (4 + ((Int) a.b & 5))");

    ASSERT_EQ(psf.fields.size(), 2);
    ASSERT_EQ(psf.fields[0], "x.y");
    ASSERT_EQ(psf.fields[1], "a.b");
    ASSERT_EQ(psf.type, DataType::INT32_T) << getName(psf.type);

    for (size_t i = 0; i < xyInputs.size(); ++i) {
        int32_t value = 0;
        bool hasValue = psf.psf(&records[i], &value);
        ASSERT_TRUE(hasValue);
        int32_t real_value = (xyInputs[i] ^ 15) | (4 + (abInputs[i] & 5));
        ASSERT_EQ(real_value, value)
                                    << xyInputs[i] << " as 'x.y', and " << abInputs[i] << " as 'a.b'. Iteration: " << i;
    }
}

