#ifndef FILE_IO_H_
#define FILE_IO_H_

#include <string>
#include <istream>
using namespace std;


// Reads the next line from the given input stream.
//
// The empty string "" is returned when:
// * current line is empty or
// * there are no more lines in the input stream
//
string read_line(istream& input);


// Returns the token at the front of the input using whitespace or ',' as delimiter.
// The input is not modified.
//
string peek_first(string input);


// Returns the second token in the input using using whitespace or ',' as delimiter.
// The input is not modified.
//
string peek_second(string input);


// Extracts from the given input the first token using whitespace or ',' as delimiter.
//
// Returns the empty string "" if no token can be found.
//
// The input is modified -- the extracted token is removed from the input.
//
string extract_token(string& input);


// Similar to extract_token, but reads the tokens from the end of the string
string extract_token_reverse(string& input);


// Modifies the given string by removing from its front and end the given symbols.
//
// string str = "$<<hello world>>$";
// strip(str, "$<>");
// str is now "hello world"
//
void strip(string& str, const string& symbols);

#endif
