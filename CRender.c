#include <stdio.h>
#include <unistd.h>
#include <math.h>

bool playing = 1; //main loop constant

unsigned int canvasWidth = 120;
unsigned int canvasHeight = 40;

//-------------points_update constants here-------------//
int point0x = 0;
int point0y = 0;
int point1x = 0;
int point1y = 0;
int point2x = 0;
int point2y = 0;
int point3x = 0;
int point3y = 0;

int point4x = 0;
int point4y = 0;
int point5x = 0;
int point5y = 0;
int point6x = 0;
int point6y = 0;
int point7x = 0;
int point7y = 0;

double sin0 = M_PI * 3/4;
double cos0 = M_PI * 3/4;
double sin1 = M_PI * 5/4;
double cos1 = M_PI * 5/4;
double sin2 = M_PI * 7/4;
double cos2 = M_PI * 7/4;
double sin3 = M_PI / 4;
double cos3 = M_PI / 4;

int firstTime = false;

int main() {
//coordinates -> {{x, y}, {x2, y2}, ...}
	int coordinates[][2] = {
	{canvasWidth/2, canvasHeight/2},
	{canvasWidth/2, (canvasHeight/2)},
	{(canvasWidth/2), (canvasHeight/2)}, 
	{(canvasWidth/2), (canvasHeight/2)},
	{(canvasWidth/2), (canvasHeight/2)},
	{(canvasWidth/2), (canvasHeight/2)},
	{(canvasWidth/2), (canvasHeight/2)},
	{(canvasWidth/2), (canvasHeight/2)}
	};
	unsigned int coordinatesSize = sizeof(coordinates) / sizeof(coordinates[0]); //coordinates array size
	
	//coordinatesCalc array, transforms each coordinate in a specific pixel with x + (y * canvasWidth)
	int coordinatesCalc[coordinatesSize];
	for (int i = 0; i < coordinatesSize; i++)
	{
		coordinatesCalc[i] = (coordinates[i][0]) + (coordinates[i][1] * canvasWidth);
	}
	
	char* frameBuffer[(canvasWidth * canvasHeight) - 1]; //frame buffer array
	
	unsigned int frameBufferSize = sizeof(frameBuffer) / sizeof(frameBuffer[0]);//frame buffer array size 
	unsigned int coordinatesCalcSize = sizeof(coordinatesCalc) / sizeof(coordinatesCalc[0]); //coordinatesCalc array size 
	//printf("%d", coordinatesCalcSize);

	//big loop starts
	playing = 1;
	while (playing)
	{
		//printf("playing");
		for (int i = 0; i < coordinatesSize; i++)
		{
			//printf("carregando coordenadas %d\n", i);
			coordinatesCalc[i] = (coordinates[i][0]) + (coordinates[i][1] * canvasWidth);
			//printf("\n%d = %d\n", i, coordinatesCalc[i]	);
		}
		
		//order of vertexes connection, in the coordinatesCalc array
		int connectionOrder[][2] = {{coordinatesCalc[0], 0}, {coordinatesCalc[1], 1},
		{coordinatesCalc[2], 2}, {coordinatesCalc[6], 6},
		{coordinatesCalc[7], 7}, {coordinatesCalc[4], 4},
		{coordinatesCalc[0], 0}, {coordinatesCalc[3], 3},
		{coordinatesCalc[2], 2}, {coordinatesCalc[1], 1},
		{coordinatesCalc[5], 5}, {coordinatesCalc[6], 6},
		{coordinatesCalc[5], 5}, {coordinatesCalc[4], 4},
		{coordinatesCalc[7], 7}, {coordinatesCalc[3], 3}}; //this could def be improved
		unsigned int connectionOrderSize = sizeof(connectionOrder) / sizeof(connectionOrder[0]	);//connectionOrder array size
		
		for (int i = 0; i <= frameBufferSize; i++) //puts everything in the frameBuffer array
		{
			//printf("in the buffer");
			for (int j = 0; j < coordinatesCalcSize; j++)
			{
				if (coordinatesCalc[j] == i) //puts the coordinatesCalc into the array
				{
					frameBuffer[i] = ".";
					//printf("coordinate |%d %d| \n", j, coordinatesCalc[j]);
					j = coordinatesCalcSize;
				}
				else 
				{
					frameBuffer[i] = " "; //fills the framebuffer with " "
				}			
			}
		}
		//printf("\nbuffer finished\n");
		
		
		//LINES CALCULATIONS
		for (int i = 0; i < connectionOrderSize - 1; i++) //connects points, I should make it do in sets of 4 to make triangles
		{
			//printf("line calculations begin\n");
			int coordinateN = connectionOrder[i][1]; //to be able to go to a coordinate different than the current i, this will need to change
			int coordinateM = connectionOrder[i + 1][1];
			//printf(" lines i = %d, coordinateN = %d, coordinateM = %d", i, coordinateN, coordinateM);
			if (connectionOrder[i][0] < connectionOrder[i + 1][0]) // first point is behind the point to connect to
			{
				double x1 = coordinates[coordinateN][0];
				double x2 = coordinates[coordinateM][0];
				double y1 = coordinates[coordinateN][1];
				double y2 = coordinates[coordinateM][1];
				double m = ((y2 - y1) / (x2 - x1));
				//printf("\nfirst point %d is behind the second one %d\n", connectionOrder[i][0], connectionOrder[i + 1][0]);
				//printf("m = %f\n", m);
				if (m > 0)
				{
					for (int j = connectionOrder[i][0]; j <= connectionOrder[i + 1][0]; j++) //checks every pixel in between both points
					{
						
						//printf(" | start = %d | end = %d | current = %d |\n", connectionOrder[i], connectionOrder[i + 1], j);
						if (frameBuffer[j] == " ")
						{
							int k = j;
							//printf("| %d | ", j);
							//printf("começando cálculo... \n");
							//takes the x and y coordinate of the current pixel being checked.
							//Then, puts the x coordinate of the current pixel in the equation to find what y should be and vice versa.
							//if the current pixel Y is equal to the expected X coordinate, put the pixel in the buffer.
							//if the current pixel X is equal to the expected Y coordinate, put the pixel in the buffer.
							double xCurrent = j % canvasWidth;
							double yCurrent = (j - xCurrent)/canvasWidth;
							//printf("\n| xCurrent = %d | yCurrent = %d | x1 = %d | x2 = %d | y1 = %d | y2 = %d | j = %d | m = %d | y2 - y1 = %d | x2 - x1 = %d |", xCurrent, yCurrent, x1, x2, y1, y2, j, m, y2 - y1, x2 - x1);
							double yTarget = m * (xCurrent - x1) + y1;
							//printf("| yTarget calculated %d | ", yTarget);
							double xTarget = yCurrent / m - (y1 / m) + x1; 
							//printf("xTarget calculated %d |\n", xTarget);
							
							//printf("\n|y - %f -> %f | x - %f -> %f|", yCurrent, yTarget, xCurrent, xTarget); 
							if ((int)yCurrent == (int)yTarget)
							{
								frameBuffer[j] = ".";
								//	printf("| %d |", j);
								//printf("-> botou ponto da linha no buffer i = %d | j = %d | pixel = %s |\n", i, j, frameBuffer[j]);
							}
							if ((int)xCurrent == (int)xTarget)
							{
								frameBuffer[j] = ".";
								//printf("| %d |", j);
							}
							//printf("fim do cálculo\n");
						}
					}
				}
				else if (m < 0)
				{
					int offset1 = connectionOrder[i][0] % canvasWidth + 1; //offset needed as when m is negative, the formula doesn't work as well as it should for some reason
					int offset2 = connectionOrder[i + 1][0] % canvasWidth + 1;
					for (int j = connectionOrder[i][0] - offset1; j <= connectionOrder[i + 1][0] - offset2; j++) //checks every pixel in between both points
					{
						//printf(" | start = %d | end = %d | current = %d |\n", connectionOrder[i], connectionOrder[i + 1], j);
						if (frameBuffer[j] == " ")
						{
							int k = j;
							//printf("| %d | ", j);
							//printf("começando cálculo... \n");
							double xCurrent = j % canvasWidth;
							double yCurrent = (j - xCurrent)/canvasWidth;
							//printf("\n| xCurrent = %d | yCurrent = %d | x1 = %d | x2 = %d | y1 = %d | y2 = %d | j = %d | m = %d | y2 - y1 = %d | x2 - x1 = %d |", xCurrent, yCurrent, x1, x2, y1, y2, j, m, y2 - y1, x2 - x1);
							double yTarget = m * (xCurrent - x1) + y1;
							//printf("| yTarget calculated %d | ", yTarget);
							double xTarget = yCurrent / m - (y1 / m) + x1; 
							//printf("xTarget calculated %d |\n", xTarget);
							
							//printf("\n|y - %f -> %f | x - %f -> %f|", yCurrent, yTarget, xCurrent, xTarget); 
							if ((int)yCurrent == (int)yTarget)
							{
								frameBuffer[j] = ".";
								//	printf("| %d |", j);
								//printf("-> botou ponto da linha no buffer i = %d | j = %d | pixel = %s |\n", i, j, frameBuffer[j]);
							}
							if ((int)xCurrent == (int)xTarget)
							{
								frameBuffer[j] = ".";
								//printf("| %d |", j);
							}
							//printf("fim do cálculo\n");
						}
					}
				}
				else if (m == 0) //needed as when m is 0, the formula would divide by 0.
				{
					for (int j = connectionOrder[i][0]; j <= connectionOrder[i + 1][0]; j++)
					{
						frameBuffer[j] = ".";
					}
				}
			}
			else if (connectionOrder[i][0] > connectionOrder[i + 1][0])
			{
				double x1 = coordinates[coordinateN][0];
				double x2 = coordinates[coordinateM][0];
				double y1 = coordinates[coordinateN][1];
				double y2 = coordinates[coordinateM][1];
				double m = ((y2 - y1) / (x2 - x1));
				//printf("\nfirst point %d is ahead of the second one %d\n", connectionOrder[i][0], connectionOrder[i + 1][0]);
				//printf("m = %f\n", m);
				if (m > 0)
				{
					for (int j = connectionOrder[i][0]; j >= connectionOrder[i + 1][0]; j--)
					{
						if (frameBuffer[j] == " ")
						{
							//printf("começando cálculo, %d", connectionOrder[i]);
							double xCurrent = j % canvasWidth;
							double yCurrent = (j - xCurrent)/canvasWidth;
							double yTarget = m * (xCurrent - x1) + y1;
							double xTarget = yCurrent / m - (y1 / m) + x1;
							//printf("| xCurrent = %doubled | yCurrent = %d | x1 = %d | x2 = %d | y1 = %d | y2 = %d | j = %d | m = %d |\n", xCurrent, yCurrent, x1, x2, y1, y2, j, m);
							if ((int)yCurrent == (int)yTarget)
							{
								frameBuffer[j] = ".";
								//printf("| %d |", j);
								//printf("-> botou ponto da linha no buffer i = %d | j = %d | pixel = %s |\n", i, j, frameBuffer[j]);
							}
							if ((int)xCurrent == (int)xTarget)
							{
								frameBuffer[j] = ".";
								//printf("| %d |", j);
							}
						}
					}
				}
				else if (m < 0)
				{
					int offset1 = connectionOrder[i][0] % canvasWidth + 1;
					int offset2 = connectionOrder[i + 1][0] % canvasWidth + 1;
					for (int j = connectionOrder[i][0] - offset1; j >= connectionOrder[i + 1][0] - offset2; j--) //checks every pixel in between both points
					{
						//printf(" | start = %d | end = %d | current = %d |\n", connectionOrder[i], connectionOrder[i + 1], j);
						if (frameBuffer[j] == " ")
						{
							int k = j;
							//printf("| %d | ", j);
							//printf("começando cálculo... \n");
							double xCurrent = j % canvasWidth;
							double yCurrent = (j - xCurrent)/canvasWidth;
							//printf("\n| xCurrent = %d | yCurrent = %d | x1 = %d | x2 = %d | y1 = %d | y2 = %d | j = %d | m = %d | y2 - y1 = %d | x2 - x1 = %d |", xCurrent, yCurrent, x1, x2, y1, y2, j, m, y2 - y1, x2 - x1);
							double yTarget = m * (xCurrent - x1) + y1;
							//printf("| yTarget calculated %d | ", yTarget);
							double xTarget = yCurrent / m - (y1 / m) + x1; 
							//printf("xTarget calculated %d |\n", xTarget);
							
							//printf("\n|y - %f -> %f | x - %f -> %f|", yCurrent, yTarget, xCurrent, xTarget); 
							if ((int)yCurrent == (int)yTarget)
							{
								frameBuffer[j] = ".";
								//printf("| %d |", j);
								//printf("-> botou ponto da linha no buffer i = %d | j = %d | pixel = %s |\n", i, j, frameBuffer[j]);
							}
							if ((int)xCurrent == (int)xTarget)
							{
								frameBuffer[j] = ".";
								//printf("| %d |", j);
							}
							//printf("fim do cálculo\n");
						}
					}
				}
				else
				{
					for (int j = connectionOrder[i][0]; j >= connectionOrder[i + 1][0]; j--)
					{
						frameBuffer[j] = ".";
					}
				}					
			}
			if (coordinateN == coordinatesSize)
			{
				coordinateN = 0;
				//printf("coordinateSize reset");
			}
		}
		
		
	  //-------------------------------------------Points Update-------------------------------------------//
		if (firstTime == false)
		{
			point0x = coordinates[0][0];
			point0y = coordinates[0][1];
			
			point1x = coordinates[1][0];
			point1y = coordinates[1][1];
			
			point2x = coordinates[2][0];
			point2y = coordinates[2][1];
			
			point3x = coordinates[3][0];
			point3y = coordinates[3][1];
			
			point4x = coordinates[4][0];
			point4y = coordinates[4][1];
			
			point5x = coordinates[4][0];
			point5y = coordinates[4][1];
			
			point6x = coordinates[4][0];
			point6y = coordinates[4][1];
			
			point7x = coordinates[4][0];
			point7y = coordinates[4][1];
			
			firstTime = true;
		}
		coordinates[0][0] = point0x + (sin(sin0) * 30);
		coordinates[0][1] = point0y + (cos(cos0) * 10) - 8;
	
		coordinates[1][0] = point1x + (sin(sin1) * 30);
		coordinates[1][1] = point1y + (cos(cos1) * 10) - 8;
		
		coordinates[2][0] = point2x + (sin(sin2) * 30);
		coordinates[2][1] = point2y + (cos(cos2) * 10) - 8;
		
		coordinates[3][0] = point3x + (sin(sin3) * 30);
		coordinates[3][1] = point3y + (cos(cos3) * 10) - 8;
		
		coordinates[4][0] = point4x + (sin(sin0) * 30);
		coordinates[4][1] = point4y + (cos(cos0) * 10) + 3;
		
		coordinates[5][0] = point4x + (sin(sin1) * 30);
		coordinates[5][1] = point4y + (cos(cos1) * 10) + 3;
		
		coordinates[6][0] = point4x + (sin(sin2) * 30);
		coordinates[6][1] = point4y + (cos(cos2) * 10) + 3;
		
		coordinates[7][0] = point4x + (sin(sin3) * 30);
		coordinates[7][1] = point4y + (cos(cos3) * 10) + 3;
		
		sin0 += 0.3;
		cos0 += 0.3;
		sin1 += 0.3;
		cos1 += 0.3;
		sin2 += 0.3;
		cos2 += 0.3;
		sin3 += 0.3;
		cos3 += 0.3;
	  //---------------------------------------------Rendering---------------------------------------------//
		//sleep(1); //necessary for bigger canvas sizes
		printf("\e[1;1H\e[2J"); //clears the screen
		printf("|"); //prints the first border
		for (int i = 0; i <= frameBufferSize; i++)
		{
			printf("%s", frameBuffer[i]);
			if (i % canvasWidth == canvasWidth - 1)
			{
				printf("|\n|");
			}
		}
	}
} 