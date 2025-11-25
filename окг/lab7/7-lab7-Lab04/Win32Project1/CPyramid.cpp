#include "stdafx.h"
#define pi 3.14159265;



////////////////////////// Конструктор ///////////////////////////////////
////////////////////////// Конструктор ///////////////////////////////////
CPyramid::CPyramid()
{
	// Усеченная пирамида имеет 8 вершин: нижнее основание (A,B,C,D) и верхнее основание (E,F,G,H)
	Vertices.RedimMatrix(4, 8);

	// Нижнее основание (большой квадрат)
	Vertices(0, 0) = -10; Vertices(1, 0) = -10; Vertices(2, 0) = 0;   // A
	Vertices(0, 1) = 10;  Vertices(1, 1) = -10; Vertices(2, 1) = 0;   // B  
	Vertices(0, 2) = 10;  Vertices(1, 2) = 10;  Vertices(2, 2) = 0;   // C
	Vertices(0, 3) = -10; Vertices(1, 3) = 10;  Vertices(2, 3) = 0;   // D

	// Верхнее основание (малый квадрат)
	Vertices(0, 4) = -5; Vertices(1, 4) = -5; Vertices(2, 4) = 15;    // E
	Vertices(0, 5) = 5;  Vertices(1, 5) = -5; Vertices(2, 5) = 15;    // F
	Vertices(0, 6) = 5;  Vertices(1, 6) = 5;  Vertices(2, 6) = 15;    // G
	Vertices(0, 7) = -5; Vertices(1, 7) = 5;  Vertices(2, 7) = 15;    // H

	for (int i = 0; i < 8; i++) Vertices(3, i) = 1;

	// Нормали для оснований
	Nabcd.RedimMatrix(3);  // Нижнее основание
	Nabcd(0) = 0; Nabcd(1) = 0; Nabcd(2) = -1;

	Nefgh.RedimMatrix(3);  // Верхнее основание  
	Nefgh(0) = 0; Nefgh(1) = 0; Nefgh(2) = 1;
}

////////////////////////// Draw1(...) ///////////////////////////////

////////////////////////// Draw1(...) ///////////////////////////////
void CPyramid::Draw1(CDC& dc, CMatrix& PView, CRect& RW)
{
	CMatrix ViewCart = SphereToCart(PView); // сферические в декартовы
	CMatrix MV = CreateViewCoord(PView(0), PView(1), PView(2)); // для перевода из мировой в видовую
	CMatrix ViewVert = MV * Vertices; // вершины пирамиды от лица камеры
	CRectD RectView;
	GetRect(ViewVert, RectView); // прямоугольник для отображения
	CMatrix MW = SpaceToWindow(RectView, RW); // перевод из видовой в оконные

	CPoint MasVert[8];
	CMatrix V(3);
	V(2) = 1;
	for (int i = 0; i < 8; i++) // видовые координаты в оконные
	{
		V(0) = ViewVert(0, i);
		V(1) = ViewVert(1, i);
		V = MW * V;
		MasVert[i].x = (int)V(0);
		MasVert[i].y = (int)V(1);
	}

	CPen Pen(PS_SOLID, 2, RGB(0, 0, 255));
	CPen* pOldPen = dc.SelectObject(&Pen);

	CMatrix R1(3), R2(3), VN(3);
	double sm;

	// Кисти для разных частей
	CBrush BrusSide(RGB(255, 0, 0));     // Красный - боковые грани
	CBrush BrusBottom(RGB(0, 255, 0));   // Зеленый - нижнее основание
	CBrush BrusTop(RGB(0, 0, 255));      // Синий - верхнее основание

	// Боковые грани (трапеции)
	int sideFaces[4][4] = {
		{0, 1, 5, 4},  // Передняя грань: A-B-F-E
		{1, 2, 6, 5},  // Правая грань: B-C-G-F
		{2, 3, 7, 6},  // Задняя грань: C-D-H-G
		{3, 0, 4, 7}   // Левая грань: D-A-E-H
	};

	for (int i = 0; i < 4; i++)
	{
		R1 = Vertices.GetCol(sideFaces[i][0], 0, 2);
		R2 = Vertices.GetCol(sideFaces[i][1], 0, 2);
		CMatrix R3 = Vertices.GetCol(sideFaces[i][2], 0, 2);

		CMatrix V1 = R2 - R1;
		CMatrix V2 = R3 - R1;
		VN = VectorMult(V2, V1);
		sm = ScalarMult(VN, ViewCart);

		if (sm >= 0) // Боковая грань видима
		{
			CBrush* pOldBrush = dc.SelectObject(&BrusSide);
			CPoint face[4];
			for (int j = 0; j < 4; j++) {
				face[j] = MasVert[sideFaces[i][j]];
			}
			dc.Polygon(face, 4);
			dc.SelectObject(pOldBrush);
		}
	}

	// Нижнее основание
	sm = ScalarMult(Nabcd, ViewCart);
	if (sm >= 0) // Основание видно
	{
		CBrush* pOldBrush = dc.SelectObject(&BrusBottom);
		CPoint bottom[4] = { MasVert[0], MasVert[1], MasVert[2], MasVert[3] };
		dc.Polygon(bottom, 4);
		dc.SelectObject(pOldBrush);
	}

	// Верхнее основание
	sm = ScalarMult(Nefgh, ViewCart);
	if (sm >= 0) // Верхнее основание видно
	{
		CBrush* pOldBrush = dc.SelectObject(&BrusTop);
		CPoint top[4] = { MasVert[4], MasVert[5], MasVert[6], MasVert[7] };
		dc.Polygon(top, 4);
		dc.SelectObject(pOldBrush);
	}

	dc.SelectObject(pOldPen);

	// Отображение координат камеры
	CString strCoords;
	strCoords.Format(L"r=%.2f, fi=%.2f, theta=%.2f", PView(0), PView(1), PView(2));

	CFont font;
	font.CreatePointFont(100, _T("Arial"));
	CFont* pOldFont = dc.SelectObject(&font);
	COLORREF oldTextColor = dc.SetTextColor(RGB(0, 0, 0));
	dc.TextOut(10, 10, strCoords);
	dc.SetTextColor(oldTextColor);
	dc.SelectObject(pOldFont);
}
////////////////////////// Draw(...) ///////////////////////////////
////////////////////////// Draw(...) ///////////////////////////////
void CPyramid::Draw(CDC& dc, CMatrix& PView, CRect& RW)
{
	CMatrix ViewCart = SphereToCart(PView);
	CMatrix MV = CreateViewCoord(PView(0), PView(1), PView(2));
	CMatrix ViewVert = MV * Vertices;
	CRectD RectView;
	GetRect(ViewVert, RectView);
	CMatrix MW = SpaceToWindow(RectView, RW);

	CPoint MasVert[8];
	CMatrix V(3);
	V(2) = 1;
	for (int i = 0; i < 8; i++)
	{
		V(0) = ViewVert(0, i);
		V(1) = ViewVert(1, i);
		V = MW * V;
		MasVert[i].x = (int)V(0);
		MasVert[i].y = (int)V(1);
	}

	// Рисуем каркас усеченной пирамиды
	CPen Pen(PS_SOLID, 2, RGB(0, 0, 255));
	CPen* pOldPen = dc.SelectObject(&Pen);

	// Нижнее основание
	dc.MoveTo(MasVert[0]);
	dc.LineTo(MasVert[1]);
	dc.LineTo(MasVert[2]);
	dc.LineTo(MasVert[3]);
	dc.LineTo(MasVert[0]);

	// Верхнее основание
	dc.MoveTo(MasVert[4]);
	dc.LineTo(MasVert[5]);
	dc.LineTo(MasVert[6]);
	dc.LineTo(MasVert[7]);
	dc.LineTo(MasVert[4]);

	// Боковые ребра
	for (int i = 0; i < 4; i++)
	{
		dc.MoveTo(MasVert[i]);
		dc.LineTo(MasVert[i + 4]);
	}

	// Диагонали оснований
	CPen Pen1(PS_DASH, 1, RGB(255, 0, 0));
	dc.SelectObject(&Pen1);

	// Диагонали нижнего основания
	dc.MoveTo(MasVert[0]);
	dc.LineTo(MasVert[2]);
	dc.MoveTo(MasVert[1]);
	dc.LineTo(MasVert[3]);

	// Диагонали верхнего основания
	dc.MoveTo(MasVert[4]);
	dc.LineTo(MasVert[6]);
	dc.MoveTo(MasVert[5]);
	dc.LineTo(MasVert[7]);

	dc.SelectObject(pOldPen);

	// Отображение координат камеры
	CString strCoords;
	strCoords.Format(L"r=%.2f, fi=%.2f, theta=%.2f", PView(0), PView(1), PView(2));

	CFont font;
	font.CreatePointFont(100, _T("Arial"));
	CFont* pOldFont = dc.SelectObject(&font);
	COLORREF oldTextColor = dc.SetTextColor(RGB(0, 0, 0));
	dc.TextOut(10, 10, strCoords);
	dc.SetTextColor(oldTextColor);
	dc.SelectObject(pOldFont);
}

void CPyramid::Draw2(CDC& dc, CMatrix& PView, CRect& RW)
// Рисует пирамиду БЕЗ удаления невидимых ребер
// Пересчет координат выполняет Windows, для этого
// должен быть установлен режим отображения MM_ANISOTROPIC 
// Pежима MM_ANISOTROPIC устанавливается в самой функции! 
// dc - ссылка на класс CDC MFC
// PView - координаты точки наблюдения в мировой сферической системе координат
// (r,fi(град.), q(град.))
// RW - область в окне для отображения
{
	CMatrix ViewCart = SphereToCart(PView);		// Декартовы координаты точки наблюдения
	CMatrix MV = CreateViewCoord(PView(0), PView(1), PView(2));	//Матрица(4x4) 
	//для пересчета в видовую СК 
	CMatrix ViewVert = MV * Vertices; // Координаты вершин пирамиды в видовой СК
	CRectD RectView;
	GetRect(ViewVert, RectView);		// Получаем охватывающий прямоугольник
	CPoint MasVert[5];	// 
	for (int i = 0; i < 5; i++)
	{
		MasVert[i].x = (int)ViewVert(0, i);
		MasVert[i].y = (int)ViewVert(1, i);
	}

	CRect IRV((int)RectView.left, (int)RectView.top, (int)RectView.right, (int)RectView.bottom);
	SetMyMode(dc, IRV, RW); // Установка режима MM_ANISOTROPIC 

	// Рисуем
	CPen Pen(PS_SOLID, 0, RGB(255, 0, 0));
	CPen* pOldPen = dc.SelectObject(&Pen);
	dc.Polygon(MasVert, 4);	// Основание
	for (int i = 0; i < 4; i++)
	{
		dc.MoveTo(MasVert[4]);	// Перо на вершину E
		dc.LineTo(MasVert[i]);
	}
	// Координаты центра O 	пересечения диагоналей основания
	int x0 = (MasVert[0].x + MasVert[2].x) / 2;
	int y0 = (MasVert[0].y + MasVert[2].y) / 2;
	// Рисуем диагонали и линию из E в O 
	CPen Pen1(PS_DASH, 0, RGB(0, 255, 0));
	dc.SelectObject(&Pen1);
	dc.MoveTo(MasVert[4]);	// Перо на вершину E
	dc.LineTo(x0, y0);				// Линия EO
	dc.MoveTo(MasVert[0]);	// Перо на вершину A
	dc.LineTo(MasVert[2]);	// Диагональ
	dc.MoveTo(MasVert[1]);	// Перо на вершину B
	dc.LineTo(MasVert[3]);	// Диагональ
	dc.SelectObject(pOldPen);
	dc.SetMapMode(MM_TEXT); // Установка режима MM_TEXT
	CString strCoords;
	strCoords.Format(L" r=%.2f, fi=%.2f, theta=%.2f",
		PView(0), PView(1), PView(2));

	int xText = 10;
	int yText = 10;

	CFont font;
	font.CreatePointFont(100, _T("Arial"));
	CFont* pOldFont = dc.SelectObject(&font);

	COLORREF oldTextColor = dc.SetTextColor(RGB(0, 0, 0));

	dc.TextOut(xText, yText, strCoords);

	dc.SetTextColor(oldTextColor);
	dc.SelectObject(pOldFont);
}




////////////////////////// ColorDraw(...) ///////////////////////////////

void CPyramid::ColorDraw(CDC& dc, CMatrix& PView, CRect& RW, COLORREF Color)

// Рисует пирамиду С УДАЛЕНИЕМ невидимых ребер
// Самостоятельный пересчет координат из мировых в оконные (MM_TEXT)
// dc - ссылка на класс CDC MFC
// PView - координаты точки наблюдения в мировой сферической системе координат
// (r,fi(град.), q(град.))
// RW - область в окне для отображения
{

	BYTE red = GetRValue(Color);
	BYTE green = GetGValue(Color);
	BYTE blue = GetBValue(Color);

	CMatrix ViewCart = SphereToCart(PView);		// Декартовы координаты точки наблюдения
	CMatrix MV = CreateViewCoord(PView(0), PView(1), PView(2));	//Матрица(4x4) 
	//для пересчета в видовую СК 
	CMatrix ViewVert = MV * Vertices; // Координаты вершин пирамиды в видовой СК
	CRectD RectView;
	CPyramid::GetRect(ViewVert, RectView);		// Получаем охватывающий прямоугольник
	CMatrix MW = SpaceToWindow(RectView, RW);	// Матрица (3x3) для пересчета 
	// в оконную систему координат
// Готовим массив оконных координат для рисования
	CPoint MasVert[5];	// Массив оконных координат вершин
	CMatrix V(3); //V0(3);
	V(2) = 1;
	// Цикл по количеству вершин - вычисляем оконные координаты вершин
	for (int i = 0; i < 5; i++)
	{
		V(0) = ViewVert(0, i); // x
		V(1) = ViewVert(1, i); // y
		//V=V-V0;			// x-xL, y-yL, 1
		V = MW * V;			// Оконные координаты точки
		MasVert[i].x = (int)V(0);
		MasVert[i].y = (int)V(1);
	}
	// Рисуем

	CMatrix VE = Vertices.GetCol(4, 0, 2);	// Вершина E
	CMatrix R1(3), R2(3), VN(3);
	double sm;
	for (int i = 0; i < 4; i++)
	{
		int k;
		if (i == 3) k = 0;
		else k = i + 1;
		R1 = Vertices.GetCol(i, 0, 2);
		R2 = Vertices.GetCol(k, 0, 2);
		CMatrix V1 = R2 - R1;			      // Вектор - ребро в основании
		CMatrix V2 = VE - R1;			      // Вектор - ребро к вершине E 
		VN = VectorMult(V2, V1);		  	// Вектор ВНЕШНЕЙ(!)нормали к грани

		sm = CosV1V2(VN, ViewCart);	// Косинус угла между вектором нормали 
		// к грани и вектором точки наблюдения 
		if (sm >= 0) // Грань видима - рисуем боковую грань
		{
			CPen Pen(PS_SOLID, 2, RGB(sm * sm * red, sm * sm * green, sm * sm * blue));
			CPen* pOldPen = dc.SelectObject(&Pen);
			CBrush Brus(RGB(sm * sm * red, sm * sm * green, sm * sm * blue));
			CBrush* pOldBrush = dc.SelectObject(&Brus);
			CPoint MasVertR[3] = { MasVert[i],MasVert[k],MasVert[4] };
			dc.Polygon(MasVertR, 3);	// Ребро
			dc.SelectObject(pOldBrush);
			dc.SelectObject(pOldPen);
		}
	}
	VN = VectorMult(R1, R2);
	sm = CosV1V2(VN, ViewCart);
	if (sm >= 0)
	{
		CPen Pen(PS_SOLID, 2, RGB(sm * sm * red, sm * sm * green, sm * sm * blue));
		CPen* pOldPen = dc.SelectObject(&Pen);
		CBrush Brus(RGB(sm * sm * red, sm * sm * green, sm * sm * blue));
		CBrush* pOldBrush = dc.SelectObject(&Brus);
		dc.Polygon(MasVert, 4);	// Основание
		dc.SelectObject(pOldBrush);
		dc.SelectObject(pOldPen);
	}
	CString strCoords;
	strCoords.Format(L"r=%.2f, fi=%.2f, theta=%.2f",
		PView(0), PView(1), PView(2));

	int xText = 10;
	int yText = 10;

	CFont font;
	font.CreatePointFont(100, _T("Arial"));
	CFont* pOldFont = dc.SelectObject(&font);

	COLORREF oldTextColor = dc.SetTextColor(RGB(0, 0, 0));

	dc.TextOut(xText, yText, strCoords);

	dc.SetTextColor(oldTextColor);
	dc.SelectObject(pOldFont);

}



////////////////////////// GetRect(...) ///////////////////////////

////////////////////////// GetRect(...) ///////////////////////////
void CPyramid::GetRect(CMatrix& Vert, CRectD& RectView)
{
	CMatrix V = Vert.GetRow(0); // x - координаты
	double xMin = V.MinElement();
	double xMax = V.MaxElement();
	V = Vert.GetRow(1); // y - координаты
	double yMin = V.MinElement();
	double yMax = V.MaxElement();
	RectView.SetRectD(xMin, yMax, xMax, yMin);
}


//---------------------------------------------------------------------