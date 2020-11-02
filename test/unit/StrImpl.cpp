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

TEST(StrImpl, fromTest) {
    //const char *t[] = {"a","b","c","d"};

    //char v[] = "abcd";
    Str v = fixture_abcd();
    Str u = Str_from("abcd"); 
    char *buffer = (char*) u.buffer;
    char *buffer2 = (char*) v.buffer;
    //for (size_t i = 0; i < u.length; ++i) {
    //  ASSERT_STREQ(buffer[i], v[i]);
    //
    ASSERT_STREQ(buffer, buffer2);
}// TODO: Test remaining Str functions

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
