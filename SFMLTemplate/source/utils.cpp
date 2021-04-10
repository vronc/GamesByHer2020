//
//  
//  SpaceRace
//
//  Created by Veronica Hage on 2021-02-07.
//

#include <string>
#include <stdio.h>
#include "utils.h"
#include <random>

std::mt19937 randomGenerator = std::mt19937(std::time(NULL));

bool isNumber(std::string s)
{
    for (int i = 0; i < s.length(); i++)
        if (isdigit(s[i]) == false)
            return false;
    
    return true;
}

int getRandomPosInt(int min, int max) {
    std::uniform_int_distribution<int> distribution(min, max-1);
    return (int)1 + distribution(randomGenerator) % max;
}

float getRandomNormalized() {
    std::uniform_int_distribution<int> distribution(-1, 1);
    return distribution(randomGenerator);
}
