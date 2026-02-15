#include "token_io.h"

#include <string>
#include <istream>
#include <sstream>
#include <regex>
using namespace std;


string read_line(istream& input)
{
    string line;
    getline(input, line, '\n');

    regex remove_comma(",");
    line = regex_replace(line, remove_comma, " ");
    regex trim_ws("^\\s+|\\s+$");
    line = regex_replace(line, trim_ws, "");
    regex pack_ws("\\s+");
    line = regex_replace(line, pack_ws, " ");

    return line;
}


string peek_first(string input)
{
    return extract_token(input);
}


string peek_second(string input)
{
    extract_token(input);
    return extract_token(input);
}


string extract_token(string& input)
{
    char delim = ' ';

    regex remove_comment(";.*");
    input = regex_replace(input, remove_comment, " ");
    regex remove_comma(",");
    input = regex_replace(input, remove_comma, " ");
    regex trim_ws("^\\s+|\\s+$");
    input = regex_replace(input, trim_ws, "");
    regex pack_ws("\\s+");
    input = regex_replace(input, pack_ws, " ");
    
    string token;
    stringstream stream(input);
    getline(stream, token, delim);
    token = regex_replace(token, trim_ws, "");

    regex trim_delim(string("^.*?(") + delim + "|$)");
    input = regex_replace(input, trim_delim, "");
    input = regex_replace(input, trim_ws, " ");
    
    return token;
}


string extract_token_reverse(string& input)
{
    input = string(input.rbegin(), input.rend());
    string token = extract_token(input);

    input = string(input.rbegin(), input.rend());
    token = string(token.rbegin(), token.rend());
    
    return token;
}


void strip(string& str, const string& symbols)
{
    auto cut = str.find_first_not_of(symbols);
    cut = (cut == string::npos) ? str.size() : cut;
    str.erase(0, cut);
    
    cut = str.find_last_not_of(symbols);
    cut = (cut == string::npos) ? str.size() : cut+1;
    str.erase(cut, str.size());
}
