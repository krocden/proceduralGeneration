#include "PerlinNoise.h"

#include <vector>
#include <iostream>

using namespace std;

PerlinNoise::PerlinNoise(int mapSize, int octaves, float maxHeight, float scale, float persistence, float lacunarity) {
    this->mapSize = mapSize;
    this->octaves = octaves;
    this->amplitude = maxHeight;
    this->scale = scale;
    this->persistence = persistence;
    this->lacunarity = lacunarity;
}

vector<float> PerlinNoise::generate(int offsetX, int offsetY) {
    
    vector<int> table = getPermutationTable();
    vector<float> noise;
    vector<float> finalValue;
    
    float amplitude  = 1;
    float maxHeight = 0;
    float frequency = 1;

    for (int i = 0; i < octaves; i++) {
        maxHeight += amplitude;
        amplitude *= persistence;
    }

    for (int i = 0; i < mapSize; i++) {
        for (int j = 0; j < mapSize; j++) {
            amplitude  = 1;
            frequency = 1;
            float noiseHeight = 0;

            for (int k = 0; k < octaves; k++) {
                float s1 = (i + offsetY * (mapSize - 1)) / scale * frequency;
                float s2 = (j + offsetX * (mapSize - 1))  / scale * frequency;
                
                float perlinValue = perlinNoise(s1, s2, table);
                noiseHeight += perlinValue * amplitude;
                
                amplitude  *= persistence;
                frequency *= lacunarity;
            }
            
            noise.push_back(noiseHeight);
        }
    }

    for (int i = 0; i < mapSize; i++) {
        for (int j = 0; j < mapSize; j++) {
            finalValue.push_back((noise[j + i * mapSize] + 1) / maxHeight);
        }
    }

    return finalValue;
}

double PerlinNoise::fade(double t) { 
    return t * t * t * (t * (t * 6 - 15) + 10); 
};
    
double PerlinNoise::lerp(double x, double a, double b) { 
    return a + x * (b - a); 
}
    
double PerlinNoise::grad(int hash, double x, double y, double z) {
   int h = hash & 15;
   double u = h < 8 ? x : y,
          v = h < 4 ? y : h == 12 || h == 14 ? x : z;
          
   return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}
    
double PerlinNoise::perlinNoise(float x, float y, vector<int> &p) {
    int z = 0.5;
    
    int X = (int)floor(x) & 255,
        Y = (int)floor(y) & 255,
        Z = (int)floor(z) & 255;
    x -= floor(x);
    y -= floor(y);
    z -= floor(z);
    double u = fade(x),
           v = fade(y),
           w = fade(z);
    int A = p[X  ]+Y, AA = p[A]+Z, AB = p[A+1]+Z,
        B = p[X+1]+Y, BA = p[B]+Z, BB = p[B+1]+Z;

    return lerp(w, lerp(v, lerp(u, grad(p[AA  ], x  , y  , z   ),
                                   grad(p[BA  ], x-1, y  , z   )),
                           lerp(u, grad(p[AB  ], x  , y-1, z   ),
                                   grad(p[BB  ], x-1, y-1, z   ))),
                   lerp(v, lerp(u, grad(p[AA+1], x  , y  , z-1 ),
                                   grad(p[BA+1], x-1, y  , z-1 )),
                           lerp(u, grad(p[AB+1], x  , y-1, z-1 ),
                                   grad(p[BB+1], x-1, y-1, z-1 ))));
}

vector<int> PerlinNoise::getPermutationTable() {
    vector<int> table;

    for (int j = 0; j < 2; j++) {
        for (int i = 0; i < 256; i++) {
            table.push_back(Permutation[i]);
        }
    }
    
    return table;
}

const vector<int> PerlinNoise::Permutation = { 151,160,137,91,90,15,
    131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
    190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
    88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
    77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
    102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
    135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
    5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
    223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
    129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
    251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
    49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
    138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
};