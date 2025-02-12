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

// GatherTokens TEST CASES
TEST(GatherTokens, LeadingSpace){
  string text = "                 Hello my naMe is ALeX";
  set<string> expected = {"hello", "my", "name", "is", "alex"};

  EXPECT_THAT(gatherTokens(text), ContainerEq(expected))
  << "text=\"" << text << "\"";
}

TEST(GatherTokens, TrailingSpaces){
  string text = "Hello my naMe is ALeX                  ";
  set<string> expected = {"hello", "my", "name", "is", "alex"};

  EXPECT_THAT(gatherTokens(text), ContainerEq(expected))
  << "text=\"" << text << "\"";
}

TEST(GatherTokens, ManySpaces){
  string text = "Hello   my      naMe    is ALeX";
  set<string> expected = {"hello", "my", "name", "is", "alex"};

  EXPECT_THAT(gatherTokens(text), ContainerEq(expected))
  << "text=\"" << text << "\"";
}

// buildIndex TEST CASES


// check that the output of running buildIndex results in an int >= 1 bc that means that atleast one url was read
TEST(BuildIndex, NoFile){
    string filename = "idontexist.txt"
    string f_expected = not
    string retTestFeedback =
      "buildIndex(\"" + filename + "\", ...) file not found \n";
    EXPECT_THAT(filename, Eq())    
}

TEST(BuildIndex, TinyTxt) {
  string filename = "data/tiny.txt";

  map<string, set<string>> expectedIndex = {
      {"gre-en", {"www.rainbow.org"}},
      {"orange", {"www.rainbow.org"}},
      {"yellow", {"www.rainbow.org"}},
      {"indigo", {"www.rainbow.org"}},
      {"violet", {"www.rainbow.org"}},

      {"blue", {"www.dr.seuss.net","www.rainbow.org"}},
      {"red", {"www.dr.seuss.net", "www.rainbow.org"}},
      {"one", {"www.dr.seuss.net"}},
      {"two", {"www.dr.seuss.net"}},
      {"fish", {"www.dr.seuss.net", "www.shoppinglist.com"}},

      {"eggs", {"www.shoppinglist.com"}},
      {"milk", {"www.shoppinglist.com"}},
      {"bread", {"www.shoppinglist.com"}},
      {"cheese", {"www.shoppinglist.com"}},

      {"i'm", {"www.bigbadwolf.com"}},
      {"not", {"www.bigbadwolf.com"}},
      {"trying", {"www.bigbadwolf.com"}},
      {"to", {"www.bigbadwolf.com"}},
      {"eat", {"www.bigbadwolf.com"}},
      {"you", {"www.bigbadwolf.com"}}
  };
  map<string, set<string>> studentIndex;
  int studentNumProcesed = buildIndex(filename, studentIndex);

  string indexTestFeedback =
      "buildIndex(\"" + filename + "\", ...) index incorrect\n";
  EXPECT_THAT(studentIndex, ContainerEq(expectedIndex)) << indexTestFeedback;

  string retTestFeedback =
      "buildIndex(\"" + filename + "\", ...) return value incorrect\n";
  EXPECT_THAT(studentNumProcesed, Eq(4)) << retTestFeedback;
}



}  // namespace