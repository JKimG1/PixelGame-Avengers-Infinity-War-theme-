#include "gba.h"
#include "images/characters.h"

volatile unsigned short *videoBuffer = (volatile unsigned short *)0x6000000;
u32 vBlankCounter = 0;
OamEntry shadow[128];
OamEntry* hero;
OamEntry* thanos;

void waitForVBlank(void) {
    // Write a while loop that loops until we're NOT in vBlank anymore:
    // (This prevents counting one VBlank more than once if your app is too fast)
    while(*SCANLINECOUNTER > 160);
    // Write a while loop that keeps going until we're in vBlank:
    while(*SCANLINECOUNTER < 160);
    // Finally, increment the vBlank counter:
    vBlankCounter++;
}

static int __qran_seed= 42;
static int qran(void) {
    __qran_seed= 1664525*__qran_seed+1013904223;
    return (__qran_seed>>16) & 0x7FFF;
}

int randint(int min, int max) {
    return (qran()*(max-min)>>15)+min;
}

void setPixel(int x, int y, u16 color) {
    videoBuffer[OFFSET(y, x, 240)] = color;
}

void drawRectDMA(int x, int y, int width, int height, volatile u16 color) {
    for (int r = 0; r < height; r++) {
        DMA[3].src = &color;
        DMA[3].dst = &videoBuffer[OFFSET(y + r, x, 240)];
        DMA[3].cnt = width | DMA_ON | DMA_SOURCE_FIXED;
    }
}

void drawFullScreenImageDMA(const u16 *image) {
    DMA[3].src = image;
    DMA[3].dst = videoBuffer;
    DMA[3].cnt = 240 * 160 | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT | DMA_ON;
}

void drawImageDMA(int x, int y, int width, int height, const u16 *image) {
    for (int r = 0; r < height; r++) {
        DMA[3].src = image + (r * width);
        DMA[3].dst = &videoBuffer[OFFSET(y + r, x, 240)];
        DMA[3].cnt = width | DMA_ON | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT;
    }
}

void fillScreenDMA(volatile u16 color) {
    // TA-TODO: IMPLEMENT
    DMA[3].src = &color;
    DMA[3].dst = videoBuffer;
    DMA[3].cnt = 240 * 160 | DMA_SOURCE_FIXED | DMA_DESTINATION_INCREMENT | DMA_ON;
}

void drawChar(int col, int row, char ch, u16 color) {
    for(int r = 0; r<8; r++) {
        for(int c=0; c<6; c++) {
            if(fontdata_6x8[OFFSET(r, c, 6) + ch*48]) {
                setPixel(col+c, row+r, color);
            }
        }
    }
}

void drawString(int col, int row, char *str, u16 color) {
    while(*str) {
        drawChar(col, row, *str++, color);
        col += 6;
    }
}

void drawCenteredString(int x, int y, int width, int height, char *str, u16 color) {
    u32 len = 0;
    char *strCpy = str;
    while (*strCpy) {
        len++;
        strCpy++;
    }

    u32 strWidth = 6 * len;
    u32 strHeight = 8;

    int col = x + ((width - strWidth) >> 1);
    int row = y + ((height - strHeight) >> 1);
    drawString(col, row, str, color);
}

void eraseCharacters(void) {
    for(int i = 0; i < 128; i++)
        shadow[i].attr0 = ATTR0_HIDE;
    
    DMA[3].src = shadow;
    DMA[3].dst = SPRITEMEM;
    DMA[3].cnt = 128 * 4 | DMA_ON;
}

// Draw Ironman and Thanos (sprites)
void drawIronmanVSThanos(int x1, int y1, int x2, int y2) {
    DMA[3].src = characters_palette;
    DMA[3].dst = SPRITEPAL;
    DMA[3].cnt = CHARACTERS_PALETTE_LENGTH | DMA_ON;
    
    DMA[3].src = characters;
    DMA[3].dst = &CHARBLOCKBASE[5];
    DMA[3].cnt = CHARACTERS_LENGTH | DMA_ON;
    
    for(int i = 0; i < 128; i++)
        shadow[i].attr0 = ATTR0_HIDE;
    
    hero = shadow;
    hero->attr0 = y1 | CHARACTERS_PALETTE_TYPE | IRONMAN_SPRITE_SHAPE;
    hero->attr1 = x1 | IRONMAN_SPRITE_SIZE;
    hero->attr2 = IRONMAN_PALETTE_ID | IRONMAN_ID;
    
    thanos = shadow + 1;
    thanos->attr0 = y2 | CHARACTERS_PALETTE_TYPE | THANOS_SPRITE_SHAPE;
    thanos->attr1 = x2 | THANOS_SPRITE_SIZE;
    thanos->attr2 = THANOS_PALETTE_ID | THANOS_ID;
 
    DMA[3].src = shadow;
    DMA[3].dst = SPRITEMEM;
    DMA[3].cnt = 128 * 4 | DMA_ON;
}

// Draw Captain America and Thanos (sprites)
void drawCaptainVSThanos(int x1, int y1, int x2, int y2) {
    DMA[3].src = characters_palette;
    DMA[3].dst = SPRITEPAL;
    DMA[3].cnt = CHARACTERS_PALETTE_LENGTH | DMA_ON;
    
    DMA[3].src = characters;
    DMA[3].dst = &CHARBLOCKBASE[5];
    DMA[3].cnt = CHARACTERS_LENGTH | DMA_ON;
    
    for(int i = 0; i < 128; i++)
        shadow[i].attr0 = ATTR0_HIDE;
    
    hero = shadow;
    hero->attr0 = y1 | CHARACTERS_PALETTE_TYPE | CAPTAIN_SPRITE_SHAPE;
    hero->attr1 = x1 | CAPTAIN_SPRITE_SIZE;
    hero->attr2 = CAPTAIN_PALETTE_ID | CAPTAIN_ID;
    
    thanos = shadow + 1;
    thanos->attr0 = y2 | CHARACTERS_PALETTE_TYPE | THANOS_SPRITE_SHAPE;
    thanos->attr1 = x2 | THANOS_SPRITE_SIZE;
    thanos->attr2 = THANOS_PALETTE_ID | THANOS_ID;
    
    DMA[3].src = shadow;
    DMA[3].dst = SPRITEMEM;
    DMA[3].cnt = 128 * 4 | DMA_ON;
}


// Draw Thor and Thanos (sprites)
void drawThorVSThanos(int x1, int y1, int x2, int y2) {
    DMA[3].src = characters_palette;
    DMA[3].dst = SPRITEPAL;
    DMA[3].cnt = CHARACTERS_PALETTE_LENGTH | DMA_ON;
    
    DMA[3].src = characters;
    DMA[3].dst = &CHARBLOCKBASE[5];
    DMA[3].cnt = CHARACTERS_LENGTH | DMA_ON;
    
    for(int i = 0; i < 128; i++)
        shadow[i].attr0 = ATTR0_HIDE;
    
    hero = shadow;
    hero->attr0 = y1 | CHARACTERS_PALETTE_TYPE | THOR_SPRITE_SHAPE;
    hero->attr1 = x1 | THOR_SPRITE_SIZE;
    hero->attr2 = THOR_PALETTE_ID | THOR_ID;
    
    thanos = shadow + 1;
    thanos->attr0 = y2 | CHARACTERS_PALETTE_TYPE | THANOS_SPRITE_SHAPE;
    thanos->attr1 = x2 | THANOS_SPRITE_SIZE;
    thanos->attr2 = THANOS_PALETTE_ID | THANOS_ID;
    
    DMA[3].src = shadow;
    DMA[3].dst = SPRITEMEM;
    DMA[3].cnt = 128 * 4 | DMA_ON;
}
