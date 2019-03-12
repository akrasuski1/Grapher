#pragma comment(linker,"/manifestdependency:\"type='win32' "\
                   "name='Microsoft.Windows.Common-Controls' "\
                   "version='6.0.0.0' "\
                   "processorArchitecture='x86' "\
                   "publicKeyToken='6595b64144ccf1df' "\
                   "language='*' "\
                   "\"")
#include <windows.h>
#include "kontrolki10.hpp"
#include <vector>


//todo:
// podzialka
// krzywe
// integral max min itd
// dane z pliku i fitting
// zeby lepiej rysowac integrale i takie tam





LRESULT CALLBACK WndProc(HWND hWindow, UINT Msg, WPARAM wParam, LPARAM lParam) {
	if(Msg==WM_CLOSE){
		DestroyWindow(hWindow);
	}
	else if(Msg==WM_DESTROY){
		PostQuitMessage(0);
	}
	else if(Msg==WM_PAINT){
		PAINTSTRUCT ps;
		HDC hdcOkno=BeginPaint(hOkno,&ps);


		RysujKropkeZKolorem(hdcOkno);
		if(!nie_rysuj_wykresu){
            RysujWykres(hdcOkno);
		}
        nie_rysuj_wykresu=0;

		EndPaint(hOkno,&ps);
	}
	else if(Msg==WM_MOUSEWHEEL){
		POINT p;
		GetCursorPos(&p);
		ScreenToClient(hOkno,&p);
		int x=p.x;
		int y=p.y;
		if(y>=gorny_naroznik&&y<=gorny_naroznik+height&&x>=lewy_naroznik&&x<=lewy_naroznik+width){
			ZmienWielkoscRolka(GET_WHEEL_DELTA_WPARAM(wParam),x,y);
		}
	}
	else if(Msg==WM_LBUTTONDOWN){
		POINT p;
		GetCursorPos(&p);
		ScreenToClient(hOkno,&p);
		int x=p.x;
		int y=p.y;
		if(y>=gorny_naroznik&&y<=gorny_naroznik+height&&x>=lewy_naroznik&&x<=lewy_naroznik+width){
			przesuwanie_wykresu_w_toku=1;
			SetCapture(hOkno);
		}
		x_old=x;
		y_old=y;
	}
	else if(Msg==WM_LBUTTONUP){
		if(przesuwanie_wykresu_w_toku==1){
			przesuwanie_wykresu_w_toku=0;
			ReleaseCapture();
		}
	}
	else if(Msg==WM_MOUSEMOVE){
		if(przesuwanie_wykresu_w_toku==1){
			POINT p;
			GetCursorPos(&p);
			ScreenToClient(hOkno,&p);
			int x=p.x;
			int y=p.y;
			PrzesunWykres(x-x_old,y-y_old);
			x_old=x;
			y_old=y;
		}
	}
	else if(Msg==WM_COMMAND){
        if((HWND)lParam==hComboFunkcje&&HIWORD(wParam)==CBN_SELCHANGE){
            type typ=int_to_type(SendMessage(hComboFunkcje,CB_GETCURSEL,0,0));

            if(typ==POLYNOMIAL||typ==SUM||typ==PRODUCT){
                nie_rysuj_wykresu=1;
                ShowWindow(hTekstWspolA,0);
                nie_rysuj_wykresu=1;
                ShowWindow(hTekstWspolB,0);
                nie_rysuj_wykresu=1;
                ShowWindow(hTekstWspolC,0);
                nie_rysuj_wykresu=1;
                ShowWindow(hTekstWspolD,0);
                nie_rysuj_wykresu=1;
                ShowWindow(hTekstWspolE,0);
                nie_rysuj_wykresu=1;
                ShowWindow(hTekstWspolWiele,1);
                nie_rysuj_wykresu=1;
                ShowWindow(hNapisWspolA,0);
                nie_rysuj_wykresu=1;
                ShowWindow(hNapisWspolB,0);
                nie_rysuj_wykresu=1;
                ShowWindow(hNapisWspolC,0);
                nie_rysuj_wykresu=1;
                ShowWindow(hNapisWspolD,0);
                nie_rysuj_wykresu=1;
                ShowWindow(hNapisWspolE,0);
                nie_rysuj_wykresu=1;
                ShowWindow(hNapisWspolWiele,1);
                nie_rysuj_wykresu=1;
                ShowWindow(hNapisWspolWielePlus,1);
                if(typ==POLYNOMIAL){
                    nie_rysuj_wykresu=1;
                    ShowWindow(hNapisWspolWielePlus,0);
                }
                nie_rysuj_wykresu=1;
                ShowWindow(hNapisWspolFunkcja,0);
                nie_rysuj_wykresu=1;
                ShowWindow(hNapisWspolStep,0);
                nie_rysuj_wykresu=1;
                ShowWindow(hNapisWspolStala,0);
            }
            else if(typ==FUNCTION_DERIVATIVE||typ==FUNCTION_INTEGRAL){
                nie_rysuj_wykresu=1;
                ShowWindow(hTekstWspolA,1);
                nie_rysuj_wykresu=1;
                ShowWindow(hTekstWspolB,1);
                nie_rysuj_wykresu=1;
                ShowWindow(hTekstWspolC,1);
                nie_rysuj_wykresu=1;
                ShowWindow(hTekstWspolD,1);
                nie_rysuj_wykresu=1;
                ShowWindow(hTekstWspolE,1);
                nie_rysuj_wykresu=1;
                ShowWindow(hTekstWspolWiele,0);
                nie_rysuj_wykresu=1;
                ShowWindow(hNapisWspolA,0);
                nie_rysuj_wykresu=1;
                ShowWindow(hNapisWspolB,0);
                nie_rysuj_wykresu=1;
                ShowWindow(hNapisWspolC,1);
                nie_rysuj_wykresu=1;
                ShowWindow(hNapisWspolD,1);
                nie_rysuj_wykresu=1;
                ShowWindow(hNapisWspolE,1);
                nie_rysuj_wykresu=1;
                ShowWindow(hNapisWspolWiele,0);
                nie_rysuj_wykresu=1;
                ShowWindow(hNapisWspolWielePlus,0);
                nie_rysuj_wykresu=1;
                ShowWindow(hNapisWspolFunkcja,1);
                nie_rysuj_wykresu=1;
                ShowWindow(hNapisWspolStep,1);
                if(typ==FUNCTION_INTEGRAL){
                    nie_rysuj_wykresu=1;
                    ShowWindow(hNapisWspolStala,1);
                }
                else{
                    nie_rysuj_wykresu=1;
                    ShowWindow(hNapisWspolStala,0);
                }
            }
            else{
                nie_rysuj_wykresu=1;
                ShowWindow(hTekstWspolA,1);
                nie_rysuj_wykresu=1;
                ShowWindow(hTekstWspolB,1);
                nie_rysuj_wykresu=1;
                ShowWindow(hTekstWspolC,1);
                nie_rysuj_wykresu=1;
                ShowWindow(hTekstWspolD,1);
                nie_rysuj_wykresu=1;
                ShowWindow(hTekstWspolE,1);
                nie_rysuj_wykresu=1;
                ShowWindow(hTekstWspolWiele,0);
                nie_rysuj_wykresu=1;
                ShowWindow(hNapisWspolA,1);
                nie_rysuj_wykresu=1;
                ShowWindow(hNapisWspolB,1);
                nie_rysuj_wykresu=1;
                ShowWindow(hNapisWspolC,1);
                nie_rysuj_wykresu=1;
                ShowWindow(hNapisWspolD,1);
                nie_rysuj_wykresu=1;
                ShowWindow(hNapisWspolE,1);
                nie_rysuj_wykresu=1;
                ShowWindow(hNapisWspolWiele,0);
                nie_rysuj_wykresu=1;
                ShowWindow(hNapisWspolWielePlus,0);
                nie_rysuj_wykresu=1;
                ShowWindow(hNapisWspolFunkcja,0);
                nie_rysuj_wykresu=1;
                ShowWindow(hNapisWspolStep,0);
                nie_rysuj_wykresu=1;
                ShowWindow(hNapisWspolStala,0);
            }
            if(LOWORD(wParam)!=NO_COEFF){
                SetDefaultCoefficients(typ);
            }
        }
		if(wParam==ID_RADIO_LEFT){
			CheckRadioButton(hOkno,ID_RADIO_LEFT,ID_RADIO_RIGHT,ID_RADIO_LEFT);
		}
		if(wParam==ID_RADIO_RIGHT){
			CheckRadioButton(hOkno,ID_RADIO_LEFT,ID_RADIO_RIGHT,ID_RADIO_RIGHT);
		}
		if(wParam==ID_CHECKBOX_SHOW){
			int zaznacz;
            int index=SendMessage(hListFunkcje,LB_GETCURSEL,0,0);
            index=numery_funkcji[index];
            funkcja f=funkcje[index];
            f.czy_rysowac=!f.czy_rysowac;
            if(index!=CB_ERR){
                funkcje[index]=f;
            }
			if(IsDlgButtonChecked(hOkno,wParam)==BST_CHECKED){
				zaznacz=BST_UNCHECKED;
			}
			else{
				zaznacz=BST_CHECKED;
			}
			CheckDlgButton(hOkno,wParam,zaznacz);
			InvalidateRect(hOkno,&rect_w,1);
		}
		if((HWND)lParam==hPrzyciskGora){
			double d=(bok_gora-bok_dol)*delta;
			bok_gora+=d;
			bok_dol+=d;
			InvalidateRect(hOkno,&rect_w,1);
			SetTextPolozenie();
		}
		if((HWND)lParam==hPrzyciskDol){
			double d=(bok_gora-bok_dol)*delta;
			bok_gora-=d;
			bok_dol-=d;
			InvalidateRect(hOkno,&rect_w,1);
			SetTextPolozenie();
		}
		if((HWND)lParam==hPrzyciskLewo){
			double d=(bok_prawo-bok_lewo)*delta;
			bok_lewo-=d;
			bok_prawo-=d;
			InvalidateRect(hOkno,&rect_w,1);
			SetTextPolozenie();
		}
		if((HWND)lParam==hPrzyciskPrawo){
			double d=(bok_prawo-bok_lewo)*delta;
			bok_lewo+=d;
			bok_prawo+=d;
			InvalidateRect(hOkno,&rect_w,1);
			SetTextPolozenie();
		}
		if((HWND)lParam==hPrzyciskZero){
			double bl=(bok_lewo-bok_prawo)/2;
			bok_prawo=(bok_prawo-bok_lewo)/2;
			bok_lewo=bl;
			double bg=(bok_gora-bok_dol)/2;
			bok_dol=(bok_dol-bok_gora)/2;
			bok_gora=bg;
			InvalidateRect(hOkno,&rect_w,1);
			SetTextPolozenie();
		}
		if((HWND)lParam==hPrzyciskZoomInVert){
			double d2=(bok_gora+bok_dol)/2-(bok_gora-bok_dol)/2/factor;
			bok_gora=(bok_gora+bok_dol)/2+(bok_gora-bok_dol)/2/factor;
			bok_dol=d2;
			InvalidateRect(hOkno,&rect_w,1);
			SetTextPolozenie();
		}
		if((HWND)lParam==hPrzyciskZoomOutVert){
			double d2=(bok_gora+bok_dol)/2-(bok_gora-bok_dol)/2*factor;
			bok_gora=(bok_gora+bok_dol)/2+(bok_gora-bok_dol)/2*factor;
			bok_dol=d2;
			InvalidateRect(hOkno,&rect_w,1);
			SetTextPolozenie();
		}
		if((HWND)lParam==hPrzyciskZoomInHoriz){
			double l2=(bok_prawo+bok_lewo)/2-(bok_prawo-bok_lewo)/2/factor;
			bok_prawo=(bok_prawo+bok_lewo)/2+(bok_prawo-bok_lewo)/2/factor;
			bok_lewo=l2;
			InvalidateRect(hOkno,&rect_w,1);
			SetTextPolozenie();
		}
		if((HWND)lParam==hPrzyciskZoomOutHoriz){
			double l2=(bok_prawo+bok_lewo)/2-(bok_prawo-bok_lewo)/2*factor;
			bok_prawo=(bok_prawo+bok_lewo)/2+(bok_prawo-bok_lewo)/2*factor;
			bok_lewo=l2;
			InvalidateRect(hOkno,&rect_w,1);
			SetTextPolozenie();
		}
		if((HWND)lParam==hPrzyciskZoomIn){
			double l2=(bok_prawo+bok_lewo)/2-(bok_prawo-bok_lewo)/2/factor;
			bok_prawo=(bok_prawo+bok_lewo)/2+(bok_prawo-bok_lewo)/2/factor;
			bok_lewo=l2;
			double d2=(bok_gora+bok_dol)/2-(bok_gora-bok_dol)/2/factor;
			bok_gora=(bok_gora+bok_dol)/2+(bok_gora-bok_dol)/2/factor;
			bok_dol=d2;
			InvalidateRect(hOkno,&rect_w,1);
			SetTextPolozenie();
		}
		if((HWND)lParam==hPrzyciskZoomOut){
			double l2=(bok_prawo+bok_lewo)/2-(bok_prawo-bok_lewo)/2*factor;
			bok_prawo=(bok_prawo+bok_lewo)/2+(bok_prawo-bok_lewo)/2*factor;
			bok_lewo=l2;
			double d2=(bok_gora+bok_dol)/2-(bok_gora-bok_dol)/2*factor;
			bok_gora=(bok_gora+bok_dol)/2+(bok_gora-bok_dol)/2*factor;
			bok_dol=d2;
			InvalidateRect(hOkno,&rect_w,1);
			SetTextPolozenie();
		}
		if((HWND)lParam==hPrzyciskResize){
			if(IsDlgButtonChecked(hOkno,ID_RADIO_LEFT)){
				stringstream ss;
				char cxs[20],cys[20],sxs[20],sys[20];
				GetWindowText(hTekstCenterX,cxs,20);
				GetWindowText(hTekstCenterY,cys,20);
				GetWindowText(hTekstSizeX,sxs,20);
				GetWindowText(hTekstSizeY,sys,20);
				ss<<cxs<<" "<<cys<<" "<<sxs<<" "<<sys;
				double cx,cy,sx,sy;
				ss>>cx>>cy>>sx>>sy;
				ss<<cx;
				bok_lewo=cx-sx/2;
				bok_prawo=cx+sx/2;
				bok_dol=cy-sy/2;
				bok_gora=cy+sy/2;
			}
			else{
				stringstream ss;
				char bl[20],bp[20],bg[20],bd[20];
				GetWindowText(hTekstTop,bg,20);
				GetWindowText(hTekstBottom,bd,20);
				GetWindowText(hTekstLeft,bl,20);
				GetWindowText(hTekstRight,bp,20);
				ss<<bg<<" "<<bd<<" "<<bl<<" "<<bp;
				ss>>bok_gora>>bok_dol>>bok_lewo>>bok_prawo;
			}
			InvalidateRect(hOkno,&rect_w,1);
			SetTextPolozenie();
		}
		if((HWND)lParam==hPrzyciskChangeColor){
			CHOOSECOLOR ccl;
			COLORREF tab[16];
			int result;
			ccl.lStructSize=sizeof(CHOOSECOLOR);
			ccl.hwndOwner=hOkno;
			ccl.lpCustColors=tab;
			ccl.Flags=CC_ANYCOLOR;
			result=ChooseColor(&ccl);
			if(result){
				wybranykolor=ccl.rgbResult;
                nie_rysuj_wykresu=1;
				InvalidateRect(hOkno,&rect_kol,1);
			}
		}
		if((HWND)lParam==hPrzyciskAddFunction){
			dodajFunkcje();
		}
		if((HWND)lParam==hListFunkcje){
			if(HIWORD(wParam)==LBN_SELCHANGE){
				int index=SendMessage(hListFunkcje,LB_GETCURSEL,0,0);
				zaznaczFunkcje(numery_funkcji[index]);
			}
		}
		if((HWND)lParam==hPrzyciskRemoveFunction){
			int index=SendMessage(hListFunkcje,LB_GETCURSEL,0,0);
			usunFunkcje(numery_funkcji[index]);
		}
		if((HWND)lParam==hPrzyciskChangeNumber){
			int index=SendMessage(hListFunkcje,LB_GETCURSEL,0,0);
			int numer1=numery_funkcji[index];
			char str[20];
			stringstream ss;
			GetWindowText(hTekstNumerFunkcji,str,20);
			ss<<str;
			int numer2;
			ss>>numer2;
			zmienNumerTry(numer1,numer2);
		}
		if((HWND)lParam==hPrzyciskOK){
			int index1=SendMessage(hListFunkcje,LB_GETCURSEL,0,0);
			int index=numery_funkcji[index1];
			int thickness=SendMessage(hComboThickness,CB_GETCURSEL,0,0)+1;
			string wspolczynniki;
			stringstream ss;
			char ch_a[20],ch_b[20],ch_c[20],ch_d[20],ch_e[20],ch_wiele[200];
			GetWindowText(hTekstWspolA,ch_a,20);GetWindowText(hTekstWspolB,ch_b,20);
			GetWindowText(hTekstWspolC,ch_c,20);GetWindowText(hTekstWspolD,ch_d,20);
			GetWindowText(hTekstWspolE,ch_e,20);GetWindowText(hTekstWspolWiele,ch_wiele,200);
			ss<<ch_a<<" "<<ch_b<<" "<<ch_c<<" "<<ch_d<<" "<<ch_e<<" "<<ch_wiele;

			wspolczynniki=ss.str();
			int numerTypu=SendMessage(hComboFunkcje,CB_GETCURSEL,0,0);
			type typ=int_to_type(numerTypu);

			int czy_rysowac=0;
			if(IsDlgButtonChecked(hOkno,ID_CHECKBOX_SHOW)==BST_CHECKED){
				czy_rysowac=1;
			}

			if(!edytujFunkcjeTry(index,typ,czy_rysowac,wybranykolor,thickness,wspolczynniki)){
				InvalidateRect(hOkno,&rect_w,1);
			}

		}
	}


	return DefWindowProc(hWindow, Msg, wParam, lParam);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	WNDCLASSEX wc;
	MSG Msg;


	memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc;
	wc.hInstance	 = hInstance;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);

	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = "WindowClass";
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION);

	wc.style=CS_SAVEBITS;

	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	hOkno = CreateWindowEx(WS_EX_CLIENTEDGE,"WindowClass","Grapher",
		WS_VISIBLE|WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_BORDER|WS_MINIMIZEBOX,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		790,
		590,
		NULL,NULL,hInstance,NULL);

	if(hOkno == NULL) {
		MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	Inicjalizuj(hInstance);
	SetTextPolozenie();
	//debug
	type g=LINEAR;
	type h=CONSTANT;
	type d=QUADRATIC;
	dodajFunkcje(1,d,1,RGB(0,0,0),1,"1 2.75 -7");
	dodajFunkcje(3,h,1,RGB(0,0,0),1,"2");
	dodajFunkcje(5,g,1,RGB(0,0,0),1,"5 -5");
	edytujFunkcje(3,h,1,RGB(0,0,255),2,"3");
	zmienNumer(1,0);

	//petla
	while(GetMessage(&Msg, NULL, 0, 0) > 0) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}
