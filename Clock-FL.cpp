/*
Author: Flavio Vilante
twitter: @fvilante
GitHub: fvilante
Licence: Open-source
Purpose: Help new learners studying C language.
*Designed to run on Turbo C compiler, but can also run on Code::Blocks
*/
#include <stdio.h>
#include <stdlib.h>
#include "graphics.h"
#include <time.h>
//#Portability to: Code::Blocks/gcc
	//#include <iostream>
//Portability to: Turbo C
	#include <iostream.h>
	#include <conio.h>
#include <string.h>
#include <math.h>
//Pre-processor constants
#define PI      3.1415
#define ON      1
#define OFF     0
//Global Variables
extern int digitsMap[11][7];
struct timeOBJ_t
{
    int hour;
    int minute;
    int second;
};
timeOBJ_t timeObj;
struct Vector2D
{
    float x;
    float y;
};
int digitsMap[11][7]= {
    // Segment LEDS's that are ON or
    // OFF for each digit.
	// 1=On / 0=off  -> segment status
    //   a b c d e f g
        {1,1,1,1,1,1,0},    //Digit 0
        {0,1,1,0,0,0,0},    //Digit 1
        {1,1,0,1,1,0,1},    //Digit 2
		{1,1,1,1,0,0,1},    //Digit 3
        {0,1,1,0,0,1,1},    //Digit 4
        {1,0,1,1,0,1,1},    //Digit 5
        {1,0,1,1,1,1,1},    //Digit 6
        {1,1,1,0,0,0,0},    //Digit 7
        {1,1,1,1,1,1,1},    //Digit 8
        {1,1,1,1,0,1,1},    //Digit 9
        {0,0,0,0,0,0,0},    //Digit CLEAR
    };

//FUNCTIONS ------

//Convert Degree to Radian
float degToRad(float degree)
{
	return degree *  PI / (180.0);
}

//Delay routine
void timer(int milisecondsToDelay)
{
        const float MILISECONDS_TO_SECOND_RATE = 1000;
        clock_t initialClockReference = clock();
        clock_t finalClockTarget = initialClockReference +
            (milisecondsToDelay / MILISECONDS_TO_SECOND_RATE)
            * CLOCKS_PER_SEC;
        while (clock() < finalClockTarget);
}

void scaleVertices(Vector2D *charVertex,
                   int totalVertices,
                   float scale = 10,
                   float xyRatio = 0.5)
{
    for (int k=0; k<=totalVertices-1;k++)
	{
        charVertex[k].x = charVertex[k].x * scale * xyRatio;
        charVertex[k].y = charVertex[k].y * scale * (-1);
            //this (-1) on line above is to adapt coordinates from our own
            //reference system to screen-coordinate system.
	}
}

void rotateVertices (Vector2D *charVertex, int totalVertices,
                    float rotationAngle)
{
    //notes:    RotationAngle is passed in degrees.
    //          The rotationAngle is measured clockwise
    double startAngle = 0;
    double finalAngle = 0;
    double newX = 0;
    double newY = 0;
    double x = 0;
    double y = 0;
    double radius = 0;

    for (int vertexCount=0; vertexCount<=totalVertices-1;vertexCount++)
    {
        x = (double) charVertex[vertexCount].x;
        y = (double) charVertex[vertexCount].y;

        newX = x * cos(degToRad(rotationAngle)) - y * sin(degToRad(rotationAngle));
        newY = y * cos(degToRad(rotationAngle)) + x * sin(degToRad(rotationAngle));
        charVertex[vertexCount].x = (float) newX;
        charVertex[vertexCount].y = (float) newY;
    }
}

void translateVertices(Vector2D *charVertex, int totalVertices,
                       float x=0, float y=0)
{
    for (int k=0; k<=totalVertices-1;k++)
    {
        charVertex[k].x += x;
        charVertex[k].y += y;
	}
}

void drawOneSegment(Vector2D *charVertex, int segmentNumber, int colorToUse)
{
	//Sequence in witch each segment is printed on screen.
    //First line bellow means: Segment 'a' is drawn starting from notable
    //vertex 0 to notable vertex 1 (of our 7-segment vertices display model)
    int sequence[7][2] = {
        {0,1},      //segment a
        {1,2},      //segment b
        {2,3},      //segment c
        {3,4},      //segment d
        {4,5},      //segment e
        {5,0},      //segment f
        {5,2},      //segment g
    };
	setlinestyle(0,0,1);
    setcolor(colorToUse);
    int initialVertex = sequence[segmentNumber][0];
	int finalVertex = sequence[segmentNumber][1];

    //Draw a line from startVertex to finalVertex
    line(   (int) charVertex[initialVertex].x,
            (int) charVertex[initialVertex].y,
			(int) charVertex[finalVertex].x,
            (int) charVertex[finalVertex].y);
	setlinestyle(0,0,0);
}

void drawSegments(Vector2D *charVertex, int digit, int colorON = 15,
                  int colorOFF = 0)
{
	int colorToUse;
	for (int segmentCount=0; segmentCount<=6; segmentCount++)
	{
		int segmentCondition = digitsMap [digit][segmentCount];

		if (segmentCondition == ON ) {colorToUse = colorON;}
		else {colorToUse = colorOFF;}

		drawOneSegment(charVertex, segmentCount, colorToUse);
	}
}

void drawOneDigit(int digit, int x, int y, float scale = 10,
                  float rotationAngle = 0, int colorON = 15,
                  int colorOFF = 0, float xyRatio = 0.5)
{
	Vector2D charVertex[6] = {
    //
    // OUR SEVEN-SEGMENT VERTICES' DISPLAY MODEL
    //
    // The model consists in adopt notable vertex for our calculation
    // and rendering of the character.
    //         a
    //   (1)********(2)
    //    *          *
    //  f *          * b
    //    *     g    *
	//   (6)**(R)***(3)
    //    *          *
    //  e *          * c
    //    *          *
    //   (5)********(4)
	//         d
    //
    // Notes:   (1), (2), (3), (4), (5), (6) are notable vertices
    //          a, b, c, d, e, f and g are segments of the display
    //          (R) is the reference point for rotation of the character
        {-1.0,+1.0},        //reference point 1
        {+1.0,+1.0},        //reference point 2
        {+1.0,+0.0},        //reference point 3
        {+1.0,-1.0},        //reference point 4
        {-1.0,-1.0},        //reference point 5
        {-1.0,+0.0},        //reference point 6
    };
    //SRT transformation
    scaleVertices (charVertex, 6, scale, xyRatio);
    rotateVertices (charVertex, 6, rotationAngle);
	translateVertices (charVertex, 6, x, y);
    //Draw segments of the digit on screen
    drawSegments (charVertex, digit, colorON, colorOFF);
}

void drawNumbers(int Number, int x, int y, float scale = 10,
				 float rotationAngle = 0, int colorON = 15,
				 int colorOFF = 0, float xyRatio = 0.5,
				 float spaceBetweenChar = 0.2)
{
	char stringNumber[3] = {'0','0',0};
	sprintf(stringNumber, "%i", Number);
	int lengthOfNumber = strlen(stringNumber);

	if (lengthOfNumber <= 1) {
		drawOneDigit(Number, x, y, scale, rotationAngle,
					 colorON, colorOFF, xyRatio);
	}
	else if (lengthOfNumber <= 2) {
		float lengthX = ((scale * xyRatio) * lengthOfNumber) + spaceBetweenChar ;
		float lengthY = scale;

		Vector2D charSet[2] = {
			{(float) -1-spaceBetweenChar,0},
			{(float) +1+spaceBetweenChar,0} };
		//SRT transformation
		scaleVertices(charSet, 2, scale, xyRatio);
		rotateVertices (charSet, 2, rotationAngle);
		translateVertices (charSet, 2, x, y);
		//draw digits
		int rightDigit = atoi(&stringNumber[1]);
		stringNumber[1] = 0;
		int leftDigit = atoi(&stringNumber[0]);
		drawOneDigit(leftDigit, charSet[0].x, charSet[0].y,
					 scale, rotationAngle, colorON, colorOFF,
					 xyRatio);
		drawOneDigit(rightDigit, charSet[1].x, charSet[1].y,
					 scale, rotationAngle, colorON, colorOFF,
					 xyRatio);
	}
	else
	{
		return;
	}
}

#define TICK_CLOCK 't'
#define WAIT_UNTIL_NEW_SECOND 'n'
int getTime(char typeInfo)
{
	time_t baseTime;
	struct tm *baseTimeObject;
	time(&baseTime);
	baseTimeObject = localtime(&baseTime);
	switch(typeInfo) {
		case TICK_CLOCK: {
			timeObj.hour = baseTimeObject->tm_hour;
			timeObj.minute = baseTimeObject->tm_min;
			timeObj.second = baseTimeObject->tm_sec;
			return 0;}
		case WAIT_UNTIL_NEW_SECOND: {
			int actualsecond = baseTimeObject->tm_sec;
			while(actualsecond == baseTimeObject->tm_sec) {
				time(&baseTime);
				baseTimeObject = localtime(&baseTime);}
			}
	return 0;
	}
}

void drawClockPointer(float x, float y, float number, float length, float angle, int linewidth = 2, int color = 15)
{
	if (number>=60) {number=0;}
	number = 360*(number/60);
	float x0 = x + (length * cos(degToRad(number+angle-90)));
	float y0 = y + (length * sin(degToRad(number+angle-90)));
	setlinestyle(0,0,linewidth);
	setcolor(color);
	line(x, y, x0, y0);
}

#define NUMBERCLOCKDASHS 12*5
void drawClock(int x, int y, float scale = 100, float rotationAngle = 0,
			   int lengthOfMajorDash = 10, int lengthOfMinorDash = 5,
			   int clearClock = 0)
{
	const double angleDash = degToRad(360)/(NUMBERCLOCKDASHS);
	int k, color = 0;

	Vector2D dashVertices[NUMBERCLOCKDASHS];

	//Create dash vectors.
	for (k=0; k<=NUMBERCLOCKDASHS-1;k++)
	{
		dashVertices[k].x = (float) sin(angleDash*k);
		dashVertices[k].y = (float) cos(angleDash*k);
	}
	//SRT transformation
	scaleVertices(dashVertices, NUMBERCLOCKDASHS, scale, 1);
	rotateVertices(dashVertices, NUMBERCLOCKDASHS, rotationAngle);
	translateVertices(dashVertices, NUMBERCLOCKDASHS, x, y);
	//Draw clock hour and min pointers
	color = 8; if (clearClock == 1) {color = 0;}
	float _minutes = timeObj.minute;
	drawClockPointer(x, y, _minutes, scale*0.8, rotationAngle, 3, color);
	float _hour = timeObj.hour;
	if (_hour >= 12) {_hour -= 12;}
	drawClockPointer(x, y, 60*(_hour/12), scale*0.5, rotationAngle, 4, color);
	//Draw clock dashes and numbers
	int number=0;
	for (k=0; k<=NUMBERCLOCKDASHS-1;k++)
	{
		x = (int) dashVertices[k].x;
		y = (int) dashVertices[k].y;
		if (k % 5 == 0) {
			//Special design for 5-minutes's dashes.
			//Draw clock' numbers
			color = (k/5)+1; if (clearClock == 1) {color = 0;}
			if (color == 8) {++color;}
			if (number == 0)
			{ drawNumbers(12, x, y, scale*0.07, rotationAngle,
											color,0,0.5,0.5);}
			else { drawNumbers(number, x, y, scale*0.07, rotationAngle,
												color,0,0.5,0.5);}
			++number;
		}
		else {
			//Design of normal dashes.
			if (clearClock == 1) { setcolor(0) ; } else { setcolor(3);}
			fillellipse(x, y, lengthOfMinorDash, lengthOfMinorDash);
		}
	}
}

void InitializeScreen()
{
	int gd=DETECT,gm; 
	initgraph(&gd,&gm,NULL);
	
	setcolor(4);
	setlinestyle (0, 0, 0);
	getTime(TICK_CLOCK); //start program clock object
}

int main(void)
{
	InitializeScreen();
	float x = getmaxx()/2;
	float y = getmaxy()/2;
	float radiusPivotCircle = 30;
	float x0, y0 = 0;
	float radiusClock = 120;
	int _hour, _minute, _second = 0;

	//for (int k=1;k<=10;k++) {  //#Portability to Code::Blocks
	while (!kbhit()) {           //#Portability to Turbo C
		float seconds = timeObj.second; //getTime(GET_ACTUAL_SECOND);
		float angleSecondsPointer = (360*(seconds/60))-90;
		getTime(TICK_CLOCK);
		//reset drawing
		cleardevice();
		//Draw rotating clock and seconds pointer
		x0 = x + ((radiusPivotCircle + (radiusClock+(10*1.5))) *
				cos(degToRad(angleSecondsPointer)));
		y0 = y + ((radiusPivotCircle + (radiusClock+(10*1.5))) *
				sin(degToRad(angleSecondsPointer))) ;
		drawClockPointer(x, y, seconds, (radiusPivotCircle+radiusClock)*0.9, 0, 4, 14);
		fillellipse(x,y,radiusPivotCircle,radiusPivotCircle);
		drawClock(x0, y0, radiusClock, angleSecondsPointer, 10, 1,0);
		//Draw clock in digital format
		_hour = timeObj.hour;
		drawNumbers(_hour,x-50,100,20,-10);
		_minute = timeObj.minute;
		drawNumbers(_minute,x,100-20,17,0);
		_second = timeObj.second;
		drawNumbers(_second,x+40,100,15,+10);
		//wait
		getTime(WAIT_UNTIL_NEW_SECOND);

	}
	closegraph();
	return(0);
}
