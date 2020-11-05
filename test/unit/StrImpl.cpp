#include "gtest/gtest.h"

extern "C" {
#include "Str.h"
#include "string.h"
}


TEST(StrImpl, value) {
    Str s = Str_value(2);
    ASSERT_EQ(1, s.length); // Vec's length includes null char
    ASSERT_EQ(3, s.capacity); // Requested space + 1 for null char
    char *buffer = (char*) s.buffer;
    ASSERT_EQ('\0', buffer[0]);
    Str_drop(&s);
}

TEST(StrImpl, drop) {
    Str s = Str_value(2);
    Str_drop(&s);
    ASSERT_EQ(0, s.capacity);
    ASSERT_EQ(0, s.length);
    ASSERT_EQ(nullptr, s.buffer);
}

/**
 * Helper fixture to setup a Str whose buffer
 * is prefilled with "abcd". 
 */
Str fixture_abcd()
{
    Str s = Str_value(4);
    char *buffer = (char*) s.buffer;
    buffer[0] = 'a';
    buffer[1] = 'b';
    buffer[2] = 'c';
    buffer[3] = 'd';
    buffer[4] = '\0';
    s.length = 5; // Includes null char
    return s;
}

TEST(StrImpl, length) {
    Str s = Str_value(2);
    ASSERT_EQ(0, Str_length(&s));
    Str_drop(&s);

    s = fixture_abcd();
    ASSERT_EQ(4, Str_length(&s));
    Str_drop(&s);
}

TEST(StrImpl, cstr) {
    Str s = fixture_abcd();
    ASSERT_STREQ("abcd", Str_cstr(&s));
    Str_drop(&s);
}

TEST(StrImpl, ref) {
    Str s = fixture_abcd();
    char *buffer = (char*) s.buffer;
    for (size_t i = 0; i < s.length; ++i) {
        ASSERT_EQ(&buffer[i], Str_ref(&s, i));
    }
    Str_drop(&s);
}

TEST(StrImpl, fromWrongInputTest) {
   Str v = fixture_abcd();
    Str u = Str_from("abcde"); 
    char *buffer = (char*) u.buffer;
    char *buffer2 = (char*) v.buffer;
    ASSERT_NE(buffer, buffer2);

}// TODO: Test remaining Str functions


TEST(StrImpl, fromTest) {
   Str v = fixture_abcd();
    Str u = Str_from("abcd"); 
    char *buffer = (char*) u.buffer;
    char *buffer2 = (char*) v.buffer;
    ASSERT_STREQ(buffer, buffer2);
}

TEST(StrGrader, splice_switch)
{
    Str str = Str_from("dadand");
    const char *insert = "mom";
    Str_splice(&str, 0, 0, insert, strlen(insert));

    const char *expected = "momdadand";
    ASSERT_STREQ(expected, Str_cstr(&str));
    ASSERT_EQ(strlen(expected), Str_length(&str));
    Str_drop(&str);
}

TEST(StrGrader, splice_append1)
{
    Str str = Str_from("before&");
    const char *insert = "after";
    Str_splice(&str, 7, 0, insert, strlen(insert));

    const char *expected = "before&after";
    ASSERT_STREQ(expected, Str_cstr(&str));
    ASSERT_EQ(strlen(expected), Str_length(&str));
    Str_drop(&str);
}

TEST(StrGrader, splice_insertInMiddle)
{
    Str str = Str_from("abcdcba");
    const char *insert = "1234";
    Str_splice(&str, 3, 0, insert, strlen(insert));

    const char *expected = "abc1234dcba";
    ASSERT_STREQ(expected, Str_cstr(&str));
    ASSERT_EQ(strlen(expected), Str_length(&str));
    Str_drop(&str);
}

TEST(StrGrader, splice_delete2)
{
    Str str = Str_from("1234567");
    Str_splice(&str, 3, 2, NULL, 0);

    const char *expected = "12367";
    ASSERT_STREQ(expected, Str_cstr(&str));
    ASSERT_EQ(strlen(expected), Str_length(&str));
    Str_drop(&str);
}

TEST(StrGrader, splice_replace_eqLen)
{
    Str str = Str_from("1234567");
    const char *insert = "bob";
    Str_splice(&str, 2, 3, insert, strlen(insert));

    const char *expected = "12bob67";
    ASSERT_STREQ(expected, Str_cstr(&str));
    ASSERT_EQ(strlen(expected), Str_length(&str));
    Str_drop(&str);
}

TEST(StrGrader, splice_replace_longerLen)
{
    Str str = Str_from("1234567");
    const char *insert = "job";
    Str_splice(&str, 2, 4, insert, strlen(insert));

    const char *expected = "12job7";
    ASSERT_STREQ(expected, Str_cstr(&str));
    ASSERT_EQ(strlen(expected), Str_length(&str));
    Str_drop(&str);
}

TEST(StrGrader, splice_replace_longerIns)
{
    Str str = Str_from("1234567");
    const char *insert = "tot";
    Str_splice(&str, 2, 1, insert, strlen(insert));

    const char *expected = "12tot4567";
    ASSERT_STREQ(expected, Str_cstr(&str));
    ASSERT_EQ(strlen(expected), Str_length(&str));
    Str_drop(&str);
}


TEST(StrImpl, str_spliceWzeroParams) {
    Str v = Str_value(3);
    char *buffer = (char*) v.buffer;
    buffer[0] = 'a';
    buffer[1] = 'b';
    buffer[2] = 'c';

    v.length = 3;
    char items[] = {'h', 'i'};
    Str_splice(&v, 0, 0, items, 2);
    char *bufferAgain = (char*) v.buffer;
    Str fin = Str_value(5);
    char *result = (char*)fin.buffer;
    result[0] = 'h';
    result[1] = 'i';
    result[2] = 'a';
    result[3] = 'b';
    result[4] = 'c';
    result[5] = '\0';
    ASSERT_STREQ(bufferAgain, result);
}

TEST(StrImpl, spliceWparams) {
 
    Str v = Str_value(3);
    char *buffer = (char*) v.buffer;
    buffer[0] = 'a';
    buffer[1] = 'b';
    buffer[2] = 'c';

    v.length = 3;
    char items[] = {'h', 'i'};
    Str_splice(&v, 1, 2, items, 2);
    char *bufferAgain = (char*) v.buffer;
    Str fin = Str_value(3);
    char *result = (char*)fin.buffer;
    result[0] = 'a';
    result[1] = 'h';
    result[2] = 'i';
    result[3] = '\0';
    ASSERT_STREQ(bufferAgain, result);
}

TEST(StrImpl, append) {
    Str s = Str_from("abcde");
    const char *append = "123";
    Str_append(&s, append);
    
    const char *expected = "abcde123";
    ASSERT_STREQ(expected, Str_cstr(&s));
    ASSERT_EQ(strlen(expected),Str_length(&s));
    Str_drop(&s); 

}

TEST(StrImpl, getMultiple) {
    Str s = Str_value(2);
    char *buffer = (char*) s.buffer;
    buffer[0] = 'a';
    buffer[1] = 'b';
    s.length = 2;
    char get = Str_get(&s, 1);
    char get0 = Str_get(&s, 0);
    ASSERT_EQ(get, buffer[1]); 
    ASSERT_EQ(get0, buffer[0]); 
}

TEST(StrImpl, getSingle) {
    Str s = Str_value(2);
    char *buffer = (char*) s.buffer;
    buffer[0] = 'a';
    s.length = 1;
    char get = Str_get(&s, 0);
    ASSERT_EQ(get, buffer[0]);  
}

TEST(StrImpl, getOverBounds) {
    Str s = Str_value(2);
    char *buffer = (char*) s.buffer;
    buffer[0] = 'a';
    buffer[1] = 'b';
    s.length = 2;
    ASSERT_DEATH({ Str_ref(&s, 3);
    }, ".* - Out of Bounds");
     
}


TEST(StrImpl, getNegativeBounds) {
    Str s = Str_value(2);
    char *buffer = (char*) s.buffer;
    buffer[0] = 'a';
    buffer[1] = 'b';
    s.length = 2;
    ASSERT_DEATH({ Str_ref(&s, -1);
    }, ".* - Out of Bounds");
     
}

TEST(StrImpl, set) {
    Str s = fixture_abcd();
    char c = 'c';
    Str_set(&s, 1, c);
    char *bufferAgain = (char*) s.buffer;
    ASSERT_EQ(bufferAgain[1], c);
}

TEST(StrImpl, setOverBounds) {
    Str s = fixture_abcd();
    char c = 'c';
    //char *bufferAgain = (char*) s.buffer;
    ASSERT_DEATH({ Str_set(&s, 6, c);
    }, ".* - Out of Bounds");
     
}

TEST(StrImpl, setNegativeBounds) {
    Str s = fixture_abcd();
    char c = 'c';
    //char *bufferAgain = (char*) s.buffer;
    ASSERT_DEATH({ Str_set(&s, -1, c);
    }, ".* - Out of Bounds");
     
}

TEST(StrImpl, SetIndexIsLen) {
    Str s = fixture_abcd();
    char c = 'c';
    Str_set(&s, 4, c);
    char *bufferAgain = (char*) s.buffer;
    ASSERT_EQ(bufferAgain[4], c);
    char d = '\0';
    ASSERT_EQ(bufferAgain[5], d);
}
