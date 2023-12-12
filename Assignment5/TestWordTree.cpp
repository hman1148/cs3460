#include "WordTree.hpp"

#include <algorithm>
#include <gtest/gtest.h>

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(WordTree_Size, TreeInitiallyEmpty)
{
    WordTree wordTree;

    EXPECT_EQ(0, wordTree.size());
}

TEST(WordTree_Add, CanAddArbitraryValues)
{
    WordTree wordTree;

    wordTree.add("what");
    wordTree.add("apple");
    wordTree.add("hello");

    EXPECT_EQ(wordTree.size(), 3);

    EXPECT_TRUE(wordTree.find("what"));
    EXPECT_TRUE(wordTree.find("apple"));
    EXPECT_TRUE(wordTree.find("hello"));
}

TEST(WordTree_Add, DoesNotAddEmptyStrings)
{
    WordTree wordTree;

    wordTree.add("");

    EXPECT_EQ(wordTree.size(), 0);
}

TEST(WordTree_Add, CanAddDuplicateValues)
{
    WordTree wordTree;

    wordTree.add("apple");
    wordTree.add("apple");

    EXPECT_EQ(wordTree.size(), 1);

    EXPECT_TRUE(wordTree.find("apple"));
}

TEST(WordTree_Add, DoesNotAddNonLetters)
{
    WordTree wordTree;

    wordTree.add("apple9");
    wordTree.add("apple!");
    wordTree.add("apples=oranges");
    wordTree.add("hello world");
    wordTree.add("end\n");

    EXPECT_EQ(wordTree.size(), 0);
}

TEST(WordTree_Add, CanAddCapitalLettersAsLowercase)
{
    WordTree wordTree;

    wordTree.add("Apple");
    wordTree.add("pineAPPLEs");
    wordTree.add("CapitalLettersWow");
    wordTree.add("ILOVECS");

    EXPECT_EQ(wordTree.size(), 4);
    EXPECT_TRUE(wordTree.find("apple"));
    EXPECT_TRUE(wordTree.find("pineapples"));
    EXPECT_TRUE(wordTree.find("capitalletterswow"));
    EXPECT_TRUE(wordTree.find("ilovecs"));
}

TEST(WordTree_Find, CanFindWithNoWordsInTree)
{
    WordTree wordTree;

    EXPECT_FALSE(wordTree.find("hello"));
}

TEST(WordTree_Find, FindEmptyStringIsFalse)
{
    WordTree wordTree;

    EXPECT_FALSE(wordTree.find(""));

    wordTree.add("abc");

    EXPECT_FALSE(wordTree.find(""));
}

TEST(WordTree_Find, CanFindWordWithUniquePrefix)
{
    WordTree wordTree;

    wordTree.add("what");
    wordTree.add("how");
    wordTree.add("nowhere");

    EXPECT_TRUE(wordTree.find("what"));
    EXPECT_TRUE(wordTree.find("how"));
    EXPECT_TRUE(wordTree.find("nowhere"));

    EXPECT_FALSE(wordTree.find("unknown"));
    EXPECT_FALSE(wordTree.find("wh"));
    EXPECT_FALSE(wordTree.find("wha"));
}

TEST(WordTree_Find, CanFindWordWithCommonPrefix)
{
    WordTree wordTree;

    wordTree.add("what");
    wordTree.add("who");
    wordTree.add("when");
    wordTree.add("where");
    wordTree.add("why");

    EXPECT_TRUE(wordTree.find("what"));
    EXPECT_TRUE(wordTree.find("who"));
    EXPECT_TRUE(wordTree.find("when"));
    EXPECT_TRUE(wordTree.find("where"));
    EXPECT_TRUE(wordTree.find("why"));

    EXPECT_FALSE(wordTree.find("wh"));
    EXPECT_FALSE(wordTree.find("w"));
    EXPECT_FALSE(wordTree.find("whosoever"));
}

TEST(WordTree_Find, DoesNotFindSuffix)
{
    WordTree wordTree;

    wordTree.add("apple");

    EXPECT_FALSE(wordTree.find("apples"));
    EXPECT_FALSE(wordTree.find("applesauce"));
}

TEST(WordTree_Find, DoesNotFindNonLetters)
{
    WordTree wordTree;

    wordTree.add("apple");
    wordTree.add("orange");
    wordTree.add("applesoranges");
    wordTree.add("helloworld");
    wordTree.add("end");

    EXPECT_FALSE(wordTree.find("apple9"));
    EXPECT_FALSE(wordTree.find("apple!"));
    EXPECT_FALSE(wordTree.find("apples=oranges"));
    EXPECT_FALSE(wordTree.find("hello world"));
    EXPECT_FALSE(wordTree.find("end\n"));
}

TEST(WordTree_Find, CanFindCapitalLetters)
{
    WordTree wordTree;

    wordTree.add("apple");
    wordTree.add("pineapples");
    wordTree.add("capitalletterswow");
    wordTree.add("ilovecs");

    EXPECT_TRUE(wordTree.find("Apple"));
    EXPECT_TRUE(wordTree.find("applE"));
    EXPECT_TRUE(wordTree.find("APPLE"));

    EXPECT_TRUE(wordTree.find("pineAPPLEs"));
    EXPECT_TRUE(wordTree.find("PineAppleS"));
    EXPECT_TRUE(wordTree.find("PiNeApPlEs"));

    EXPECT_TRUE(wordTree.find("CapitalLettersWow"));
    EXPECT_TRUE(wordTree.find("cAPITALlETTERSwOW"));
    EXPECT_TRUE(wordTree.find("capitalLETTERSwow"));

    EXPECT_TRUE(wordTree.find("ILoveCS"));
    EXPECT_TRUE(wordTree.find("iloveCS"));
    EXPECT_TRUE(wordTree.find("ILOVECS"));
}

TEST(WordTree_Predict, CanPredictWithNoWordsInTree)
{
    WordTree wordTree;

    EXPECT_EQ(0, wordTree.predict("hello", 1).size());
}

TEST(WordTree_Predict, CanPredictEmptyString)
{
    WordTree wordTree;

    wordTree.add("hello");

    EXPECT_EQ(0, wordTree.predict("", 1).size());
}

TEST(WordTree_Predict, CanPredictWithSingleLetter)
{
    WordTree wordTree;

    wordTree.add("zoo");
    wordTree.add("acknowledges");
    wordTree.add("acknowledging");
    wordTree.add("acorn");
    wordTree.add("acorns");
    wordTree.add("acoustic");
    wordTree.add("zebras");

    const auto predictions = wordTree.predict("a", 5);

    EXPECT_EQ(5, predictions.size());

    EXPECT_NE(end(predictions), std::find(begin(predictions), end(predictions), "acknowledges"));
    EXPECT_NE(end(predictions), std::find(begin(predictions), end(predictions), "acknowledging"));
    EXPECT_NE(end(predictions), std::find(begin(predictions), end(predictions), "acorn"));
    EXPECT_NE(end(predictions), std::find(begin(predictions), end(predictions), "acorns"));
    EXPECT_NE(end(predictions), std::find(begin(predictions), end(predictions), "acoustic"));
}

TEST(WordTree_Predict, CanPredictWithArbitraryPrefix)
{
    WordTree wordTree;

    wordTree.add("zoo");
    wordTree.add("acknowledges");
    wordTree.add("acknowledging");
    wordTree.add("acorn");
    wordTree.add("acorns");
    wordTree.add("acoustic");
    wordTree.add("bounce");
    wordTree.add("bound");
    wordTree.add("boundaries");
    wordTree.add("boundary");
    wordTree.add("zebras");

    const auto predictions = wordTree.predict("aco", 3);

    EXPECT_EQ(3, predictions.size());

    EXPECT_NE(end(predictions), std::find(begin(predictions), end(predictions), "acorn"));
    EXPECT_NE(end(predictions), std::find(begin(predictions), end(predictions), "acorns"));
    EXPECT_NE(end(predictions), std::find(begin(predictions), end(predictions), "acoustic"));
}

TEST(WordTree_Predict, DoesNotIncludePrefixInPrediction)
{
    WordTree wordTree;

    wordTree.add("acknowledging");
    wordTree.add("acorn");
    wordTree.add("acorns");
    wordTree.add("acoustic");

    const auto predictions = wordTree.predict("acorn", 2);

    EXPECT_EQ(1, predictions.size());

    EXPECT_NE(end(predictions), std::find(begin(predictions), end(predictions), "acorns"));
}

TEST(WordTree_Predict, DoesLimitPredictionCount)
{
    WordTree wordTree;

    wordTree.add("acknowledges");
    wordTree.add("acknowledging");
    wordTree.add("acorn");
    wordTree.add("acorns");
    wordTree.add("acoustic");

    EXPECT_EQ(3, wordTree.predict("ac", 3).size());
    EXPECT_EQ(1, wordTree.predict("ac", 1).size());

    EXPECT_EQ(2, wordTree.predict("aco", 2).size());
}

TEST(WordTree_Predict, DoesNotPredictNonLetters)
{
    WordTree wordTree;

    wordTree.add("apple");
    wordTree.add("apples");
    wordTree.add("applesauce");
    wordTree.add("applesoranges");
    wordTree.add("helloworld");
    wordTree.add("end");

    const auto predictions1 = wordTree.predict("4ppl", 3);
    const auto predictions2 = wordTree.predict("app!", 2);
    const auto predictions3 = wordTree.predict("apples=oran", 4);
    const auto predictions4 = wordTree.predict("hello wor", 1);
    const auto predictions5 = wordTree.predict("e\nn", 1);

    EXPECT_EQ(0, predictions1.size());
    EXPECT_EQ(0, predictions2.size());
    EXPECT_EQ(0, predictions3.size());
    EXPECT_EQ(0, predictions4.size());
    EXPECT_EQ(0, predictions5.size());
}

TEST(WordTree_Predict, CanPredictCapitalLetters)
{
    WordTree wordTree;

    wordTree.add("apple");
    wordTree.add("apples");
    wordTree.add("applesauce");
    wordTree.add("applesoranges");
    wordTree.add("helloworld");
    wordTree.add("end");

    const auto predictions1 = wordTree.predict("Appl", 4);
    const auto predictions2 = wordTree.predict("appL", 4);
    const auto predictions3 = wordTree.predict("APPL", 4);

    EXPECT_EQ(4, predictions1.size());
    EXPECT_EQ(4, predictions2.size());
    EXPECT_EQ(4, predictions3.size());

    const auto predictions4 = wordTree.predict("HeLlO", 1);
    const auto predictions5 = wordTree.predict("heLLO", 1);

    EXPECT_EQ(1, predictions4.size());
    EXPECT_EQ(1, predictions5.size());

    const auto predictions6 = wordTree.predict("En", 1);

    EXPECT_EQ(1, predictions6.size());
}

TEST(WordTree_Predict, DictionarySizeLimitsPredictions)
{
    WordTree wordTree;

    wordTree.add("app");
    wordTree.add("apple");
    wordTree.add("apples");
    wordTree.add("applesauce");
    wordTree.add("applesoranges");

    const auto predictions = wordTree.predict("ap", 10);

    EXPECT_EQ(5, predictions.size());
}

TEST(WordTree_Predict, CorrectBreadthFirstPredictions)
{
    WordTree wordTree;

    wordTree.add("apple");
    wordTree.add("apply");
    wordTree.add("bound");
    wordTree.add("boring");
    wordTree.add("horror");
    wordTree.add("exam");
    wordTree.add("exit");
    wordTree.add("exist");
    wordTree.add("exits");
    wordTree.add("extra");
    wordTree.add("excite");
    wordTree.add("expect");
    wordTree.add("execute");
    wordTree.add("exciting");
    wordTree.add("executive");
    wordTree.add("explode");
    wordTree.add("explore");
    wordTree.add("explosion");
    wordTree.add("never");
    wordTree.add("xylophone");
    wordTree.add("zebra");

    const auto predictions = wordTree.predict("ex", 6);

    EXPECT_EQ(6, predictions.size());

    EXPECT_EQ("exam", predictions[0]);
    EXPECT_EQ("exit", predictions[1]);
    EXPECT_EQ("exist", predictions[2]);
    EXPECT_EQ("exits", predictions[3]);
    EXPECT_EQ("extra", predictions[4]);
    EXPECT_EQ("excite", predictions[5]);
}