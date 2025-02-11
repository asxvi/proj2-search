#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "include/search.h"

using namespace std;
using namespace testing;
namespace {
// Tokens with punctuation in beginning and end
TEST(CleanToken, BothEndsCleaning) {
  ASSERT_THAT(cleanToken("!!!HELLO!!!"), StrEq("hello"));
  ASSERT_THAT(cleanToken("**!heLlo*"), StrEq("hello"));
  ASSERT_THAT(cleanToken("././hellO.\"!?"), StrEq("hello"));
  ASSERT_THAT(cleanToken(".HELLO."), StrEq("hello"));
  ASSERT_THAT(cleanToken("...Hello..."), StrEq("hello"));
  ASSERT_THAT(cleanToken(".\"!?heLLo!!!!"), StrEq("hello"));
}

// Tokens with punct in beginning and middle, but not end
TEST(CleanToken, StartMiddleCleaning) {
  ASSERT_THAT(cleanToken("!!!HE!!o"), StrEq("he!!o"));
  ASSERT_THAT(cleanToken("!a@A"), StrEq("a@a"));
  ASSERT_THAT(cleanToken("!@*&#calCU!ator"), StrEq("calcu!ator"));
  ASSERT_THAT(cleanToken("**uU*U"), StrEq("uu*u"));
  ASSERT_THAT(cleanToken("$$$moN$$eY"), StrEq("mon$$ey"));
}

// Tokens with numbers at the end
TEST(CleanToken, MiddleEndCleaning) {
  ASSERT_THAT(cleanToken("he.ll.o."), StrEq("he.ll.o"));
  ASSERT_THAT(cleanToken("hel#lo..."), StrEq("hel#lo"));
  ASSERT_THAT(cleanToken("S@Vings.\"!?"), StrEq("s@vings"));
  ASSERT_THAT(cleanToken("time$$$heet$$$$$$"), StrEq("time$$$heet"));
  ASSERT_THAT(cleanToken("time@#$sheet@$@#$@#$"), StrEq("time@#$sheet"));
  ASSERT_THAT(cleanToken("t@im@es@he@et@.,.!?"), StrEq("t@im@es@he@et"));
}

// Tokens with punctuation in the actual string to keep
TEST(CleanToken, middleCleaning) {
  ASSERT_THAT(cleanToken("HEL$LO"), StrEq("hel$lo"));
  ASSERT_THAT(cleanToken("heL???lo"), StrEq("hel???lo"));
  ASSERT_THAT(cleanToken("h?e?l?l?O"), StrEq("h?e?l?l?o"));
  ASSERT_THAT(cleanToken("HE@!@#LL!#!O"), StrEq("he@!@#ll!#!o"));
  ASSERT_THAT(cleanToken("H@llo"), StrEq("h@llo"));
  ASSERT_THAT(cleanToken("h#eL#Lo"), StrEq("h#el#lo"));
}

// Tokens with numbers at the end
TEST(CleanToken, SuffixCleaning) {
  ASSERT_THAT(cleanToken("9"), StrEq(""));
  ASSERT_THAT(cleanToken("h9"), StrEq("h9"));
  ASSERT_THAT(cleanToken("hello999"), StrEq("hello999"));
  ASSERT_THAT(cleanToken("ti84"), StrEq("ti84"));
  ASSERT_THAT(cleanToken("goat23"), StrEq("goat23"));
}

// Tokens with numbers at the end
TEST(CleanToken, oneLetter) {
  ASSERT_THAT(cleanToken("a"), StrEq("a"));
  ASSERT_THAT(cleanToken("z"), StrEq("z"));
  ASSERT_THAT(cleanToken("1"), StrEq(""));
  ASSERT_THAT(cleanToken("&"), StrEq(""));
  ASSERT_THAT(cleanToken("."), StrEq(""));
}

// Tokens with numbers at the end
TEST(CleanToken, SymbolCleaning) {
  ASSERT_THAT(cleanToken(".......a"), StrEq("a"));
  ASSERT_THAT(cleanToken("z......."), StrEq("z"));
  ASSERT_THAT(cleanToken(".........a........."), StrEq("a"));
  ASSERT_THAT(cleanToken("!@&^*%#!#*%^!#aAA@*(&^!#%^$@#)"), StrEq("aaa"));
  ASSERT_THAT(cleanToken("<<<<<<XYz>>>>>>"), StrEq("xyz"));
}

TEST(GatherTokens, leadingSpace){
  string text = "   Hello  my naMe   is    ALeX";
  set<string> expected = {"Hello", "my", "name", "is", "alex"};

  EXPECT_THAT(gatherTokens(text), ContainerEq(expected))
  << "text=\"" << text << "\"";
}


TEST(GatherTokens, trailingSpaces){
  string text = "   Hello  my naMe   is    ALeX";
  set<string> expected = {"Hello", "my", "name", "is", "alex"};

  EXPECT_THAT(gatherTokens(text), ContainerEq(expected))
  << "text=\"" << text << "\"";
}

}  // namespace