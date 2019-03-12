#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <algorithm>
#include <queue>
#include <math.h>
#include <iostream>


//todo:
// TODO (admin#1#): podzialka
// TODO (admin#1#): krzywe
// TODO (admin#1#): integral max min itd
// TODO (admin#1#): dane z pliku i fitting


using namespace std;

enum type{//do wspolczynnikow mozna wpisywac np. f1, e, pi
	CONSTANT=0,//A
	LINEAR=1,//Ax+B
	QUADRATIC=2,//Ax^2+Bx+C
	POLYNOMIAL=3,//Ax^n+....+Z      dlugi tekst
	EXPONENTIAL=4,//A*B^(Cx)
	SUM=5,//f1+f2-5...              dlugi tekst
	PRODUCT=6,//f1*f2/7...          dlugi tekst
	SINE=7,//Asin(Bx+C)
	COSINE=8,//Acos(Bx+C)
	TANGENT=9,//Atan(Bx+C)
	ARCSIN=10,//Aarcsin(Bx)+C
	ARCCOS=11,//Aarccos(Bx)+C
	ARCTAN=12,//Aarctan(Bx)+C
	SINE_H=13,//Asinh(Bx+C)
	COSINE_H=14,//Acosh(Bx+C)
	TANGENT_H=15,//Atanh(Bx+C)
    LOG=16,//Alog_B x
	ROOT=17,//x^(1/A) (Ath root of x)
	POWER=18,//Ax^B
	FUNCTION_DERIVATIVE=19,//pochodna (ze skokiem) f1,0.001
	FUNCTION_INTEGRAL=20,//calka?                  f1,0.001
	FUNCTION_COMPOSITION=21,//f o g - f(g(x))
	CODE=22,//zakodowana osobno

	FN_Y=1101,
};
struct fn_int{
    bool czy_funkcja;
    double liczba;
    int numer;
};
struct funkcja{
	type typ;
	int czy_rysowac;
	COLORREF kolor;
	int thickness;
	vector<fn_int> wspolczynniki;
};

double bok_gora=10,bok_dol=-10,bok_lewo=-10,bok_prawo=10;
int lewy_naroznik=290,gorny_naroznik=6,width=774-lewy_naroznik,height=399-gorny_naroznik;
RECT rect_w={lewy_naroznik,gorny_naroznik,lewy_naroznik+width,gorny_naroznik+height};
RECT rect_kol={120,102,140,122};
double factor=1.5; //o ile resize'owac przyciskami
double factor2=1.5; // o ile resize'owac rolka
double delta=0.1; //o ile przesuwac, mnoznik


map <int,funkcja> funkcje;
vector<int> numery_funkcji;
int przesuwanie_wykresu_w_toku=0;
int x_old,y_old;
COLORREF wybranykolor;
int nie_rysuj_wykresu=0;


HWND hOkno;
HFONT hNormalFont;
//ramki
HWND hWykresRamka;
HWND hPrzesuwanieRamka;
HWND hSrodekRozmiarRamka;
HWND hOknoWlasciwosciRamka;
HWND hBokiRamka;
HWND hListaFunkcjiRamka;
HWND hMenuFunkcjiRamka;
HWND hListaKrzywychRamka;
//przyciski
HWND hPrzyciskGora;
HWND hPrzyciskDol;
HWND hPrzyciskLewo;
HWND hPrzyciskPrawo;
HWND hPrzyciskZero;
HWND hPrzyciskZoomInVert;
HWND hPrzyciskZoomOutVert;
HWND hPrzyciskZoomInHoriz;
HWND hPrzyciskZoomOutHoriz;
HWND hPrzyciskZoomIn;
HWND hPrzyciskZoomOut;

HWND hPrzyciskRadioUseLeft;
HWND hPrzyciskRadioUseRight;
HWND hPrzyciskResize;

HWND hPrzyciskAddFunction;
HWND hPrzyciskRemoveFunction;
HWND hPrzyciskCheckBoxShowFunction;
HWND hPrzyciskChangeNumber;

HWND hPrzyciskOK;
HWND hPrzyciskChangeColor;

//pola tekstowe
HWND hTekstCenterX;
HWND hTekstCenterY;
HWND hTekstSizeX;
HWND hTekstSizeY;

HWND hTekstTop;
HWND hTekstBottom;
HWND hTekstLeft;
HWND hTekstRight;

HWND hTekstNumerFunkcji;

HWND hTekstWspolA;
HWND hTekstWspolB;
HWND hTekstWspolC;
HWND hTekstWspolD;
HWND hTekstWspolE;
HWND hTekstWspolWiele;

//static
HWND hNapisCenter;
HWND hNapisCenterX;
HWND hNapisCenterY;
HWND hNapisSize;
HWND hNapisSizeX;
HWND hNapisSizeY;
HWND hNapisTop;
HWND hNapisBottom;
HWND hNapisLeft;
HWND hNapisRight;
HWND hNapisSides;
HWND hNapisWspolA;
HWND hNapisWspolB;
HWND hNapisWspolC;
HWND hNapisWspolD;
HWND hNapisWspolE;
HWND hNapisWspolWiele;
HWND hNapisWspolWielePlus;
HWND hNapisWspolFunkcja;
HWND hNapisWspolStep;
HWND hNapisWspolStala;
HWND hNapisThickness;

//inne
HWND hComboFunkcje;
HWND hComboThickness;

HWND hListFunkcje;


int ZmienKolor(COLORREF);
int Inicjalizuj(HINSTANCE hInstance){
    vector<fn_int> f;
    funkcje[1101]={FN_Y,0,0x000000,1,f};
    f.push_back({0,1,0});
    f.push_back({0,0,0});
    funkcje[1100]={LINEAR,0,0x000000,1,f};

	hNormalFont=(HFONT)GetStockObject(DEFAULT_GUI_FONT);

	// RAMKI:
	hPrzesuwanieRamka=CreateWindowEx(0,"BUTTON","Move or zoom graph",BS_GROUPBOX|WS_CHILD,
	430,400,345,155,hOkno,0,hInstance,0);
	SendMessage(hPrzesuwanieRamka,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hPrzesuwanieRamka,1);
	hOknoWlasciwosciRamka=CreateWindowEx(0,"BUTTON","Graph window",BS_GROUPBOX|WS_CHILD,
	5,400,420,155,hOkno,0,hInstance,0);
	SendMessage(hOknoWlasciwosciRamka,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hOknoWlasciwosciRamka,1);
	hSrodekRozmiarRamka=CreateWindowEx(0,"BUTTON","",BS_GROUPBOX|WS_CHILD,
	10,410,155,140,hOkno,0,hInstance,0);
	SendMessage(hSrodekRozmiarRamka,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hSrodekRozmiarRamka,1);
	hBokiRamka=CreateWindowEx(0,"BUTTON","",BS_GROUPBOX|WS_CHILD,
	280,410,140,140,hOkno,0,hInstance,0);
	SendMessage(hBokiRamka,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hBokiRamka,1);
	hMenuFunkcjiRamka=CreateWindowEx(0,"BUTTON","Function",BS_GROUPBOX|WS_CHILD,
	5,0,280,130,hOkno,0,hInstance,0);
	SendMessage(hMenuFunkcjiRamka,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hMenuFunkcjiRamka,1);
	hListaFunkcjiRamka=CreateWindowEx(0,"BUTTON","Functions",BS_GROUPBOX|WS_CHILD,
	5,130,280,149,hOkno,0,hInstance,0);
	SendMessage(hListaFunkcjiRamka,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hListaFunkcjiRamka,1);
    hListaKrzywychRamka=CreateWindowEx(0,"BUTTON","Differential equations",BS_GROUPBOX|WS_CHILD,
	5,279,280,121,hOkno,0,hInstance,0);
	SendMessage(hListaKrzywychRamka,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hListaKrzywychRamka,1);

	//BUTTONY
	hPrzyciskZoomOutHoriz=CreateWindowEx(0,"BUTTON","<-------->\nOUT",WS_CHILD|BS_MULTILINE,
	680,420,80,40,hOkno,0,hInstance,0);
	SendMessage(hPrzyciskZoomOutHoriz,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hPrzyciskZoomOutHoriz,1);
	hPrzyciskZoomInHoriz=CreateWindowEx(0,"BUTTON","<-------->\nIN",WS_CHILD|BS_MULTILINE,
	680,470,80,40,hOkno,0,hInstance,0);
	SendMessage(hPrzyciskZoomInHoriz,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hPrzyciskZoomInHoriz,1);
	hPrzyciskZoomInVert=CreateWindowEx(0,"BUTTON","/\\\n|\n|\n\\/\nIN",WS_CHILD|BS_MULTILINE,
	630,420,40,90,hOkno,0,hInstance,0);
	SendMessage(hPrzyciskZoomInVert,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hPrzyciskZoomInVert,1);
	hPrzyciskZoomOutVert=CreateWindowEx(0,"BUTTON","/\\\n|\n|\n\\/\nOUT",WS_CHILD|BS_MULTILINE,
	580,420,40,90,hOkno,0,hInstance,0);
	SendMessage(hPrzyciskZoomOutVert,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hPrzyciskZoomOutVert,1);
	hPrzyciskZoomIn=CreateWindowEx(0,"BUTTON","IN",WS_CHILD,
	680,515,80,30,hOkno,0,hInstance,0);
	SendMessage(hPrzyciskZoomIn,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hPrzyciskZoomIn,1);
	hPrzyciskZoomOut=CreateWindowEx(0,"BUTTON","OUT",WS_CHILD,
	580,515,90,30,hOkno,0,hInstance,0);
	SendMessage(hPrzyciskZoomOut,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hPrzyciskZoomOut,1);
	hPrzyciskPrawo=CreateWindowEx(0,"BUTTON","----->",WS_CHILD,
	530,470,40,30,hOkno,0,hInstance,0);
	SendMessage(hPrzyciskPrawo,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hPrzyciskPrawo,1);
	hPrzyciskZero=CreateWindowEx(0,"BUTTON","(0;0)",WS_CHILD,
	485,465,40,40,hOkno,0,hInstance,0);
	SendMessage(hPrzyciskZero,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hPrzyciskZero,1);
	hPrzyciskDol=CreateWindowEx(0,"BUTTON","|\n\\/",WS_CHILD|BS_MULTILINE,
	490,510,30,40,hOkno,0,hInstance,0);
	SendMessage(hPrzyciskDol,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hPrzyciskDol,1);
	hPrzyciskGora=CreateWindowEx(0,"BUTTON","/\\\n|",WS_CHILD|BS_MULTILINE,
	490,420,30,40,hOkno,0,hInstance,0);
	SendMessage(hPrzyciskGora,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hPrzyciskGora,1);
	hPrzyciskLewo=CreateWindowEx(0,"BUTTON","<-----",WS_CHILD,
	440,470,40,30,hOkno,0,hInstance,0);
	SendMessage(hPrzyciskLewo,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hPrzyciskLewo,1);
	hPrzyciskChangeColor=CreateWindowEx(0,"BUTTON","Change color",WS_CHILD,
	15,100,100,23,hOkno,0,hInstance,0);
	SendMessage(hPrzyciskChangeColor,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hPrzyciskChangeColor,1);
	hPrzyciskOK=CreateWindowEx(0,"BUTTON","OK",WS_CHILD,
	245,100,35,23,hOkno,0,hInstance,0);
	SendMessage(hPrzyciskOK,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hPrzyciskOK,1);
	hPrzyciskAddFunction=CreateWindowEx(0,"BUTTON","Add",WS_CHILD,
	235,145,45,23,hOkno,0,hInstance,0);
	SendMessage(hPrzyciskAddFunction,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hPrzyciskAddFunction,1);
	hPrzyciskRemoveFunction=CreateWindowEx(0,"BUTTON","Remove",WS_CHILD,
	235,170,45,23,hOkno,0,hInstance,0);
	SendMessage(hPrzyciskRemoveFunction,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hPrzyciskRemoveFunction,1);
	hPrzyciskChangeNumber=CreateWindowEx(0,"BUTTON","Change number",WS_CHILD|BS_MULTILINE,
	235,238,45,35,hOkno,0,hInstance,0);
	SendMessage(hPrzyciskChangeNumber,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hPrzyciskChangeNumber,1);
#define ID_CHECKBOX_SHOW 501
	hPrzyciskCheckBoxShowFunction=CreateWindowEx(0,"BUTTON","Show",WS_CHILD|BS_CHECKBOX,
	235,193,45,20,hOkno,(HMENU)ID_CHECKBOX_SHOW,hInstance,0);
	SendMessage(hPrzyciskCheckBoxShowFunction,WM_SETFONT,(WPARAM)hNormalFont,0);
	CheckDlgButton(hOkno,ID_CHECKBOX_SHOW,BST_CHECKED);
	ShowWindow(hPrzyciskCheckBoxShowFunction,1);
#define ID_RADIO_LEFT 401
	hPrzyciskRadioUseLeft=CreateWindowEx(0,"BUTTON","Use center and size",WS_CHILD|BS_RADIOBUTTON|BS_MULTILINE,
	170,430,100,50,hOkno,(HMENU)ID_RADIO_LEFT,hInstance,0);
	SendMessage(hPrzyciskRadioUseLeft,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hPrzyciskRadioUseLeft,1);
#define ID_RADIO_RIGHT 402
	hPrzyciskRadioUseRight=CreateWindowEx(0,"BUTTON","Use four sides",WS_CHILD|BS_RADIOBUTTON|BS_MULTILINE,
	170,480,100,20,hOkno,(HMENU)ID_RADIO_RIGHT,hInstance,0);
	SendMessage(hPrzyciskRadioUseRight,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hPrzyciskRadioUseRight,1);
	CheckRadioButton(hOkno,ID_RADIO_LEFT,ID_RADIO_RIGHT,ID_RADIO_LEFT);
	hPrzyciskResize=CreateWindowEx(0,"BUTTON","Resize",WS_CHILD,
	200,510,45,25,hOkno,0,hInstance,0);
	SendMessage(hPrzyciskResize,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hPrzyciskResize,1);


	//static
	hNapisCenter=CreateWindowEx(0,"STATIC","Center",WS_CHILD,
	15,420,50,30,hOkno,0,hInstance,0);
	SendMessage(hNapisCenter,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hNapisCenter,1);
	hNapisCenterX=CreateWindowEx(0,"STATIC","X:",WS_CHILD,
	15,440,10,20,hOkno,0,hInstance,0);
	SendMessage(hNapisCenterX,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hNapisCenterX,1);
	hNapisCenterY=CreateWindowEx(0,"STATIC","Y:",WS_CHILD,
	15,490,10,20,hOkno,0,hInstance,0);
	SendMessage(hNapisCenterY,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hNapisCenterY,1);
	hNapisSize=CreateWindowEx(0,"STATIC","Size",WS_CHILD,
	90,420,50,20,hOkno,0,hInstance,0);
	SendMessage(hNapisSize,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hNapisSize,1);
	hNapisSizeX=CreateWindowEx(0,"STATIC","X:",WS_CHILD,
	90,440,10,20,hOkno,0,hInstance,0);
	SendMessage(hNapisSizeX,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hNapisSizeX,1);
	hNapisSizeY=CreateWindowEx(0,"STATIC","Y:",WS_CHILD,
	90,490,10,20,hOkno,0,hInstance,0);
	SendMessage(hNapisSizeY,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hNapisSizeY,1);
	hNapisSides=CreateWindowEx(0,"STATIC","Sides:",WS_CHILD,
	300,420,50,20,hOkno,0,hInstance,0);
	SendMessage(hNapisSides,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hNapisSides,1);
	hNapisTop=CreateWindowEx(0,"STATIC","Top:",WS_CHILD,
	285,440,50,20,hOkno,0,hInstance,0);
	SendMessage(hNapisTop,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hNapisTop,1);
	hNapisBottom=CreateWindowEx(0,"STATIC","Bottom:",WS_CHILD,
	285,465,50,20,hOkno,0,hInstance,0);
	SendMessage(hNapisBottom,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hNapisBottom,1);
	hNapisLeft=CreateWindowEx(0,"STATIC","Left:",WS_CHILD,
	285,490,50,20,hOkno,0,hInstance,0);
	SendMessage(hNapisLeft,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hNapisLeft,1);
	hNapisRight=CreateWindowEx(0,"STATIC","Right:",WS_CHILD,
	285,515,50,20,hOkno,0,hInstance,0);
	SendMessage(hNapisRight,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hNapisRight,1);
	hNapisWspolA=CreateWindowEx(0,"STATIC","A",WS_CHILD,
	30,55,20,15,hOkno,0,hInstance,0);
	SendMessage(hNapisWspolA,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hNapisWspolA,1);
	hNapisWspolB=CreateWindowEx(0,"STATIC","B",WS_CHILD,
	85,55,20,15,hOkno,0,hInstance,0);
	SendMessage(hNapisWspolB,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hNapisWspolB,1);
	hNapisWspolC=CreateWindowEx(0,"STATIC","C",WS_CHILD,
	140,55,20,15,hOkno,0,hInstance,0);
	SendMessage(hNapisWspolC,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hNapisWspolC,1);
	hNapisWspolD=CreateWindowEx(0,"STATIC","D",WS_CHILD,
	195,55,20,15,hOkno,0,hInstance,0);
	SendMessage(hNapisWspolD,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hNapisWspolD,1);
	hNapisWspolE=CreateWindowEx(0,"STATIC","E",WS_CHILD,
	250,55,20,15,hOkno,0,hInstance,0);
	SendMessage(hNapisWspolE,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hNapisWspolE,1);
	hNapisWspolWiele=CreateWindowEx(0,"STATIC","Coefficients separated with spaces",WS_CHILD,
	30,55,200,15,hOkno,0,hInstance,0);
	SendMessage(hNapisWspolWiele,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hNapisWspolWiele,0);
	hNapisWspolWielePlus=CreateWindowEx(0,"STATIC","Coefficients and operators separated with spaces",WS_CHILD,
	30,55,250,15,hOkno,0,hInstance,0);
	SendMessage(hNapisWspolWielePlus,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hNapisWspolWielePlus,0);
	hNapisWspolFunkcja=CreateWindowEx(0,"STATIC","Function",WS_CHILD,
	10,55,200,15,hOkno,0,hInstance,0);
	SendMessage(hNapisWspolFunkcja,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hNapisWspolFunkcja,0);
	hNapisWspolStep=CreateWindowEx(0,"STATIC","Step",WS_CHILD,
	65,55,200,15,hOkno,0,hInstance,0);
	SendMessage(hNapisWspolStep,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hNapisWspolStep,0);
	hNapisWspolStala=CreateWindowEx(0,"STATIC","Constant",WS_CHILD,
	120,55,140,15,hOkno,0,hInstance,0);
	SendMessage(hNapisWspolStala,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hNapisWspolStala,0);
	hNapisThickness=CreateWindowEx(0,"STATIC","Thickness:",WS_CHILD,
	145,105,70,15,hOkno,0,hInstance,0);
	SendMessage(hNapisThickness,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hNapisThickness,1);


	//pola tekstowe
	hTekstCenterX=CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT","",WS_CHILD,
	15,460,70,23,hOkno,0,hInstance,0);
	SendMessage(hTekstCenterX,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hTekstCenterX,1);
	hTekstCenterY=CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT","",WS_CHILD,
	15,510,70,23,hOkno,0,hInstance,0);
	SendMessage(hTekstCenterY,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hTekstCenterY,1);
	hTekstSizeX=CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT","",WS_CHILD,
	90,460,70,23,hOkno,0,hInstance,0);
	SendMessage(hTekstSizeX,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hTekstSizeX,1);
	hTekstSizeY=CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT","",WS_CHILD,
	90,510,70,23,hOkno,0,hInstance,0);
	SendMessage(hTekstSizeY,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hTekstSizeY,1);
	hTekstTop=CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT","",WS_CHILD,
	335,440,80,23,hOkno,0,hInstance,0);
	SendMessage(hTekstTop,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hTekstTop,1);
	hTekstBottom=CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT","",WS_CHILD,
	335,465,80,23,hOkno,0,hInstance,0);
	SendMessage(hTekstBottom,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hTekstBottom,1);
	hTekstLeft=CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT","",WS_CHILD,
	335,490,80,23,hOkno,0,hInstance,0);
	SendMessage(hTekstLeft,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hTekstLeft,1);
	hTekstRight=CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT","",WS_CHILD,
	335,515,80,23,hOkno,0,hInstance,0);
	SendMessage(hTekstRight,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hTekstRight,1);
	hTekstNumerFunkcji=CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT","",WS_CHILD,
	235,213,45,23,hOkno,0,hInstance,0);
	SendMessage(hTekstNumerFunkcji,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hTekstNumerFunkcji,1);
	hTekstWspolA=CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT","",WS_CHILD,
	10,70,50,23,hOkno,0,hInstance,0);
	SendMessage(hTekstWspolA,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hTekstWspolA,1);
	hTekstWspolB=CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT","",WS_CHILD,
	65,70,50,23,hOkno,0,hInstance,0);
	SendMessage(hTekstWspolB,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hTekstWspolB,1);
	hTekstWspolC=CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT","",WS_CHILD,
	120,70,50,23,hOkno,0,hInstance,0);
	SendMessage(hTekstWspolC,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hTekstWspolC,1);
	hTekstWspolD=CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT","",WS_CHILD,
	175,70,50,23,hOkno,0,hInstance,0);
	SendMessage(hTekstWspolD,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hTekstWspolD,1);
	hTekstWspolE=CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT","",WS_CHILD,
	230,70,50,23,hOkno,0,hInstance,0);
	SendMessage(hTekstWspolE,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hTekstWspolE,1);
	hTekstWspolWiele=CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT","",WS_CHILD,
	10,70,270,23,hOkno,0,hInstance,0);
	SendMessage(hTekstWspolWiele,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hTekstWspolWiele,0);

	//inne
	hComboFunkcje=CreateWindowEx(WS_EX_CLIENTEDGE,"COMBOBOX","",WS_CHILD|CBS_DROPDOWNLIST,
	10,25,270,400,hOkno,0,hInstance,0);
	SendMessage(hComboFunkcje,WM_SETFONT,(WPARAM)hNormalFont,0);
	SendMessage(hComboFunkcje,CB_ADDSTRING,0,(LPARAM)"Constant A");
	SendMessage(hComboFunkcje,CB_ADDSTRING,0,(LPARAM)"Linear Ax+B");
	SendMessage(hComboFunkcje,CB_ADDSTRING,0,(LPARAM)"Quadratic Ax^2+Bx+C");
	SendMessage(hComboFunkcje,CB_ADDSTRING,0,(LPARAM)"Polynomial");
	SendMessage(hComboFunkcje,CB_ADDSTRING,0,(LPARAM)"Exponential A*B^(Cx)");
	SendMessage(hComboFunkcje,CB_ADDSTRING,0,(LPARAM)"Sum or difference");
	SendMessage(hComboFunkcje,CB_ADDSTRING,0,(LPARAM)"Product or quotient");
	SendMessage(hComboFunkcje,CB_ADDSTRING,0,(LPARAM)"Sine A*sin(Bx+C)");
	SendMessage(hComboFunkcje,CB_ADDSTRING,0,(LPARAM)"Cosine A*cos(Bx+C)");
	SendMessage(hComboFunkcje,CB_ADDSTRING,0,(LPARAM)"Tangent A*tan(Bx+C)");
	SendMessage(hComboFunkcje,CB_ADDSTRING,0,(LPARAM)"Arcsin A*arcsin(Bx)+C");
	SendMessage(hComboFunkcje,CB_ADDSTRING,0,(LPARAM)"Arccos A*arccos(Bx)+C");
	SendMessage(hComboFunkcje,CB_ADDSTRING,0,(LPARAM)"Arctan A*arctan(Bx)+C");
	SendMessage(hComboFunkcje,CB_ADDSTRING,0,(LPARAM)"Hyperbolic sine A*sinh(Bx+C)");
	SendMessage(hComboFunkcje,CB_ADDSTRING,0,(LPARAM)"Hyperbolic cosine A*cosh(Bx+C)");
	SendMessage(hComboFunkcje,CB_ADDSTRING,0,(LPARAM)"Hyperbolic tangent A*tanh(Bx+C)");
	SendMessage(hComboFunkcje,CB_ADDSTRING,0,(LPARAM)"Logarithm A*log_B x");
	SendMessage(hComboFunkcje,CB_ADDSTRING,0,(LPARAM)"Root x^(1/A)");
	SendMessage(hComboFunkcje,CB_ADDSTRING,0,(LPARAM)"Power A*x^B");
	SendMessage(hComboFunkcje,CB_ADDSTRING,0,(LPARAM)"Derivative of function");
	SendMessage(hComboFunkcje,CB_ADDSTRING,0,(LPARAM)"Integral of function");
	SendMessage(hComboFunkcje,CB_ADDSTRING,0,(LPARAM)"Function composition f(g(x))");
	SendMessage(hComboFunkcje,CB_ADDSTRING,0,(LPARAM)"Hard-coded function");
	SendMessage(hComboFunkcje,CB_SELECTSTRING,0,(LPARAM)"Constant A"); //default
	ShowWindow(hComboFunkcje,1);


	hComboThickness=CreateWindowEx(WS_EX_CLIENTEDGE,"COMBOBOX","",WS_CHILD|CBS_DROPDOWNLIST,
	200,100,40,100,hOkno,0,hInstance,0);
	SendMessage(hComboThickness,WM_SETFONT,(WPARAM)hNormalFont,0);
	SendMessage(hComboThickness,CB_ADDSTRING,0,(LPARAM)"1");
	SendMessage(hComboThickness,CB_ADDSTRING,0,(LPARAM)"2");
	SendMessage(hComboThickness,CB_ADDSTRING,0,(LPARAM)"3");
	SendMessage(hComboThickness,CB_ADDSTRING,0,(LPARAM)"4");
	SendMessage(hComboThickness,CB_ADDSTRING,0,(LPARAM)"5");

	SendMessage(hComboThickness,CB_SELECTSTRING,0,(LPARAM)"1");//default
	ShowWindow(hComboThickness,1);

#define ID_LIST_FUNKCJE 601
	hListFunkcje=CreateWindowEx(WS_EX_CLIENTEDGE,"LISTBOX","",WS_CHILD|WS_BORDER|WS_VSCROLL|LBS_NOTIFY,
	10,145,220,145,hOkno,(HMENU)ID_LIST_FUNKCJE,hInstance,0);
	SendMessage(hListFunkcje,WM_SETFONT,(WPARAM)hNormalFont,0);
	ShowWindow(hListFunkcje,1);

	wybranykolor=0x000000;
	return 0;
}

type int_to_type(int a){
    type typ;
    if(a==0){typ=CONSTANT;}
	if(a==1){typ=LINEAR;}
	if(a==2){typ=QUADRATIC;}
	if(a==3){typ=POLYNOMIAL;}
	if(a==4){typ=EXPONENTIAL;}
	if(a==5){typ=SUM;}
	if(a==6){typ=PRODUCT;}
	if(a==7){typ=SINE;}
	if(a==8){typ=COSINE;}
	if(a==9){typ=TANGENT;}
	if(a==10){typ=ARCSIN;}
	if(a==11){typ=ARCCOS;}
	if(a==12){typ=ARCTAN;}
	if(a==13){typ=SINE_H;}
	if(a==14){typ=COSINE_H;}
	if(a==15){typ=TANGENT_H;}
	if(a==16){typ=LOG;}
	if(a==17){typ=ROOT;}
	if(a==18){typ=POWER;}
	if(a==19){typ=FUNCTION_DERIVATIVE;}
	if(a==20){typ=FUNCTION_INTEGRAL;}
	if(a==21){typ=FUNCTION_COMPOSITION;}
	if(a==22){typ=CODE;}
	return typ;
}

string int_to_str(int a){
    stringstream ss;
    ss<<a;
    return ss.str();
}

char* int_to_cstr(int a){
    stringstream ss;
    ss<<a;
    return (char*)ss.str().c_str();
}

char* fn_int_to_cstr(fn_int a){
    stringstream ss;
    if(a.czy_funkcja==0){
        ss<<a.liczba;
    }
    else{
        if(a.numer==1100){
            ss<<"x";
        }
        else if(a.numer==1101){
            ss<<"y";
        }
        else{
            ss<<"f"<<a.numer;
        }
    }
    return (char*)ss.str().c_str();
}

vector<fn_int> str_to_vec(string wspolczynniki){
	stringstream ss;
	ss<<wspolczynniki;
	vector<fn_int> wspolczynniki_tab;
	string a;
	int czyplusrazy=1;
	while(ss>>a){
        fn_int fi;
        if(a[0]=='e'){
            fi.czy_funkcja=0;
            fi.liczba=M_E;
            fi.numer=czyplusrazy;
        }
        else if(a[0]=='p'&&a[1]=='i'){
            fi.czy_funkcja=0;
            fi.liczba=M_PI;
            fi.numer=czyplusrazy;
        }
        else if(a[0]=='f'&&a[1]>='0'&&a[1]<='9'){
            fi.czy_funkcja=1;
            stringstream ss2;
            ss2<<a.substr(1);
            ss2>>fi.numer;
            fi.liczba=czyplusrazy;
        }
        else if(a[0]>='0'&&a[0]<='9'||a[0]=='-'&&a.length()>1){
            fi.czy_funkcja=0;
            stringstream ss2;
            ss2<<a;
            ss2>>fi.liczba;
            fi.numer=czyplusrazy;
        }
        else if(a[0]=='x'){
            fi.czy_funkcja=1;
            fi.numer=1100;
            fi.liczba=czyplusrazy;
        }
        else if(a[0]=='y'){
            fi.czy_funkcja=1;
            fi.numer=1101;
            fi.liczba=czyplusrazy;
        }
        else if(a[0]=='-'||a[0]=='/'){
            czyplusrazy=0;
            continue;
        }
        else if(a[0]=='+'||a[0]=='*'){
            czyplusrazy=1;
            continue;
        }
        else{
            fi.liczba=0;
            fi.numer=czyplusrazy;
        }
		wspolczynniki_tab.push_back(fi);
	}
	return wspolczynniki_tab;
}

string parse_one_coeff(fn_int a){
    stringstream ss;
    if(a.czy_funkcja==0){
        ss<<a.liczba;
    }
    else{
        if(a.numer==1100){
            ss<<"x";
        }
        else if(a.numer==1101){
            ss<<"y";
        }
        else{
            ss<<"f"<<a.numer;
        }

    }
    return ss.str();
}

string func_to_str(funkcja f,int numer){
	string str;
	type typ=f.typ;
	vector<fn_int> wsp=f.wspolczynniki;
	stringstream ss2;
	ss2<<"f";
	ss2<<numer;
	ss2<<"=";
	if(typ==CONSTANT){
        ss2<<parse_one_coeff(wsp[0]);
	}
	else if(typ==LINEAR){
        string a=parse_one_coeff(wsp[0]);
        string b=parse_one_coeff(wsp[1]);
        if(a!="0"){
            if(a=="1"){
                ss2<<"x";
            }
            else if(a=="-1"){
                ss2<<"-x";
            }
            else{
                ss2<<a<<"x";
            }
            if(b[0]>='1'&&b[0]<='9'){
                ss2<<"+"<<b;
            }
            else if(b[0]=='-'){
                ss2<<b;
            }
        }
        else{
            ss2<<b;
        }
	}
	else if(typ==QUADRATIC){
        string a=parse_one_coeff(wsp[0]);
        string b=parse_one_coeff(wsp[1]);
        string c=parse_one_coeff(wsp[2]);
        if(a!="0"){
            if(a=="1"){
                ss2<<"x^2";
            }
            else if(a=="-1"){
                ss2<<"-x^2";
            }
            else{
                ss2<<a<<"x^2";
            }
            if(b=="1"){
                ss2<<"+x";
            }
            else if(b=="-1"){
                ss2<<"-x";
            }
            else if(b[0]>='1'&&b[0]<='9'){
                ss2<<"+"<<b<<"x";
            }
            else if(b[0]=='-'){
                ss2<<b<<"x";
            }
            if(c[0]>='1'&&c[0]<='9'){
                ss2<<"+"<<c;
            }
            else if(c[0]=='-'){
                ss2<<c;
            }
        }
        else{
            if(b!="0"){
                if(b=="1"){
                    ss2<<"x";
                }
                else if(b=="-1"){
                    ss2<<"-x";
                }
                else{
                    ss2<<b<<"x";
                }
                if(c[0]>='1'&&c[0]<='9'){
                    ss2<<"+"<<c;
                }
                else if(c[0]=='-'){
                    ss2<<c;
                }
            }
            else{
                ss2<<c;
            }
        }
	}
	else if(typ==POLYNOMIAL){
        int czyjuzbylo=0;
		for(int i=0;i<wsp.size();i++){
            string a=parse_one_coeff(wsp[i]);
            if(a!="0"){
                if(a[0]=='-'){
                    if(a=="-1"){
                        ss2<<"-";
                        if(i==wsp.size()-1){
                            ss2<<"1";
                        }
                    }
                    else{
                        ss2<<a;
                    }
                }
                else{
                    if(czyjuzbylo){
                        ss2<<"+";
                    }
                    if(a=="1"){
                        if(i==wsp.size()-1){
                            ss2<<"1";
                        }
                    }
                    else{
                        ss2<<a;
                    }
                }
                if(i!=wsp.size()-1){
                    ss2<<"x";
                    if(i!=wsp.size()-2){
                        ss2<<"^"<<(wsp.size()-i-1);
                    }
                }
                czyjuzbylo=1;
            }
		}
	}
	else if(typ==EXPONENTIAL){
        string a=parse_one_coeff(wsp[0]);
        string b=parse_one_coeff(wsp[1]);
        string c=parse_one_coeff(wsp[2]);
        if(a=="-1"){
            ss2<<"-";
        }
        else if(a!="1"){
            ss2<<a<<"*";
        }
        if(b=="2.71828"){
            ss2<<"e";
        }
        else{
            ss2<<b;
        }
        ss2<<"^(";
        if(c=="-1"){
            ss2<<"-";
        }
        else if(c!="1"){
            ss2<<c;
        }
        ss2<<"x)";
	}
	else if(typ==SUM){
        for(int i=0;i<wsp.size();i++){
            ss2<<parse_one_coeff(wsp[i]);
            if(i!=wsp.size()-1){
                if(wsp[i+1].czy_funkcja==0&&wsp[i+1].numer==0||wsp[i+1].czy_funkcja==1&&wsp[i+1].liczba==0){
                    ss2<<"-";
                }
                else{
                    ss2<<"+";
                }
            }
        }
	}
	else if(typ==PRODUCT){
        for(int i=0;i<wsp.size();i++){
            ss2<<parse_one_coeff(wsp[i]);
            if(i!=wsp.size()-1){
                if(wsp[i+1].czy_funkcja==0&&wsp[i+1].numer==0||wsp[i+1].czy_funkcja==1&&wsp[i+1].liczba==0){
                    ss2<<"/";
                }
                else{
                    ss2<<"*";
                }
            }
        }
	}
    else if(typ==SINE){
        string a=parse_one_coeff(wsp[0]);
        string b=parse_one_coeff(wsp[1]);
        string c=parse_one_coeff(wsp[2]);
        if(a=="-1"){
            ss2<<"-";
        }
        else if(a!="1"){
            ss2<<a<<"*";
        }
        ss2<<"sin(";
        if(b=="-1"){
            ss2<<"-";
        }
        else if(b!="1"){
            ss2<<b;
        }
        ss2<<"x";
        if(c!="0"){
            if(c[0]=='-'){
                ss2<<c;
            }
            else{
                ss2<<"+"<<c;
            }
        }
        ss2<<")";
	}
	else if(typ==COSINE){
        string a=parse_one_coeff(wsp[0]);
        string b=parse_one_coeff(wsp[1]);
        string c=parse_one_coeff(wsp[2]);
        if(a=="-1"){
            ss2<<"-";
        }
        else if(a!="1"){
            ss2<<a<<"*";
        }
        ss2<<"cos(";
        if(b=="-1"){
            ss2<<"-";
        }
        else if(b!="1"){
            ss2<<b;
        }
        ss2<<"x";
        if(c!="0"){
            if(c[0]=='-'){
                ss2<<c;
            }
            else{
                ss2<<"+"<<c;
            }
        }
        ss2<<")";
	}
	else if(typ==TANGENT){
        string a=parse_one_coeff(wsp[0]);
        string b=parse_one_coeff(wsp[1]);
        string c=parse_one_coeff(wsp[2]);
        if(a=="-1"){
            ss2<<"-";
        }
        else if(a!="1"){
            ss2<<a<<"*";
        }
        ss2<<"tan(";
        if(b=="-1"){
            ss2<<"-";
        }
        else if(b!="1"){
            ss2<<b;
        }
        ss2<<"x";
        if(c!="0"){
            if(c[0]=='-'){
                ss2<<c;
            }
            else{
                ss2<<"+"<<c;
            }
        }
        ss2<<")";
	}
	else if(typ==ARCSIN){
        string a=parse_one_coeff(wsp[0]);
        string b=parse_one_coeff(wsp[1]);
        string c=parse_one_coeff(wsp[2]);
        if(a=="-1"){
            ss2<<"-";
        }
        else if(a!="1"){
            ss2<<a<<"*";
        }
        ss2<<"arcsin(";
        if(b=="-1"){
            ss2<<"-";
        }
        else if(b!="1"){
            ss2<<b;
        }
        ss2<<"x)";
        if(c!="0"){
            if(c[0]=='-'){
                ss2<<c;
            }
            else{
                ss2<<"+"<<c;
            }
        }
	}
	else if(typ==ARCCOS){
        string a=parse_one_coeff(wsp[0]);
        string b=parse_one_coeff(wsp[1]);
        string c=parse_one_coeff(wsp[2]);
        if(a=="-1"){
            ss2<<"-";
        }
        else if(a!="1"){
            ss2<<a<<"*";
        }
        ss2<<"arccos(";
        if(b=="-1"){
            ss2<<"-";
        }
        else if(b!="1"){
            ss2<<b;
        }
        ss2<<"x)";
        if(c!="0"){
            if(c[0]=='-'){
                ss2<<c;
            }
            else{
                ss2<<"+"<<c;
            }
        }
	}
	else if(typ==ARCTAN){
        string a=parse_one_coeff(wsp[0]);
        string b=parse_one_coeff(wsp[1]);
        string c=parse_one_coeff(wsp[2]);
        if(a=="-1"){
            ss2<<"-";
        }
        else if(a!="1"){
            ss2<<a<<"*";
        }
        ss2<<"arctan(";
        if(b=="-1"){
            ss2<<"-";
        }
        else if(b!="1"){
            ss2<<b;
        }
        ss2<<"x)";
        if(c!="0"){
            if(c[0]=='-'){
                ss2<<c;
            }
            else{
                ss2<<"+"<<c;
            }
        }
	}
    else if(typ==SINE_H){
        string a=parse_one_coeff(wsp[0]);
        string b=parse_one_coeff(wsp[1]);
        string c=parse_one_coeff(wsp[2]);
        if(a=="-1"){
            ss2<<"-";
        }
        else if(a!="1"){
            ss2<<a<<"*";
        }
        ss2<<"sinh(";
        if(b=="-1"){
            ss2<<"-";
        }
        else if(b!="1"){
            ss2<<b;
        }
        ss2<<"x";
        if(c!="0"){
            if(c[0]=='-'){
                ss2<<c;
            }
            else{
                ss2<<"+"<<c;
            }
        }
        ss2<<")";
	}
	else if(typ==COSINE_H){
        string a=parse_one_coeff(wsp[0]);
        string b=parse_one_coeff(wsp[1]);
        string c=parse_one_coeff(wsp[2]);
        if(a=="-1"){
            ss2<<"-";
        }
        else if(a!="1"){
            ss2<<a<<"*";
        }
        ss2<<"cosh(";
        if(b=="-1"){
            ss2<<"-";
        }
        else if(b!="1"){
            ss2<<b;
        }
        ss2<<"x";
        if(c!="0"){
            if(c[0]=='-'){
                ss2<<c;
            }
            else{
                ss2<<"+"<<c;
            }
        }
        ss2<<")";
	}
	else if(typ==TANGENT_H){
        string a=parse_one_coeff(wsp[0]);
        string b=parse_one_coeff(wsp[1]);
        string c=parse_one_coeff(wsp[2]);
        if(a=="-1"){
            ss2<<"-";
        }
        else if(a!="1"){
            ss2<<a<<"*";
        }
        ss2<<"tanh(";
        if(b=="-1"){
            ss2<<"-";
        }
        else if(b!="1"){
            ss2<<b;
        }
        ss2<<"x";
        if(c!="0"){
            if(c[0]=='-'){
                ss2<<c;
            }
            else{
                ss2<<"+"<<c;
            }
        }
        ss2<<")";
	}
	else if(typ==LOG){
        string a=parse_one_coeff(wsp[0]);
        string b=parse_one_coeff(wsp[1]);
        if(a=="-1"){
            ss2<<"-";
        }
        else if(a!="1"){
            ss2<<a<<"*";
        }
        if(b=="2.71828"){
            ss2<<"ln x";
        }
        else{
            ss2<<"log_"<<b<<" x";
        }
	}
    else if(typ==ROOT){
        string a=parse_one_coeff(wsp[0]);
        ss2<<"x^(1/"<<a<<")";
	}
	else if(typ==POWER){
        string a=parse_one_coeff(wsp[0]);
        string b=parse_one_coeff(wsp[1]);
        if(a=="-1"){
            ss2<<"-";
        }
        else if(a!="1"){
            ss2<<a<<"*";
        }
        ss2<<"x^"<<b;
	}
	else if(typ==FUNCTION_DERIVATIVE){
        ss2<<"d/dx "<<parse_one_coeff(wsp[0]);
	}
	else if(typ==FUNCTION_INTEGRAL){
        ss2<<"integral of "<<parse_one_coeff(wsp[0]);
	}
	else if(typ==FUNCTION_COMPOSITION){
        ss2<<parse_one_coeff(wsp[0])<<"( "<<parse_one_coeff(wsp[1])<<"(x) )";
	}
	else if(typ==CODE){
        ss2<<"HARD-CODED FUNCTION";
	}
	return ss2.str();
}

double zwrocWartoscY(int,double,double);

double zwrocWartoscWsp(fn_int a,double arg,double y){
    if(a.czy_funkcja==0){
        return a.liczba;
    }
    return zwrocWartoscY(a.numer,arg,y);
}


double zwrocWartoscY(int numer,double arg,double y){
    if(funkcje.count(numer)==0){
        return 0;
    }
	funkcja f=funkcje[numer];
	vector <fn_int> w=f.wspolczynniki;
	if(f.typ==FN_Y){
        return y;
	}
	if(f.typ==CONSTANT){
		return zwrocWartoscWsp(w[0],arg,y);
	}
	else if(f.typ==LINEAR){
		return zwrocWartoscWsp(w[0],arg,y)*arg+zwrocWartoscWsp(w[1],arg,y);
	}
	else if(f.typ==QUADRATIC){
		return zwrocWartoscWsp(w[0],arg,y)*arg*arg+zwrocWartoscWsp(w[1],arg,y)*arg+zwrocWartoscWsp(w[2],arg,y);
	}
	else if(f.typ==POLYNOMIAL){
        double suma=0;
        double c=1;
        for(int i=w.size()-1;i>=0;i--){
            suma+=zwrocWartoscWsp(w[i],arg,y)*c;
            c*=arg;
        }
        return suma;
	}
	else if(f.typ==EXPONENTIAL){
        return zwrocWartoscWsp(w[0],arg,y)*pow(zwrocWartoscWsp(w[1],arg,y),zwrocWartoscWsp(w[2],arg,y)*arg);
	}
	else if(f.typ==SUM){
        double suma=0;
        for(int i=0;i<w.size();i++){
            if(w[i].czy_funkcja==0&&w[i].numer==0||w[i].czy_funkcja==1&&w[i].liczba==0){
                suma-=zwrocWartoscWsp(w[i],arg,y);
            }
            else{
                suma+=zwrocWartoscWsp(w[i],arg,y);
            }
        }
        return suma;
	}
	else if(f.typ==PRODUCT){
        double il=1;
        for(int i=0;i<w.size();i++){
            if(w[i].czy_funkcja==0&&w[i].numer==0||w[i].czy_funkcja==1&&w[i].liczba==0){
                il/=zwrocWartoscWsp(w[i],arg,y);
            }
            else{
                il*=zwrocWartoscWsp(w[i],arg,y);
            }
        }
        return il;
	}
    else if(f.typ==SINE){
        return zwrocWartoscWsp(w[0],arg,y)*sin(zwrocWartoscWsp(w[1],arg,y)*arg+zwrocWartoscWsp(w[2],arg,y));
	}
	else if(f.typ==COSINE){
        return zwrocWartoscWsp(w[0],arg,y)*cos(zwrocWartoscWsp(w[1],arg,y)*arg+zwrocWartoscWsp(w[2],arg,y));
	}
	else if(f.typ==TANGENT){
        return zwrocWartoscWsp(w[0],arg,y)*tan(zwrocWartoscWsp(w[1],arg,y)*arg+zwrocWartoscWsp(w[2],arg,y));
	}
	else if(f.typ==ARCSIN){
        if(zwrocWartoscWsp(w[1],arg,y)*arg>1||zwrocWartoscWsp(w[1],arg,y)*arg<-1){
            return 0;
        }
        return zwrocWartoscWsp(w[0],arg,y)*asin(zwrocWartoscWsp(w[1],arg,y)*arg)+zwrocWartoscWsp(w[2],arg,y);
	}
	else if(f.typ==ARCCOS){
        if(zwrocWartoscWsp(w[1],arg,y)*arg>1||zwrocWartoscWsp(w[1],arg,y)*arg<-1){
            return 0;
        }
        return zwrocWartoscWsp(w[0],arg,y)*acos(zwrocWartoscWsp(w[1],arg,y)*arg)+zwrocWartoscWsp(w[2],arg,y);
	}
	else if(f.typ==ARCTAN){
        return zwrocWartoscWsp(w[0],arg,y)*atan(zwrocWartoscWsp(w[1],arg,y)*arg)+zwrocWartoscWsp(w[2],arg,y);
	}
    else if(f.typ==SINE_H){
        return zwrocWartoscWsp(w[0],arg,y)*sinh(zwrocWartoscWsp(w[1],arg,y)*arg+zwrocWartoscWsp(w[2],arg,y));
	}
	else if(f.typ==COSINE_H){
        return zwrocWartoscWsp(w[0],arg,y)*cosh(zwrocWartoscWsp(w[1],arg,y)*arg+zwrocWartoscWsp(w[2],arg,y));
	}
	else if(f.typ==TANGENT_H){
        return zwrocWartoscWsp(w[0],arg,y)*tanh(zwrocWartoscWsp(w[1],arg,y)*arg+zwrocWartoscWsp(w[2],arg,y));
	}
	else if(f.typ==LOG){
	    if(zwrocWartoscWsp(w[1],arg,y)<=0||zwrocWartoscWsp(w[1],arg,y)==1||arg<=0){
            return 0;
	    }
        return zwrocWartoscWsp(w[0],arg,y)*log(arg)/log(zwrocWartoscWsp(w[1],arg,y));
	}
    else if(f.typ==ROOT){
        if(zwrocWartoscWsp(w[0],arg,y)*arg==0){
            return 0;
        }
        double t=pow(arg,1/zwrocWartoscWsp(w[0],arg,y));
        if(t==EDOM){
            return 0;
        }
        return t;
	}
	else if(f.typ==POWER){
        if(arg==0){
            return 0;
        }
        double t=pow(arg,zwrocWartoscWsp(w[1],arg,y));
        if(t==EDOM){
            return 0;
        }
        return zwrocWartoscWsp(w[0],arg,y)*t;
	}
	else if(f.typ==FUNCTION_DERIVATIVE){
        return (zwrocWartoscWsp(w[0],arg+zwrocWartoscWsp(w[1],arg,y),y)-
                zwrocWartoscWsp(w[0],arg-zwrocWartoscWsp(w[1],arg,y),y)) /2/zwrocWartoscWsp(w[1],arg,y);
	}
	else if(f.typ==FUNCTION_INTEGRAL){
        double suma=zwrocWartoscWsp(w[2],arg,y);
        double inc=zwrocWartoscWsp(w[1],arg,y);
        suma/=inc;
        if(arg>=0){
            for(double i=0;i<arg;i+=inc){
                suma+=zwrocWartoscWsp(w[0],i,y);
            }
            return suma*inc;
        }
        else{
            for(double i=0;i>arg;i-=inc){
                suma-=zwrocWartoscWsp(w[0],i,y);
            }
            return suma*inc;
        }
	}
	else if(f.typ==FUNCTION_COMPOSITION){
        return zwrocWartoscWsp(w[0],zwrocWartoscWsp(w[1],arg,y),y);
	}
	else if(f.typ==CODE){
        return 1/sin(arg);
	}

	return 0;
}

double zwrocWartosc(int numer,double arg){
    return zwrocWartoscY(numer,arg,0);

}
int zaznaczFunkcje(int numer){
	funkcja f=funkcje[numer];
	type typ=f.typ;
	string str=func_to_str(f,numer);
	SendMessage(hListFunkcje,LB_SELECTSTRING,0,(LPARAM)str.c_str());
	string typ_s;
	wybranykolor=f.kolor;
    nie_rysuj_wykresu=1;
	InvalidateRect(hOkno,&rect_kol,1);
	int zaznacz;
	if(f.czy_rysowac){
        zaznacz=BST_CHECKED;
	}
	else{
        zaznacz=BST_UNCHECKED;
	}
	CheckDlgButton(hOkno,ID_CHECKBOX_SHOW,zaznacz);

    SetWindowText(hTekstWspolA,"");
    SetWindowText(hTekstWspolB,"");
    SetWindowText(hTekstWspolC,"");
    SetWindowText(hTekstWspolD,"");
    SetWindowText(hTekstWspolE,"");
    SetWindowText(hTekstWspolWiele,"");
	if(typ==CONSTANT){
		typ_s="Constant A";
		SetWindowText(hTekstWspolA,fn_int_to_cstr(f.wspolczynniki[0]));
	}
	else if(typ==LINEAR){
		typ_s="Linear Ax+B";
		SetWindowText(hTekstWspolA,fn_int_to_cstr(f.wspolczynniki[0]));
		SetWindowText(hTekstWspolB,fn_int_to_cstr(f.wspolczynniki[1]));
	}
	else if(typ==QUADRATIC){
		typ_s="Quadratic Ax^2+Bx+C";
		SetWindowText(hTekstWspolA,fn_int_to_cstr(f.wspolczynniki[0]));
		SetWindowText(hTekstWspolB,fn_int_to_cstr(f.wspolczynniki[1]));
		SetWindowText(hTekstWspolC,fn_int_to_cstr(f.wspolczynniki[2]));
	}
	else if(f.typ==POLYNOMIAL){
		typ_s="Polynomial";
        stringstream ss;
        for(int i=0;i<f.wspolczynniki.size();i++){
            ss<<fn_int_to_cstr(f.wspolczynniki[i])<<" ";
        }
        SetWindowText(hTekstWspolWiele,ss.str().c_str());
	}
	else if(f.typ==EXPONENTIAL){
		typ_s="Exponential A*B^(Cx)";
		SetWindowText(hTekstWspolA,fn_int_to_cstr(f.wspolczynniki[0]));
		SetWindowText(hTekstWspolB,fn_int_to_cstr(f.wspolczynniki[1]));
		SetWindowText(hTekstWspolC,fn_int_to_cstr(f.wspolczynniki[2]));
	}
	else if(f.typ==SUM){
		typ_s="Sum or difference";
		stringstream str;
		if(f.wspolczynniki.size()>0){
            str<<fn_int_to_cstr(f.wspolczynniki[0]);
            MessageBox(hOkno,"",str.str().c_str(),0);
		}
        for(int i=1;i<f.wspolczynniki.size();i++){
            if(f.wspolczynniki[i].czy_funkcja==0&&f.wspolczynniki[i].numer==0||
               f.wspolczynniki[i].czy_funkcja==1&&f.wspolczynniki[i].liczba==0){
                str<<" - "<<fn_int_to_cstr(f.wspolczynniki[i]);
            }
            else{
                str<<" + "<<fn_int_to_cstr(f.wspolczynniki[i]);
            }
        }
        SetWindowText(hTekstWspolWiele,str.str().c_str());
	}
	else if(f.typ==PRODUCT){
		typ_s="Product or quotient";
		stringstream str;
		if(f.wspolczynniki.size()>0){
            str<<fn_int_to_cstr(f.wspolczynniki[0]);
            MessageBox(hOkno,"",str.str().c_str(),0);
		}
        for(int i=1;i<f.wspolczynniki.size();i++){
            if(f.wspolczynniki[i].czy_funkcja==0&&f.wspolczynniki[i].numer==0||
               f.wspolczynniki[i].czy_funkcja==1&&f.wspolczynniki[i].liczba==0){
                str<<" / "<<fn_int_to_cstr(f.wspolczynniki[i]);
            }
            else{
                str<<" * "<<fn_int_to_cstr(f.wspolczynniki[i]);
            }
        }
        SetWindowText(hTekstWspolWiele,str.str().c_str());
	}
    else if(f.typ==SINE){
		typ_s="Sine A*sin(Bx+C)";
		SetWindowText(hTekstWspolA,fn_int_to_cstr(f.wspolczynniki[0]));
		SetWindowText(hTekstWspolB,fn_int_to_cstr(f.wspolczynniki[1]));
		SetWindowText(hTekstWspolC,fn_int_to_cstr(f.wspolczynniki[2]));
	}
	else if(f.typ==COSINE){
		typ_s="Cosine A*cos(Bx+C)";
		SetWindowText(hTekstWspolA,fn_int_to_cstr(f.wspolczynniki[0]));
		SetWindowText(hTekstWspolB,fn_int_to_cstr(f.wspolczynniki[1]));
		SetWindowText(hTekstWspolC,fn_int_to_cstr(f.wspolczynniki[2]));
	}
	else if(f.typ==TANGENT){
		typ_s="Tangent A*tan(Bx+C";
		SetWindowText(hTekstWspolA,fn_int_to_cstr(f.wspolczynniki[0]));
		SetWindowText(hTekstWspolB,fn_int_to_cstr(f.wspolczynniki[1]));
		SetWindowText(hTekstWspolC,fn_int_to_cstr(f.wspolczynniki[2]));
	}
	else if(f.typ==ARCSIN){
		typ_s="Arcsin A*arcsin(Bx)+C";
		SetWindowText(hTekstWspolA,fn_int_to_cstr(f.wspolczynniki[0]));
		SetWindowText(hTekstWspolB,fn_int_to_cstr(f.wspolczynniki[1]));
		SetWindowText(hTekstWspolC,fn_int_to_cstr(f.wspolczynniki[2]));
	}
	else if(f.typ==ARCCOS){
		typ_s="Arccos A*arccos(Bx)+C";
		SetWindowText(hTekstWspolA,fn_int_to_cstr(f.wspolczynniki[0]));
		SetWindowText(hTekstWspolB,fn_int_to_cstr(f.wspolczynniki[1]));
		SetWindowText(hTekstWspolC,fn_int_to_cstr(f.wspolczynniki[2]));
	}
	else if(f.typ==ARCTAN){
		typ_s="Arctan A*arctan(Bx)+C";
		SetWindowText(hTekstWspolA,fn_int_to_cstr(f.wspolczynniki[0]));
		SetWindowText(hTekstWspolB,fn_int_to_cstr(f.wspolczynniki[1]));
		SetWindowText(hTekstWspolC,fn_int_to_cstr(f.wspolczynniki[2]));
	}
    else if(f.typ==SINE_H){
		typ_s="Hyperbolic sine A*sinh(Bx+C)";
		SetWindowText(hTekstWspolA,fn_int_to_cstr(f.wspolczynniki[0]));
		SetWindowText(hTekstWspolB,fn_int_to_cstr(f.wspolczynniki[1]));
		SetWindowText(hTekstWspolC,fn_int_to_cstr(f.wspolczynniki[2]));
	}
	else if(f.typ==COSINE_H){
		typ_s="Hyperbolic cosine A*cosh(Bx+C)";
		SetWindowText(hTekstWspolA,fn_int_to_cstr(f.wspolczynniki[0]));
		SetWindowText(hTekstWspolB,fn_int_to_cstr(f.wspolczynniki[1]));
		SetWindowText(hTekstWspolC,fn_int_to_cstr(f.wspolczynniki[2]));
	}
	else if(f.typ==TANGENT_H){
		typ_s="Hyperbolic tangent A*tanh(Bx+C)";
		SetWindowText(hTekstWspolA,fn_int_to_cstr(f.wspolczynniki[0]));
		SetWindowText(hTekstWspolB,fn_int_to_cstr(f.wspolczynniki[1]));
		SetWindowText(hTekstWspolC,fn_int_to_cstr(f.wspolczynniki[2]));
	}
	else if(f.typ==LOG){
		typ_s="Logarithm A*log_B x";
		SetWindowText(hTekstWspolA,fn_int_to_cstr(f.wspolczynniki[0]));
		SetWindowText(hTekstWspolB,fn_int_to_cstr(f.wspolczynniki[1]));
	}
    else if(f.typ==ROOT){
		typ_s="Root x^(1/A)";
		SetWindowText(hTekstWspolA,fn_int_to_cstr(f.wspolczynniki[0]));
	}
	else if(f.typ==POWER){
		typ_s="Power A*x^B";
		SetWindowText(hTekstWspolA,fn_int_to_cstr(f.wspolczynniki[0]));
		SetWindowText(hTekstWspolB,fn_int_to_cstr(f.wspolczynniki[1]));
	}
	else if(f.typ==FUNCTION_DERIVATIVE){
		typ_s="Derivative of function";
		SetWindowText(hTekstWspolA,fn_int_to_cstr(f.wspolczynniki[0]));
		SetWindowText(hTekstWspolB,fn_int_to_cstr(f.wspolczynniki[1]));
	}
	else if(f.typ==FUNCTION_INTEGRAL){
		typ_s="Integral of function";
		SetWindowText(hTekstWspolA,fn_int_to_cstr(f.wspolczynniki[0]));
		SetWindowText(hTekstWspolB,fn_int_to_cstr(f.wspolczynniki[1]));
		SetWindowText(hTekstWspolC,fn_int_to_cstr(f.wspolczynniki[2]));
	}
	else if(f.typ==FUNCTION_COMPOSITION){
		typ_s="Function composition f(g(x))";
		SetWindowText(hTekstWspolA,fn_int_to_cstr(f.wspolczynniki[0]));
		SetWindowText(hTekstWspolB,fn_int_to_cstr(f.wspolczynniki[1]));
	}
	else if(f.typ==CODE){
		typ_s="Hard-coded function";
	}

	SendMessage(hComboFunkcje,CB_SELECTSTRING,0,(LPARAM)typ_s.c_str());
#define NO_COEFF 1337
    SendMessage(hOkno,WM_COMMAND,MAKEWPARAM(NO_COEFF,CBN_SELCHANGE),(LPARAM)hComboFunkcje);
	int th=f.thickness;
	SendMessage(hComboThickness,CB_SELECTSTRING,0,(LPARAM)int_to_cstr(th));
	SetFocus(hOkno);
	return 0;
}

int dodajFunkcje(int numer,type typ,int czy_rysowac,COLORREF kolor,int thickness,string wspolczynniki){
	//uzywac z numerem=maks+1
	numery_funkcji.push_back(numer);
	vector<fn_int> wsp=str_to_vec(wspolczynniki);
	funkcja f;
	f.typ=typ;
	f.czy_rysowac=czy_rysowac;
	f.kolor=kolor;
	f.thickness=thickness;
	f.wspolczynniki=wsp;
	funkcje[numer]=f;
	string str=func_to_str(f,numer);
	SendMessage(hListFunkcje,LB_ADDSTRING,0,(LPARAM)str.c_str());
	InvalidateRect(hListFunkcje,NULL,1);
	return 0;
}

int dodajFunkcje(){
	int numer;
	if(numery_funkcji.size()==0){
        numer=1;
	}
	else{
        numer=numery_funkcji[numery_funkcji.size()-1]+1;
	}
	if(numer==1100||numer==1101){
        numer=1102;
	}
	type typ=CONSTANT;
    nie_rysuj_wykresu=1;
	return dodajFunkcje(numer,typ,1,0x000000,1,"0");
}

int sprawdzZapetlenie(int numer,vector<fn_int>wsp){
    queue <int> kolejka;//numery funkcji
    for(int i=0;i<wsp.size();i++){
        if(wsp[i].czy_funkcja){
            kolejka.push(wsp[i].numer);
        }
    }
    while(!kolejka.empty()){
        int nr=kolejka.front();
        kolejka.pop();
        if(nr==numer){
            return 1;//zapetlenie
        }
        for(int i=0;i<funkcje[nr].wspolczynniki.size();i++){
            if(funkcje[nr].wspolczynniki[i].czy_funkcja){
                kolejka.push(funkcje[nr].wspolczynniki[i].numer);
            }
        }
    }
    return 0;
}

int edytujFunkcje(int numer,type typ,int czy_rysowac,COLORREF kolor,int thickness,string wspolczynniki){
	//edytuje funkcje o danym numerze
	vector<fn_int> wsp=str_to_vec(wspolczynniki);
	if(sprawdzZapetlenie(numer,wsp)){
        MessageBox(hOkno,"Function loop!","Error!",0);
        return 1;
	}
	funkcja f;
	f.typ=typ;
	f.czy_rysowac=czy_rysowac;
	f.kolor=kolor;
	f.thickness=thickness;
	f.wspolczynniki=wsp;
	funkcje[numer]=f;
	string str=func_to_str(f,numer);
	int lp;
	for(int i=0;i<numery_funkcji.size();i++){
		if(numery_funkcji[i]==numer){
			lp=i;
			break;
		}
	}
	SendMessage(hListFunkcje,LB_DELETESTRING,lp,0);
	SendMessage(hListFunkcje,LB_INSERTSTRING,lp,(LPARAM)str.c_str());
	SendMessage(hListFunkcje,LB_SELECTSTRING,0,(LPARAM)str.c_str());
	InvalidateRect(hListFunkcje,NULL,1);
	return 0;
}

int edytujFunkcjeTry(int numer,type typ,int czy_rysowac,COLORREF kolor,int thickness,string wspolczynniki){
	if(funkcje.count(numer)==1){
		return edytujFunkcje(numer,typ,czy_rysowac,kolor,thickness,wspolczynniki);
	}
	MessageBox(hOkno,"No function or wrong function selected!","Error!",0);
	return 1;
}

int usunFunkcje(int numer){
	funkcje.erase(numer);
	for(int i=0;i<numery_funkcji.size();i++){
		if(numery_funkcji[i]==numer){
			numery_funkcji.erase(numery_funkcji.begin()+i);
			SendMessage(hListFunkcje,LB_DELETESTRING,i,0);
		}
	}
	InvalidateRect(hOkno,&rect_w,1);
	return 0;
}

int zmienNumer(int a,int b){ //zamienia numer funkcji z a na b
	if(a==b){
		return 0;
	}
	funkcje[b]=funkcje[a];
	funkcje.erase(a);
	int lp,lp2=-1;
	for(int i=0;i<numery_funkcji.size();i++){
		if(numery_funkcji[i]==a){
			lp=i;
			numery_funkcji[i]=b;
		}
	}
	sort(numery_funkcji.begin(),numery_funkcji.end());
	for(int i=0;i<numery_funkcji.size();i++){
		if(numery_funkcji[i]==b){
			lp2=i;
		}
	}
	string str=func_to_str(funkcje[b],b);
	SendMessage(hListFunkcje,LB_DELETESTRING,lp,0);
	SendMessage(hListFunkcje,LB_INSERTSTRING,lp2,(LPARAM)str.c_str());
	SendMessage(hListFunkcje,LB_SELECTSTRING,0,(LPARAM)str.c_str());
	InvalidateRect(hListFunkcje,NULL,1);
	return 0;
}

int zmienNumerTry(int a,int b){
	if(b<0){
		MessageBox(hOkno,"Function number cannot be negative!","Error!",0);
		return 1;
	}
	if(b==1101||b==1102){
        MessageBox(hOkno,"Reserved number!","Error!",0);
        return 1;
	}
	int czy_jest_a=0,czy_jest_b=0;
	for(int i=0;i<numery_funkcji.size();i++){
		if(numery_funkcji[i]==a){
			czy_jest_a=1;
		}
		if(numery_funkcji[i]==b){
			czy_jest_b=1;
		}
	}
	if(czy_jest_b){
		stringstream ss;
		ss<<"Function f"<<b<<" already exists!";
		MessageBox(hOkno,ss.str().c_str(),"Error!",0);
		return 1;
	}
	if(!czy_jest_a){
		MessageBox(hOkno,"Wrong function or no function selected!","Error!",0);
		return 1;
	}
	return zmienNumer(a,b);
}

int x_do_ekranu(double x){
	int a=(x-bok_lewo)/(bok_prawo-bok_lewo)*width+lewy_naroznik;
	if(a>lewy_naroznik+width){
		return lewy_naroznik+width;
	}
	if(a<lewy_naroznik){
		return lewy_naroznik;
	}
	return a;
}
int y_do_ekranu(double y){
	int a=(bok_gora-y)/(bok_gora-bok_dol)*height+gorny_naroznik;
	if(a>gorny_naroznik+height){
		return gorny_naroznik+height-1;
	}
	if(a<gorny_naroznik){
		return gorny_naroznik;
	}
	return a;
}
double x_z_ekranu(int x){
	return ((double)x-lewy_naroznik)/width*(bok_prawo-bok_lewo)+bok_lewo;
}
double y_z_ekranu(int y){
	return ((double)y-gorny_naroznik)/height*(bok_dol-bok_gora)+bok_gora;
}

int RysujFunkcje(int numer,HDC hdcOkno){
	funkcja f=funkcje[numer];
	COLORREF kolor=f.kolor;
	int thickness=f.thickness;
	HPEN stary,nowy;
	nowy=CreatePen(PS_SOLID,thickness,kolor);
	stary=(HPEN)SelectObject(hdcOkno,nowy);
	POINT smiec;
	if(f.typ!=FUNCTION_INTEGRAL){
        double wartosc_funkcji=zwrocWartosc(numer,bok_lewo);
        int y=y_do_ekranu(wartosc_funkcji);
        MoveToEx(hdcOkno,lewy_naroznik,y,&smiec);
        for(int x=lewy_naroznik+1;x<lewy_naroznik+width;x++){
            double x_fn=x_z_ekranu(x);
            wartosc_funkcji=zwrocWartosc(numer,x_fn);
            int y=y_do_ekranu(wartosc_funkcji);
            LineTo(hdcOkno,x,y);
        }
	}
	else{
        double inc=f.wspolczynniki[1].liczba;
        fn_int fn=f.wspolczynniki[0];
        double suma=f.wspolczynniki[2].liczba;
        MoveToEx(hdcOkno,x_do_ekranu(0),y_do_ekranu(suma),&smiec);
        for(double x=0;x<bok_prawo;x+=inc){
            suma+=inc*zwrocWartoscWsp(fn,x,0);
            LineTo(hdcOkno,x_do_ekranu(x),y_do_ekranu(suma));
        }
        suma=f.wspolczynniki[2].liczba;
        MoveToEx(hdcOkno,x_do_ekranu(0),y_do_ekranu(suma),&smiec);
        for(double x=0;x>bok_lewo;x-=inc){
            suma-=inc*zwrocWartoscWsp(fn,x,0);
            LineTo(hdcOkno,x_do_ekranu(x),y_do_ekranu(suma));
        }
	}
	SelectObject(hdcOkno,stary);
	DeleteObject(nowy);
	return 0;
}

int RysujKrzywa(HDC hdcOkno){
	HPEN stary,nowy;
	nowy=CreatePen(PS_SOLID,1,0x00ff00);
	stary=(HPEN)SelectObject(hdcOkno,nowy);
	POINT smiec;
	double x0=0;
	double y0=1;
	double x=x0,y=y0;
	MoveToEx(hdcOkno,x_do_ekranu(x),y_do_ekranu(y),&smiec);
	double dydx=zwrocWartoscY(0,x,y);
	stringstream ss;
	ss<<dydx;
	while(x<bok_prawo){
        double dydx=1/(2*x+y)+2*x+y-2;//zwrocWartoscY(0,x,y);
        x+=0.001;
        y+=dydx*0.001;
        LineTo(hdcOkno,x_do_ekranu(x),y_do_ekranu(y));
	}
	x=x0,y=y0;
	MoveToEx(hdcOkno,x_do_ekranu(x),y_do_ekranu(y),&smiec);
	while(x>bok_lewo){
        double dydx=1/(2*x+y)+2*x+y-2;//zwrocWartoscY(0,x,y);
        x-=0.001;
        y-=dydx*0.001;
        LineTo(hdcOkno,x_do_ekranu(x),y_do_ekranu(y));
	}
	SelectObject(hdcOkno,stary);
	DeleteObject(nowy);
	return 0;
}

int RysujWykres(HDC hdcOkno){
	HBRUSH Pedzel,PedzelDom;
	HPEN Pen,PenDom;
	POINT smiec;
	Pedzel=CreateSolidBrush(0xFFFFFF);
	Pen=CreatePen(PS_SOLID,1,0x000000);
	PedzelDom=(HBRUSH)SelectObject(hdcOkno,Pedzel);
	PenDom=(HPEN)SelectObject(hdcOkno,Pen);
	//pole
	Rectangle(hdcOkno,lewy_naroznik,gorny_naroznik,lewy_naroznik+width,gorny_naroznik+height);
	//podzialki - todo
	//osie
	int x_os=x_do_ekranu(0);// wspolrzedna x-owa na ekranie osi y
	int y_os=y_do_ekranu(0);// vice versa
	MoveToEx(hdcOkno,x_os,gorny_naroznik,&smiec);
	LineTo(hdcOkno,x_os,gorny_naroznik+height);
	MoveToEx(hdcOkno,lewy_naroznik,y_os,&smiec);
	LineTo(hdcOkno,lewy_naroznik+width,y_os);
	//funkcje
	for(map<int,funkcja>::iterator i=funkcje.begin();i!=funkcje.end();i++){
        if(funkcje[i->first].czy_rysowac==1){
            RysujFunkcje(i->first,hdcOkno);
        }

	}
	//RysujKrzywa(hdcOkno);
	SelectObject(hdcOkno,PedzelDom);
	SelectObject(hdcOkno,PenDom);
	DeleteObject(Pedzel);
	DeleteObject(Pen);
	return 0;
}

int SetTextPolozenie(){
	stringstream ss;
	ss.precision(5);
	ss<<bok_lewo<<" "<<bok_prawo<<" "<<bok_gora<<" "<<bok_dol<<" "<<(bok_gora+bok_dol)/2<<" ";
	ss<<(bok_lewo+bok_prawo)/2<<" "<<(bok_gora-bok_dol)<<" "<<(bok_prawo-bok_lewo);
	string l,p,g,d,cy,cx,sy,sx;
	ss>>l>>p>>g>>d>>cy>>cx>>sy>>sx;
	SetWindowText(hTekstRight,p.c_str());
	SetWindowText(hTekstLeft,l.c_str());
	SetWindowText(hTekstBottom,d.c_str());
	SetWindowText(hTekstTop,g.c_str());
	SetWindowText(hTekstCenterX,cx.c_str());
	SetWindowText(hTekstCenterY,cy.c_str());
	SetWindowText(hTekstSizeX,sx.c_str());
	SetWindowText(hTekstSizeY,sy.c_str());
	return 0;
}

int SetDefaultCoefficients(type typ){
    string a,b,c,d,e,f,w;
    if(typ==CONSTANT){
        a="0";
    }
    else if(typ==LINEAR){
        a="1";
        b="0";
    }
    else if(typ==QUADRATIC){
        a="1";
        b="0";
        c="0";
    }
    else if(typ==POLYNOMIAL){
        w="1 0 0 0";
    }
	else if(typ==EXPONENTIAL){
        a="1";
        b="e";
        c="1";
	}
	else if(typ==SUM){
	}
	else if(typ==PRODUCT){
	}
    else if(typ==SINE){
        a="1";
        b="1";
        c="0";
	}
	else if(typ==COSINE){
        a="1";
        b="1";
        c="0";
	}
	else if(typ==TANGENT){
        a="1";
        b="1";
        c="0";
	}
	else if(typ==ARCSIN){
        a="1";
        b="1";
        c="0";
	}
	else if(typ==ARCCOS){
        a="1";
        b="1";
        c="0";
	}
	else if(typ==ARCTAN){
        a="1";
        b="1";
        c="0";
	}
    else if(typ==SINE_H){
        a="1";
        b="1";
        c="0";
	}
	else if(typ==COSINE_H){
        a="1";
        b="1";
        c="0";
	}
	else if(typ==TANGENT_H){
        a="1";
        b="1";
        c="0";
	}
	else if(typ==LOG){
        a="1";
        b="e";
	}
    else if(typ==ROOT){
        a="2";
	}
	else if(typ==POWER){
	    a="1";
        b="2";
	}
	else if(typ==FUNCTION_DERIVATIVE){
	    a="f0";
        b="0.01";
	}
	else if(typ==FUNCTION_INTEGRAL){
	    a="f0";
        b="0.01";
        c="0";
	}
	else if(typ==FUNCTION_COMPOSITION){
        a="f0";
        b="f1";
	}
	else if(typ==CODE){
	}

    SetWindowText(hTekstWspolA,a.c_str());
    SetWindowText(hTekstWspolB,b.c_str());
    SetWindowText(hTekstWspolC,c.c_str());
    SetWindowText(hTekstWspolD,d.c_str());
    SetWindowText(hTekstWspolE,e.c_str());
    SetWindowText(hTekstWspolWiele,w.c_str());
	return 0;
}

int ZmienWielkoscRolka(int rolka,int x_e,int y_e){
	double num=(double)rolka/WHEEL_DELTA;
	double fctr=pow(factor2,num);
	if(rolka<0){
		fctr=pow(1/factor2,-num);
	}
	double x_w=x_z_ekranu(x_e);
	double y_w=y_z_ekranu(y_e);
	double lb=x_w-(x_w-bok_lewo)/fctr;
	double pb=x_w+(bok_prawo-x_w)/fctr;
	double gb=y_w+(bok_gora-y_w)/fctr;
	double db=y_w-(y_w-bok_dol)/fctr;
	bok_dol=db;
	bok_gora=gb;
	bok_lewo=lb;
	bok_prawo=pb;
	InvalidateRect(hOkno,&rect_w,1);
	SetTextPolozenie();
	return 0;
}

int PrzesunWykres(int dx,int dy){
	double x=dx*(bok_prawo-bok_lewo)/width;
	bok_prawo-=x;
	bok_lewo-=x;
	double y=dy*(bok_gora-bok_dol)/height;
	bok_gora+=y;
	bok_dol+=y;
	InvalidateRect(hOkno,&rect_w,1);
	SetTextPolozenie();
	return 0;
}

int RysujKropkeZKolorem(HDC hdcOkno){
	HBRUSH Pedzel,PedzelDom;
	HPEN Pen,PenDom;
	Pedzel=CreateSolidBrush(wybranykolor);
	Pen=CreatePen(PS_SOLID,2,0xbbbbbb);
	PedzelDom=(HBRUSH)SelectObject(hdcOkno,Pedzel);
	PenDom=(HPEN)SelectObject(hdcOkno,Pen);
	Rectangle(hdcOkno,120,102,140,122);
	SelectObject(hdcOkno,PedzelDom);
	SelectObject(hdcOkno,PenDom);
	DeleteObject(Pedzel);
	DeleteObject(Pen);
	return 0;
}







