#ifndef UTILS_H
#define UTILS_H

#define SWAP(a, b) { __typeof__(a) temp; temp = a; a = b; b = temp; }

float clamp(float value, float min, float max);

#endif
