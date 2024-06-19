#define _CRT_SECURE_NO_WARNINGS 1
//#include "afxDirectDrawTexture.h"
#include "qwadro/draw/io/afxRaster.h"
#include "qwadro/exec/afxSystem.h"
#include <stdio.h>
#include <string.h>

#define DDS_FOURCC 0x00000004
#define DDS_RGB 0x00000040
#define DDS_RGBA 0x00000041
#define DDS_DEPTH 0x00800000

#define DDS_COMPLEX 0x00000008
#define DDS_CUBEMAP 0x00000200
#define DDS_VOLUME 0x00200000

#define FOURCC_DXT1 0x31545844 //(MAKEFOURCC('D','X','T','1'))
#define FOURCC_DXT3 0x33545844 //(MAKEFOURCC('D','X','T','3'))
#define FOURCC_DXT5 0x35545844 //(MAKEFOURCC('D','X','T','5'))

typedef struct DDS_PIXELFORMAT
{
    unsigned long dwSize;
    unsigned long dwFlags;
    unsigned long dwFourCC;
    unsigned long dwRGBBitCount;
    unsigned long dwRBitMask;
    unsigned long dwGBitMask;
    unsigned long dwBBitMask;
    unsigned long dwABitMask;
} DDS_PIXELFORMAT;

typedef struct DXTColBlock
{
    unsigned short col0;
    unsigned short col1;

    unsigned char row[4];
} DXTColBlock;

typedef struct DXT3AlphaBlock
{
    unsigned short row[4];
} DXT3AlphaBlock;

typedef struct DXT5AlphaBlock
{
    unsigned char alpha0;
    unsigned char alpha1;

    unsigned char row[6];
} DXT5AlphaBlock;

typedef struct DDS_HEADER
{
    unsigned long dwSize;
    unsigned long dwFlags;
    unsigned long dwHeight;
    unsigned long dwWidth;
    unsigned long dwPitchOrLinearSize;
    unsigned long dwDepth;
    unsigned long dwMipMapCount;
    unsigned long dwReserved1[11];
    DDS_PIXELFORMAT ddspf;
    unsigned long dwCaps1;
    unsigned long dwCaps2;
    unsigned long dwReserved2[3];
} DDS_HEADER;

typedef struct DdsMap
{
    int width;
    int height;
    int depth;
    int size;

    char *pixels;
} DdsMap;

typedef struct DdsImage
{
    DdsMap data;
    afxArray mipmaps; // DdsMap *
} DdsImage;

typedef struct DdsData
{
    int (*clamp_size)(int size);
    int(*get_line_width)(int width, int bpp);
    int(*size_dxtc)(int width, int height);
    int(*size_rgb)(int width, int height);
    void(*align_memory)(DdsImage *surface);

    void(*flip)(char *image, int width, int height, int depth, int size);

    void(*swap)(void *byte1, void *byte2, int size);
    void(*flip_blocks_dxtc1)(DXTColBlock *line, int numBlocks);
    void(*flip_blocks_dxtc3)(DXTColBlock *line, int numBlocks);
    void(*flip_blocks_dxtc5)(DXTColBlock *line, int numBlocks);
    void(*flip_dxt5_alpha)(DXT5AlphaBlock *block);

    int format;
    int components;
    afxBool compressed;
    afxBool cubemap;
    afxBool volume;
    afxBool valid;

    afxArray images; // DdsImage *
} DdsData;

// clamps input size to [1-size]
inline int clamp_size(int size)
{
    if (size <= 0)
        size = 1;

    return size;
}

// calculates 4-byte aligned width of image
inline int get_line_width(int width, int bpp)
{
    return ((width * bpp + 31) & -32) >> 3;
}

// calculates size of DXTC texture in bytes
inline int size_dxtc(DdsData *dds, int width, int height)
{
    return ((width + 3) / 4)*((height + 3) / 4)*
        (dds->format == avxPixelFormat_RGBA_DXT1 ? 8 : 16);
}

// calculates size of uncompressed RGB texture in bytes
inline int size_rgb(DdsData *dds, int width, int height)
{
    return width * height* dds->components;
}

// align to 4 byte boundary (add pad bytes to end of each line in the image)
void align_memory(afxMmu mmu, DdsData *dds, DdsImage *surface)
{
    // don't bother with compressed images, volume textures, or cubemaps
    if (dds->compressed || dds->volume || dds->cubemap)
        return;

    // calculate new image size
    int linesize = get_line_width(surface->data.width, dds->components * 8);
    int imagesize = linesize * surface->data.height;

    // exit if already aligned
    if (surface->data.size == imagesize)
        return;

    // create new image of new size
    DdsImage newSurface;
    newSurface.data.width = surface->data.width;
    newSurface.data.height = surface->data.height;
    newSurface.data.depth = surface->data.depth;
    newSurface.data.size = imagesize;
    newSurface.data.pixels = AfxAllocate(mmu, 1, imagesize, AfxHere());
    AfxArrayDeploy(&newSurface.mipmaps, NIL, sizeof(DdsMap), 1);

    // add pad bytes to end of each line
    char *srcimage = (char*)surface->data.pixels;
    char *dstimage = (char*)newSurface.data.pixels;
    for (int n = 0; n < surface->data.depth; n++)
    {
        char *curline = srcimage;
        char *newline = dstimage;

        int imsize = surface->data.size / surface->data.depth;
        int lnsize = imsize / surface->data.height;

        for (int i = 0; i < surface->data.height; i++)
        {
            AfxCopy2(1, lnsize, curline, newline);
            newline += linesize;
            curline += lnsize;
        }
    }

    // save padded image
    *surface = newSurface;
}

// swap to sections of memory
void swap(afxMmu mmu, void *byte1, void *byte2, int size)
{
    unsigned char *tmp = AfxAllocate(mmu, size, sizeof(afxByte), 0, AfxHere());
    AfxCopy2(1, size, byte1, tmp);
    AfxCopy2(1, size, byte2, byte1);
    AfxCopy2(1, size, tmp, byte2);
    AfxDeallocate(mmu, tmp);
}

// flip a DXT1 color block
void flip_blocks_dxtc1(afxMmu mmu, DXTColBlock *line, int numBlocks)
{
    DXTColBlock *curblock = line;

    for (int i = 0; i < numBlocks; i++)
    {
        swap(mem, &curblock->row[0], &curblock->row[3], sizeof(unsigned char));
        swap(mem, &curblock->row[1], &curblock->row[2], sizeof(unsigned char));

        curblock++;
    }
}

// flip a DXT3 color block
void flip_blocks_dxtc3(afxMmu mmu, DXTColBlock *line, int numBlocks)
{
    DXTColBlock *curblock = line;
    DXT3AlphaBlock *alphablock;

    for (int i = 0; i < numBlocks; i++)
    {
        alphablock = (DXT3AlphaBlock*)curblock;

        swap(mem, &alphablock->row[0], &alphablock->row[3], sizeof(unsigned short));
        swap(mem, &alphablock->row[1], &alphablock->row[2], sizeof(unsigned short));

        curblock++;

        swap(mem, &curblock->row[0], &curblock->row[3], sizeof(unsigned char));
        swap(mem, &curblock->row[1], &curblock->row[2], sizeof(unsigned char));

        curblock++;
    }
}

// flip a DXT5 alpha block
void flip_dxt5_alpha(DXT5AlphaBlock *block)
{
    unsigned char gBits[4][4];

    const unsigned long mask = 0x00000007;		    // bits = 00 00 01 11
    unsigned long bits = 0;
    AfxCopy2(1, sizeof(unsigned char), &block->row[0], &bits);

    gBits[0][0] = (unsigned char)(bits & mask);
    bits >>= 3;
    gBits[0][1] = (unsigned char)(bits & mask);
    bits >>= 3;
    gBits[0][2] = (unsigned char)(bits & mask);
    bits >>= 3;
    gBits[0][3] = (unsigned char)(bits & mask);
    bits >>= 3;
    gBits[1][0] = (unsigned char)(bits & mask);
    bits >>= 3;
    gBits[1][1] = (unsigned char)(bits & mask);
    bits >>= 3;
    gBits[1][2] = (unsigned char)(bits & mask);
    bits >>= 3;
    gBits[1][3] = (unsigned char)(bits & mask);

    bits = 0;
    AfxCopy2(3, sizeof(unsigned char), &block->row[3], &bits);

    gBits[2][0] = (unsigned char)(bits & mask);
    bits >>= 3;
    gBits[2][1] = (unsigned char)(bits & mask);
    bits >>= 3;
    gBits[2][2] = (unsigned char)(bits & mask);
    bits >>= 3;
    gBits[2][3] = (unsigned char)(bits & mask);
    bits >>= 3;
    gBits[3][0] = (unsigned char)(bits & mask);
    bits >>= 3;
    gBits[3][1] = (unsigned char)(bits & mask);
    bits >>= 3;
    gBits[3][2] = (unsigned char)(bits & mask);
    bits >>= 3;
    gBits[3][3] = (unsigned char)(bits & mask);

    unsigned long *pBits = ((unsigned long*) &(block->row[0]));

    *pBits &= 0xff000000;

    *pBits = *pBits | (gBits[3][0] << 0);
    *pBits = *pBits | (gBits[3][1] << 3);
    *pBits = *pBits | (gBits[3][2] << 6);
    *pBits = *pBits | (gBits[3][3] << 9);

    *pBits = *pBits | (gBits[2][0] << 12);
    *pBits = *pBits | (gBits[2][1] << 15);
    *pBits = *pBits | (gBits[2][2] << 18);
    *pBits = *pBits | (gBits[2][3] << 21);

    pBits = ((unsigned long*) &(block->row[3]));

    *pBits &= 0xff000000;

    *pBits = *pBits | (gBits[1][0] << 0);
    *pBits = *pBits | (gBits[1][1] << 3);
    *pBits = *pBits | (gBits[1][2] << 6);
    *pBits = *pBits | (gBits[1][3] << 9);

    *pBits = *pBits | (gBits[0][0] << 12);
    *pBits = *pBits | (gBits[0][1] << 15);
    *pBits = *pBits | (gBits[0][2] << 18);
    *pBits = *pBits | (gBits[0][3] << 21);
}

// flip a DXT5 color block
void flip_blocks_dxtc5(afxMmu mmu, DXTColBlock *line, int numBlocks)
{
    DXTColBlock *curblock = line;
    DXT5AlphaBlock *alphablock;

    for (int i = 0; i < numBlocks; i++)
    {
        alphablock = (DXT5AlphaBlock*)curblock;

        flip_dxt5_alpha(alphablock);

        curblock++;

        swap(mem, &curblock->row[0], &curblock->row[3], sizeof(unsigned char));
        swap(mem, &curblock->row[1], &curblock->row[2], sizeof(unsigned char));

        curblock++;
    }
}

// flip image around X axis
void flip(afxMmu mmu, DdsData *dds, char *image, int width, int height, int depth, int size)
{
    int linesize;
    int offset;

    if (!dds->compressed)
    {
        assert(depth > 0);

        int imagesize = size / depth;
        linesize = imagesize / height;

        for (int n = 0; n < depth; n++)
        {
            offset = imagesize * n;
            char *top = image + offset;
            char *bottom = top + (imagesize - linesize);

            for (int i = 0; i < (height >> 1); i++)
            {
                swap(mem, bottom, top, linesize);

                top += linesize;
                bottom -= linesize;
            }
        }
    }
    else
    {
        void (*flipblocks)(afxMmu, DXTColBlock*, int);
        int xblocks = width / 4;
        int yblocks = height / 4;
        int blocksize;

        switch (dds->format)
        {
        case avxPixelFormat_RGBA_DXT1:
            blocksize = 8;
            flipblocks = flip_blocks_dxtc1;
            break;
        case avxPixelFormat_RGBA_DXT3:
            blocksize = 16;
            flipblocks = flip_blocks_dxtc3;
            break;
        case avxPixelFormat_RGBA_DXT5:
            blocksize = 16;
            flipblocks = flip_blocks_dxtc5;
            break;
        default:
            return;
        }

        linesize = xblocks * blocksize;

        DXTColBlock *top;
        DXTColBlock *bottom;

        for (int j = 0; j < (yblocks >> 1); j++)
        {
            top = (DXTColBlock*)(image + j * linesize);
            bottom = (DXTColBlock*)(image + (((yblocks - j) - 1) * linesize));

            (flipblocks)(mem, top, xblocks);
            (flipblocks)(mem, bottom, xblocks);

            swap(mem, bottom, top, linesize);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// loads DDS image
//
// filename - fully qualified name of DDS image
// flipImage - specifies whether image is flipped on load, default is true

DLLEXPORT afxBool _AfxDdsLoad(afxMmu mmu, afxUri *filename, afxBool flipImage, DdsData *dds)
{
    DDS_HEADER ddsh;
    char filecode[4];
    FILE *fp;
    int width, height, depth;
    int (*sizefunc)(DdsData *dds, int, int);

    // clear any previously loaded images
    //clear();

    // open file
    fp = fopen(AfxGetUriData(filename, 0), "rb");
    if (fp == NULL)
        return FALSE;

    // read in file marker, make sure its a DDS file
    fread(filecode, 1, 4, fp);
    if (strncmp(filecode, "DDS ", 4) != 0)
    {
        fclose(fp);
        return FALSE;
    }

    // read in DDS header
    fread(&ddsh, sizeof(ddsh), 1, fp);

    // check if image is a cubempa
    if (ddsh.dwCaps2 & DDS_CUBEMAP)
        dds->cubemap = TRUE;

    // check if image is a volume texture
    if ((ddsh.dwCaps2 & DDS_VOLUME) && (ddsh.dwDepth > 0))
        dds->volume = TRUE;

    // figure out what the image format is
    if (ddsh.ddspf.dwFlags & DDS_FOURCC)
    {
        switch (ddsh.ddspf.dwFourCC)
        {
        case FOURCC_DXT1:
            dds->format = avxPixelFormat_RGBA_DXT1;
            dds->components = 3;
            dds->compressed = TRUE;
            break;
        case FOURCC_DXT3:
            dds->format = avxPixelFormat_RGBA_DXT3;
            dds->components = 4;
            dds->compressed = TRUE;
            break;
        case FOURCC_DXT5:
            dds->format = avxPixelFormat_RGBA_DXT5;
            dds->components = 4;
            dds->compressed = TRUE;
            break;
        default:
            fclose(fp);
            return FALSE;
        }
    }
    else if (ddsh.ddspf.dwFlags == DDS_RGBA && ddsh.ddspf.dwRGBBitCount == 32)
    {
        dds->format = avxPixelFormat_BGRA8;
        dds->compressed = FALSE;
        dds->components = 4;
    }
    else if (ddsh.ddspf.dwFlags == DDS_RGB && ddsh.ddspf.dwRGBBitCount == 32)
    {
        dds->format = avxPixelFormat_BGRA8;
        dds->compressed = FALSE;
        dds->components = 4;
    }
    else if (ddsh.ddspf.dwFlags == DDS_RGB && ddsh.ddspf.dwRGBBitCount == 24)
    {
        dds->format = avxPixelFormat_BGR8;
        dds->compressed = FALSE;
        dds->components = 3;
    }
    else
    {
        fclose(fp);
        return FALSE;
    }

    // store primary surface width/height/depth
    width = ddsh.dwWidth;
    height = ddsh.dwHeight;
    depth = clamp_size(ddsh.dwDepth);   // set to 1 if 0

    // use correct size calculation function depending on whether image is 
    // compressed
    sizefunc = (dds->compressed ? size_dxtc : size_rgb);

    AfxArrayDeploy(&dds->images, NIL, sizeof(DdsImage), (dds->cubemap ? 6 : 1));

    // load mem surfaces for the image (6 surfaces for cubemaps)
    for (int n = 0; n < (dds->cubemap ? 6 : 1); n++)
    {
        int size;

        // calculate surface size
        size = sizefunc(dds, width, height) * depth;

        // load surface
        DdsImage img;
        img.data.width = width;
        img.data.height = height;
        img.data.depth = depth;
        img.data.size = size;
        img.data.pixels = AfxAllocate(mmu, size, sizeof(afxByte), AfxHere());
        
        fread(img.data.pixels, 1, img.data.size, fp);

        align_memory(mem, dds, &img);

        if (flipImage)
            flip(mem, dds, img.data.pixels, img.data.width, img.data.height, img.data.depth, img.data.size);

        int w = clamp_size(width >> 1);
        int h = clamp_size(height >> 1);
        int d = clamp_size(depth >> 1);

        // store number of mipmaps
        int numMipmaps = ddsh.dwMipMapCount;

        // number of mipmaps in file includes main surface so decrease count 
        // by one
        if (numMipmaps != 0)
            numMipmaps--;

        AfxArrayDeploy(&img.mipmaps, NIL, sizeof(DdsMap), numMipmaps);

        // load mem mipmaps for current surface
        for (int i = 0; i < numMipmaps && (w || h); i++)
        {
            // calculate mipmap size
            size = sizefunc(dds, w, h)*d;

            DdsMap mipmap;
            mipmap.width = width;
            mipmap.height = height;
            mipmap.depth = depth;
            mipmap.size = size;
            mipmap.pixels = AfxAllocate(mmu, size, sizeof(afxByte), AfxHere());

            fread(&mipmap, 1, mipmap.size, fp);

            if (flipImage)
            {
                flip(mem, dds, mipmap.pixels, mipmap.width, mipmap.height, mipmap.depth, mipmap.size);
            }

            AfxArrayPushElement(&img.mipmaps, &mipmap);
            
            // shrink to next power of 2
            w = clamp_size(w >> 1);
            h = clamp_size(h >> 1);
            d = clamp_size(d >> 1);
        }

        AfxArrayPushElement(&dds->images, &img);
    }

    // swap cubemaps on y axis (since image is flipped in OGL)
    if (dds->cubemap && flipImage)
    {
        //CTexture tmp;
        //tmp = images[3];
        //images[3] = images[2];
        //images[2] = tmp;
    }

    fclose(fp);

    dds->valid = TRUE;

    return TRUE;
}

DLLEXPORT afxError AfxLoadTexturesDds(avxDrawContext dctx, afxNat cnt, afxUri const uri[], avxRaster tex[]);

DLLEXPORT afxError AfxFetchRasterDds(avxRaster tex, afxUri const *uri);
DLLEXPORT afxError AfxFetchRasterRegionsDds(avxRaster tex, afxNat cnt, avxRasterRegion const rgn[], afxUri const uri[]);

DLLEXPORT afxError AfxPrintRasterDds(avxRaster tex, afxUri const *uri);
DLLEXPORT afxError AfxPrintRasterRegionsDds(avxRaster tex, afxNat cnt, avxRasterRegion const rgn[], afxUri const uri[]);
