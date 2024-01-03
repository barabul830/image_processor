#include "filters.h"

std::vector<std::vector<std::string>> ParseAll(int argc, char** argv);
std::pair<int, std::vector<std::string>> ParseFilter(int argc, char** argv, int cur);
void Control(Image& image, std::vector<std::string> par);
