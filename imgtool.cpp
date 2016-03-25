#include <stdio.h>
#include <stdint.h>

int diskgeom[10][5] =
{
    //Size, Tracks, Sides, Sectors, Disk size
    //0 for disk size means 5 1/4 inch, 1 means 3 1/2 inch
    { 8*1*40*512,  8, 1, 40, 0},
    { 8*2*40*512,  8, 2, 40, 0},
    { 9*1*40*512,  9, 1, 40, 0},
    { 9*2*40*512,  9, 2, 40, 0},
    {10*2*40*512, 10, 2, 40, 0},
    { 9*2*80*512,  9, 2, 80, 1},
    {15*2*80*512, 15, 2, 80, 0},
    {18*2*80*512, 18, 2, 80, 1},
    {21*2*80*512, 21, 2, 80, 1},
    {36*2*80*512, 36, 2, 80, 1},
}

int main(int ac, char** av)
{
    if(ac<2) return 1;
    
    FILE* img = fopen(av[1], "rb");
    
    //Figure out the size of the input file.
    fseek(img,0,SEEK_END);
    long size = ftell(img);
    fseek(img,0,SEEK_SET);
    
    fseek(img,510,SEEK_SET);
    bool bootable;
    {
        uint8_t tmp1 = fgetc(img);
        uint8_t tmp2 = fgetc(img);
        if(tmp1 == 0x55 && tmp2 == 0xaa) bootable = true;
        else bootable = false;
    }
    
    int tracks, sides, sectors, physsize;
    bool weirdgeom;
    
    int i;
    for(i = 0; i < 9; i++)
    {
        if(size == diskgeom[i][0])
        {
            tracks = diskgeom[i][1];
            sides = diskgeom[i][2];
            sectors = diskgeom[i][3];
            physsize = diskgeom[i][4];
            weirdgeom = false;
            break;
        }
        weirdgeom = true;
    }
    
    printf("MoochMcGee's IMGtool for floppy disk images\n");
    printf("Disk size: %i\nTracks: %i, Sides: %i, Sectors: %i\n",size,tracks,sides,sectors);
    printf("Physical disk size: %s", physsize ? "5 1/4 inch" : "3 1/2 inch");
    printf("Bootable: %s", bootable ? "No" : "Yes");
    if(tracks == 21)
    {
        printf("Your disk is probably DMF formatted\n");
    }
    if(weirdgeom)
    {
        printf("The disk geometry for this image isn't in this tool's database.\nPlease make an issue on GitHub with the size of your image and I'll see what I can do.");
    }
}