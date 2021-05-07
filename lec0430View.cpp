
// lec0430View.cpp : Clec0430View クラスの実装
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS は、プレビュー、縮小版、および検索フィルター ハンドラーを実装している ATL プロジェクトで定義でき、
// そのプロジェクトとのドキュメント コードの共有を可能にします。
#ifndef SHARED_HANDLERS
#include "lec0430.h"
#endif

#include "lec0430Doc.h"
#include "lec0430View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Clec0430View

IMPLEMENT_DYNCREATE(Clec0430View, CView)

BEGIN_MESSAGE_MAP(Clec0430View, CView)
	// 標準印刷コマンド
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// Clec0430View コンストラクション/デストラクション

Clec0430View::Clec0430View() noexcept
{
	// TODO: 構築コードをここに追加します。
	// PointsNum = 0; // 起動した直後頂点はゼロ
	
	mIsLButtonDown = false; // アプリを起動してウィンドウが開いたらマウスはクリックされていない
}

Clec0430View::~Clec0430View()
{
}

BOOL Clec0430View::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

// Clec0430View 描画

void Clec0430View::OnDraw(CDC* pDC)
{
	Clec0430Doc* pDoc = GetDocument(); 
	ASSERT_VALID(pDoc);
	if (!pDoc) return;
	pDoc->DrawLines(pDC);
	//pDC->Polyline(Points, PointsNum);
	//pDC->MoveTo(Points[0]);
	//for(int i= 1; i <= PointsNum ; i++)pDC->LineTo(Points[i]);


}


// Clec0430View の印刷

BOOL Clec0430View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 既定の印刷準備
	return DoPreparePrinting(pInfo);
}

void Clec0430View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷前の特別な初期化処理を追加してください。
}

void Clec0430View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷後の後処理を追加してください。
}


// Clec0430View の診断

#ifdef _DEBUG
void Clec0430View::AssertValid() const
{
	CView::AssertValid();
}

void Clec0430View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Clec0430Doc* Clec0430View::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Clec0430Doc)));
	return (Clec0430Doc*)m_pDocument;
}
#endif //_DEBUG


// Clec0430View メッセージ ハンドラー
// マウスを押しているときに動かすと描画される．
// 放しているときは描画されない -> mIsLButtonDown

void Clec0430View::OnLButtonDown(UINT nFlags, CPoint point) // point -> お前にとってこの座標
{
	mIsLButtonDown = true; // viewだけ知っていればいい マウスを押されたらtrue
	Clec0430Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc) return;

	pDoc->NewLine(point);
	// Points[0] = point; // -> 本来viewにあったらあかん 頂点を保存
	// PointsNum = 1; // 超点群の総数を保存
	// Invalidate(); // In -> 否定する invalidate -> ちゃんとしていない状態にする(無効化) -> ウィンドウを無効化
	// CView::OnLButtonDown(nFlags, point);
}

void Clec0430View::OnLButtonUp(UINT nFlags, CPoint point)
{
	mIsLButtonDown = false;
	// CView::OnLButtonUp(nFlags, point);
}

void Clec0430View::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!mIsLButtonDown) return; // ! -> not Lボタンは押されているときtrue mIsLButtonDownが押されていなければreturn
	Clec0430Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc) return;
	pDoc->AddPoint(point);
	
	// Points[PointsNum] = point; // マウスを動かした直後 頂点は一個だけ Points[1~1000]
	// PointsNum++;
	// Invalidate(); // 自分自身が管理しているウィンドウのみInvalidate
	// CView::OnMouseMove(nFlags, point); 
}
