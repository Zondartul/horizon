#ifndef MORESTRINGS_GUARD
#define MORESTRINGS_GUARD
unsigned int explode(const string &txt, vector<string> &strs, char ch);
string itoa(double I);
std::string operator+(std::string const &a, int b);
std::string operator+(std::string const &a, double b);
string tolower(string A);
string toupper(string A);
string fstring(const char* fmt, ...);
string translateEnum(uint32_t E, string type); //provide the enum and the name of the function returning it
												//a string representation of that enum will be returned	
#endif