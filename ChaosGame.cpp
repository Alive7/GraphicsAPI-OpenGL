#include "ChaosGame.h"

#include <cstdlib>
#include <ctime>
#include <cmath>

float* chaos_game(std::vector<float> shape, const float* seed, float jump, size_t num_points) {
    size_t n_vs = shape.size() / 3;
    float v[]{ seed[0], seed[1], seed[2] };
    float* points = new float[3 * num_points];
    std::srand(std::time(0));

    size_t cutoff = 50;
    for (size_t p = 0; p < 3 * num_points + 3 * cutoff; p += 3) {
        size_t vertex = rand() % n_vs;
        for (size_t i = 0; i < 3; i++) {
            // remove inital transient points
            if (p >= 3 * cutoff) points[p + i - 3 * cutoff] = v[i];
            v[i] = (v[i] + shape[i + 3 * vertex]) * jump;
        }
    }

    return points;
}

float* chaos_game_restricted(std::vector<float> shape, const float* seed, float jump, size_t num_points) {
    size_t n_vs = shape.size() / 3;
    float v[]{ seed[0], seed[1], seed[2] };
    float* points = new float[3 * num_points];
    std::srand(std::time(0));

    size_t cutoff = 50;
    size_t vs_old[]{-1,-2};
    size_t vertex = -3;
    for (size_t p = 0; p < 3 * num_points + 3 * cutoff; p += 3) {
        vertex = rand() % n_vs;
        if (vs_old[0] == vs_old[1]) {
            while ((vertex + 1) % n_vs == vs_old[0] || vertex == (vs_old[0] + 1) % n_vs) {
                vertex = rand() % n_vs;
            }
        }
        for (size_t i = 0; i < 3; i++) {
            // remove inital transient points
            if (p >= 3 * cutoff) points[p + i - 3 * cutoff] = v[i];
            v[i] = (v[i] + shape[i + 3 * vertex]) * jump;
        }
        vs_old[0] = vs_old[1];
        vs_old[1] = vertex;
    }

    return points;
}