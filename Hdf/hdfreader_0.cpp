// hdfreader3.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "hdf5func.h"
#include "hdfreader3.h"
#include "atlimage.h"
#define NLINEDISTANCE 15
#define NWIDTH 999
#define NHEIGHT 999
#define NGIRDSIZE 1
#define ZOOM 1
#define XPOS 0
#define YPOS 0

#define INVALIDVALUE 0
#define VALIDVALUE 1000000

int gheight = ((NHEIGHT) / NGIRDSIZE); //#define gheight ((NHEIGHT)/NGIRDSIZE)
int gwidth = ((NWIDTH) / NGIRDSIZE);//#define gwidth ((NWIDTH)/NGIRDSIZE)

short * pData = new short [1000000];
SEGMENT * segments = new SEGMENT[3000000];
line * plineheader = NULL;
line * plineend;
int _tmain(int argc, _TCHAR* argv[])
{
	HDF5Func m_hdf;
	hsize_t a2;
	int a1,a3,a4;
	//hid_t hInf = H5Fopen("E:\\hdf\\FY3A_VIRRX_50D0_L2_OLR_MLT_GLL_20110628_AOAD_1000M_MS.HDF", H5F_ACC_RDONLY, H5P_DEFAULT);
	//m_hdf.GetHdfSdsParam("D:\\virr\\FY3A_VIRRX_H090_L2_OLR_MLT_GLL_20110703_AOAD_1000M_MS.HDF","OLR",&a1,&a2,a3,a4);
	//short **pData= new short*[1000];
	//for(int i = 0;i<=999;i++)
	//{
	//	pData[i] = new short [1000];
	//}

	m_hdf.ReadHdf5FileData("D:\\temp\\product\\FY3A_VIRRX_H090_L2_OLR_MLT_GLL_20110703_AOAD_1000M_MS.HDF","OLR",pData);

	///////////////////////
	OnIsolineDrawline(100,100,100,85,160);
	CImage m_img;
	m_img.Create(1000,1000,24);
	line * pt = plineheader;
	HDC dc = m_img.GetDC();
	::Rectangle(dc,0,0,999,999);
	double t,x,y;
	//while(pt)
	//{
	//	POT * ps= pt->pheader;
	//	while(ps&&ps->next)
	//	{
	//		::MoveToEx(dc,ps->x,ps->y,NULL);
	//		::LineTo(dc,ps->next->x,ps->next->y);
	//		ps = ps->next;
	//	}
	//	pt= pt->next;
	//}

	while(pt)
	{
		POT *p1 = NULL, *p2 = NULL, *p3 = NULL;
		int num = NGIRDSIZE * 2;
		POT * ps = pt->pheader;
		POT * pender;
		while(ps)
		{
			pender = ps;
			ps = ps->next;
		}
		if (pt->pheader!=NULL&&IsClosure(pt->pheader, pender) == true) 
		{
			//是闭合的曲线
			ps = pt->pheader;
			while(ps)
			{
				p1 = ps;
				if(ps == pender)
				{
					p2 = pt->pheader;
					p3 = p2->next;
				}
				else if(ps->next == pender)
				{
					p2 = ps->next;
					p3 = pt->pheader;
				}
				else
				{
					p2 = ps->next;
					p3 = ps->next->next;
				}

				bool isMoveTo = false; //是否已经有过MoveTo定位

				int prex = 0, prey = 0;
				for (int k = 0; k < num; k++) {
					t = (double) (k) / (double) (num);
					x = ((p1->x - 2 * p2->x + p3->x) * t * t + (-2 * p1->x + 2 * p2->x) * t + (p1->x + p2->x)) / 2.0f;
					y = ((p1->y - 2 * p2->y + p3->y) * t * t + (-2 * p1->y + 2 * p2->y) * t + (p1->y + p2->y)) / 2.0f;

					if (isMoveTo == false) {
						prex = (int) x;
						prey = (int) y;
						isMoveTo = true;
					} else {
//						g2d.drawLine((int) (prex * ZOOM + XPOS), (int) (prey * ZOOM + YPOS), (int) (x * ZOOM + XPOS), (int) (y * ZOOM + YPOS));
						::MoveToEx(dc,(int)(prex * ZOOM + XPOS),(int)(prey*ZOOM+YPOS),NULL);
						::LineTo(dc,(int) (x * ZOOM + XPOS), (int) (y * ZOOM + YPOS));
						prex = (int) x;
						prey = (int) y;
					}
				}
				ps = ps->next;
			}
		} 
		else if(pt->pheader!=NULL&&IsClosure(pt->pheader, pender) == false) 
		{
			//非闭合
			ps = pt->pheader;
			while(ps->next->next!=NULL)
			{

		//		isFirst = true;
				p1 = ps;
				p2 = ps->next;
				p3 = ps->next->next;

				boolean isMoveTo = false; //是否已经有过MoveTo定位

				int prex = 0, prey = 0;
				for (int k = 0; k < num; k++) {
					t = (double) (k) / (double) (num);
					x = ((p1->x - 2 * p2->x + p3->x) * t * t + (-2 * p1->x + 2 * p2->x) * t + (p1->x + p2->x)) / 2.0f;
					y = ((p1->y - 2 * p2->y + p3->y) * t * t + (-2 * p1->y + 2 * p2->y) * t + (p1->y + p2->y)) / 2.0f;

					if (isMoveTo == false) {
						prex = (int) x;
						prey = (int) y;
						isMoveTo = true;
					} else {
						//						g2d.drawLine((int) (prex * ZOOM + XPOS), (int) (prey * ZOOM + YPOS), (int) (x * ZOOM + XPOS), (int) (y * ZOOM + YPOS));
						::MoveToEx(dc,(int)(prex * ZOOM + XPOS),(int)(prey*ZOOM+YPOS),NULL);
						::LineTo(dc,(int) (x * ZOOM + XPOS), (int) (y * ZOOM + YPOS));
						prex = (int) x;
						prey = (int) y;
					}
				}
				
				ps = ps->next;
			}
		}
		pt = pt->next;
	}
	m_img.Save("D:\\temp\\product\\1.bmp");
	m_img.ReleaseDC();
	release();
	return 0;

}
void release()
{
	delete pData;
	delete segments;
	line * pt = plineheader;
	while(plineheader)
	{
		pt = plineheader->next;
		POT * ps= plineheader->pheader;
		while(ps)
		{
			plineheader->pheader = plineheader->pheader->next;
//			cout<<ps->x<<","<<ps->y<<"	";
			delete ps;
			ps = plineheader->pheader;
		}
//		cout<<endl;
		delete plineheader;
		plineheader = pt;
	}
}


void OnIsolineDrawline(int r, int g, int b, double startValue, double endValue) {
	//初始化颜色表
	COLORREF m_col = RGB(r,g,b);

	//每隔
	int colorRef = 0;

	for (double i = startValue; i < endValue; i += NLINEDISTANCE) {

		//DrawIsoline(i, bShowLineValue, gLineColors[0], canvas);
		DrawIsoline(i);
	}
}

void DrawIsoline(double EHValue) {
	//	lines.clear();
	//计算线段
	int X = gwidth;
	int Y = gheight;


	for (int j = 0; j < 1000/NGIRDSIZE; j++) {
		for (int i = 0; i < 1000/NGIRDSIZE; i++) {
			int p = (j*NGIRDSIZE) * (999 + 1) + (i*NGIRDSIZE), t;//XY2P1(i, j), t;
			int p1 = j*(1000/NGIRDSIZE)+i;



			//line 0
			if (i + 1 < 1000/NGIRDSIZE) {
				t = p * 3 + 0;
				t= p1*3;
				segments[t].flag = false;
				segments[t].x1 = i;
				segments[t].y1 = j;
				segments[t].v1 = getvalue(i*NGIRDSIZE,j*NGIRDSIZE);////grid[i][j];
				//segments[t].v1 = pData[p];

				segments[t].x2 = i + 1;
				segments[t].y2 = j;
				segments[t].v2 = getvalue((i+1)*NGIRDSIZE,j*NGIRDSIZE);////grid[i + 1][j];
				//segments[t].v2 = pData[p+1];
			}

			//line 1
			if (i + 1 < 1000/NGIRDSIZE && j + 1 < 1000/NGIRDSIZE) {
				t = p * 3 + 1;
				t = p1*3+1;
				segments[t].flag = false;
				segments[t].x1 = i;
				segments[t].y1 = j;
				segments[t].v1 = getvalue(i*NGIRDSIZE,j*NGIRDSIZE);//grid[i][j];
				//segments[t].v1 = pData[p];

				segments[t].x2 = i + 1;
				segments[t].y2 = j + 1;
				segments[t].v2 = getvalue((i+1)*NGIRDSIZE,(j+1)*NGIRDSIZE);//grid[i + 1][j + 1];
				//segments[t].v2 = pData[p+1000+1];
			}

			//line 2
			if (j + 1 < 1000/NGIRDSIZE) {
				t = p * 3 + 2;
				t = p1*3+2;
				segments[t].flag = false;
				segments[t].x1 = i;
				segments[t].y1 = j;
				segments[t].v1 = getvalue(i*NGIRDSIZE,j*NGIRDSIZE);//grid[i][j];
				//segments[t].v1 = pData[p];

				segments[t].x2 = i;
				segments[t].y2 = j + 1;
				segments[t].v2 = getvalue(i*NGIRDSIZE,(j+1)*NGIRDSIZE);//grid[i][j + 1];
				//segments[t].v2 = pData[p+1000];
			}
		}
	}

	//生成等值线
	for (int j = 0; j <1000/NGIRDSIZE; j++) {
		for (int i = 0; i <1000/NGIRDSIZE; i++) {
			for (int k = 0; k < 3; k++) {
				int t = (j * (1000/NGIRDSIZE-1) + i) * 3 + k;
	//			cout<<segments[t].v1<<","<<segments[t].v2<<"	";
				if (segments[t].flag == false) {
					if (VALUEBETWEEN(segments[t].v1, segments[t].v2, EHValue) == true) {
						
						GenerateOneLine(t, EHValue);
					}
				}
			}
		}
	//	cout<<endl;
	}
	//System.out.println("All Line count = "+lines.size());

	//画等值线
	//double t, x, y;
	//Graphics2D g2d = getDraw2D(canvas);
	////blw
	//// g2d.setBackground(Color.WHITE);
	////g2d.clearRect(0, 0, NWIDTH, NHEIGHT);
	////        g2d.setColor(Color.WHITE);


	////normal
	//g2d.setColor(new Color(color));


	//for (int i = 0; i < lines.size(); i++) {
	//	ArrayList<POT> line = lines.get(i);
	//	if (bIngoreShortLine == true && line.size() <= 6) {
	//		continue;
	//	}
	//	//标数值
	//	if (bShowLineValue == true) {
	//		if (IsClosure(line.get(0), line.get(line.size() - 1)) == true) {
	//			//是闭合的曲线
	//			g2d.drawString("" + (int) EHValue, (int) (line.get(0).x * ZOOM + XPOS), (int) (line.get(0).y * ZOOM + YPOS));
	//		} else {
	//			//非闭合,则数字标在两端
	//			DrawPointValue(EHValue, line.get(0), g2d);
	//			DrawPointValue(EHValue, line.get(line.size() - 1), g2d);
	//		}
	//	}


	//	//画线条语句
	//	boolean isFirst = false;
	//	for (int j = 0; j < line.size(); j++) {
	//		if (B2LINE == false) {
	//			//不需要平滑处理
	//			if (j < line.size() - 1) {
	//				g2d.drawLine((int) (line.get(j).x * ZOOM + XPOS), (int) (line.get(j).y * ZOOM + YPOS), (int) (line.get(j + 1).x * ZOOM + XPOS), (int) (line.get(j + 1).y * ZOOM + YPOS));
	//			}
	//		} else {
	//			//二次B样条
	//			POT p1 = null, p2 = null, p3 = null;
	//			int num = NGIRDSIZE * 2;
	//			if (IsClosure(line.get(0), line.get(line.size() - 1)) == true) {
	//				//是闭合的曲线
	//				p1 = line.get(j % line.size());
	//				p2 = line.get((j + 1) % line.size());
	//				p3 = line.get((j + 2) % line.size());

	//				boolean isMoveTo = false; //是否已经有过MoveTo定位

	//				int prex = 0, prey = 0;
	//				for (int k = 0; k < num; k++) {
	//					t = (double) (k) / (double) (num);
	//					x = ((p1.x - 2 * p2.x + p3.x) * t * t + (-2 * p1.x + 2 * p2.x) * t + (p1.x + p2.x)) / 2.0f;
	//					y = ((p1.y - 2 * p2.y + p3.y) * t * t + (-2 * p1.y + 2 * p2.y) * t + (p1.y + p2.y)) / 2.0f;

	//					if (isMoveTo == false) {
	//						prex = (int) x;
	//						prey = (int) y;
	//						isMoveTo = true;
	//					} else {
	//						g2d.drawLine((int) (prex * ZOOM + XPOS), (int) (prey * ZOOM + YPOS), (int) (x * ZOOM + XPOS), (int) (y * ZOOM + YPOS));
	//						prex = (int) x;
	//						prey = (int) y;
	//					}
	//				}
	//			} else {
	//				//非闭合
	//				if (j + 2 < line.size()) {
	//					isFirst = true;
	//					p1 = line.get(j);
	//					p2 = line.get(j + 1);
	//					p3 = line.get(j + 2);

	//					boolean isMoveTo = false; //是否已经有过MoveTo定位

	//					int prex = 0, prey = 0;
	//					for (int k = 0; k < num; k++) {
	//						t = (double) (k) / (double) (num);
	//						x = ((p1.x - 2 * p2.x + p3.x) * t * t + (-2 * p1.x + 2 * p2.x) * t + (p1.x + p2.x)) / 2.0f;
	//						y = ((p1.y - 2 * p2.y + p3.y) * t * t + (-2 * p1.y + 2 * p2.y) * t + (p1.y + p2.y)) / 2.0f;

	//						if (isMoveTo == false) {
	//							prex = (int) x;
	//							prey = (int) y;
	//							isMoveTo = true;
	//						} else {
	//							g2d.drawLine((int) (prex * ZOOM + XPOS), (int) (prey * ZOOM + YPOS), (int) (x * ZOOM + XPOS), (int) (y * ZOOM + YPOS));
	//							prex = (int) x;
	//							prey = (int) y;
	//						}
	//					}
	//				} else {                            //未处理
	//					//应该加入自动连接到底部的代码

	//				}

	//			}
	//		}
	//	}
	//	if (B2LINE == false) {
	//		if (IsClosure(line.get(0), line.get(line.size() - 1))) {
	//			//					pDC->MoveTo(line[0].x*ZOOM+XPOS,line[0].y*ZOOM+YPOS);
	//			//					pDC->LineTo(line[line.size()-1].x*ZOOM+XPOS,line[line.size()-1].y*ZOOM+YPOS);

	//			g2d.drawLine((int) (line.get(0).x * ZOOM + XPOS), (int) (line.get(0).y * ZOOM + YPOS),
	//				(int) (line.get(line.size() - 1).x * ZOOM + XPOS), (int) (line.get(line.size() - 1).y * ZOOM + YPOS));
	//		}
	//	}
	//}
}

//生成等值线,以isegment为起点,EHValue为等值线的值,跟踪生成等值线
//在isegment开始画线
//以尽可能的顺时针
//两个方向都进行尝试,再合并

void GenerateOneLine(int isegment, double EHValue) {
	int icurrent = isegment;
	//ArrayList<POT> line = new ArrayList<POT>();
	//POT ps = new POT();
	POT * pheader = NULL;
	POT * pend;
	POT * point;


	for (int direct = 0; direct <= 1; direct++) {
		//两个方向进行跟踪
		icurrent = isegment;
		while (true) {
			int p = icurrent / 3, t;
			int x = P2X(p);
			int y = P2Y(p);
			int inext = icurrent;
			segments[icurrent].flag = true;

			if (icurrent % 3 == 0) {
				//line 0,邻接三角边为 (p*3 + 1), XY2P(x+1,y)*3 + 2, XY2P(x,y-1)*3 + 1, XY2P(x,y-1)*3 + 2, 
				if (inext == icurrent && LINE1CONFIRM(x, y)) {
					// (p*3 + 1)
					t = p * 3 + 1;
					if (segments[t].flag == false && VALUEBETWEEN(segments[t].v1, segments[t].v2, EHValue) == true) {
						inext = t;
					}
				}

				if (inext == icurrent && LINE2CONFIRM(x + 1, y)) {
					//XY2P(x+1,y)*3 + 2
					t = XY2P(x + 1, y) * 3 + 2;
					if (segments[t].flag == false && VALUEBETWEEN(segments[t].v1, segments[t].v2, EHValue) == true) {
						inext = t;
					}
				}

				if (inext == icurrent && LINE1CONFIRM(x, y - 1)) {
					//XY2P(x,y-1)*3 + 1
					t = XY2P(x, y - 1) * 3 + 1;
					if (segments[t].flag == false && VALUEBETWEEN(segments[t].v1, segments[t].v2, EHValue) == true) {
						inext = t;
					}
				}

				if (inext == icurrent && LINE2CONFIRM(x, y - 1)) {
					//XY2P(x,y-1)*3 + 2
					t = XY2P(x, y - 1) * 3 + 2;
					if (segments[t].flag == false && VALUEBETWEEN(segments[t].v1, segments[t].v2, EHValue) == true) {
						inext = t;
					}
				}

			} else if (icurrent % 3 == 1) {
				//line 1,邻接三角边为 (p*3 + 2), XY2P(x,y+1)*3+0,XY2P(x+1,y)*3+2,(P*3+0)
				if (inext == icurrent && LINE2CONFIRM(x, y)) {
					// (p*3 + 2)
					t = (p * 3 + 2);
					if (segments[t].flag == false && VALUEBETWEEN(segments[t].v1, segments[t].v2, EHValue) == true) {
						inext = t;
					}
				}

				if (inext == icurrent && LINE0CONFIRM(x, y + 1)) {
					// XY2P(x,y+1)*3+0
					t = XY2P(x, y + 1) * 3 + 0;
					if (segments[t].flag == false && VALUEBETWEEN(segments[t].v1, segments[t].v2, EHValue) == true) {
						inext = t;
					}
				}

				if (inext == icurrent && LINE2CONFIRM(x + 1, y)) {
					// XY2P(x+1,y)*3+2
					t = XY2P(x + 1, y) * 3 + 2;
					if (segments[t].flag == false && VALUEBETWEEN(segments[t].v1, segments[t].v2, EHValue) == true) {
						inext = t;
					}
				}

				if (inext == icurrent && LINE0CONFIRM(x, y)) {
					//(P*3+0)
					t = (p * 3 + 0);
					if (segments[t].flag == false && VALUEBETWEEN(segments[t].v1, segments[t].v2, EHValue) == true) {
						inext = t;
					}
				}

			} else if (icurrent % 3 == 2) {
				//line 2,邻接三角边为 XY2P(x-1,y)*3+1,XY2P(x,y+1)*3+0,(p*3+1),XY2P(x-1,y)*3+0
				if (inext == icurrent && LINE1CONFIRM(x - 1, y)) {
					//XY2P(x-1,y)*3+1
					t = XY2P(x - 1, y) * 3 + 1;
					if (segments[t].flag == false && VALUEBETWEEN(segments[t].v1, segments[t].v2, EHValue) == true) {
						inext = t;
					}
				}

				if (inext == icurrent && LINE0CONFIRM(x, y + 1)) {
					//XY2P(x,y+1)*3+0
					t = XY2P(x, y + 1) * 3 + 0;
					if (segments[t].flag == false && VALUEBETWEEN(segments[t].v1, segments[t].v2, EHValue) == true) {
						inext = t;
					}
				}

				if (inext == icurrent && LINE1CONFIRM(x, y)) {
					//(p*3+1)
					t = (p * 3 + 1);
					if (segments[t].flag == false && VALUEBETWEEN(segments[t].v1, segments[t].v2, EHValue) == true) {
						inext = t;
					}
				}

				if (inext == icurrent && LINE0CONFIRM(x - 1, y)) {
					//(XY2P(x-1,y)*3+0
					t = XY2P(x - 1, y) * 3 + 0;
					if (segments[t].flag == false && VALUEBETWEEN(segments[t].v1, segments[t].v2, EHValue) == true) {
						inext = t;
					}
				}
			}


			if (inext == icurrent || inext <= 0) {
				//找不到下一个三角边,或到边界
				break;
			} else {
				//线性插值处理
				double a, px, py;
				if (icurrent == isegment && direct == 0) {
					a = (EHValue - segments[icurrent].v1) / (double) (segments[icurrent].v2 - segments[icurrent].v1);
					px = segments[icurrent].x1 * NGIRDSIZE + a * (segments[icurrent].x2 * NGIRDSIZE - segments[icurrent].x1 * NGIRDSIZE);
					py = segments[icurrent].y1 * NGIRDSIZE + a * (segments[icurrent].y2 * NGIRDSIZE - segments[icurrent].y1 * NGIRDSIZE);

					POT *ps = new POT();
					ps->x = (int) px;
					ps->y = (int) py;
					//line.push_back(ps);
					if(pheader == NULL)
					{
						pheader = ps;
						pend = ps;
					}
					else
					{
						pend->next = ps;
						pend = ps;
					}
					pend->next = NULL;
				}

				a = (EHValue - segments[inext].v1) / (double) (segments[inext].v2 - segments[inext].v1);
				px = segments[inext].x1 * NGIRDSIZE + a * (segments[inext].x2 * NGIRDSIZE - segments[inext].x1 * NGIRDSIZE);
				py = segments[inext].y1 * NGIRDSIZE + a * (segments[inext].y2 * NGIRDSIZE - segments[inext].y1 * NGIRDSIZE);

				POT* ps = new POT();
				ps->x = (int) px;
				ps->y = (int) py;

				if (direct == 0) {
					//line.push_back(ps);
					//line.add(ps);
					if(pheader == NULL)
					{
						pheader = ps;
						pend = ps;
					}
					else
					{
						pend->next = ps;
						pend = ps;
					}
					pend->next = NULL;
				} else {
					//line.push_front(ps);
					//line.add(0, ps);
					if(pheader == NULL)
					{
						pheader = ps;
						pend = ps;
					}
					else
					{
						ps->next = pheader;
						pheader = ps;
					}
					pend->next = NULL;
				}
				icurrent = inext;
			}
		}
	}

	//		std::vector<POT> tempLine;
	//		for(int i=0;i<line.size();i++){
	//			tempLine.push_back(line[i]);
	//		}
	//		lines.push_back(tempLine);
	//ArrayList<POT> tempLine = new ArrayList<POT>();
	//for (int i = 0; i < line.size(); i++) {
	//	tempLine.add(line.get(i));
	//}
	//lines.add(tempLine);

	line * pl = new line();
	pl->pheader = pheader;
	if(plineheader == NULL)
	{
		plineheader = pl;
		plineend = pl;
	}
	else
	{
		plineend->next = pl;
		plineend = pl;
	}
	plineend->next = NULL;

	//System.out.println("Line:"+tempLine.size());
}
bool VALUEBETWEEN(int A, int B, double VALUE) {
	return ((((A) <= VALUE && VALUE < (B)) || ((B) <= VALUE && VALUE < (A))) ? true : false);
}
//    int gheight = ((NHEIGHT) / NGIRDSIZE); //#define gheight ((NHEIGHT)/NGIRDSIZE)
//    int gwidth = ((NWIDTH) / NGIRDSIZE);//#define gwidth ((NWIDTH)/NGIRDSIZE)

bool LINE0CONFIRM(int x, int y) {
	return (x >= 0 && y >= 0 && x + 1 <= gwidth && y <= gheight);
}

bool LINE1CONFIRM(int x, int y) {
	return (x >= 0 && y >= 0 && x + 1 <= gwidth && y + 1 <= gheight);
}

bool LINE2CONFIRM(int x, int y) {
	return (x >= 0 && y >= 0 && x <= gwidth && y + 1 <= gheight);
}

int XY2P(int x, int y) {//(x,y)坐标转化为P点坐标

	return ((y) * (gwidth + 1) + (x));
}

int P2X(int p) {//p坐标转化为(x,y)坐标中的x

	return ((p) % (gwidth + 1));
}

int P2Y(int p) {
	return ((p) / (gwidth + 1));//p坐标转化为(x,y)坐标中的y

}

bool IsClosure(POT *p1, POT *p2) {
	if ((p1->x - p2->x) * (p1->x - p2->x) + (p1->y - p2->y) * (p1->y - p2->y) <= 2 * NGIRDSIZE * NGIRDSIZE) {
		return true;
	} else {
		return false;
	}
}
int getvalue(int x,int y)
{
	int sum = 0;
	int t = XY2P(x,y);
	for(int i = 0;i<NGIRDSIZE;i++)
	{
		for(int j = 0;j<NGIRDSIZE;j++)
		{
			sum+= pData[XY2P(x+i,y+j)];
		}
	}
	int tp = NGIRDSIZE*NGIRDSIZE;
	return (int)(sum/tp);

}



short ** ReadData(short * pData) {        //计算网络点
	short ** grid = new short*[gheight];
	for (int j = 0; j <= gheight; j++) {
		//            if(j==gheight && j*NGIRDSIZE == NHEIGHT){//测试高度为偶数时
		//                gheight -=1;
		//            }
		grid[j] = new short[gwidth];
		for (int i = 0; i <= gwidth; i++) {
			//估计grid中每一个点的值grid[i][j]
			int h = min((j + 1) * NGIRDSIZE, NHEIGHT);
			int w = min((i + 1) * NGIRDSIZE, NWIDTH);
			int cnt = 0;
			int invalidValueNum = 0;
			//                if(i == gwidth && (i*NGIRDSIZE)==NWIDTH){//测试宽度为偶数时
			//                    gwidth -=1;
			//                }
			for (int jj = max(0, (j) * NGIRDSIZE); jj < h; jj++) {
				if (invalidValueNum > 0) {
					break;
				}
				for (int ii = max(0, (i) * NGIRDSIZE); ii < w; ii++) {
					//						grid[i][j] += ((MapData_OLR)data[jj][ii]).data;
					int currentValue = pData[jj * NWIDTH + ii];
					if (currentValue == INVALIDVALUE) {
						invalidValueNum++;
						break;
					}
					grid[i][j] += currentValue;
					cnt++;
				}
			}

			if (invalidValueNum != 0) {
				grid[i][j] = VALIDVALUE;
			} else if (cnt != 0) {
				grid[i][j] /= cnt;
			} else {//针对图像长度为偶数的情况，按找gridsize分成整数个格子后画的框离边界远

				if (i != 0) {//填充右边框数据

					grid[i][j] = grid[i - 1][j] - 1;
				} else {
					grid[i][j] = grid[i][j - 1];
				}
				if (j == gheight) {//填充底部边框数据

					grid[i][j] = grid[i][j - 1];
				}

			}

		}
	}
	return grid;
}