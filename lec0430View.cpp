#include "pch.h"
#include "framework.h"
#include "atlimage.h"
#include <iostream>
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
	ON_COMMAND(ID_PEN_WIDTH_1, &Clec0430View::OnPenWidth1)
	ON_COMMAND(ID_PEN_WIDTH_3, &Clec0430View::OnPenWidth3)
	ON_COMMAND(ID_PEN_WIDTH_5, &Clec0430View::OnPenWidth5)
	ON_UPDATE_COMMAND_UI(ID_PEN_WIDTH_1, &Clec0430View::OnUpdatePenWidth1)
	ON_UPDATE_COMMAND_UI(ID_PEN_WIDTH_3, &Clec0430View::OnUpdatePenWidth3)
	ON_UPDATE_COMMAND_UI(ID_PEN_WIDTH_5, &Clec0430View::OnUpdatePenWidth5)
	ON_COMMAND(ID_PEN_BLACK, &Clec0430View::OnPenBlack)
	ON_COMMAND(ID_PEN_BLUE, &Clec0430View::OnPenBlue)
	ON_COMMAND(ID_PEN_COLOR, &Clec0430View::OnPenColor)
	ON_COMMAND(ID_PEN_RED, &Clec0430View::OnPenRed)
	ON_COMMAND(ID_PEN_GREEN, &Clec0430View::OnPenGreen)
	ON_COMMAND(ID_IMAGE_LOAD, &Clec0430View::OnImageLoad)
	ON_COMMAND(ID_IMAGE_GRAY, &Clec0430View::OnImageGray)
	ON_COMMAND(ID_IMAGE_ORIGINAL, &Clec0430View::OnImageOriginal)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_ORIGINAL, &Clec0430View::OnUpdateImage)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_GRAY, &Clec0430View::OnUpdateImage)
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_IMAGE_AVERAGE, &Clec0430View::OnImageAverage)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_AVERAGE, &Clec0430View::OnUpdateImage)
	ON_COMMAND(ID_IMAGE_AVERAGED, &Clec0430View::OnImageAveraged)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_AVERAGED, &Clec0430View::OnUpdateImage)
	ON_COMMAND(ID_IMAGE_MASK, &Clec0430View::OnImageMask)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_MASK, &Clec0430View::OnUpdateImage)
	ON_COMMAND(ID_IMAGE_LOAD2, &Clec0430View::OnImageLoad2)
END_MESSAGE_MAP()

// Clec0430View コンストラクション/デストラクション

Clec0430View::Clec0430View() noexcept
{
	// PointsNum = 0; // 起動した直後頂点はゼロ
	
	mIsLButtonDown = false; // アプリを起動してウィンドウが開いたらマウスはクリックされていない
	mPenWidth = 3; // ペンの幅初期値は3
	mPenColor = RGB(0, 0, 0); // ペンの色初期値は黒
	mFilterType = 0;
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
	if (mFilterDC.m_hDC == NULL) { // 出来上がった画像をmFilterDCで管理
		mFilter.CreateCompatibleBitmap(pDC, pDC->GetDeviceCaps(HORZRES), pDC->GetDeviceCaps(VERTRES));
		mFilterDC.CreateCompatibleDC(pDC);
		mFilterDC.SelectObject(&mFilter);
		mFilterDC.SelectStockObject(WHITE_BRUSH);
		mFilterDC.PatBlt(0, 0, pDC->GetDeviceCaps(HORZRES), pDC->GetDeviceCaps(VERTRES), PATCOPY);
		// BackBuffer処理(作る処理)
		mBackBuffer.CreateCompatibleBitmap(pDC, pDC->GetDeviceCaps(HORZRES), pDC->GetDeviceCaps(VERTRES));
		mBackBufferDC.CreateCompatibleDC(pDC);
		mBackBufferDC.SelectObject(mBackBuffer);
	}
	Clec0430Doc* pDoc = GetDocument(); 
	ASSERT_VALID(pDoc);
	if (!pDoc) return;
	// pDoc->DrawImage(pDC);
	CRect windowSize;
	this->GetClientRect(&windowSize); // 今開いているウィンドウの大きさ取得
	// pDCの処理をBackBufferに 絵と線の処理
	mBackBufferDC.BitBlt(0, 0, windowSize.Width(), windowSize.Height(), &mFilterDC, 0, 0, SRCCOPY);
	pDoc->DrawLines(&mBackBufferDC);
	// 処理が終わったらBackBufferをpDCにコピーすればよい 最後画面のコピーだけをする
	pDC->BitBlt(0, 0, windowSize.Width(), windowSize.Height(), &mBackBufferDC, 0, 0, SRCCOPY);

	// ↓old
	// pDC->BitBlt(0, 0, windowSize.Width(), windowSize.Height(), &mFilterDC, 0, 0, SRCCOPY);
	// pDoc->DrawLines(pDC);
	// pDC->Polyline(Points, PointsNum);
	// pDC->MoveTo(Points[0]);
	// for(int i= 1; i <= PointsNum ; i++)pDC->LineTo(Points[i]);
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

	pDoc->NewLine(point, mPenWidth, mPenColor);

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

// メニュー呼ばれたら新しいペン幅にする
void Clec0430View::OnPenWidth1()
{
	mPenWidth = 1;
}

void Clec0430View::OnPenWidth3()
{
	mPenWidth = 3;
}

void Clec0430View::OnPenWidth5()
{
	mPenWidth = 5;
}


void Clec0430View::OnUpdatePenWidth1(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(mPenWidth == 1);
}

void Clec0430View::OnUpdatePenWidth3(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(mPenWidth == 3);
}

void Clec0430View::OnUpdatePenWidth5(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(mPenWidth == 5);
}

void Clec0430View::OnPenBlack()
{
	mPenColor = RGB(0, 0, 0);
}

void Clec0430View::OnPenBlue()
{
	mPenColor = RGB(0, 0, 255);
}

void Clec0430View::OnPenRed()
{
	mPenColor = RGB(255, 0, 0);
}

void Clec0430View::OnPenGreen()
{
	mPenColor = RGB(0, 255, 0);
}

void Clec0430View::OnPenColor()
{
	CMFCColorDialog dialog; // カラーセレクターを表示
	if(dialog.DoModal() == IDOK)
		mPenColor = dialog.GetColor(); // 
}

void Clec0430View::OnImageLoad()
{
	Clec0430Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc) return;
	CFileDialog dialog(true); // ファイル読み込み
	if (dialog.DoModal() != IDOK) return; // IDOKじゃないものはreturn
	pDoc->LoadImage(dialog.GetPathName()); // ユーザが選んだファイルパスはGetPathNameに
	mFilterType = ID_IMAGE_ORIGINAL;
	ApplyFilter();
}

void Clec0430View::OnImageLoad2()
{	
	Clec0430Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc) return;
	CFileDialog dialog(true); // ファイル読み込み
	if (dialog.DoModal() != IDOK) return; // IDOKじゃないものはreturn
	pDoc->LoadImage(dialog.GetPathName()); // ユーザが選んだファイルパスはGetPathNameに
	
	/*
	CString filePath;
	Clec0430Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc) return;
	filePath = "XP.jpg";
	pDoc->LoadImage(filePath); // ユーザが選んだファイルパスはGetPathNameに
	CImage* pImage = pDoc->GetImage(); // イメージをもらう
	if (pImage->IsNull()) return;
	int w = pImage->GetWidth(); // イメージの幅と高さをもらう
	int h = pImage->GetHeight();
	TRACE("w = %d, h = %d \n", w,h);
	*/
	mFilterType = ID_IMAGE_LOAD;
	ApplyFilter();
}

void Clec0430View::OnImageGray()
{
	mFilterType = ID_IMAGE_GRAY;
	ApplyFilter();
}

void Clec0430View::OnImageOriginal() // 元に戻す
{
	mFilterType = ID_IMAGE_ORIGINAL;
	ApplyFilter();
}
/*
void Clec0430View::OnUpdateImageOriginal(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(mFilterType == ID_IMAGE_ORIGINAL);
}

void Clec0430View::OnUpdateImageGray(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(mFilterType == ID_IMAGE_GRAY);
}
*/

int w = 0, h = 0, w2 = 0, h2 = 0;
void Clec0430View::ApplyFilter()
{
	/*
	Clec0430Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc) return;
	CImage* pImage = pDoc->GetImage(); // イメージをもらう
	if (pImage->IsNull()) return;
	
		int w = pImage->GetWidth(); // イメージの幅と高さをもらう
		int h = pImage->GetHeight();
		TRACE("w = %d, h = %d\n", w,h);
	}
	else {
		// int w2 = pImage->GetWidth(); // イメージの幅と高さをもらう
		// int h2 = pImage->GetHeight();
		TRACE("w = %d, h = %d\n", w2, h2);
	}
	*/
	
	COLORREF src, res; // 元画像の色情報を保存
	int i, ii, j, jj, r, g, b;
	double dr, dg, db;
	double Hue;			//色相
	double Saturation;	//彩度
	double Value;
	double rr, gg, bb;
	BeginWaitCursor();
	if (mFilterType == ID_IMAGE_ORIGINAL || mFilterType == ID_IMAGE_GRAY || mFilterType == ID_IMAGE_AVERAGE || mFilterType == ID_IMAGE_AVERAGED || mFilterType == ID_IMAGE_MASK) {
		Clec0430Doc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		if (!pDoc) return;
		CImage* pImage = pDoc->GetImage(); // イメージをもらう
		if (pImage->IsNull()) return;
		int w = pImage->GetWidth(); // イメージの幅と高さをもらう
		int h = pImage->GetHeight();
		if (mFilterType == ID_IMAGE_ORIGINAL) {
			pDoc->DrawImage(&mFilterDC);
		}
		else if (mFilterType == ID_IMAGE_GRAY) {
			for (i = 0; i < w; i++) for (j = 0; j < h; j++) { // 画像を全てスキャン
				src = pImage->GetPixel(i, j); // 元画像を取ってくる 32bitのint型　rgbが1つに入ってる感じ
				r = GetRValue(src); // 最初の8bitを取得
				g = GetGValue(src); // 8bit以降を取得
				b = GetBValue(src); // 17bit以降取得
				// 少数計算はfloatより，doubleの方がよい
				res = int(double(r) * 0.3 + double(g) * 0.59 + double(b) * 0.11);
				mFilterDC.SetPixel(i, j, RGB(res, res, res));
			}
		}
		else if (mFilterType == ID_IMAGE_AVERAGE) {
			for (i = 1; i < w - 1; i++)for (j = 1; j < h - 1; j++) {
				r = g = b = 0;
				for (ii = i - 1; ii <= i + 1; ii++)for (jj = j - 1; jj <= j + 1; jj++) {
					src = pImage->GetPixel(ii, jj);
					r += GetRValue(src);
					g += GetGValue(src);
					b += GetBValue(src);
				}
				mFilterDC.SetPixel(i, j, RGB(r / 9, g / 9, b / 9));
			}
		}
		else if (mFilterType == ID_IMAGE_AVERAGED) {
			double averageFilter[3][3] = { {0.11,0.11,0.11},{0.11,0.12,0.11},{0.11,0.11,0.11} };
			for (i = 1; i < w - 1; i++)for (j = 1; j < h - 1; j++) {
				dr = dg = db = 0.0;
				for (ii = 0; ii < 3; ii++)for (jj = 0; jj < 3; jj++) {
					src = pImage->GetPixel(i + ii - 1, j + jj - 1);
					dr += double(GetRValue(src)) * averageFilter[ii][jj];
					dg += double(GetGValue(src)) * averageFilter[ii][jj];
					db += double(GetBValue(src)) * averageFilter[ii][jj];
				}
				mFilterDC.SetPixel(i, j, RGB(int(dr), int(dg), int(db)));
			}
		}
		else if (mFilterType == ID_IMAGE_MASK) {
			for (i = 0; i < w; i++) for (j = 0; j < h; j++) { // 画像を全てスキャン
				src = pImage->GetPixel(i, j); // 元画像を取ってくる 32bitのint型　rgbが1つに入ってる感じ
				rr = GetRValue(src); // 最初の8bitを取得
				gg = GetGValue(src); // 8bit以降を取得
				bb = GetBValue(src); // 17bit以降取得
				// RGB -> HSV
				double MAX = max((max(rr, gg)), bb);
				double MIN = min((min(rr, gg)), bb);
				Value = MAX / 256 * 100;

				if (MAX == MIN) {
					Hue = 0;
					Saturation = 0;
				}
				else {
					if (MAX == rr) Hue = 60.0 * ((gg - bb) / (MAX - MIN));
					else if (MAX == gg) Hue = 60.0 * ((bb - rr) / (MAX - MIN)) + 120.0;
					else if (MAX == bb) Hue = 60.0 * ((rr - gg) / (MAX - MIN)) + 240.0;

					if (Hue > 360.0) Hue = Hue - 360.0;
					else if (Hue < 0) Hue = Hue + 360.0;
					Saturation = ((MAX - MIN) / MAX) * 100;
				}
				// TRACE("R = %f, G = %f, B = %f \n", rr, gg, bb);
				// TRACE("H = %f, S = %f, V = %f \n", Hue, Saturation, Value);
				// 白色に

				if (Hue > 100 && Hue <= 180 && Saturation > 70 && Value > 40) {
					mFilterDC.SetPixel(i, j, RGB(255, 255, 255));
				}
				else {
					mFilterDC.SetPixel(i, j, RGB(int(rr), int(gg), int(bb)));
				}

			}
		}
	}
	else if (mFilterType == ID_IMAGE_LOAD) {
		Clec0430Doc* pDoc2 = GetDocument();
		ASSERT_VALID(pDoc2);
		if (!pDoc2) return;
		CImage* pImage2 = pDoc2->GetImage(); // イメージをもらう
		if (pImage2->IsNull()) return;
		// pDoc2->DrawImage(&mFilterDC);
		
		CString filePath2;
		Clec0430Doc* pDoc3 = GetDocument();
		ASSERT_VALID(pDoc3);
		if (!pDoc3) return;
		filePath2 = "XP.jpg";
		pDoc3->LoadImage(filePath2); // ユーザが選んだファイルパスはGetPathNameに
		CImage* pImage = pDoc3->GetImage(); // イメージをもらう
		if (pImage->IsNull()) return;
		int w = pImage->GetWidth(); // イメージの幅と高さをもらう
		int h = pImage->GetHeight();
		pDoc3->DrawImage(&mFilterDC);
		TRACE("w = %d, h = %d \n", w, h);
	}
	Invalidate();
	EndWaitCursor();
}

// ちらつき防止
BOOL Clec0430View::OnEraseBkgnd(CDC* pDC)
{
	return true;
	// return CView::OnEraseBkgnd(pDC);
}

void Clec0430View::OnImageAverage()
{
	mFilterType = ID_IMAGE_AVERAGE;
	ApplyFilter();
}

void Clec0430View::OnUpdateImage(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(mFilterType == pCmdUI->m_nID);
}

void Clec0430View::OnImageAveraged()
{
	mFilterType = ID_IMAGE_AVERAGED;
	ApplyFilter();
}

void Clec0430View::OnImageMask()
{
	mFilterType = ID_IMAGE_MASK;
	ApplyFilter();
}
/*
void Clec0430View::OnImageLoad2()
{
	
	Clec0430Doc* pDoc2 = GetDocument();
	ASSERT_VALID(pDoc2);
	if (!pDoc2) return;
	CFileDialog dialog(true); // ファイル読み込み
	if (dialog.DoModal() != IDOK) return; // IDOKじゃないものはreturn
	pDoc2->LoadImage(dialog.GetPathName()); // ユーザが選んだファイルパスはGetPathNameに
	// mFilterType = ID_IMAGE_LOAD;
	// ApplyFilter();
	

	// CImage* pImage2 = pDoc2->GetImage(); // イメージをもらう
	// if (pImage2->IsNull()) return;
	
	// int w2 = pImage2->GetWidth(); // イメージの幅と高さをもらう
	// int h2 = pImage2->GetHeight();
	// if (w2 * h2 > w * h) return;
	mFilterType = ID_IMAGE_LOAD;
	ApplyFilter();

}
*/