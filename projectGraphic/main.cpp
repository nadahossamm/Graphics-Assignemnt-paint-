#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include <string>

#define CLEAR_Window 1
#define SAVE 2
#define LOAD 3
#define Line_DDA 4
#define Line_PARAMETRIC 5
#define Line_MIDPOINT 6
#define CIRCLE_DIRECT 7
#define CIRCLE_ITERATIVE_POLAR 8
#define CIRCLE_POLAR 9
#define CIRCLE_MIDPOINT 10
#define CIRCLE_MODIFIED_MIDPOINT 11
#define CLIPPING_LINE 12
#define CLIPPING_POINT 13
#define ELLIPSE_DIRECT 14
#define ELLIPSE_POLAR 15
#define FILLING 16
#define NEW_FILE 17
#define GREEN 18
#define BLACK 19
#define BLUE 20
#define ORANGE 21
#define RED 22
#define YELLOW 23
#define PURPLE 24
#define CURSOR_HAND 25
#define CURSOR_ARROW 26
#define CURSOR_CROSS 27
#define CURSOR_HELP 28
#define CURSOR_UPARROW 29
#define CURSOR_IBEAM 30
#define CURSOR_WAIT 31

using namespace std;

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */

TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

HMENU hMenu;

COLORREF color;
string saveFile = "";
int choice = 0;
int x_1,x_2,y_1,y_2,x_3,y_3,r1,r2,xFilling = -1,yFilling = -1,rediusFilling = -1,x_4,y_4;
double theta;
int Num_of_Points=0;
LPCSTR curs = IDC_CROSS;

void printColorOptions()
{
    int c;
    cout<<"color options:"<<endl;
    cout<<"1-red"<<endl;
    cout<<"2-yellow"<<endl;
    cout<<"3-black"<<endl;
    cout<<"4-blue"<<endl;
    cout<<"5-green"<<endl;
    cout<<"6-orange"<<endl;
    cout<<"7-purple"<<endl;
    cin>>c;
    switch(c)
    {
    case 1:
        color = RGB(255,0,0);
        break;
    case 2:
        color = RGB(255,255,0);
        break;
    case 3:
        color = RGB(0,0,0);
        break;
    case 4:
        color = RGB(0,0,255);
        break;
    case 5:
        color = RGB(0,255,0);
        break;
    case 6:
        color = RGB(255,102,0);
        break;
    case 7:
        color = RGB(255,0,255);
        break;
    default:
        break;

    }
}

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, curs);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = CreateSolidBrush(RGB(255,255,255));  /// background color white

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
               0,                   /* Extended possibilites for variation */
               szClassName,         /* Classname */
               _T("Code::Blocks Template Windows App"),       /* Title Text */
               WS_OVERLAPPEDWINDOW, /* default window */
               CW_USEDEFAULT,       /* Windows decides the position */
               CW_USEDEFAULT,       /* where the window ends up on the screen */
               544,                 /* The programs width */
               375,                 /* and height in pixels */
               HWND_DESKTOP,        /* The window is a child-window to desktop */
               NULL,                /* No menu */
               hThisInstance,       /* Program Instance handler */
               NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    printColorOptions();

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}

int Round(double x)
{
    return (int)(x+0.5);
}

void Swap(int &a, int &b)
{
    int t = a;
    a = b;
    b = t;
}


void ddaLine(HDC hdc,int X0, int Y0, int X1, int Y1)
{
    int dx = X1 - X0;
    int dy = Y1 - Y0;
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
    float Xinc = dx / (float) steps;
    float Yinc = dy / (float) steps;
    float X = X0;
    float Y = Y0;
    for (int i = 0; i <= steps; i++)
    {
        SetPixel (hdc,round(X),round(Y),color);
        X += Xinc;
        Y += Yinc;
    }
}

void parametricline(HDC hdc,int x1,int y1,int x2,int y2)
{
    double dx=x2-x1;
    double dy=y2-y1;
    for(double t=0; t<1; t+=0.001)
    {
        int x=x1+(dx*t);
        int y=y1+(dy*t);
        SetPixel(hdc,x,y,color);
    }
}

void midpointLine(HDC hdc,int x1,int y1,int x2,int y2)
{
    int dx = x2-x1;
    int dy = y2-y1;
    double slope = (((double)dy)/dx);

    if(slope < 1 && slope > 0)
    {
        if(x1>x2)
        {
            Swap(x1,x2);
            Swap(y1,y2);
            dy = y2-y1;
            dx = x2-x1;
        }
        int x = x1, y = y1;
        int d = dx - (2 * dy);
        int change1 = 2*(dx-dy);
        int change2 = -2*dy;
        SetPixel(hdc,x,y,color);
        while(x<x2)
        {
            if(d<=0)
            {
                d+=change1;
                y++;
            }
            else
                d+=change2;
            x++;
            SetPixel(hdc,x,y,color);
        }
    }
    else if(slope > 1)
    {
        if(y1>y2)
        {
            Swap(x1,x2);
            Swap(y1,y2);
            dy = y2-y1;
            dx = x2-x1;
        }

        int x = x1, y = y1;
        int d = (2 * dx) - dy;
        int change1 = 2*(dx-dy);
        int change2 = 2*dx;

        SetPixel(hdc,x,y,color);

        while(y<y2)
        {
            if(d>=0)
            {
                d+=change1;
                x++;
            }
            else
                d+=change2;

            y++;
            SetPixel(hdc,x,y,color);
        }
    }
    else if(slope > -1 && slope < 0)
    {
        if(x1<x2)
        {
            Swap(x1,x2);
            Swap(y1,y2);
            dy = y2-y1;
            dx = x2-x1;
        }
        int x = x1, y = y1;
        int d = dx + (2 *dy);
        int change1 = 2*(dx+dy);
        int change2 = 2*dy;
        SetPixel(hdc,x,y,color);
        while(x>x2)
        {
            if(d>=0)
            {
                d+=change1;
                y++;
            }
            else
                d+=change2;
            x--;
            SetPixel(hdc,x,y,color);
        }
    }
    else
    {
        if(y1>y2)
        {
            Swap(x1,x2);
            Swap(y1,y2);
            dy = y2-y1;
            dx = x2-x1;
        }
        int x = x1, y = y1;
        int d = (2 * dx) + dy;
        int change1 = 2*(dx+dy);
        int change2 = 2*dx;
        SetPixel(hdc,x,y,color);
        while(y<y2)
        {
            if(d<=0)
            {
                d+=change1;
                x--;
            }
            else
                d+=change2;
            y++;
            SetPixel(hdc,x,y,color);
        }
    }
}

void drawQuarter(HDC hdc,int x,int y,int xc,int yc,int p)
{
    switch(p)
    {
    case 1:
        parametricline(hdc,xc+y,yc-x,xc,yc);
        parametricline(hdc,xc+x,yc-y,xc,yc);
        break;
    case 2:
        parametricline(hdc,xc-y,yc+x,xc,yc);
        parametricline(hdc,xc-x,yc+y,xc,yc);
        break;
    case 3:
        parametricline(hdc,xc-x,yc-y,xc,yc);
        parametricline(hdc,xc-y,yc-x,xc,yc);
        break;
    case 4:
        parametricline(hdc,xc+x,yc+y,xc,yc);
        parametricline(hdc,xc+y,yc+x,xc,yc);
        break;
    }
}

void filling(HDC hdc,int xc,int yc, int r,int f)
{
    int x=0;
    int y=r;
    int d=1-r;
    int c1=3, c2=5-(2*r);
    drawQuarter(hdc,x,y,xc,yc,f);
    while(x<y)
    {
        if(d<0)
        {
            d+=c1;
            c2+=2;
        }
        else
        {
            d+=c2;
            c2+=4;
            y--;
        }
        c1+=2;
        x++;
        drawQuarter(hdc,x,y,xc,yc,f);
    }
}

bool checkPoint(int x, int y, double r, int xc, int yc)
{
    int dx = abs(x - xc) * abs(x - xc);
    int dy = abs(y - yc) * abs(y - yc);
    double l = sqrt(dx + dy);

    if(r >= l)
        return true;
    else
        return false;
}


void checkFilling(HDC hdc,int x,int y)
{
    if(rediusFilling == -1 || !checkPoint(x,y,rediusFilling,xFilling,yFilling))
        return;
    int f = 1;
    if(x > xFilling && y > yFilling)
    {
        f = 4;
    }
    else if(x < xFilling && y > yFilling)
    {
        f = 2;
    }
    else if(x < xFilling && y < yFilling)
    {
        f = 3;
    }

    filling(hdc,xFilling,yFilling,rediusFilling,f);
}


void draweight(HDC hdc,int x,int y,int xc,int yc)
{
    SetPixel(hdc,xc+x,yc+y,color);
    SetPixel(hdc,xc-x,yc+y,color);
    SetPixel(hdc,xc+x,yc-y,color);
    SetPixel(hdc,xc-x,yc-y,color);

    SetPixel(hdc,xc-y,yc+x,color);
    SetPixel(hdc,xc+y,yc-x,color);
    SetPixel(hdc,xc+y,yc+x,color);
    SetPixel(hdc,xc-y,yc-x,color);
}

void drawFour(HDC hdc,int h,int k,int x,int y)
{
    SetPixel(hdc,h+x,k+y,color);
    SetPixel(hdc,h+x,k-y,color);
    SetPixel(hdc,h-x,k-y,color);
    SetPixel(hdc,h-x,k+y,color);
}


void polarEllipse(HDC hdc,int h,int k,int rx,int ry)
{
    float x=rx;
    float y=0;
    float angle=0;

    float inc;
    if(rx > ry)
        inc = 1.0/rx;
    else
        inc = 1.0/ry;

    do
    {
        drawFour(hdc,h,k,round(x),round(y));
        angle+=inc;
        x=(rx*cos(angle));
        y=(ry*sin(angle));
    }
    while(x > 0);
}

void directEllipse(HDC hdc,int h,int k,int rx,int ry)
{
    double rx2 = rx*rx;
    double ry2 = ry*ry;
    if(rx > ry)
    {
        int x=0,y=ry;
        do
        {
            drawFour(hdc,h,k,x+0.5,y+0.5);
            x++;
            y=round(sqrt(ry2 - ((ry2 * (x*x)) / rx2)));

        }
        while(x < rx);
    }
    else
    {
        int x=rx,y=0;
        do
        {
            drawFour(hdc,h,k,x+0.5,y+0.5);
            y++;
            x=round(sqrt(rx2 - ((rx2 * (y*y)) / ry2)));

        }
        while(y < ry);
    }
}


void polar(HDC hdc,int xc,int yc,int r)
{
    double x=r,y=0,theta=0,dtheta=1.0/r;
    draweight(hdc,xc,yc,r,0);
    while(x>y)
    {
        theta+=dtheta;
        x=r*cos(theta);
        y=r*sin(theta);
        draweight(hdc,Round(x),Round(y),xc,yc);
    }
}

void midpoint(HDC hdc,int xc,int yc,int r)
{
    int x=0;
    int y=r;
    double d=1-r;
    while(x<y)
    {
        if(d<=0)
        {
            d=d+2*x+3;
            x++;
        }
        else
        {
            d=d+2*(x-y)+5;
            x++;
            y--;
        }
        draweight(hdc,x,y,xc,yc);
    }
}

void modified_midpoint(HDC hdc,int xc,int yc, int r)
{
    int x=0;
    int y=r;
    int d=1-r;
    int c1=3, c2=5-(2*r);
    draweight(hdc,x,y,xc,yc);
    while(x<y)
    {
        if(d<0)
        {
            d+=c1;
            c2+=2;
        }
        else
        {
            d+=c2;
            c2+=4;
            y--;
        }
        c1+=2;
        x++;
        draweight(hdc,x,y,xc,yc);
    }
}

void CircleDirect(HDC hdc,int xc,int yc, int R)
{
    int x=0,y=R;
    int R2=R*R;
    draweight(hdc,xc,yc,x,y);
    while(x<y)
    {
        x++;
        y=round(sqrt((double)(R2-x*x)));
        draweight(hdc,x,y,xc,yc);
    }
}

void  iterativePolar(HDC hdc,int xc,int yc,int r)
{
    double x=r,y=0.0,dtheta=1.0/r;
    draweight(hdc,xc,yc,r,0);
    double c=cos(dtheta);
    double s=sin(dtheta);

    while(x>y)
    {
        double x1=(x*c)-(y*s);
        y=(x*s)+(y*c);
        x=x1;
        draweight(hdc,round(x),round(y),xc,yc);
    }
}


union OutCode
{
    unsigned All:4;
    struct
    {
        unsigned left:1,top:1,right:1,bottom:1;
    };
};

OutCode GetOutCode(double x,double y,int xleft,int ytop,int xright,int ybottom)
{
    OutCode out;
    out.All=0;
    if(x<xleft)
        out.left=1;
    else if(x>xright)
        out.right=1;
    if(y<ytop)
        out.top=1;
    else if(y>ybottom)
        out.bottom=1;
    return out;
}

void VIntersect(double xs,double ys,double xe,double ye,int x,double *xi,double *yi)
{
    *xi=x;
    *yi=ys+(x-xs)*(ye-ys)/(xe-xs);
}

void HIntersect(double xs,double ys,double xe,double ye,int y,double *xi,double *yi)
{
    *yi=y;
    *xi=xs+(y-ys)*(xe-xs)/(ye-ys);
}

void CohenSuth(HDC hdc,int xs,int ys,int xe,int ye,int xleft,int ytop,int xright,int ybottom)
{
    double x1=xs,y1=ys,x2=xe,y2=ye;
    OutCode out1=GetOutCode(x1,y1,xleft,ytop,xright,ybottom);
    OutCode out2=GetOutCode(x2,y2,xleft,ytop,xright,ybottom);
    while( (out1.All || out2.All) && !(out1.All & out2.All))
    {
        double xi,yi;
        if(out1.All)
        {
            if(out1.left)
                VIntersect(x1,y1,x2,y2,xleft,&xi,&yi);
            else if(out1.top)
                HIntersect(x1,y1,x2,y2,ytop,&xi,&yi);
            else if(out1.right)
                VIntersect(x1,y1,x2,y2,xright,&xi,&yi);
            else
                HIntersect(x1,y1,x2,y2,ybottom,&xi,&yi);
            x1=xi;
            y1=yi;
            out1=GetOutCode(x1,y1,xleft,ytop,xright,ybottom);
        }
        else
        {
            if(out2.left)
                VIntersect(x1,y1,x2,y2,xleft,&xi,&yi);
            else if(out2.top)
                HIntersect(x1,y1,x2,y2,ytop,&xi,&yi);
            else if(out2.right)
                VIntersect(x1,y1,x2,y2,xright,&xi,&yi);
            else
                HIntersect(x1,y1,x2,y2,ybottom,&xi,&yi);
            x2=xi;
            y2=yi;
            out2=GetOutCode(x2,y2,xleft,ytop,xright,ybottom);
        }
    }
    if(!out1.All && !out2.All)
    {
        midpointLine(hdc,x1,y1,x2,y2);
    }
}

void PointClipping(HDC hdc, int x,int y,int xleft,int ytop,int xright,int ybottom)
{
    if(x>=xleft && x<= xright && y<=ytop && y>=ybottom)
        SetPixel(hdc,x,y,color);
}

void addMenus(HWND hWnd)
{
    hMenu = CreateMenu();

    HMENU colorMenu  = CreateMenu();

    AppendMenu(colorMenu, MF_STRING, RED, _T("Red"));
    AppendMenu(colorMenu, MF_STRING, BLACK, _T("Black"));
    AppendMenu(colorMenu, MF_STRING, ORANGE, _T("Orange"));
    AppendMenu(colorMenu, MF_STRING, YELLOW, _T("Yellow"));
    AppendMenu(colorMenu, MF_STRING, BLUE, _T("Blue"));
    AppendMenu(colorMenu, MF_STRING, PURPLE, _T("Purple"));
    AppendMenu(colorMenu, MF_STRING, GREEN, _T("Green"));

    HMENU cursorMenu  = CreateMenu();

    AppendMenu(cursorMenu, MF_STRING, CURSOR_HAND, _T("Hand"));
    AppendMenu(cursorMenu, MF_STRING, CURSOR_ARROW, _T("Standard arrow"));
    AppendMenu(cursorMenu, MF_STRING, CURSOR_CROSS, _T("Crosshair"));
    AppendMenu(cursorMenu, MF_STRING, CURSOR_HELP, _T("Arrow and question mark"));
    AppendMenu(cursorMenu, MF_STRING, CURSOR_UPARROW, _T("Vertical arrow"));
    AppendMenu(cursorMenu, MF_STRING, CURSOR_IBEAM, _T("I-beam"));
    AppendMenu(cursorMenu, MF_STRING, CURSOR_WAIT, _T("Hourglass"));

    HMENU fMenu  = CreateMenu();

    AppendMenu(fMenu, MF_STRING, CLEAR_Window, _T("Clear"));
    AppendMenu(fMenu, MF_STRING, SAVE, _T("Save"));
    AppendMenu(fMenu, MF_STRING, LOAD, _T("Load"));
    AppendMenu(fMenu, MF_STRING, NEW_FILE, _T("New file"));

    HMENU lMenu  = CreateMenu();

    AppendMenu(lMenu, MF_STRING, Line_DDA, _T("DDA"));
    AppendMenu(lMenu, MF_STRING, Line_MIDPOINT, _T("Midpoint"));
    AppendMenu(lMenu, MF_STRING, Line_PARAMETRIC, _T("parametric"));

    HMENU cMenu  = CreateMenu();

    AppendMenu(cMenu, MF_STRING, CIRCLE_DIRECT, _T("Direct"));
    AppendMenu(cMenu, MF_STRING, CIRCLE_MIDPOINT, _T("Midpoint"));
    AppendMenu(cMenu, MF_STRING, CIRCLE_POLAR, _T("Polar"));
    AppendMenu(cMenu, MF_STRING, CIRCLE_ITERATIVE_POLAR, _T("Iterative polar"));
    AppendMenu(cMenu, MF_STRING, CIRCLE_MODIFIED_MIDPOINT, _T("Modified midpoint"));

    HMENU eMenu  = CreateMenu();

    AppendMenu(eMenu, MF_STRING, ELLIPSE_DIRECT, _T("Direct"));
    AppendMenu(eMenu, MF_STRING, ELLIPSE_POLAR, _T("Polar"));

    HMENU clMenu  = CreateMenu();

    AppendMenu(clMenu, MF_STRING, CLIPPING_LINE, _T("Line"));
    AppendMenu(clMenu, MF_STRING, CLIPPING_POINT, _T("Point"));

    HMENU fiMenu  = CreateMenu();

    AppendMenu(fiMenu, MF_STRING, FILLING, _T("quarter"));

    AppendMenu(hMenu, MF_POPUP, (UINT_PTR) fMenu, _T("File"));
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR) lMenu, _T("Line"));
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR) cMenu, _T("Circle"));
    AppendMenu(hMenu, MF_POPUP,(UINT_PTR) fiMenu, _T("Filling"));
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR) eMenu, _T("ellipse"));
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR) clMenu, _T("clipping"));
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR) colorMenu, _T("Color"));
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR) cursorMenu, _T("Cursor"));

    SetMenu(hWnd,hMenu);
}


void save()
{
    ofstream myfile;
    myfile.open ("draw.txt",ios::app);
    myfile << saveFile;
    myfile.close();
}

void load(HDC hdc)
{
    string line;
    ifstream myfile ("draw.txt");
    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            string temp = "";
            vector<string> vec;
            for(int i = 0; i < line.length(); i++)
            {
                if(line[i] != ',')
                {
                    temp += line[i];
                }
                else
                {
                    vec.push_back(temp);
                    temp = "";
                }
            }
            vec.push_back(temp);
            temp = "";
            color = stoi(vec[vec.size()-1]);
            if("checkFilling" == vec[0])
            {
                checkFilling(hdc,stoi(vec[1]),stoi(vec[2]));
            }
            else if("parametricline" == vec[0])
            {
                parametricline(hdc,stoi(vec[1]),stoi(vec[2]),stoi(vec[3]),stoi(vec[4]));
            }
            else if("midpointLine" == vec[0])
            {
                midpointLine(hdc,stoi(vec[1]),stoi(vec[2]),stoi(vec[3]),stoi(vec[4]));
            }
            else if("ddaLine" == vec[0])
            {
                ddaLine(hdc,stoi(vec[1]),stoi(vec[2]),stoi(vec[3]),stoi(vec[4]));
            }
            else if("modified_midpoint" == vec[0])
            {
                r1 = sqrt(pow((stoi(vec[3])-stoi(vec[1])),2)+pow((stoi(vec[4])-stoi(vec[2])),2));
                rediusFilling = r1;
                xFilling = stoi(vec[1]);
                yFilling = stoi(vec[2]);
                modified_midpoint(hdc,stoi(vec[1]),stoi(vec[2]),r1);
            }
            else if("midpoint" == vec[0])
            {
                r1 = sqrt(pow((stoi(vec[3])-stoi(vec[1])),2)+pow((stoi(vec[4])-stoi(vec[2])),2));
                rediusFilling = r1;
                xFilling = stoi(vec[1]);
                yFilling = stoi(vec[2]);
                midpoint(hdc,stoi(vec[1]),stoi(vec[2]),r1);
            }
            else if("iterativePolar" == vec[0])
            {
                r1 = sqrt(pow((stoi(vec[3])-stoi(vec[1])),2)+pow((stoi(vec[4])-stoi(vec[2])),2));
                rediusFilling = r1;
                xFilling = stoi(vec[1]);
                yFilling = stoi(vec[2]);
                iterativePolar(hdc,stoi(vec[1]),stoi(vec[2]),r1);
            }
            else if("CircleDirect" == vec[0])
            {
                r1 = sqrt(pow((stoi(vec[3])-stoi(vec[1])),2)+pow((stoi(vec[4])-stoi(vec[2])),2));
                rediusFilling = r1;
                xFilling = stoi(vec[1]);
                yFilling = stoi(vec[2]);
                CircleDirect(hdc,stoi(vec[1]),stoi(vec[2]),r1);
            }
            else if("polar" == vec[0])
            {
                r1 = sqrt(pow((stoi(vec[3])-stoi(vec[1])),2)+pow((stoi(vec[4])-stoi(vec[2])),2));
                rediusFilling = r1;
                xFilling = stoi(vec[1]);
                yFilling = stoi(vec[2]);
                polar(hdc,stoi(vec[1]),stoi(vec[2]),r1);
            }
            else if("Rectangle" == vec[0])
            {
                Rectangle(hdc,stoi(vec[1]),stoi(vec[2]),stoi(vec[3]),stoi(vec[4]));
                x_1 = stoi(vec[1]);
                x_2 = stoi(vec[3]);
                y_1 = stoi(vec[2]);
                y_2 = stoi(vec[4]);
            }
            else if("polarEllipse" == vec[0])
            {
                r1 = abs(stoi(vec[1]) - stoi(vec[3]));
                r2 = abs(stoi(vec[2]) - stoi(vec[6]));
                polarEllipse(hdc,stoi(vec[1]),stoi(vec[2]),r1,r2);
            }
            else if("directEllipse" == vec[0])
            {
                r1 = abs(stoi(vec[1]) - stoi(vec[3]));
                r2 = abs(stoi(vec[2]) - stoi(vec[6]));
                directEllipse(hdc,stoi(vec[1]),stoi(vec[2]),r1,r2);
            }
            else if("PointClipping" == vec[0])
            {
                x_3 = stoi(vec[1]);
                y_3 = stoi(vec[2]);
                PointClipping(hdc,x_3,y_3,x_1,y_1,x_2,y_2);
            }
            else if("CohenSuth" == vec[0])
            {
                x_3 = stoi(vec[1]);
                y_3 = stoi(vec[2]);
                x_4 = stoi(vec[3]);
                y_4 = stoi(vec[4]);
                CohenSuth(hdc,x_3,y_3,x_4,y_4,x_1,y_2,x_2,y_1);
            }
        }
        myfile.close();
    }
}

void write1(string t,int x_1,int y_1,int x_2,int y_2)
{
    saveFile += t + "," + to_string(x_1)+ "," +to_string(y_1) + "," + to_string(x_2) + "," + to_string(y_2)+ "," + to_string(color) + "\n";
}

void write2(string t,int x_1,int y_1,int x_2,int y_2,int x_3,int y_3)
{
    saveFile += t + "," + to_string(x_1)+ "," +to_string(y_1) + "," + to_string(x_2) + "," + to_string(y_2) + "," + to_string(x_3) + "," + to_string(y_3) +"," + to_string(color) + "\n";
}

void write3(string t,int x_1,int y_1)
{
    saveFile += t + "," + to_string(x_1)+ "," +to_string(y_1) + "," + to_string(color) + "\n";
}

/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc=GetDC(hwnd);
    switch (message)                  /* handle the messages */
    {
    case WM_LBUTTONDBLCLK:
        if(Num_of_Points==0)
        {
            x_1=LOWORD(lParam);
            y_1=HIWORD(lParam);
            Num_of_Points++;
            if(choice == FILLING)
            {
                checkFilling(hdc,x_1,y_1);
                write3("checkFilling",x_1,y_1);
                Num_of_Points = 0;
            }

        }
        else if (Num_of_Points==1)
        {
            Num_of_Points++;
            x_2=LOWORD(lParam);
            y_2=HIWORD(lParam);
            r1 = sqrt(pow((x_2-x_1),2)+pow((y_2-y_1),2));
            switch(choice)
            {
            case Line_DDA:
                ddaLine(hdc,x_1,y_1,x_2,y_2);
                write1("ddaLine",x_1,y_1,x_2,y_2);
                Num_of_Points = 0;
                break;
            case Line_MIDPOINT:
                midpointLine(hdc,x_1,y_1,x_2,y_2);
                write1("midpointLine",x_1,y_1,x_2,y_2);
                Num_of_Points = 0;
                break;
            case Line_PARAMETRIC:
                parametricline(hdc,x_1,y_1,x_2,y_2);
                write1("parametricline",x_1,y_1,x_2,y_2);
                Num_of_Points = 0;
                break;
            case CIRCLE_DIRECT:
                xFilling = x_1;
                yFilling = y_1;
                rediusFilling = r1;
                CircleDirect(hdc,x_1,y_1,r1);
                write1("CircleDirect",x_1,y_1,x_2,y_2);
                Num_of_Points = 0;
                break;
            case CIRCLE_ITERATIVE_POLAR:
                xFilling = x_1;
                yFilling = y_1;
                rediusFilling = r1;
                iterativePolar(hdc,x_1,y_1,r1);
                write1("iterativePolar",x_1,y_1,x_2,y_2);
                Num_of_Points = 0;
                break;
            case CIRCLE_MIDPOINT:
                xFilling = x_1;
                yFilling = y_1;
                rediusFilling = r1;
                midpoint(hdc,x_1,y_1,r1);
                write1("midpoint",x_1,y_1,x_2,y_2);
                Num_of_Points = 0;
                break;
            case CIRCLE_MODIFIED_MIDPOINT:
                xFilling = x_1;
                yFilling = y_1;
                rediusFilling = r1;
                modified_midpoint(hdc,x_1,y_1,r1);
                write1("modified_midpoint",x_1,y_1,x_2,y_2);
                Num_of_Points = 0;
                break;
            case CIRCLE_POLAR:
                xFilling = x_1;
                yFilling = y_1;
                rediusFilling = r1;
                polar(hdc,x_1,y_1,r1);
                write1("polar",x_1,y_1,x_2,y_2);
                Num_of_Points = 0;
                break;
            case CLIPPING_LINE:
            case CLIPPING_POINT:
                Rectangle(hdc, x_1,y_1,x_2,y_2);
                write1("Rectangle",x_1,y_1,x_2,y_2);
                break;
            default:
                break;
            }
        }
        else if(Num_of_Points==2)
        {
            Num_of_Points++;
            x_3=LOWORD(lParam);
            y_3=HIWORD(lParam);
            r1 = abs(x_1 - x_2);
            r2 = abs(y_1 - y_3);
            switch(choice)
            {
            case ELLIPSE_DIRECT:
                directEllipse(hdc,x_1,y_1,r1,r2);
                write2("directEllipse",x_1,y_1,x_2,y_2,x_3,y_3);
                Num_of_Points = 0;
                break;
            case ELLIPSE_POLAR:
                polarEllipse(hdc,x_1,y_1,r1,r2);
                write2("polarEllipse",x_1,y_1,x_2,y_2,x_3,y_3);
                Num_of_Points = 0;
                break;
            case CLIPPING_POINT:
                PointClipping(hdc,x_3,y_3,x_1,y_1,x_2,y_2);
                write3("PointClipping",x_3,y_3);
                Num_of_Points=2;
                break;
            default:
                break;
            }

        }
        else if (Num_of_Points==3)
        {
            Num_of_Points = 0;
            if(CLIPPING_LINE == choice)
            {
                x_4=LOWORD(lParam);
                y_4=HIWORD(lParam);
                CohenSuth(hdc,x_3,y_3,x_4,y_4,x_1,y_2,x_2,y_1);
                write1("CohenSuth",x_3,y_3,x_4,y_4);
                Num_of_Points=2;
            }
        }
        break;
    case WM_CREATE:
        addMenus(hwnd);
        break;
    case WM_COMMAND:
        Num_of_Points = 0;
        switch(LOWORD(wParam))
        {
        case CLEAR_Window:
            InvalidateRect(hwnd, NULL, TRUE);
            saveFile = "";
            choice = 0;
            break;
        case SAVE:
            save();
            choice = 0;
            break;
        case LOAD:
            load(hdc);
            choice = 0;
            break;
        case NEW_FILE:
            remove("draw.txt");
            choice = 0;
            break;
        case RED:
            color = RGB(255,0,0);
            choice = 0;
            break;
        case ORANGE:
            color = RGB(255,102,0);
            choice = 0;
            break;
        case YELLOW:
            color = RGB(255,255,0);
            choice = 0;
            break;
        case BLACK:
            color = RGB(0,0,0);
            choice = 0;
            break;
        case PURPLE:
            color = RGB(255,0,255);
            choice = 0;
            break;
        case BLUE:
            color = RGB(0,0,255);
            choice = 0;
            break;
        case GREEN:
            color = RGB(0,255,0);
            choice = 0;
            break;
        case CURSOR_HAND:
            curs = IDC_HAND;
            choice = 0;
            break;
        case CURSOR_ARROW:
            curs = IDC_ARROW;
            choice = 0;
            break;
        case CURSOR_CROSS:
            curs = IDC_CROSS;
            choice = 0;
            break;
        case CURSOR_HELP:
            curs = IDC_HELP;
            choice = 0;
            break;
        case CURSOR_UPARROW:
            curs =IDC_UPARROW;
            choice = 0;
            break;
        case CURSOR_IBEAM:
            curs = IDC_IBEAM;
            choice = 0;
            break;
        case CURSOR_WAIT:
            curs = IDC_WAIT;
            choice = 0;
            break;
        default:
            choice = LOWORD(wParam);
            break;
        }
        break;
    case WM_SETCURSOR:
        if (LOWORD(lParam) == HTCLIENT)
        {
            SetCursor(LoadCursor(NULL,curs));
            return TRUE;
        }
        break;
    case WM_DESTROY:
        PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
        break;
    default:                      /* for messages that we don't deal with */
        return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}


