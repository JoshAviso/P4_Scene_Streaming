#pragma once

#include <random>
#include <ctime>

class Random {
public:
   static float RandFloat() {
       static std::mt19937 generator(static_cast<unsigned int>(std::time(nullptr)));
       std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
       return distribution(generator);
   }

   static float RandFloat(float inclusiveMin, float inclusiveMax) {
       static std::mt19937 generator(static_cast<unsigned int>(std::time(nullptr)));
       std::uniform_real_distribution<float> distribution(inclusiveMin, inclusiveMax);
       return distribution(generator);
   }

   static int RandInt(int inclusiveMin, int inclusiveMax) {
       static std::mt19937 generator(static_cast<unsigned int>(std::time(nullptr)));
       std::uniform_int_distribution<int> distribution(inclusiveMin, inclusiveMax);
       return distribution(generator);
   }
};