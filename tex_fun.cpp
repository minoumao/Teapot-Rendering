/* Texture functions for cs580 GzLib	*/
#include    "stdafx.h" 
#include	"stdio.h"
#include	"Gz.h"

GzColor	*image=NULL;
int xs, ys;
int reset = 1;

/* Image texture function */
int tex_fun(float u, float v, GzColor color)
{
  unsigned char		pixel[3];
  unsigned char     dummy;
  char  		foo[8];
  int   		i, j;
  FILE			*fd;

  if (reset) 
  {          /* open and load texture file */
    fd = fopen ("texture", "rb");
    if (fd == NULL) {
      fprintf (stderr, "texture file not found\n");
      exit(-1);
    }
    fscanf (fd, "%s %d %d %c", foo, &xs, &ys, &dummy);
    image = (GzColor*)malloc(sizeof(GzColor)*(xs+1)*(ys+1));
    if (image == NULL) {
      fprintf (stderr, "malloc for texture image failed\n");
      exit(-1);
    }

    for (i = 0; i < xs*ys; i++) {	/* create array of GzColor values */
      fread(pixel, sizeof(pixel), 1, fd);
      image[i][RED] = (float)((int)pixel[RED]) * (1.0 / 255.0);
      image[i][GREEN] = (float)((int)pixel[GREEN]) * (1.0 / 255.0);
      image[i][BLUE] = (float)((int)pixel[BLUE]) * (1.0 / 255.0);
      }

    reset = 0;          /* init is done */
	fclose(fd);
  }

/* bounds-test u,v to make sure nothing will overflow image array bounds */
/* determine texture cell corner values and perform bilinear interpolation */
/* set color to interpolated GzColor value and return */

  // Clamp the U,V
  {
      if (u > 1.0f)
          u = 1.0f;
      if (u < 0)
          u = 0;
      if (v > 1.0f)
          v = 1.0f;
      if (v < 0)
          v = 0;
  }

  float factorU = (xs - 1);
  float factorV = (ys - 1);
  float unadjustedU = u * factorU;
  float unadjustedV = v * factorV;

  int upperBoundX = ceil(unadjustedU);
  int lowerBoundX = floor(unadjustedU);
  int upperBoundY = ceil(unadjustedV);
  int lowerBoundY = floor(unadjustedV);

  float distanceS = unadjustedU - (float)lowerBoundX;
  float distanceT = unadjustedV - (float)lowerBoundY;
   
  // Set Texture from file
  GzColor colorA, colorB, colorC, colorD;
  {
      colorA[RED]   = image[lowerBoundY * xs + lowerBoundX][RED];
      colorA[GREEN] = image[lowerBoundY * xs + lowerBoundX][GREEN];
      colorA[BLUE]  = image[lowerBoundY * xs + lowerBoundX][BLUE];

      colorB[RED]   = image[lowerBoundY * xs + upperBoundX][RED];
      colorB[GREEN] = image[lowerBoundY * xs + upperBoundX][GREEN];
      colorB[BLUE]  = image[lowerBoundY * xs + upperBoundX][BLUE];

      colorC[RED]   = image[upperBoundY * xs + upperBoundX][RED];
      colorC[GREEN] = image[upperBoundY * xs + upperBoundX][GREEN];
      colorC[BLUE]  = image[upperBoundY * xs + upperBoundX][BLUE];

      colorD[RED]   = image[upperBoundY * xs + lowerBoundX][RED];
      colorD[GREEN] = image[upperBoundY * xs + lowerBoundX][GREEN];
      colorD[BLUE]  = image[upperBoundY * xs + lowerBoundX][BLUE];

      color[RED] 
          = distanceS * distanceT * colorC[RED] 
          + (1.0f - distanceS) * distanceT * colorD[RED]
          + distanceS * (1.0f - distanceT) * colorB[RED] 
          + (1.0f - distanceS) * (1.0f - distanceT) * colorA[RED];

      color[GREEN] 
          = distanceS * distanceT * colorC[GREEN] 
          + (1 - distanceS) * distanceT * colorD[GREEN]
          + distanceS * (1.0f - distanceT) * colorB[GREEN] 
          + (1.0f - distanceS) * (1.0f - distanceT) * colorA[GREEN];

      color[BLUE] = distanceS * distanceT * colorC[BLUE] 
          + (1.0f - distanceS) * distanceT * colorD[BLUE]
          + distanceS * (1.0f - distanceT) * colorB[BLUE] 
          + (1.0f - distanceS) * (1.0f - distanceT) * colorA[BLUE];
  }
	return GZ_SUCCESS;
}

/* Procedural texture function */
int ptex_fun(float u, float v, GzColor color)
{
    // "Circleboard" of "Blue and GreyBlue"
    xs = 200;
    ys = 200;

    int quadSize = 40;
    if (u > 1.0f)
        u = 1.0f;
    if (u < 0)
        u = 0;
    if (v > 1.0f)
        v = 1.0f;
    if (v < 0)
        v = 0;

    float factorU = (float)(xs - 1);
    float factorV = (float)(ys - 1);
    int rawU = (int)round(u * factorU);
    int rawV = (int)round(v * factorV);

    int centerU, centerV;
    if (rawU % quadSize < quadSize / 2) 
    {
        centerU = rawU + quadSize / 2 - (rawU % quadSize);
    }
    else 
    {
        centerU = rawU - (rawU % quadSize - quadSize / 2);
    }
    if (rawV % quadSize < quadSize / 2) 
    {
        centerV = rawV + quadSize / 2 - (rawV % quadSize);
    }
    else 
    {
        centerV = rawV - (rawV % quadSize - quadSize / 2);
    }

    if (pow(centerU - rawU, 2.0) + pow(centerV - rawV, 2.0) < pow(quadSize / 2 - 1.0f, 2.0)) 
    {
        color[RED]      = 0.2f;
        color[GREEN]    = 0.4f;
        color[BLUE]     = 0.8f;
    }
    else 
    {
        color[RED]      = 0.5f;
        color[GREEN]    = 0.9f;
        color[BLUE]     = 0.9f;
    }

	return GZ_SUCCESS;
}

/* Free texture memory */
int GzFreeTexture()
{
	if(image!=NULL)
		free(image);
	return GZ_SUCCESS;
}

