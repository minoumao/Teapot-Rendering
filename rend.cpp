/* CS580 Homework 3 */

#include	"stdafx.h"
#include	"stdio.h"
#include	"math.h"
#include	"Gz.h"
#include	"rend.h"
#include	<string>

#define PI (float) 3.14159265358979323846
int pushMatrixToStack(short& matLevel, GzMatrix* matStack, GzMatrix mat);

int GzRender::GzRotXMat(float degree, GzMatrix mat)
{
	/* HW 3.1
	// Create rotate matrix : rotate along x axis
	// Pass back the matrix using mat value
	*/

	float radianAngle = degree * PI / 180.0f;
	// Initialize matrix

	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			mat[i][j] = 0;
		}
	}

	// Calculate rotation matrix
	mat[0][0] = 1.0;
	mat[1][1] = (float)cos((double)radianAngle);
	mat[1][2] = -1.0f * (float)sin((double)radianAngle);
	mat[2][1] = (float)sin((double)radianAngle);
	mat[2][2] = (float)cos((double)radianAngle);
	mat[3][3] = 1.0;

	return GZ_SUCCESS;
}

int GzRender::GzRotYMat(float degree, GzMatrix mat)
{
	/* HW 3.2
	// Create rotate matrix : rotate along y axis
	// Pass back the matrix using mat value
	*/

	float radianAngle = degree * PI / 180.0f;
	// Initialize matrix
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			mat[i][j] = 0;
		}
	}

	// Calculate rotation matrix
	mat[0][0] = (float)cos((double)radianAngle);
	mat[0][2] = (float)sin((double)radianAngle);
	mat[1][1] = 1.0;
	mat[2][0] = -1.0f * (float)sin((double)radianAngle);
	mat[2][2] = (float)cos((double)radianAngle);
	mat[3][3] = 1.0;

	return GZ_SUCCESS;
	return GZ_SUCCESS;
}

int GzRender::GzRotZMat(float degree, GzMatrix mat)
{
	/* HW 3.3
	// Create rotate matrix : rotate along z axis
	// Pass back the matrix using mat value
	*/
	float radianAngle = degree * PI / 180.0f;
	// Initialize matrix
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			mat[i][j] = 0;
		}
	}

	// Calculate rotation matrix
	mat[0][0] = (float)cos((double)radianAngle);
	mat[0][1] = -1.0f * (float)sin((double)radianAngle);
	mat[1][0] = (float)sin((double)radianAngle);
	mat[1][1] = (float)cos((double)radianAngle);
	mat[2][2] = 1.0;
	mat[3][3] = 1.0;

	return GZ_SUCCESS;
}

int GzRender::GzTrxMat(GzCoord translate, GzMatrix mat)
{
	/* HW 3.4
	// Create translation matrix
	// Pass back the matrix using mat value
	*/

	// Initialize matrix
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			mat[i][j] = 0;
		}
	}

	// Calculate translation matrix
	mat[0][0] = 1.0;
	mat[0][3] = translate[0];
	mat[1][1] = 1.0;
	mat[1][3] = translate[1];
	mat[2][2] = 1.0;
	mat[2][3] = translate[2];
	mat[3][3] = 1.0;

	return GZ_SUCCESS;
}

int GzRender::GzScaleMat(GzCoord scale, GzMatrix mat)
{
	/* HW 3.5
	// Create scaling matrix
	// Pass back the matrix using mat value
	*/

	// Initialize matrix
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			mat[i][j] = 0;
		}
	}

	// Calculate scale matrix
	mat[0][0] = scale[0];
	mat[1][1] = scale[1];
	mat[2][2] = scale[2];
	mat[3][3] = 1.0;

	return GZ_SUCCESS;
}

GzRender::GzRender(int xRes, int yRes)
{
	/* HW1.1 create a framebuffer for MS Windows display:
	 -- set display resolution
	 -- allocate memory for framebuffer : 3 bytes(b, g, r) x width x height
	 -- allocate memory for pixel buffer
	 */
	xres = xRes;
	yres = yRes;

	int resolution = 0;
	resolution = xres * yres;

	framebuffer = new char[(3)*resolution];
	pixelbuffer = new GzPixel[resolution];

	/* HW 3.6
	- setup Xsp and anything only done once
	- init default camera
	*/
	matlevel = -1;
	matlevelNormal = -1;
	numlights = 0;

	// Initialize Xsp
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			Xsp[i][j] = 0;
		}
	}

	// Set up the Xsp matrix
	Xsp[0][0] = (float)xres / 2.0f;
	Xsp[0][3] = (float)xres / 2.0f;
	Xsp[1][1] = -1.0f * (float)yres / 2.0f;
	Xsp[1][3] = (float)yres / 2.0f;
	Xsp[2][2] = (float)MAXINT;
	Xsp[3][3] = 1.0;

	// Set cam position
	m_camera.position[0] = DEFAULT_IM_X;
	m_camera.position[1] = DEFAULT_IM_Y;
	m_camera.position[2] = DEFAULT_IM_Z;

	// Initialize camera
	for (int i = 0; i < 3; i++)
	{
		m_camera.lookat[i] = 0;
		m_camera.worldup[i] = 0;
	}

	m_camera.worldup[1] = 1.0;
	m_camera.FOV = DEFAULT_FOV;
}

GzRender::~GzRender()
{
	/* HW1.2 clean up, free buffer memory */
	delete[] framebuffer;
	delete[] pixelbuffer;
}

int GzRender::GzDefault()
{
	/* HW1.3 set pixel buffer to some default values - start a new frame */
	if (pixelbuffer == NULL)
		return GZ_FAILURE;

	int resolution = xres * yres;

	GzPixel defaultPixel = { 2055, 1798, 1514, 1, MAXINT };

	for (int i = 0; i < resolution; i++)
	{
		pixelbuffer[i] = defaultPixel;
		// Note: framebuffer is a 1d array being treated as frame[resolution][3] 
		framebuffer[3 * i] = (char)1514;
		framebuffer[3 * i + 1] = (char)1798;
		framebuffer[3 * i + 2] = (char)2055;
	}

	return GZ_SUCCESS;
}

int GzRender::GzBeginRender()
{
	/* HW 3.7
	- setup for start of each frame - init frame buffer color,alpha,z
	- compute Xiw and projection xform Xpi from camera definition
	- init Ximage - put Xsp at base of stack, push on Xpi and Xiw
	- now stack contains Xsw and app can push model Xforms when needed
	*/
	GzCoord cameraLookAt, cameraUp, xDirection, yDirection, zDirection;
	// Set up the matrix values
	{
		// Create cameraLookAt direction
		cameraLookAt[0] = m_camera.lookat[0] - m_camera.position[0];
		cameraLookAt[1] = m_camera.lookat[1] - m_camera.position[1];
		cameraLookAt[2] = m_camera.lookat[2] - m_camera.position[2];

		// Create zDirection direction
		float normalizeZDistance = cameraLookAt[0] * cameraLookAt[0] + cameraLookAt[1] * cameraLookAt[1] + cameraLookAt[2] * cameraLookAt[2];
		zDirection[0] = cameraLookAt[0] / (float)sqrt((double)(normalizeZDistance));
		zDirection[1] = cameraLookAt[1] / (float)sqrt((double)(normalizeZDistance));
		zDirection[2] = cameraLookAt[2] / (float)sqrt((double)(normalizeZDistance));

		// Create cameraUp direction
		float upDotZ = m_camera.worldup[0] * zDirection[0] + m_camera.worldup[1] * zDirection[1] + m_camera.worldup[2] * zDirection[2];
		cameraUp[0] = m_camera.worldup[0] - upDotZ * zDirection[0];
		cameraUp[1] = m_camera.worldup[1] - upDotZ * zDirection[1];
		cameraUp[2] = m_camera.worldup[2] - upDotZ * zDirection[2];

		// create yCamera direction
		float yDistance = cameraUp[0] * cameraUp[0] + cameraUp[1] * cameraUp[1] + cameraUp[2] * cameraUp[2];
		yDirection[0] = cameraUp[0] / (float)sqrt((double)(yDistance));
		yDirection[1] = cameraUp[1] / (float)sqrt((double)(yDistance));
		yDirection[2] = cameraUp[2] / (float)sqrt((double)(yDistance));

		//  create xCamera direction
		xDirection[0] = yDirection[1] * zDirection[2] - yDirection[2] * zDirection[1];
		xDirection[1] = yDirection[2] * zDirection[0] - yDirection[0] * zDirection[2];
		xDirection[2] = yDirection[0] * zDirection[1] - yDirection[1] * zDirection[0];
	}

	// Create the Xwi Matrix
	{
		// Set X row
		m_camera.Xiw[0][0] = xDirection[0];
		m_camera.Xiw[0][1] = xDirection[1];
		m_camera.Xiw[0][2] = xDirection[2];

		// Set Y row
		m_camera.Xiw[1][0] = yDirection[0];
		m_camera.Xiw[1][1] = yDirection[1];
		m_camera.Xiw[1][2] = yDirection[2];

		// Set Z row
		m_camera.Xiw[2][0] = zDirection[0];
		m_camera.Xiw[2][1] = zDirection[1];
		m_camera.Xiw[2][2] = zDirection[2];

		// Adjust for postion
		m_camera.Xiw[0][3] = -1.0f * (xDirection[0] * m_camera.position[0] + xDirection[1] * m_camera.position[1] + xDirection[2] * m_camera.position[2]);
		m_camera.Xiw[1][3] = -1.0f * (yDirection[0] * m_camera.position[0] + yDirection[1] * m_camera.position[1] + yDirection[2] * m_camera.position[2]);
		m_camera.Xiw[2][3] = -1.0f * (zDirection[0] * m_camera.position[0] + zDirection[1] * m_camera.position[1] + zDirection[2] * m_camera.position[2]);

		//Set w Vector
		m_camera.Xiw[3][0] = 0;
		m_camera.Xiw[3][1] = 0;
		m_camera.Xiw[3][2] = 0;
		m_camera.Xiw[3][3] = 1;
	}

	// Compute Xpi
	{
		// Initialize the Matrix
		for (int j = 0; j < 4; j++)
		{
			for (int i = 0; i < 4; i++)
			{
				m_camera.Xpi[i][j] = 0;
			}
		}

		// Set the Xpi Matrix
		m_camera.Xpi[0][0] = 1;
		m_camera.Xpi[1][1] = 1;
		m_camera.Xpi[2][2] = (float)tan((double)(m_camera.FOV * PI / 180.0) / 2.0);
		m_camera.Xpi[3][2] = (float)tan((double)(m_camera.FOV * PI / 180.0) / 2.0);
		m_camera.Xpi[3][3] = 1;
	}

	int status = 0;

	status |= GzPushMatrix(Xsp);
	status |= GzPushMatrix(m_camera.Xpi);
	status |= GzPushMatrix(m_camera.Xiw);

	if (status)
		return GZ_FAILURE;
	else
		return GZ_SUCCESS;

	return GZ_SUCCESS;
}

int GzRender::GzPutCamera(GzCamera camera)
{
	/* HW 3.8
	/*- overwrite renderer camera structure with new camera definition
	*/
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			m_camera.Xiw[i][j] = camera.Xiw[i][j];
			m_camera.Xpi[i][j] = camera.Xpi[i][j];
		}
	}

	for (int i = 0; i < 3; i++)
	{
		m_camera.position[i] = camera.position[i];
		m_camera.lookat[i] = camera.lookat[i];
		m_camera.worldup[i] = camera.worldup[i];
	}
	m_camera.FOV = camera.FOV;

	return GZ_SUCCESS;
}

int GzRender::GzPushMatrix(GzMatrix	matrix)
{
	/* HW 3.9
	- push a matrix onto the Ximage stack
	- check for stack overflow
	*/
	int status = 0;
	status |= pushMatrixToStack(matlevel, Ximage, matrix);

	GzMatrix identityMatrix;
	for (int j = 0; j < 4; j++) 
	{
		for (int i = 0; i < 4; i++) 
		{
			identityMatrix[j][i] = 0;
		}
	}
	identityMatrix[0][0] = 1.0f;
	identityMatrix[1][1] = 1.0f;
	identityMatrix[2][2] = 1.0f;
	identityMatrix[3][3] = 1.0f;

	if (matlevelNormal < 2) 
	{
		status |= pushMatrixToStack(matlevelNormal, Xnorm, identityMatrix);
	}
	// Xiw
	else if (matlevelNormal == 2) 
	{	
		GzMatrix Xnorm_iw;
		for (int j = 0; j < 4; j++) 
		{
			for (int i = 0; i < 4; i++) 
			{
				Xnorm_iw[j][i] = m_camera.Xiw[j][i];
			}
		}
		Xnorm_iw[0][3] = 0;
		Xnorm_iw[1][3] = 0;
		Xnorm_iw[2][3] = 0;

		status |= pushMatrixToStack(matlevelNormal, Xnorm, Xnorm_iw);
	}
	else 
	{	
		if (
			matrix[0][1] == 0 && matrix[0][2] == 0 &&
			matrix[1][0] == 0 && matrix[1][2] == 0 &&
			matrix[2][0] == 0 && matrix[2][1] == 0
			) 
		{
			status |= pushMatrixToStack(matlevelNormal, Xnorm, identityMatrix);
		}
		else {
			status |= pushMatrixToStack(matlevelNormal, Xnorm, matrix);
		}
	}

	if (status)
		return GZ_FAILURE;
	return GZ_SUCCESS;
}

int GzRender::GzPopMatrix()
{
	/* HW 3.10
	- pop a matrix off the Ximage stack
	- check for stack underflow
	*/
	if (matlevel > -1)
	{
		matlevel--;
	}
	else
		return GZ_FAILURE;

	return GZ_SUCCESS;
}

int GzRender::GzPut(int i, int j, GzIntensity r, GzIntensity g, GzIntensity b, GzIntensity a, GzDepth z)
{
	/* HW1.4 write pixel values into the buffer */
	if (i >= 0 && i < xres && j >= 0 && j < yres)
	{
		// Clamp input.

		if (r > 4095) r = 4095;
		if (g > 4095) g = 4095;
		if (b > 4095) b = 4095;
		if (r < 0) r = 0;
		if (g < 0) g = 0;
		if (b < 0) b = 0;


		int position = ARRAY(i, j);
		if (z < pixelbuffer[position].z) {
			pixelbuffer[position] = { r, g, b, a, z };
		}
	}
	return GZ_SUCCESS;
}

int GzRender::GzGet(int i, int j, GzIntensity* r, GzIntensity* g, GzIntensity* b, GzIntensity* a, GzDepth* z)
{
	/* HW1.5 retrieve a pixel information from the pixel buffer */
	if (i >= 0 && i < xres && j >= 0 && j < yres)
	{
		int position = ARRAY(i, j);
		GzPixel currentPixel = pixelbuffer[position];

		// Copy color values
		{
			*r = currentPixel.red;
			*g = currentPixel.green;
			*b = currentPixel.blue;
			*a = currentPixel.alpha;
			*z = currentPixel.z;
		}
	}
	return GZ_SUCCESS;
}

int GzRender::GzFlushDisplay2File(FILE* outfile)
{
	/* HW1.6 write image to ppm file -- "P6 %d %d 255\r" */
	fprintf(outfile, "P6 %d %d 255\n", xres, yres);
	int resolution = xres * yres;

	for (int i = 0; i < resolution; i++) {
		GzPixel current_pixel = pixelbuffer[i];

		GzIntensity current_red_16_bit = current_pixel.red;
		GzIntensity current_green_16_bit = current_pixel.green;
		GzIntensity current_blue_16_bit = current_pixel.blue;

		// Shift values to 2 bytes and write to file
		{
			GzIntensity red_shifted = current_red_16_bit >> 4;
			char red_value = (char)(red_shifted & 0xFF);

			GzIntensity green_shifted = current_green_16_bit >> 4;
			char green_value = (char)(green_shifted & 0xFF);

			GzIntensity blue_shifted = current_blue_16_bit >> 4;
			char blue_value = (char)(blue_shifted & 0xFF);

			char color[3];
			color[0] = red_value;
			color[1] = green_value;
			color[2] = blue_value;

			fwrite(color, 1, 3, outfile);
		}
	}
	return GZ_SUCCESS;
}

int GzRender::GzFlushDisplay2FrameBuffer()
{
	/* HW1.7 write pixels to framebuffer:
		- put the pixels into the frame buffer
		- CAUTION: when storing the pixels into the frame buffer, the order is blue, green, and red
		- NOT red, green, and blue !!!
	*/
	int resolution = xres * yres;
	for (int i = 0; i < resolution; i++) {
		GzPixel current_pixel = pixelbuffer[i];

		GzIntensity current_red_16_bit = current_pixel.red;
		GzIntensity current_green_16_bit = current_pixel.green;
		GzIntensity current_blue_16_bit = current_pixel.blue;

		// Shift color to 2 bytes and write to buffer
		{
			GzIntensity red_shifted = current_red_16_bit >> 4;
			char red_value = (char)(red_shifted & 0xFF);

			GzIntensity green_shifted = current_green_16_bit >> 4;
			char green_value = (char)(green_shifted & 0xFF);

			GzIntensity blue_shifted = current_blue_16_bit >> 4;
			char blue_value = (char)(blue_shifted & 0xFF);

			framebuffer[3 * i + 2] = red_value;
			framebuffer[3 * i + 1] = green_value;
			framebuffer[3 * i] = blue_value;

		}

	}
	return GZ_SUCCESS;
}

/***********************************************/
/* HW2 methods: implement from here */

int GzRender::GzPutAttribute(int numAttributes, GzToken* nameList, GzPointer* valueList)
{
	/* HW 2.1
	-- Set renderer attribute states (e.g.: GZ_RGB_COLOR default color)
	-- In later homeworks set shaders, interpolaters, texture maps, and lights
	*/
	int index = 0;
	while (index < numAttributes)
	{
		GzToken token = nameList[index];

		switch (token)
		{
			case GZ_RGB_COLOR:
			{
				GzColor* colorRGB = (GzColor*)valueList[index];
				flatcolor[0] = (*colorRGB)[0];
				flatcolor[1] = (*colorRGB)[1];
				flatcolor[2] = (*colorRGB)[2];
				break;
			}
			case GZ_INTERPOLATE:
			{
				int* mode = (int*)valueList[index];
				interp_mode = *mode;
				break;
			}
			case GZ_DIRECTIONAL_LIGHT:
			{
				GzLight* lightDir = (GzLight*)valueList[index];
				lights[numlights].direction[0] = lightDir->direction[0];
				lights[numlights].direction[1] = lightDir->direction[1];
				lights[numlights].direction[2] = lightDir->direction[2];
				lights[numlights].color[0] = lightDir->color[0];
				lights[numlights].color[1] = lightDir->color[1];
				lights[numlights].color[2] = lightDir->color[2];
				numlights++;
				break;
			}
			case GZ_AMBIENT_LIGHT:
			{
				GzLight* lightAmb = (GzLight*)valueList[index];
				ambientlight.direction[0] = lightAmb->direction[0];
				ambientlight.direction[1] = lightAmb->direction[1];
				ambientlight.direction[2] = lightAmb->direction[2];
				ambientlight.color[0] = lightAmb->color[0];
				ambientlight.color[1] = lightAmb->color[1];
				ambientlight.color[2] = lightAmb->color[2];
				break;
			}
			case GZ_AMBIENT_COEFFICIENT: 
			{
				GzColor* colorAmb = (GzColor*)valueList[index];
				Ka[0] = (*colorAmb)[0];
				Ka[1] = (*colorAmb)[1];
				Ka[2] = (*colorAmb)[2];
				break;
			}
			case GZ_DIFFUSE_COEFFICIENT: 
			{
				GzColor* colorDiff = (GzColor*)valueList[index];
				Kd[0] = (*colorDiff)[0];
				Kd[1] = (*colorDiff)[1];
				Kd[2] = (*colorDiff)[2];
				break;
			}
			case GZ_SPECULAR_COEFFICIENT: 
			{
				GzColor* colorSpec = (GzColor*)valueList[index];
				Ks[0] = (*colorSpec)[0];
				Ks[1] = (*colorSpec)[1];
				Ks[2] = (*colorSpec)[2];
				break;
			}
			case GZ_DISTRIBUTION_COEFFICIENT: 
			{
				float* power = (float*)valueList[index];
				spec = *power;
				break;
			}
			case GZ_TEXTURE_MAP: 
			{
				GzTexture textureFunc = (GzTexture)valueList[index];
				tex_fun = textureFunc;
				break;
			}
			case GZ_AASHIFTX:
			{
				xOffset = *(float*)valueList[index];
			}

			case GZ_AASHIFTY: 
			{
				yOffset = *(float*)valueList[index];
			}

			default:
			{
				break;
			}
		}
		index++;
	}

	return GZ_SUCCESS;
}

int GzRender::GzPutTriangle(int	numParts, GzToken* nameList, GzPointer* valueList)
/* numParts - how many names and values */
{
	/* HW 2.2
	-- Pass in a triangle description with tokens and values corresponding to
		  GZ_NULL_TOKEN:		do nothing - no values
		  GZ_POSITION:		3 vert positions in model space
	-- Invoke the rastrizer/scanline framework
	-- Return error code
	*/

	GzCoord* verticesPointer = (GzCoord*)valueList[0];
	GzCoord* normalsPointer = (GzCoord*)valueList[1];
	GzCoord vertices[3];
	GzCoord normals[3];

	// Construct 4D vector:
	float vertices4D[3][4];
	float normals4D[3][4];
	
	for (int j = 0; j < 3; j++)
	{
		for (int i = 0; i < 3; i++) 
		{
			vertices4D[j][i] = verticesPointer[j][i];
			normals4D[j][i] = normalsPointer[j][i];
		}
		vertices4D[j][3] = 1.0f;
		normals4D[j][3] = 1.0f;
	}
	float transformedVertices4D[3][4];
	float transformedNormals4D[3][4];
	// transform to vertieces
	for (int k = 0; k < 3; k++) 
	{
		for (int j = 0; j < 4; j++) {
			float sumVert = 0, sumNorm = 0;
			for (int i = 0; i < 4; i++) 
			{
				sumVert += Ximage[matlevel][j][i] * vertices4D[k][i];
				sumNorm += Xnorm[matlevelNormal][j][i] * normals4D[k][i];
			}
			transformedVertices4D[k][j] = sumVert;
			transformedNormals4D[k][j] = sumNorm;
		}
	}
	// Project 4d into 3d
	for (int j = 0; j < 3; j++)
	{
		if (transformedVertices4D[j][2] < 0)
		{
			return GZ_SUCCESS;
		}
		for (int i = 0; i < 3; i++) 
		{
			vertices[j][i] = transformedVertices4D[j][i] / transformedVertices4D[j][3];
			normals[j][i] = transformedNormals4D[j][i] / transformedNormals4D[j][3];
		}
	}
	// For flat shading
	GzCoord unsortedNormals[3];
	for (int j = 0; j < 3; j++) 
	{
		for (int i = 0; i < 3; i++) 
		{
			unsortedNormals[j][i] = normals[j][i];
		}
	}

	for (int i = 0; i < 3; i++) {
		vertices[i][0] -= xOffset;
		vertices[i][1] -= yOffset;
	}

	// Set up UV list
	GzTextureIndex* uvListPointer = (GzTextureIndex*)valueList[2];
	GzTextureIndex uvList[3];
	for (int j = 0; j < 3; j++) 
	{
		for (int i = 0; i < 2; i++) 
		{
			uvList[j][i] = uvListPointer[j][i];
		}
	}
	// set the vertices to the local variables
	for (int i = 0; i < 3; i++)
	{
		vertices[i][0] = vertices[i][0];
		vertices[i][1] = vertices[i][1];
		vertices[i][2] = vertices[i][2];
	}
	// Sort vertices
	{
		// V1(Y) >  V2(Y)
		if (vertices[0][1] > vertices[1][1]) 
		{
			for (int i = 0; i < 3; i++) 
			{
				std::swap(vertices[0][i], vertices[1][i]);
				std::swap(normals[0][i], normals[1][i]);
				if (i < 2)
				{
					std::swap(uvList[0][i], uvList[1][i]);
				}
			}
		}
		// V1(Y) >  V3(Y)
		if (vertices[0][1] > vertices[2][1]) 
		{
			for (int i = 0; i < 3; i++) 
			{
				std::swap(vertices[0][i], vertices[2][i]);
				std::swap(normals[0][i], normals[2][i]);
				if (i < 2) 
				{
					std::swap(uvList[0][i], uvList[2][i]);
				}
			}
		}
		// V2(Y) >  V3(Y)
		if (vertices[1][1] > vertices[2][1]) 
		{
			for (int i = 0; i < 3; i++) 
			{
				std::swap(vertices[1][i], vertices[2][i]);
				std::swap(normals[1][i], normals[2][i]);
				if (i < 2)
				{
					std::swap(uvList[1][i], uvList[2][i]);
				}
			}
		}
		// V1(Y) ==  V2(Y)
		if ((int)(vertices[0][1] + 0.5) == (int)(vertices[1][1] + 0.5)) 
		{
			// V1(X) > V2(X)
			if (vertices[0][0] > vertices[1][0]) 
			{
				for (int i = 0; i < 3; i++) 
				{
					std::swap(vertices[1][i], vertices[2][i]);
					std::swap(normals[1][i], normals[2][i]);
					if (i < 2)
					{
						std::swap(uvList[1][i], uvList[2][i]);
					}
				}
			}
		}
		// V2(Y) ==  V3(Y)
		else if ((int)(vertices[1][1] + 0.5) == (int)(vertices[2][1] + 0.5)) 
		{
			// V2(X) > V3(X)
			if (vertices[2][0] > vertices[1][0]) 
			{
				for (int i = 0; i < 3; i++) 
				{
					std::swap(vertices[1][i], vertices[2][i]);
					std::swap(normals[1][i], normals[2][i]);
					if (i < 2)
					{
						std::swap(uvList[1][i], uvList[2][i]);
					}
				}
			}
		}
		// Check if order is not CCW
		else 
		{
			float midpoint_x;
			float midpoint_slope;
			// Determin midpoint for swap
			if ((int)(vertices[0][0] + 0.5) == (int)(vertices[2][0] + 0.5)) 
			{
				midpoint_x = vertices[0][0];
			}
			else 
			{
				midpoint_slope = (vertices[0][1] - vertices[2][1]) / (vertices[0][0] - vertices[2][0]);
				midpoint_x = ((vertices[1][1] - vertices[0][1]) / midpoint_slope) + vertices[0][0];
			}
			// Swap if V2 is below midpoint X
			if (midpoint_x > vertices[1][0]) 
			{
				for (int i = 0; i < 3; i++) 
				{
					std::swap(vertices[1][i], vertices[2][i]);
					std::swap(normals[1][i], normals[2][i]);
					if (i < 2)
					{
						std::swap(uvList[1][i], uvList[2][i]);
					}
				}
			}
		}
	}

	float slopeX12, slopeX23, slopeX31;
	float slopeY12, slopeY23, slopeY31;
	float distance12, distance23, distance31;
	// Set up delta(x,y) values and find slopes in screen space
	{
		float deltaX12, deltaY12;
		float deltaX23, deltaY23;
		float deltaX31, deltaY31;
		// Set up delta for clarity
		deltaX12 = vertices[1][0] - vertices[0][0];
		deltaY12 = vertices[1][1] - vertices[0][1];
		deltaX23 = vertices[2][0] - vertices[1][0];
		deltaY23 = vertices[2][1] - vertices[1][1];
		deltaX31 = vertices[0][0] - vertices[2][0];
		deltaY31 = vertices[0][1] - vertices[2][1];
		// Calculate Slopes
		slopeX12 = -1.0f * deltaX12;
		slopeY12 = deltaY12;
		distance12 = deltaX12 * vertices[0][1] - deltaY12 * vertices[0][0];
		slopeX23 = -1.0f * deltaX23;
		slopeY23 = deltaY23;
		distance23 = deltaX23 * vertices[1][1] - deltaY23 * vertices[1][0];
		slopeX31 = -1.0f * deltaX31;
		slopeY31 = deltaY31;
		distance31 = deltaX31 * vertices[2][1] - deltaY31 * vertices[2][0];
	}

	GzColor finalIntensity[3];
	GzColor flatIntensity[3];
	getFinalIntensity(finalIntensity, normals);
	getFinalIntensity(flatIntensity, unsortedNormals);

	// calculation interpolation here
	float planeA, planeB, planeC; /* slopes of the individual planes */
	float planeD; /* the unit normal slope of the plane in 2d space*/
	// Z Interpolator
	{
		float slopeX1, slopeY1, slopeZ1;
		float slopeX2, slopeY2, slopeZ2;
		//Calculate slopes for plane
		slopeX1 = vertices[1][0] - vertices[0][0];
		slopeY1 = vertices[1][1] - vertices[0][1];
		slopeZ1 = vertices[1][2] - vertices[0][2];

		slopeX2 = vertices[2][0] - vertices[0][0];
		slopeY2 = vertices[2][1] - vertices[0][1];
		slopeZ2 = vertices[2][2] - vertices[0][2];

		// Calculate the X, Y, Z plane
		planeA = (slopeY1 * slopeZ2) - (slopeY2 * slopeZ1);
		planeB = -((slopeX1 * slopeZ2) - (slopeX2 * slopeZ1));
		planeC = (slopeX1 * slopeY2) - (slopeY1 * slopeX2);
		planeD = -1.0f * (planeA * vertices[0][0] + planeB * vertices[0][1] + planeC * vertices[0][2]);
	}

	float redPlaneA, redPlaneB, redPlaneC; /* slopes of the individual planes */
	float redPlaneD; /* the unit normal slope of the plane in 2d space*/
	// Red Interpolator
	{
		float redX1, redY1, redZ1;
		float redX2, redY2, redZ2;
		//Calculate slopes for plane
		redX1 = vertices[1][0] - vertices[0][0];
		redY1 = vertices[1][1] - vertices[0][1];
		redZ1 = finalIntensity[1][0] - finalIntensity[0][0];

		redX2 = vertices[2][0] - vertices[0][0];
		redY2 = vertices[2][1] - vertices[0][1];
		redZ2 = finalIntensity[2][0] - finalIntensity[0][0];

		// Calculate the X, Y, Z plane
		redPlaneA = (redY1 * redZ2) - (redY2 * redZ1);
		redPlaneB = -((redX1 * redZ2) - (redX2 * redZ1));
		redPlaneC = (redX1 * redY2) - (redY1 * redX2);
		redPlaneD = -1.0f * (redPlaneA * vertices[0][0] + redPlaneB * vertices[0][1] + redPlaneC * finalIntensity[0][0]);
	}

	float greenPlaneA, greenPlaneB, greenPlaneC; /* slopes of the individual planes */
	float greenPlaneD; /* the unit normal slope of the plane in 2d space*/
	// Blue Interpolator
	{
		float greenX1, greenY1, greenZ1;
		float greenX2, greenY2, greenZ2;
		//Calculate slopes for plane
		greenX1 = vertices[1][0] - vertices[0][0];
		greenY1 = vertices[1][1] - vertices[0][1];
		greenZ1 = finalIntensity[1][1] - finalIntensity[0][1];

		greenX2 = vertices[2][0] - vertices[0][0];
		greenY2 = vertices[2][1] - vertices[0][1];
		greenZ2 = finalIntensity[2][1] - finalIntensity[0][1];

		// Calculate the X, Y, Z plane
		greenPlaneA = (greenY1 * greenZ2) - (greenY2 * greenZ1);
		greenPlaneB = -((greenX1 * greenZ2) - (greenX2 * greenZ1));
		greenPlaneC = (greenX1 * greenY2) - (greenY1 * greenX2);
		greenPlaneD = -1.0f * (greenPlaneA * vertices[0][0] + greenPlaneB * vertices[0][1] + greenPlaneC * finalIntensity[0][1]);
	}

	float bluePlaneA, bluePlaneB, bluePlaneC; /* slopes of the individual planes */
	float bluePlaneD; /* the unit normal slope of the plane in 2d space*/
	// Z Interpolator
	{
		float blueX1, blueY1, blueZ1;
		float blueX2, blueY2, blueZ3;

		blueX1 = vertices[1][0] - vertices[0][0];
		blueY1 = vertices[1][1] - vertices[0][1];
		blueZ1 = finalIntensity[1][2] - finalIntensity[0][2];

		blueX2 = vertices[2][0] - vertices[0][0];
		blueY2 = vertices[2][1] - vertices[0][1];
		blueZ3 = finalIntensity[2][2] - finalIntensity[0][2];

		// Calculate the X, Y, Z plane
		bluePlaneA = (blueY1 * blueZ3) - (blueY2 * blueZ1);
		bluePlaneB = -((blueX1 * blueZ3) - (blueX2 * blueZ1));
		bluePlaneC = (blueX1 * blueY2) - (blueY1 * blueX2);
		bluePlaneD = -1.0f * (bluePlaneA * vertices[0][0] + bluePlaneB * vertices[0][1] + bluePlaneC * finalIntensity[0][2]);
	}

	float normalXPlaneA, normalXPlaneB, normalXPlaneC;
	float normalXPlaneD;
	//NormalX Interpolation
	{
		float normalXX1, normalXY1, normalXZ1;
		float normalXX2, normalXY2, normalXZ2;

		normalXX1 = vertices[1][0] - vertices[0][0];
		normalXY1 = vertices[1][1] - vertices[0][1];
		normalXZ1 = normals[1][0] - normals[0][0];

		normalXX2 = vertices[2][0] - vertices[0][0];
		normalXY2 = vertices[2][1] - vertices[0][1];
		normalXZ2 = normals[2][0] - normals[0][0];

		normalXPlaneA = (normalXY1 * normalXZ2) - (normalXZ1 * normalXY2);
		normalXPlaneB = -((normalXX1 * normalXZ2) - (normalXZ1 * normalXX2));
		normalXPlaneC = (normalXX1 * normalXY2) - (normalXY1 * normalXX2);
		normalXPlaneD = -1.0f * (normalXPlaneA * vertices[0][0] + normalXPlaneB * vertices[0][1] + normalXPlaneC * normals[0][0]);
	}

	float normalYPlaneA, normalYPlaneB, normalYPlaneC;
	float normalYPlaneD;
	//NormalY Interpolation
	{
		float normalYX1, normalYY1, normalYZ1;
		float normalYX2, normalYY2, normalYZ2;

		normalYX1 = vertices[1][0] - vertices[0][0];
		normalYY1 = vertices[1][1] - vertices[0][1];
		normalYZ1 = normals[1][1] - normals[0][1];

		normalYX2 = vertices[2][0] - vertices[0][0];
		normalYY2 = vertices[2][1] - vertices[0][1];
		normalYZ2 = normals[2][1] - normals[0][1];

		normalYPlaneA = (normalYY1 * normalYZ2) - (normalYZ1 * normalYY2);
		normalYPlaneB = -((normalYX1 * normalYZ2) - (normalYZ1 * normalYX2));
		normalYPlaneC = (normalYX1 * normalYY2) - (normalYY1 * normalYX2);
		normalYPlaneD = -1.0f * (normalYPlaneA * vertices[0][0] + normalYPlaneB * vertices[0][1] + normalYPlaneC * normals[0][1]);
	}

	float normalZPlaneA, normalZPlaneB, normalZPlaneC;
	float normalZPlaneD;
	//NormalZ Interpolation
	{
		float normalZX1, normalZY1, normalZZ1;
		float normalZX2, normalZY2, normalZZ2;

		normalZX1 = vertices[1][0] - vertices[0][0];
		normalZY1 = vertices[1][1] - vertices[0][1];
		normalZZ1 = normals[1][2] - normals[0][2];

		normalZX2 = vertices[2][0] - vertices[0][0];
		normalZY2 = vertices[2][1] - vertices[0][1];
		normalZZ2 = normals[2][2] - normals[0][2];

		normalZPlaneA = (normalZY1 * normalZZ2) - (normalZZ1 * normalZY2);
		normalZPlaneB = -((normalZX1 * normalZZ2) - (normalZZ1 * normalZX2));
		normalZPlaneC = (normalZX1 * normalZY2) - (normalZY1 * normalZX2);
		normalZPlaneD = -1.0f * (normalZPlaneA * vertices[0][0] + normalZPlaneB * vertices[0][1] + normalZPlaneC * normals[0][2]);
	}
	// Perspective Correction
	float vPrimeZ;
	GzTextureIndex uvPerspectiveList[3];
	for (int j = 0; j < 3; j++) 
	{
		vPrimeZ = vertices[j][2] / ((float)MAXINT - vertices[j][2]);
		uvPerspectiveList[j][U] = uvList[j][U] / (vPrimeZ + 1.0f);
		uvPerspectiveList[j][V] = uvList[j][V] / (vPrimeZ + 1.0f);
	}
	// Interpolate U
	float uPlaneA, uPlaneB, uPlaneC;
	float uPlaneD;
	{
		float uX1 = vertices[1][0] - vertices[0][0];
		float uY1 = vertices[1][1] - vertices[0][1];
		float uZ1 = uvPerspectiveList[1][0] - uvPerspectiveList[0][0];

		float uX2 = vertices[2][0] - vertices[0][0];
		float uY2 = vertices[2][1] - vertices[0][1];
		float uZ2 = uvPerspectiveList[2][0] - uvPerspectiveList[0][0];

		uPlaneA = (uY1 * uZ2) - (uZ1 * uY2);
		uPlaneB = -((uX1 * uZ2) - (uZ1 * uX2));
		uPlaneC = (uX1 * uY2) - (uY1 * uX2);
		uPlaneD = -1.0f * (uPlaneA * vertices[0][0] + uPlaneB * vertices[0][1] + uPlaneC * uvPerspectiveList[0][0]);
	}
	// Interpolate V
	float vPlaneA, vPlaneB, vPlaneC;
	float vPlaneD;
	{
		float vX1 = vertices[1][0] - vertices[0][0];
		float vY1 = vertices[1][1] - vertices[0][1];
		float vZ1 = uvPerspectiveList[1][1] - uvPerspectiveList[0][1];

		float vX2 = vertices[2][0] - vertices[0][0];
		float vY2 = vertices[2][1] - vertices[0][1];
		float vZ2 = uvPerspectiveList[2][1] - uvPerspectiveList[0][1];

		vPlaneA = (vY1 * vZ2) - (vZ1 * vY2);
		vPlaneB = -((vX1 * vZ2) - (vZ1 * vX2));
		vPlaneC = (vX1 * vY2) - (vY1 * vX2);
		vPlaneD = -1.0f * (vPlaneA * vertices[0][0] + vPlaneB * vertices[0][1] + vPlaneC * uvPerspectiveList[0][1]);
	}
	float minX, maxX;
	float minY, maxY;
	// Bounding box adjusted for pixel
	{
		float tempMinX = min(min(vertices[0][0], vertices[1][0]), vertices[2][0]);
		float tempMaxX = max(max(vertices[0][0], vertices[1][0]), vertices[2][0]);
		float tempMinY = min(min(vertices[0][1], vertices[1][1]), vertices[2][1]);
		float tempMaxY = max(max(vertices[0][1], vertices[1][1]), vertices[2][1]);

		// Rounding up
		minX = (int)(tempMinX + 0.5);
		maxX = (int)(tempMaxX + 0.5);
		minY = (int)(tempMinY + 0.5);
		maxY = (int)(tempMaxY + 0.5);
	}

	// Shift for pixels
	for (int i = minX; i <= maxX; i++) 
	{
		for (int j = minY; j <= maxY; j++) 
		{

			float lee12 = slopeY12 * (float)i + slopeX12 * (float)j + distance12;
			float lee23 = slopeY23 * (float)i + slopeX23 * (float)j + distance23;
			float lee31 = slopeY31 * (float)i + slopeX31 * (float)j + distance31;

			if ((lee12 > 0 && lee23 > 0 && lee31 > 0 && planeC != 0) ||
				(lee12 < 0 && lee23 < 0 && lee31 < 0 && planeC != 0) ||
				(lee12 == 0 || lee23 == 0 || lee31 == 0) &&
				redPlaneC * greenPlaneC * bluePlaneC != 0 &&
				normalXPlaneC * normalYPlaneC * normalZPlaneC != 0)
			{
				float interpolated_z = -1.0f * (planeA * (float)i + planeB * (float)j + planeD) / planeC;
				int currentZ = (int)(interpolated_z + 0.5);
			
				GzIntensity redValue = 0, greenValue = 0, blueValue = 0;

				if (interp_mode == GZ_FLAT )
				{
					redValue = ctoi(flatIntensity[0][0]);
					greenValue = ctoi(flatIntensity[0][1]);
					blueValue = ctoi(flatIntensity[0][2]);
				}
				else if (interp_mode == GZ_COLOR)
				{
					GzColor intensity;
					intensity[0] = -1.0f * (redPlaneA * (float)i + redPlaneB * (float)j + redPlaneD) / redPlaneC;
					intensity[1] = -1.0f * (greenPlaneA * (float)i + greenPlaneB * (float)j + greenPlaneD) / greenPlaneC;
					intensity[2] = -1.0f * (bluePlaneA * (float)i + bluePlaneB * (float)j + bluePlaneD) / bluePlaneC;

					if (tex_fun != NULL) 
					{
						GzTextureIndex currentUV;
						GzColor uvColor;
						float vzPrimeInterp = (float)currentZ / ((float)MAXINT - (float)currentZ);

						currentUV[U] = -1.0f * (uPlaneA * (float)i + uPlaneB * (float)j + uPlaneD) / uPlaneC;
						currentUV[V] = -1.0f * (vPlaneA * (float)i + vPlaneB * (float)j + vPlaneD) / vPlaneC;

						currentUV[U] = currentUV[U] * (vzPrimeInterp + 1.0f);
						currentUV[V] = currentUV[V] * (vzPrimeInterp + 1.0f);

						int status = tex_fun(currentUV[U], currentUV[V], uvColor);
						if (status) 
						{
							return GZ_FAILURE;
						}

						for (int m = 0; m < 3; m++) 
						{
							intensity[m] *= uvColor[m];
						}
					}

					redValue = ctoi(intensity[0]);
					greenValue = ctoi(intensity[1]);
					blueValue = ctoi(intensity[2]);
				}
				else if (interp_mode == GZ_NORMAL) 
				{
					GzColor intensity;
					GzColor interpolatedNormals;
					interpolatedNormals[0] = -1.0f * (normalXPlaneA * (float)i + normalXPlaneB * (float)j + normalXPlaneD) / normalXPlaneC;
					interpolatedNormals[1] = -1.0f * (normalYPlaneA * (float)i + normalYPlaneB * (float)j + normalYPlaneD) / normalYPlaneC;
					interpolatedNormals[2] = -1.0f * (normalZPlaneA * (float)i + normalZPlaneB * (float)j + normalZPlaneD) / normalZPlaneC;
					
					if (tex_fun != NULL) 
					{
						GzTextureIndex currentUV;
						GzColor uvColor;
						float vzPrimeInterp = (float)currentZ / ((float)MAXINT - (float)currentZ);
						currentUV[U] = -1.0f * (uPlaneA * (float)i + uPlaneB * (float)j + uPlaneD) / uPlaneC;
						currentUV[V] = -1.0f * (vPlaneA * (float)i + vPlaneB * (float)j + vPlaneD) / vPlaneC;

						currentUV[U] = currentUV[U] * (vzPrimeInterp + 1.0f);
						currentUV[V] = currentUV[V] * (vzPrimeInterp + 1.0f);

						int status = tex_fun(currentUV[U], currentUV[V], uvColor);
						if (status) 
						{
							return GZ_FAILURE;
						}

						for (int m = 0; m < 3; m++) 
						{
							Kd[m] = uvColor[m];
							Ka[m] = uvColor[m];
						}

					}


					getPixelIntensity(intensity, interpolatedNormals);

					redValue = ctoi(intensity[0]);
					greenValue = ctoi(intensity[1]);
					blueValue = ctoi(intensity[2]);
				} 
				
				GzPut(i, j, redValue, greenValue, blueValue, 1, currentZ);
			}
		}
	}

	return GZ_SUCCESS;
}

int pushMatrixToStack(short& matlevel, GzMatrix* matStack, GzMatrix mat)
{
	if (matlevel < MATLEVELS)
	{
		if (matlevel == -1)
		{
			// Copy matrix over.
			for (int j = 0; j < 4; j++)
			{
				for (int i = 0; i < 4; i++)
				{
					matStack[0][i][j] = mat[i][j];
				}
			}
		}
		else
		{
			// Preform matrix multiplication
			for (int k = 0; k < 4; k++)
			{
				for (int j = 0; j < 4; j++)
				{
					float result = 0;
					for (int i = 0; i < 4; i++)
					{
						result += matStack[matlevel][k][i] * mat[i][j];
					}
					matStack[matlevel + 1][k][j] = result;
				}
			}
		}
		matlevel++;
	}
	else
		return GZ_FAILURE;

	return GZ_SUCCESS;
}

void GzRender::getFinalIntensity(GzColor* finalIntensity, GzCoord* normals)
{
	GzColor specularIntensity[3], diffuseIntensity[3], ambientIntensity[3];
	// For Flat Shading
	for (int j = 0; j < 3; j++)
	{
		for (int i = 0; i < 3; i++)
		{
			specularIntensity[j][i] = 0;
			diffuseIntensity[j][i] = 0;
			ambientIntensity[j][i] = 0;
			finalIntensity[j][i] = 0;
		}
	}
	for (int j = 0; j < 3; j++)
	{
		for (int i = 0; i < numlights; i++)
		{
			GzCoord R, E;
			E[0] = 0;
			E[1] = 0;
			E[2] = -0.1f;

			float nDotL =
				normals[j][0] * (lights[i]).direction[0] +
				normals[j][1] * (lights[i]).direction[1] +
				normals[j][2] * (lights[i]).direction[2];

			float nDotE =
				normals[j][0] * E[0] +
				normals[j][1] * E[1] +
				normals[j][2] * E[2];

			if (nDotE * nDotL > 0)
			{
				// Compute Reflection
				for (int k = 0; k < 3; k++)
				{
					R[k] = 2.0f * nDotL * normals[j][k] - (lights[i]).direction[k];
				}
				float lengthR = sqrt((R[0] * R[0] + R[1] * R[1] + R[2] * R[2]));
				for (int k = 0; k < 3; k++)
				{
					R[k] /= lengthR;
				}

				for (int k = 0; k < 3; k++)
				{
					float rDotE = R[0] * E[0] + R[1] * E[1] + R[2] * E[2];
					// Clamp
					if (rDotE < 0)
					{
						rDotE = 0;
					}
					if (rDotE > 1.0f)
					{
						rDotE = 1.0f;
					}

					specularIntensity[j][k] += Ks[k] * pow((rDotE), spec) * (lights[i]).color[k];
					if (nDotL > 0 && nDotE > 0) {
						diffuseIntensity[j][k] += Kd[k] *
								(normals[j][0] * (lights[i]).direction[0] +
								normals[j][1] * (lights[i]).direction[1] +
								normals[j][2] * (lights[i]).direction[2]) *
								(lights[i]).color[k];
					}
					else
					{
						diffuseIntensity[j][k] += Kd[k] *
								(-1.0f * normals[j][0] * (lights[i]).direction[0]
								- 1.0f * normals[j][1] * (lights[i]).direction[1]
								- 1.0f * normals[j][2] * (lights[i]).direction[2]) *
								(lights[i]).color[k];
					}
				}
			}
		}

		for (int k = 0; k < 3; k++)
		{
			ambientIntensity[j][k] += Ka[k] * ambientlight.color[k];
		}
	}

	for (int j = 0; j < 3; j++)
	{
		for (int i = 0; i < 3; i++)
		{
			finalIntensity[j][i] = specularIntensity[j][i] + diffuseIntensity[j][i] + ambientIntensity[j][i];
			if (finalIntensity[j][i] > 1.0)
			{
				finalIntensity[j][i] = 1.0;
			}
			if (finalIntensity[j][i] < 0)
			{
				finalIntensity[j][i] = 0;
			}
		}
	}
}

void GzRender::getPixelIntensity(GzColor& intensity, GzCoord& interpolatedNormal)
{
	GzColor specularIntensity, diffuseIntensity, ambientIntencity;
	for (int m = 0; m < 3; m++) {
		specularIntensity[m] = 0;
		diffuseIntensity[m] = 0;
		ambientIntencity[m] = 0;
	}

	for (int m = 0; m < numlights; m++) {
		GzCoord R, E;

		E[0] = 0;
		E[1] = 0;
		E[2] = -1.0f;

		float nDotL = interpolatedNormal[0] * (lights[m]).direction[0]
			+ interpolatedNormal[1] * (lights[m]).direction[1]
			+ interpolatedNormal[2] * (lights[m]).direction[2];
		float nDotE = interpolatedNormal[0] * E[0] + interpolatedNormal[1] * E[1] + interpolatedNormal[2] * E[2];

		if (nDotL * nDotE > 0) {
			for (int k = 0; k < 3; k++) 
			{
				R[k] = 2.0f * nDotL * interpolatedNormal[k] - (lights[m]).direction[k];
			}
			float lengthR = (float)sqrt((double)(R[0] * R[0] + R[1] * R[1] + R[2] * R[2]));
			for (int k = 0; k < 3; k++) 
			{
				R[k] /= lengthR;
			}

			for (int k = 0; k < 3; k++) 
			{
				float rDotE = R[0] * E[0] + R[1] * E[1] + R[2] * E[2];
				if (rDotE < 0) {
					rDotE = 0;
				}
				if (rDotE > 1.0f) {
					rDotE = 1.0f;
				}

				specularIntensity[k] += Ks[k] *
					(float)pow((double)(rDotE), (double)spec)
					* (lights[m]).color[k];
				if (nDotL > 0 && nDotE > 0) 
				{
					diffuseIntensity[k] += Kd[k] *
						(interpolatedNormal[0] * (lights[m]).direction[0]
							+ interpolatedNormal[1] * (lights[m]).direction[1]
							+ interpolatedNormal[2] * (lights[m]).direction[2]) * (lights[m]).color[k];
				}
				else 
				{
					diffuseIntensity[k] += Kd[k] *
						(-1.0f * interpolatedNormal[0] * (lights[m]).direction[0]
							- 1.0f * interpolatedNormal[1] * (lights[m]).direction[1]
							- 1.0f * interpolatedNormal[2] * (lights[m]).direction[2]) * (lights[m]).color[k];
				}
			}
		}
	}

	for (int m = 0; m < 3; m++) 
	{
		ambientIntencity[m] += Ka[m] * ambientlight.color[m];
	}

	for (int m = 0; m < 3; m++) 
	{
		intensity[m] = specularIntensity[m] + diffuseIntensity[m] + ambientIntencity[m];
		if (intensity[m] > 1.0) 
		{
			intensity[m] = 1.0;
		}
		if (intensity[m] < 0) 
		{
			intensity[m] = 0;
		}
	}

}