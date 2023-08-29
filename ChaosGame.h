#ifndef CHAOS_GAME_HH
#define CHAOS_GAME_HH

#include <vector>

float* chaos_game(std::vector<float> shape, const float* seed, float jump, size_t num_points);

float* chaos_game_restricted(std::vector<float> shape, const float* seed, float jump, size_t num_points);

#endif