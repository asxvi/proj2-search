#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "include/search.h"

using namespace std;
using namespace testing;
namespace {
// Tokens without any punctuation or special characters
TEST(CleanToken, NoCleaning) {
  ASSERT_THAT(cleanToken("same"), StrEq("same"));
  ASSERT_THAT(cleanToken("wander"), StrEq("wander"));
  ASSERT_THAT(cleanToken("l33tcode"), StrEq("l33tcode"));
}

// Tokens with punctuation at the beginning
TEST(CleanToken, PrefixCleaning) {
  ASSERT_THAT(cleanToken(".hello"), StrEq("hello"));
  ASSERT_THAT(cleanToken("...hello"), StrEq("hello"));
  ASSERT_THAT(cleanToken(".\"!?hello"), StrEq("hello"));
  ASSERT_THAT(cleanToken(";timesheet"), StrEq("timesheet"));
  ASSERT_THAT(cleanToken(";.!timesheet"), StrEq("timesheet"));
  ASSERT_THAT(cleanToken(".,.!?timesheet"), StrEq("timesheet"));
}

// Tokens with punctuation at the end
TEST(CleanToken, SuffixCleaning) {
  ASSERT_THAT(cleanToken("hello."), StrEq("hello"));
  ASSERT_THAT(cleanToken("hello..."), StrEq("hello"));
  ASSERT_THAT(cleanToken("hello.\"!?"), StrEq("hello"));
  ASSERT_THAT(cleanToken("timesheet;"), StrEq("timesheet"));
  ASSERT_THAT(cleanToken("timesheet;.!"), StrEq("timesheet"));
  ASSERT_THAT(cleanToken("timesheet.,.!?"), StrEq("timesheet"));
}

// Tokens without any letters
TEST(CleanToken, ToEmpty) {
  ASSERT_THAT(cleanToken("23432423"), StrEq(""));
  ASSERT_THAT(cleanToken("2343098765432345678998765434567892423"), StrEq(""));
  ASSERT_THAT(cleanToken("....$$$$......"), StrEq(""));
  ASSERT_THAT(cleanToken("....2312^#@@@...."), StrEq(""));
  ASSERT_THAT(cleanToken(""), StrEq(""));
}

// Tokens with uppercase letters and punctuation
TEST(CleanToken, Uppercase) {
  ASSERT_THAT(cleanToken("HELLO."), StrEq("hello"));
  ASSERT_THAT(cleanToken("heLlo..."), StrEq("hello"));
  ASSERT_THAT(cleanToken("hellO.\"!?"), StrEq("hello"));
  ASSERT_THAT(cleanToken(".HELLO"), StrEq("hello"));
  ASSERT_THAT(cleanToken("...Hello"), StrEq("hello"));
  ASSERT_THAT(cleanToken(".\"!?heLLo"), StrEq("hello"));
}

// Tokens with punctuation in beginning and end
TEST(bothPunct, PreNSufCleaning) {
  ASSERT_THAT(cleanToken("!!!HELLO!!!"), StrEq("hello"));
  ASSERT_THAT(cleanToken("911!heLlo0000"), StrEq("hello"));
  ASSERT_THAT(cleanToken("./123hellO.\"!?"), StrEq("hello"));
  ASSERT_THAT(cleanToken(".HELLO."), StrEq("hello"));
  ASSERT_THAT(cleanToken("...Hello..."), StrEq("hello"));
  ASSERT_THAT(cleanToken(".\"!?heLLo!"), StrEq("hello"));
}

// Tokens with punctuation in the actual string to keep
TEST(middlePunct, middleCleaning) {
  ASSERT_THAT(cleanToken("HEL$LO"), StrEq("HEL$LO"));
  ASSERT_THAT(cleanToken("heL???lo"), StrEq("heL???lo"));
  ASSERT_THAT(cleanToken("h?e?l?l?O"), StrEq("h?e?l?l?O"));
  ASSERT_THAT(cleanToken("HE@!@#LL!#!O"), StrEq("HE@!@#LL!#!O"));
  ASSERT_THAT(cleanToken("H@llo"), StrEq("H@llo"));
  ASSERT_THAT(cleanToken("h#eL#Lo"), StrEq("h#eL#Lo"));
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
TEST(oneCharLength, oneLetter) {
  ASSERT_THAT(cleanToken("a"), StrEq("a"));
  ASSERT_THAT(cleanToken("z"), StrEq("z"));
  ASSERT_THAT(cleanToken("1"), StrEq(""));
  ASSERT_THAT(cleanToken("&"), StrEq(""));
  ASSERT_THAT(cleanToken("."), StrEq(""));
}

// Tokens with numbers at the end
TEST(manySymbols, SymbolCleaning) {
  ASSERT_THAT(cleanToken(".......a"), StrEq("a"));
  ASSERT_THAT(cleanToken("z......."), StrEq("z"));
  ASSERT_THAT(cleanToken(".........a........."), StrEq("a"));
  ASSERT_THAT(cleanToken("!@&^*%#!#*%^!#aaa@*(&^!#%^$@#)"), StrEq("aaa"));
  ASSERT_THAT(cleanToken("<<<<<<xyz>>>>>>"), StrEq("xyz"));
}

}  // namespace