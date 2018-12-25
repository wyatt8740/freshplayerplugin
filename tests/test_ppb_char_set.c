#include "nih_test.h"
#include <src/ppb_char_set.c>
#include <src/ppb_var.h>
#include <src/tables.h>
#include <stdio.h>
#include <string.h>

static
void *
t_memalloc(uint32_t sz)
{
    return malloc(sz);
}

TESTSUITE_SETUP(void)
{
    // npn is globally visible struct
    npn.memalloc = t_memalloc;
    npn.memfree = free;
}

TEST(ppb_char_set, extract_relevant_part_from_locale_name)
{
    char *s;

    s = extract_relevant_part_from_locale_name("ru_RU.UTF-8");
    ASSERT_STREQ(s, "ru");
    free(s);

    s = extract_relevant_part_from_locale_name("en_US");
    ASSERT_STREQ(s, "en");
    free(s);

    s = extract_relevant_part_from_locale_name("zh_CN.utf8");
    ASSERT_STREQ(s, "zh-CN");
    free(s);

    s = extract_relevant_part_from_locale_name("zh_TW.utf8");
    ASSERT_STREQ(s, "zh-TW");
    free(s);
}

TEST(ppb_char_set, to_utf16_all_ASCII)
{
    const char *in = "Hello, world!";
    const uint16_t out[] = {'H', 'e', 'l', 'l', 'o', ',', ' ',
                            'w', 'o', 'r', 'l', 'd', '!'};
    uint32_t res_len = 7777;
    uint16_t *res = ppb_char_set_char_set_to_utf16(0, in, strlen(in), "UTF-8",
                                                   PP_CHARSET_CONVERSIONERROR_FAIL, &res_len);
    ASSERT_EQ(sizeof(out) / sizeof(uint16_t), res_len);
    ASSERT_EQ(memcmp(res, out, sizeof(out)), 0);
    free(res);
}

TEST(ppb_char_set, to_utf16_basic_UTF_8)
{
    const char *in = "Привет, мир!";
    const uint16_t out[] = {0x41f, 0x440, 0x438, 0x432, 0x435, 0x442,
                            0x2c,  0x20,  0x43c, 0x438, 0x440, 0x21};
    uint32_t res_len = 7777;
    uint16_t *res = ppb_char_set_char_set_to_utf16(0, in, strlen(in), "UTF-8",
                                                   PP_CHARSET_CONVERSIONERROR_FAIL, &res_len);
    ASSERT_EQ(sizeof(out) / sizeof(uint16_t), res_len);
    ASSERT_EQ(memcmp(res, out, sizeof(out)), 0);
    free(res);
}

TEST(ppb_char_set, to_utf16_wrong_UTF_8_with_error)
{
    const char *in = "Hel\x80lo";
    uint32_t res_len = 7777;
    uint16_t *res = ppb_char_set_char_set_to_utf16(0, in, strlen(in), "UTF-8",
                                                   PP_CHARSET_CONVERSIONERROR_FAIL, &res_len);
    ASSERT_EQ(res_len, 0);
    ASSERT_EQ(res, NULL);
    free(res);
}

// testing PP_CHARSET_CONVERSIONERROR_SKIP or PP_CHARSET_CONVERSIONERROR_SUBSTITUTE
// have no much sense since every unicode character can be represented in UTF-16

TEST(ppb_char_set, from_utf16_all_ASCII)
{
    const uint16_t in[] = {'H', 'e', 'l', 'l', 'o', ',', ' ', 'w', 'o', 'r', 'l', 'd', '!'};
    const char *out = "Hello, world!";
    uint32_t res_len = 7777;
    char *res = ppb_char_set_utf16_to_char_set(0, (const uint16_t *)in,
                                               sizeof(in) / sizeof(uint16_t), "cp1251",
                                               PP_CHARSET_CONVERSIONERROR_FAIL, &res_len);
    ASSERT_EQ(strlen(out), res_len);
    ASSERT_EQ(memcmp(res, out, res_len), 0);
    free(res);
}

TEST(ppb_char_set, to_utf16_non_ASCII_all_correct)
{
    const uint16_t in[] = {0x41f, 0x440, 0x438, 0x432, 0x435, 0x442, 0x2c,
                           0x20,  0x43c, 0x438, 0x440, 0x21};  // "Привет, мир!"
    const char *out = "\xcf\xf0\xe8\xe2\xe5\xf2\x2c\x20\xec\xe8\xf0\x21"; // "Привет, мир!"
    uint32_t res_len = 7777;
    char *res = ppb_char_set_utf16_to_char_set(0, (const uint16_t *)in,
                                               sizeof(in) / sizeof(uint16_t), "cp1251",
                                               PP_CHARSET_CONVERSIONERROR_FAIL, &res_len);
    ASSERT_EQ(strlen(out), res_len);
    ASSERT_EQ(memcmp(res, out, res_len), 0);
    free(res);
}

TEST(ppb_char_set, to_utf16_non_ASCII_PP_CHARSET_CONVERSIONERROR_FAIL)
{
    const uint16_t in[] = {0x41f, 0x440,  0x438, 0x432, 0x435, 0x442, 0x2c,
                           0x20,  0x266b, 0x43c, 0x438, 0x440, 0x21};
    // "♫" in "Привет, ♫мир!" cannot be represented in cp1251.
    // const char *out = "\xcf\xf0\xe8\xe2\xe5\xf2\x2c\x20\xec\xe8\xf0\x21"; // "Привет, мир!"
    uint32_t res_len = 7777;
    char *res = ppb_char_set_utf16_to_char_set(0, (const uint16_t *)in,
                                               sizeof(in) / sizeof(uint16_t), "cp1251",
                                               PP_CHARSET_CONVERSIONERROR_FAIL, &res_len);
    ASSERT_EQ(res_len, 0);
    ASSERT_EQ(res, NULL);
    free(res);
}

TEST(ppb_char_set, to_utf16_non_ASCII_PP_CHARSET_CONVERSIONERROR_SKIP)
{
    const uint16_t in[] = {
        0x41f, 0x440,  0x438, 0x432, 0x435, 0x442, 0x2c,
        0x20,  0x266b, 0x43c, 0x438, 0x440, 0x21};  // "Привет, ♫мир!"
    const char *out = "\xcf\xf0\xe8\xe2\xe5\xf2\x2c\x20\xec\xe8\xf0\x21"; // "Привет, мир!"
    uint32_t res_len = 7777;
    char *res = ppb_char_set_utf16_to_char_set(0, (const uint16_t *)in,
                                               sizeof(in) / sizeof(uint16_t), "cp1251",
                                               PP_CHARSET_CONVERSIONERROR_SKIP, &res_len);
    ASSERT_EQ(strlen(out), res_len);
    ASSERT_EQ(memcmp(res, out, res_len), 0);
    free(res);
}

TEST(ppb_char_set, to_utf16_non_ASCII_PP_CHARSET_CONVERSIONERROR_SUBSTITUTE)
{
    const uint16_t in[] = {
        0x41f, 0x440,  0x438, 0x432, 0x435, 0x442, 0x2c,
        0x20,  0x266b, 0x43c, 0x438, 0x440, 0x21};  // "Привет, ♫мир!"
    const char *out = "\xcf\xf0\xe8\xe2\xe5\xf2\x2c\x20\x3f\xec\xe8\xf0\x21";// "Привет, ?мир!"
    uint32_t res_len = 7777;
    char *res = ppb_char_set_utf16_to_char_set(0, (const uint16_t *)in,
                                               sizeof(in) / sizeof(uint16_t), "cp1251",
                                               PP_CHARSET_CONVERSIONERROR_SUBSTITUTE, &res_len);
    ASSERT_EQ(strlen(out), res_len);
    ASSERT_EQ(memcmp(res, out, res_len), 0);
    free(res);
}

TEST(ppb_char_set, gb2312_ASCII_to_utf16)
{
    // gb2312 is a variable-length encoding, with ASCII encoded as is

    // "Hello, world!" in ASCII
    const uint8_t in[] = {0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x2c, 0x20, 0x77, 0x6f, 0x72,
                          0x6c, 0x64, 0x21};

    // "Hello, world!" in UTF16-LE
    const uint16_t out[] = {0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x2c, 0x20,
                            0x77, 0x6f, 0x72, 0x6c, 0x64, 0x21};

    uint32_t res_len = 7777;
    uint16_t *res = ppb_char_set_char_set_to_utf16(0, (const char *)in, sizeof(in), "gb2312",
                                                   PP_CHARSET_CONVERSIONERROR_FAIL, &res_len);
    ASSERT_EQ(res_len, sizeof(out) / sizeof(uint16_t));
    ASSERT_EQ(memcmp(res, out, sizeof(out)), 0);
    free(res);
}
