
// rythmGameView.cpp: CrythmGameView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "rythmGame.h"
#endif

#include "rythmGameDoc.h"
#include "rythmGameView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int backsw = 0;		// 화면전환 스위칭 값 EX) 0이면 MAIN화면이였다가 1이 되면 게임화면으로 변경
int jump = 0;		// 1이면 점프 2이면 슬라이드 0이면 일반적인 달리기
int count = 0;		// 0.1초마다 1씩증가하는 카운트(SetTimer(0,100,NULL)로 세팅했기 때문에 0.1초를 카운트 한다.)
int sec = 0;		// 몇초인지 count를 10으로 나누면서 1씩증가
int ObstacleCount = 0;//ObstacleSec을 알맞은 초로 카운팅 시키기 위한 카운트

int Rbutton = 0;	// 왼쪽버튼 장애물 파괴 타이밍 배열 순서 설정
int Lbutton = 0;	// 오른쪽버튼 장애물 파괴 타이밍 배열 순서 설정

int Larray[10] = { 10,15,20,25,30,35,40,45,50,55 };	// 왼쪽 버튼 장애물파괴 타이밍
int Rarray[10] = { 15,20,25,30,35,40,45,50,55,60 };	// 오른쪽 버튼 장애물파괴 타이밍


// CrythmGameView

IMPLEMENT_DYNCREATE(CrythmGameView, CView)

BEGIN_MESSAGE_MAP(CrythmGameView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CrythmGameView 생성/소멸

CrythmGameView::CrythmGameView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CrythmGameView::~CrythmGameView()
{
}

BOOL CrythmGameView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CrythmGameView 그리기

void CrythmGameView::OnDraw(CDC* pDC)
{
	CrythmGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CDC MemDC;
	
	CBitmap bmpback,mainback,mainUI, * Oldbmp;
	
	bmpback.LoadBitmap(IDB_BACK);
	mainback.LoadBitmap(IDB_MAINBACK);
	mainUI.LoadBitmap(IDB_UITEST);

	MemDC.CreateCompatibleDC(pDC);
	
		if (backsw == 0) {
			
		Oldbmp = (CBitmap*)MemDC.SelectObject(&mainback);
		pDC->BitBlt(0, 0, 1000, 350, &MemDC, 0, 0, SRCCOPY);
		Oldbmp = (CBitmap*)MemDC.SelectObject(&mainUI);
		pDC->StretchBlt(250, 250, 300, 150, &MemDC, 50, 60, 700, 350, SRCCOPY);
		
		}
		else if (backsw == 1) {

		Oldbmp = (CBitmap*)MemDC.SelectObject(&bmpback);
		pDC->BitBlt(0, 0, 1000, 350, &MemDC, 0, 0, SRCCOPY);
		
		}
		MemDC.SelectObject(&Oldbmp);
		
	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	
}

void CrythmGameView::DrawBitmap()
{
	CrythmGameDoc* pDoc = GetDocument();
	CClientDC dc(this);
	static int nImage = 0;
	static int nimages = 0;
	int move = 700;

	CDC MemDC;
	
	// TODO: 여기에 구현 코드 추가.
	if (backsw == 1) {
	CClientDC dc(this);
	MemDC.CreateCompatibleDC(&dc);
	CBitmap bmpman, seat, back,bmpback,Obf,Obl, bmpmanmask,Obstaclef,Obstaclel, * oldbmp;

	bmpman.LoadBitmap(IDB_MANT3);
	seat.LoadBitmap(IDB_SEAT);
	back.LoadBitmap(IDB_BACK);
	Obf.LoadBitmap(IDB_OBSTACLEF);
	Obl.LoadBitmap(IDB_OBSTACLEL);
	bmpmanmask.LoadBitmap(IDB_MANT3M);
	Obstaclef.LoadBitmap(IDB_OBSTACLEF);
	Obstaclel.LoadBitmap(IDB_OBSTACLEL);

	

	CString str, str2, str3;
	str.Format(TEXT("초 : %d"), sec);
	dc.TextOutW(350, 0, str);
	str2.Format(TEXT("점수 %d"), pDoc->score);
	dc.TextOutW(350, 30, str2);
	
	
	oldbmp = (CBitmap*)MemDC.SelectObject(&seat);
	dc.BitBlt(0, 300, 1000, 350, &MemDC, nimages * 5, 0, SRCCOPY);
	
	
	if (jump == 0) {
	
		oldbmp = (CBitmap*)MemDC.SelectObject(&back);
		dc.BitBlt(10, 100, 100, 100, &MemDC, 10, 100, SRCCOPY);
		oldbmp = (CBitmap*)MemDC.SelectObject(&back);
		dc.BitBlt(10, 200, 100, 100, &MemDC, 10, 200, SRCCOPY);
		oldbmp = (CBitmap*)MemDC.SelectObject(&bmpmanmask);
		dc.StretchBlt(10, 200, 100, 100, &MemDC, 350 * nImage, 0, 350, 350, SRCAND);
		oldbmp = (CBitmap*)MemDC.SelectObject(&bmpman);
		//CBitmap *은 형 변환을 해서 oldbmp에 넣어주기 위한것
		dc.StretchBlt(10, 200, 100, 100, &MemDC, 350 * nImage, 0, 350, 350, SRCPAINT);

	}
	else if (jump == 1) {
		
		oldbmp = (CBitmap*)MemDC.SelectObject(&back);
		dc.BitBlt(10, 200, 100, 100, &MemDC, 10, 200, SRCCOPY);
		oldbmp = (CBitmap*)MemDC.SelectObject(&back);
		dc.BitBlt(10, 100, 100, 100, &MemDC, 10, 100, SRCCOPY);
		oldbmp = (CBitmap*)MemDC.SelectObject(&bmpmanmask);
		dc.StretchBlt(10, 100, 100, 100, &MemDC, 350 * nImage, 0, 350, 350, SRCAND);
		oldbmp = (CBitmap*)MemDC.SelectObject(&bmpman);
		//CBitmap *은 형 변환을 해서 oldbmp에 넣어주기 위한것
		dc.StretchBlt(10, 100, 100, 100, &MemDC, 350 * nImage, 0, 350, 350, SRCPAINT);
	}
	else {

	}

	if (Larray[Lbutton]-1  < sec && sec < Larray[Lbutton]+3) {

	
		oldbmp = (CBitmap*)MemDC.SelectObject(&back);
		dc.BitBlt(move + 50 - ObstacleCount * 20, 100, 20, 50, &MemDC, move + 50 - ObstacleCount * 20, 100, SRCCOPY);
		oldbmp = (CBitmap*)MemDC.SelectObject(&Obstaclef);
		dc.StretchBlt(move - ObstacleCount * 20, 100, 50, 50, &MemDC, 100, 40, 250, 200, SRCCOPY);
		oldbmp = (CBitmap*)MemDC.SelectObject(&back);
		dc.BitBlt(move+50 - ObstacleCount * 20, 100, 20, 50, &MemDC, move + 50 - ObstacleCount * 20, 100, SRCCOPY);

	}

	nImage++;
	nimages++;
	if (nImage > 3) {
		nImage = 0;
	}
	
	
	if (nimages > 79) {
		nimages = 0;
	}
	oldbmp = (CBitmap*)MemDC.SelectObject(&seat);
	dc.BitBlt(0, 300, 1000, 350, &MemDC, nimages * 5, 0, SRCCOPY);
	MemDC.SelectObject(oldbmp);
	}
}



// CrythmGameView 인쇄

BOOL CrythmGameView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CrythmGameView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CrythmGameView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CrythmGameView 진단

#ifdef _DEBUG
void CrythmGameView::AssertValid() const
{
	CView::AssertValid();
}

void CrythmGameView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}



CrythmGameDoc* CrythmGameView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CrythmGameDoc)));
	return (CrythmGameDoc*)m_pDocument;
}
#endif //_DEBUG


// CrythmGameView 메시지 처리기


void CrythmGameView::OnDestroy()
{
	CView::OnDestroy();
	KillTimer(0);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CrythmGameView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (backsw == 1) {
		DrawBitmap();
		count++;
		sec = count / 10;

		if (Larray[Lbutton]-1 < sec && sec < Larray[Lbutton]+3) {
			ObstacleCount++;
		}
		else {
			ObstacleCount = 0;
		}
		
	}

	CView::OnTimer(nIDEvent);
}




void CrythmGameView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	
		SetTimer(0, 100, NULL);
	
}


void CrythmGameView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	


	CView::OnMouseMove(nFlags, point);
}


void CrythmGameView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (backsw==0) {
		if (point.x > 150 && point.x < 450 && point.y>250 && point.y < 400) {
			backsw++;
		}
		
	}else if (backsw == 1) {
		jump = 1;
		LScoreCount();
	}
	CView::OnLButtonDown(nFlags, point);
}


void CrythmGameView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (backsw == 1) {
		jump = 0;
		
	}
	CView::OnLButtonUp(nFlags, point);
}


void CrythmGameView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (backsw==1) {
		jump = 2;
		RScoreCount();
	}
	CView::OnRButtonDown(nFlags, point);
}


void CrythmGameView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (backsw == 1) {
		jump = 0;
		
		//
	}
	CView::OnRButtonUp(nFlags, point);
}


void CrythmGameView::RScoreCount()
{
	CrythmGameDoc* pDoc = GetDocument();
	if (Rarray[Rbutton]+2 < sec && sec<Rarray[Rbutton] + +4)
	{
		pDoc->score++;
		Rbutton++;
		
	}
	else if(sec> Rarray[Rbutton] + 3){
		Rbutton++;
		
	}

	
	// TODO: 여기에 구현 코드 추가.
}


void CrythmGameView::LScoreCount()
{
	CrythmGameDoc* pDoc = GetDocument();
	// TODO: 여기에 구현 코드 추가.
	if (Larray[Lbutton]+2 < sec && sec < Larray[Lbutton] + 5)
	{
		pDoc->score++;
		Lbutton++;
		
	}
	else if (sec > Larray[Lbutton] + 3) {
		Lbutton++;
		
	}
}
