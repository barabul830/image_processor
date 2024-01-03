#include "pars_control.h"

std::pair<int, std::vector<std::string>> ParseFilter(int argc, char** argv, int cur) {
    std::vector<std::string> answer;
    answer.push_back(argv[cur]);
    cur += 1;
    while (cur < argc && argv[cur][0] != '-') {
        answer.push_back(argv[cur]);
        ++cur;
    }

    return std::pair(cur, answer);
}

std::vector<std::vector<std::string>> ParseAll(int argc, char** argv) {
    std::vector<std::vector<std::string>> answer;
    int cur = 3;
    while (cur < argc) {
        std::pair<int, std::vector<std::string>> current = ParseFilter(argc, argv, cur);
        cur = current.first;
        answer.push_back(current.second);
    }
    return answer;
}

void Control(Image& image, std::vector<std::string> par) {
    if (par[0] == "-crop") {
        if (par.size() >= 4) {
            throw std::runtime_error("wrong param");
        }
        int width = std::stoi(par[1]);
        int height = std::stoi(par[2]);
        Crop new_filter(image, width, height);
    } else if (par[0] == "-gs") {
        Grayscale new_filter(image);
    } else if (par[0] == "-neg") {
        Negative new_filter(image);
    } else if (par[0] == "-sharp") {
        Sharp new_filter(image);
    } else if (par[0] == "-edge") {
        float threshold = std::stof(par[1]);
        Edge new_filter(image, threshold);
    } else if (par[0] == "-blur") {
        float sigma = std::stof(par[1]);
        Blur new_filter(image, sigma);
    } else if (par[0] == "-crys") {
        int size = std::stoi(par[1]);
        Crystal new_filter(image, size);
    }
}