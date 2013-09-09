#ifndef HELPERS_HPP
#define HELPERS_HPP

typedef unsigned short block_t;

int ofacing(int dx, int dz);
int ofacingNeg(int dx, int dz);

int coretest(int x, int y, int z, int rad);
void downSpider(int x, int y, int z, block_t id, int tries);

void ocircleXZ(int x, int y, int z, float radius, block_t id);
void oellipsoidXZ(int x, int y, int z, int radius, float radx, float radz, block_t id);
void oellipsoidXY(int x, int y, int z, int radius, float radx, float rady, float stencil, block_t id);
void ocircleXZroots(int x, int y, int z, float radius, block_t id);

void obell(int x, int y, int z, block_t id, int lower, int height, int radius, int inner_rad, int midlevel, float midstrength, float understrength, float stencilchance);


#endif
