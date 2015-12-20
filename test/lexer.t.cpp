////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2013 - 2015, Göteborg Bit Factory.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// http://www.opensource.org/licenses/mit-license.php
//
////////////////////////////////////////////////////////////////////////////////

#include <cmake.h>
#include <Lexer.h>
#include <iostream>
#include <vector>
#include <string.h>
#include <test.h>

////////////////////////////////////////////////////////////////////////////////
int main (int, char**)
{
  UnitTest t (124);

  std::vector <std::pair <std::string, Lexer::Type>> tokens;
  std::string token;
  Lexer::Type type;

  // White space detection.
  t.notok (Lexer::isWhitespace (0x0041), "U+0041 (A) ! isWhitespace");
  t.ok (Lexer::isWhitespace (0x0020), "U+0020 isWhitespace");
  t.ok (Lexer::isWhitespace (0x0009), "U+0009 isWhitespace");
  t.ok (Lexer::isWhitespace (0x000A), "U+000A isWhitespace");
  t.ok (Lexer::isWhitespace (0x000B), "U+000B isWhitespace");
  t.ok (Lexer::isWhitespace (0x000C), "U+000C isWhitespace");
  t.ok (Lexer::isWhitespace (0x000D), "U+000D isWhitespace");
  t.ok (Lexer::isWhitespace (0x0085), "U+0085 isWhitespace");
  t.ok (Lexer::isWhitespace (0x00A0), "U+00A0 isWhitespace");
  t.ok (Lexer::isWhitespace (0x1680), "U+1680 isWhitespace"); // 10
  t.ok (Lexer::isWhitespace (0x180E), "U+180E isWhitespace");
  t.ok (Lexer::isWhitespace (0x2000), "U+2000 isWhitespace");
  t.ok (Lexer::isWhitespace (0x2001), "U+2001 isWhitespace");
  t.ok (Lexer::isWhitespace (0x2002), "U+2002 isWhitespace");
  t.ok (Lexer::isWhitespace (0x2003), "U+2003 isWhitespace");
  t.ok (Lexer::isWhitespace (0x2004), "U+2004 isWhitespace");
  t.ok (Lexer::isWhitespace (0x2005), "U+2005 isWhitespace");
  t.ok (Lexer::isWhitespace (0x2006), "U+2006 isWhitespace");
  t.ok (Lexer::isWhitespace (0x2007), "U+2007 isWhitespace");
  t.ok (Lexer::isWhitespace (0x2008), "U+2008 isWhitespace"); // 20
  t.ok (Lexer::isWhitespace (0x2009), "U+2009 isWhitespace");
  t.ok (Lexer::isWhitespace (0x200A), "U+200A isWhitespace");
  t.ok (Lexer::isWhitespace (0x2028), "U+2028 isWhitespace");
  t.ok (Lexer::isWhitespace (0x2029), "U+2029 isWhitespace");
  t.ok (Lexer::isWhitespace (0x202F), "U+202F isWhitespace");
  t.ok (Lexer::isWhitespace (0x205F), "U+205F isWhitespace");
  t.ok (Lexer::isWhitespace (0x3000), "U+3000 isWhitespace");

  // Should result in no tokens.
  Lexer l0 ("");
  t.notok (l0.token (token, type), "'' --> no tokens");

  // Should result in no tokens.
  Lexer l1 ("       \t ");
  t.notok (l1.token (token, type), "'       \\t ' --> no tokens");


  // static bool readWord (const std::string&, const std::string&, std::string::size_type&, std::string&);
  std::string::size_type cursor = 0;
  std::string word;
  t.ok (Lexer::readWord ("'one two'", "'\"", cursor, word), "readWord ''one two'' --> true");
  t.is (word, "'one two'",                                  "  word '" + word + "'");
  t.is ((int)cursor, 9,                                     "  cursor");

  // Unterminated quoted string is invalid.
  cursor = 0;
  t.notok (Lexer::readWord ("'one", "'\"", cursor, word),   "readWord ''one' --> false");

  // static bool readWord (const std::string&, std::string::size_type&, std::string&);
  cursor = 0;
  t.ok (Lexer::readWord ("input", cursor, word),            "readWord 'input' --> true");
  t.is (word, "input",                                      "  word '" + word + "'");
  t.is ((int)cursor, 5,                                     "  cursor");

  cursor = 0;
  t.ok (Lexer::readWord ("one\\ two", cursor, word),        "readWord 'one\\ two' --> true");
  t.is (word, "one two",                                    "  word '" + word + "'");
  t.is ((int)cursor, 8,                                     "  cursor");

  cursor = 0;
  t.ok (Lexer::readWord ("\\u20A43", cursor, word),         "readWord '\\u20A43' --> true");
  t.is (word, "₤3",                                         "  word '" + word + "'");
  t.is ((int)cursor, 7,                                     "  cursor");

  cursor = 0;
  t.ok (Lexer::readWord ("U+20AC4", cursor, word),          "readWord '\\u20AC4' --> true");
  t.is (word, "€4",                                         "  word '" + word + "'");
  t.is ((int)cursor, 7,                                     "  cursor");

  std::string text = "one 'two' three\\ four";
  cursor = 0;
  t.ok (Lexer::readWord (text, cursor, word),               "readWord \"one 'two' three\\ four\" --> true");
  t.is (word, "one",                                        "  word '" + word + "'");
  cursor++;
  t.ok (Lexer::readWord (text, cursor, word),               "readWord \"one 'two' three\\ four\" --> true");
  t.is (word, "'two'",                                      "  word '" + word + "'");
  cursor++;
  t.ok (Lexer::readWord (text, cursor, word),               "readWord \"one 'two' three\\ four\" --> true");
  t.is (word, "three four",                                 "  word '" + word + "'");

  text = "one     ";
  cursor = 0;
  t.ok (Lexer::readWord (text, cursor, word),               "readWord \"one     \" --> true");
  t.is (word, "one",                                        "  word '" + word + "'");

  // Test all Lexer types.
  #define NO {"",Lexer::Type::word}
  struct
  {
    const char* input;
    struct
    {
      const char* token;
      Lexer::Type type;
    } results[5];
  } lexerTests[] =
  {
    // Word
    { "1.foo.bar",                                    { { "1.foo.bar",                                    Lexer::Type::word         }, NO, NO, NO, NO }, },

    // String
    { "'one two'",                                    { { "'one two'",                                    Lexer::Type::string       }, NO, NO, NO, NO }, },
    { "\"three\"",                                    { { "\"three\"",                                    Lexer::Type::string       }, NO, NO, NO, NO }, },
    { "'\\''",                                        { { "'''",                                          Lexer::Type::string       }, NO, NO, NO, NO }, },
    { "\"\\\"\"",                                     { { "\"\"\"",                                       Lexer::Type::string       }, NO, NO, NO, NO }, },
    { "\"\tfoo\t\"",                                  { { "\"\tfoo\t\"",                                  Lexer::Type::string       }, NO, NO, NO, NO }, },
    { "\"\\u20A43\"",                                 { { "\"₤3\"",                                       Lexer::Type::string       }, NO, NO, NO, NO }, },
    { "\"U+20AC4\"",                                  { { "\"€4\"",                                       Lexer::Type::string       }, NO, NO, NO, NO }, },
  };
  #define NUM_TESTS (sizeof (lexerTests) / sizeof (lexerTests[0]))

  for (unsigned int i = 0; i < NUM_TESTS; i++)
  {
    // The isolated test puts the input string directly into the Lexer.
    Lexer isolated (lexerTests[i].input);

    for (int j = 0; j < 5; j++)
    {
      if (lexerTests[i].results[j].token[0])
      {
        // Isolated: "<token>"
        t.ok (isolated.token (token, type),                  "Isolated Lexer::token(...) --> true");
        t.is (token, lexerTests[i].results[j].token,         "  token --> " + token);
        t.is ((int)type, (int)lexerTests[i].results[j].type, "  type --> Lexer::Type::" + Lexer::typeToString (type));
      }
    }

    // The embedded test surrounds the input string with a space.
    Lexer embedded (std::string (" ") + lexerTests[i].input + " ");

    for (int j = 0; j < 5; j++)
    {
      if (lexerTests[i].results[j].token[0])
      {
        // Embedded: "<token>"
        t.ok (embedded.token (token, type),                  "Embedded Lexer::token(...) --> true");
        t.is (token, lexerTests[i].results[j].token,         "  token --> " + token);
        t.is ((int)type, (int)lexerTests[i].results[j].type, "  type --> Lexer::Type::" + Lexer::typeToString (type));
      }
    }
  }

  t.is (Lexer::typeName (Lexer::Type::string),       "string",       "Lexer::typeName (Lexer::Type::string)");
  t.is (Lexer::typeName (Lexer::Type::word),         "word",         "Lexer::typeName (Lexer::Type::word)");

  // std::string Lexer::trimLeft (const std::string& in, const std::string&)
  t.is (Lexer::trimLeft (""),                     "",            "Lexer::trimLeft '' -> ''");
  t.is (Lexer::trimLeft ("   "),                  "",            "Lexer::trimLeft '   ' -> ''");
  t.is (Lexer::trimLeft ("",              " \t"), "",            "Lexer::trimLeft '' -> ''");
  t.is (Lexer::trimLeft ("xxx"),                  "xxx",         "Lexer::trimLeft 'xxx' -> 'xxx'");
  t.is (Lexer::trimLeft ("xxx",           " \t"), "xxx",         "Lexer::trimLeft 'xxx' -> 'xxx'");
  t.is (Lexer::trimLeft ("  \t xxx \t  "),        "\t xxx \t  ", "Lexer::trimLeft '  \\t xxx \\t  ' -> '\\t xxx \\t  '");
  t.is (Lexer::trimLeft ("  \t xxx \t  ", " \t"), "xxx \t  ",    "Lexer::trimLeft '  \\t xxx \\t  ' -> 'xxx \\t  '");

  // std::string Lexer::trimRight (const std::string& in, const std::string&)
  t.is (Lexer::trimRight (""),                     "",            "Lexer::trimRight '' -> ''");
  t.is (Lexer::trimRight ("   "),                  "",            "Lexer::trimRight '   ' -> ''");
  t.is (Lexer::trimRight ("",              " \t"), "",            "Lexer::trimRight '' -> ''");
  t.is (Lexer::trimRight ("xxx"),                  "xxx",         "Lexer::trimRight 'xxx' -> 'xxx'");
  t.is (Lexer::trimRight ("xxx",           " \t"), "xxx",         "Lexer::trimRight 'xxx' -> 'xxx'");
  t.is (Lexer::trimRight ("  \t xxx \t  "),        "  \t xxx \t", "Lexer::trimRight '  \\t xxx \\t  ' -> '  \\t xxx \\t'");
  t.is (Lexer::trimRight ("  \t xxx \t  ", " \t"), "  \t xxx",    "Lexer::trimRight '  \\t xxx \\t  ' -> '  \\t xxx'");

  // std::string Lexer::trim (const std::string& in, const std::string& t)
  t.is (Lexer::trim (""),                     "",          "Lexer::trim '' -> ''");
  t.is (Lexer::trim ("   "),                  "",          "Lexer::trim '   ' -> ''");
  t.is (Lexer::trim ("",              " \t"), "",          "Lexer::trim '' -> ''");
  t.is (Lexer::trim ("xxx"),                  "xxx",       "Lexer::trim 'xxx' -> 'xxx'");
  t.is (Lexer::trim ("xxx",           " \t"), "xxx",       "Lexer::trim 'xxx' -> 'xxx'");
  t.is (Lexer::trim ("  \t xxx \t  "),        "\t xxx \t", "Lexer::trim '  \\t xxx \\t  ' -> '\\t xxx \\t'");
  t.is (Lexer::trim ("  \t xxx \t  ", " \t"), "xxx",       "Lexer::trim '  \\t xxx \\t  ' -> 'xxx'");

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
