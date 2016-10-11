#include <stdio.h>
#include <stdint.h>

int diskgeom[16][6] =
{
    //Size, Tracks, Sides, Sectors, Sector size, Disk size
    //0 for disk size means 5 1/4 inch, 1 means 3 1/2 inch, and 2 means 8 inch
    { 8*1*40*512,  8, 1, 40, 512, 0},
    { 8*2*40*512,  8, 2, 40, 512, 0},
    { 9*1*40*512,  9, 1, 40, 512, 0},
    { 9*2*40*512,  9, 2, 40, 512, 0},
    {10*1*80*512, 10, 1, 80, 512, 0},
    {10*2*80*512, 10, 2, 80, 512, 0},
    { 9*2*80*512,  9, 2, 80, 512, 1},
    {15*2*80*512, 15, 2, 80, 512, 0},
    {18*2*80*512, 18, 2, 80, 512, 1},
    {21*2*80*512, 21, 2, 80, 512, 1},
    {23*2*80*512, 23, 2, 80, 512, 1},
    {36*2*80*512, 36, 2, 80, 512, 1},
    
    //Weird geometries
    
    {8*2*77*1024, 8, 2, 80, 1024, 2},
    {39*2*80*512, 39, 2, 80, 512, 1},
    {40*2*80*512, 40, 2, 80, 512, 1},
    {41*2*80*512, 41, 2, 80, 512, 1},
};

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
    
    int tracks, sides, sectors, sectorsize, physsize;
    bool weirdgeom;
    
    int i;
    for(i = 0; i < 16; i++)
    {
        if(size == diskgeom[i][0])
        {
            tracks = diskgeom[i][1];
            sides = diskgeom[i][2];
            sectors = diskgeom[i][3];
            sectorsize = diskgeom[i][4];
            physsize = diskgeom[i][5];
            weirdgeom = false;
            break;
        }
        weirdgeom = true;
    }
    
    printf("MoochMcGee's IMGtool for floppy disk images\n");
    printf("Disk size: %i\nTracks: %i, Sides: %i, Sectors: %i, Sector size: %i\n",size,tracks,sides,sectors,sectorsize);
    printf("Probable physical disk size: %s", physsize ? (physsize == 2) ? "8 inch\n" : "3 1/2 inch\n" : "5 1/4 inch");
    
    if(physsize == 0)
    {
        printf("Your disk might also work in a 3 1/2 inch 3-mode drive\n");
    }
    
    printf("Bootable: %s", bootable ? "Yes\n" : "No\n");
    if(size == 21*2*80*512)
    {
        printf("Your disk is probably DMF formatted\n");
    }
    else if(size == 23*2*80*512)
    {
       printf("Your disk is probably XDF formatted. XDF will not work on some FDCs, so be careful!\n");
    }
    else if(size == 15*2*80*512)
    {
        printf("The physical disk size is indefinite. It could be either 5 1/4 inch or 3 1/2 inch\n");
    }
    
    if(weirdgeom)
    {
        printf("The disk geometry for this image isn't in this tool's database.\nPlease make an issue on GitHub with the size of your image and I'll see what I can do.\n");
    }
}