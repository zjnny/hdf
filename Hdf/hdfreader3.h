#include <stdio.h>


//线段定义
bool VALUEBETWEEN(int A, int B, double VALUE);
bool LINE0CONFIRM(int x, int y);
bool LINE1CONFIRM(int x, int y);
bool LINE2CONFIRM(int x, int y);
int XY2P(int x, int y);
int P2X(int p) ;
int P2Y(int p);
void GenerateOneLine(int isegment, double EHValue);
struct SEGMENT {

	bool flag;
	int x1,  y1,  v1;//是grid中的坐标,如果要转化为真实的坐标,需要x1*NGIRDSIZE,y1*NGIRDSIZE
	int x2,  y2,  v2;
};


struct POT {
	int x,  y;
	POT * next;
};
struct line
{
	double value;
	POT * pheader;
	line * next;
};
//arraylist<POT> lines ;//存放每一条等值线的

void release();
void DrawValueString(HDC dc,int value,int x,int y,COLORREF color = RGB(0,0,0)/*BYTE red = 0,BYTE green = 0,BYTE blue = 0*/ );
void DrawPointValue(double value, POT p, HDC dc);
void DrawIsoline(double EHValue);
void DrawIsolineEx(double EHValue);
void OnIsolineDrawline(int r, int g, int b, double startValue, double endValue);
bool IsClosure(POT *p1, POT *p2);
int getvalue(int,int);
int ** ReadData(short * pData) ;