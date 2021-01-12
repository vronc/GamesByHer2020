//
//  utils.cpp
//  text-adventure
//
//  Created by Veronica Hage on 2021-01-09.
//  Copyright © 2021 Veronica Hage. All rights reserved.
//

#include <string>
#include <stdio.h>
#include "utils.hpp"
#include <random>
#include <iostream>

std::mt19937 randomGenerator = std::mt19937(std::time(NULL));

bool isNumber(std::string s)
{
    for (int i = 0; i < s.length(); i++)
        if (isdigit(s[i]) == false)
            return false;
    
    return true;
}

bool isPositiveIntInput(std::string input) {
    return (!isNumber(input) || std::stoi(input) < 1);
}

int getRandomPosInt(int min, int max) {
    std::uniform_int_distribution<int> distribution(min, max-1);
    return (int)1 + distribution(randomGenerator) % max;
}

// copied from https://stackoverflow.com/questions/33802205/how-to-sample-without-replacement-using-c-uniform-int-distribution
std::vector<std::string> getRandomSelection(std::vector<std::string> v, int numSamples) {
    std::vector<std::string> out;
    std::sample(v.begin(), v.end(), std::back_inserter(out), numSamples, std::mt19937{std::random_device{}()});
    return out;
}
