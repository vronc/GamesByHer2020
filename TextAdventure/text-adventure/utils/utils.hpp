
#pragma once

#include <string>
#include <vector>
#include "../json/single_include/nlohmann/json.hpp"

bool isPositiveIntInput(std::string input);
int getRandomPosInt(int min, int max);
std::vector<std::string> getRandomSelection(std::vector<std::string> v, int numSamples);

struct node
{
    nlohmann::json data;
    struct node *next;
};
