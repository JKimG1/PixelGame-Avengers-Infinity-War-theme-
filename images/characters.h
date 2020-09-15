/*
 * Exported with nin10kit v1.7
 * Invocation command was nin10kit --mode=sprites --for_bitmap --bpp=8 --transparent=000000 characters ironman.png thor.png captain.png thanos.png 
 * Time-stamp: Monday 11/19/2018, 04:41:00
 * 
 * Image Information
 * -----------------
 * ironman.png 64@64
 * thor.png 64@64
 * captain.png 64@64
 * thanos.png 64@64
 * Transparent color: (0, 0, 0)
 * 
 * All bug reports / feature requests are to be filed here https://github.com/TricksterGuy/nin10kit/issues
 */

#ifndef CHARACTERS_H
#define CHARACTERS_H

#define CHARACTERS_PALETTE_TYPE (1 << 13)
#define CHARACTERS_DIMENSION_TYPE (1 << 6)

extern const unsigned short characters_palette[255];
#define CHARACTERS_PALETTE_SIZE 510
#define CHARACTERS_PALETTE_LENGTH 255

extern const unsigned short characters[8192];
#define CHARACTERS_SIZE 16384
#define CHARACTERS_LENGTH 8192

#define IRONMAN_PALETTE_ID (0 << 12)
#define IRONMAN_SPRITE_SHAPE (0 << 14)
#define IRONMAN_SPRITE_SIZE (3 << 14)
#define IRONMAN_ID 512

#define THOR_PALETTE_ID (0 << 12)
#define THOR_SPRITE_SHAPE (0 << 14)
#define THOR_SPRITE_SIZE (3 << 14)
#define THOR_ID 640

#define CAPTAIN_PALETTE_ID (0 << 12)
#define CAPTAIN_SPRITE_SHAPE (0 << 14)
#define CAPTAIN_SPRITE_SIZE (3 << 14)
#define CAPTAIN_ID 768

#define THANOS_PALETTE_ID (0 << 12)
#define THANOS_SPRITE_SHAPE (0 << 14)
#define THANOS_SPRITE_SIZE (3 << 14)
#define THANOS_ID 896

#endif

