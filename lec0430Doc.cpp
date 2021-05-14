
// lec0430Doc.cpp : Clec0430Doc クラスの実装
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS は、プレビュー、縮小版、および検索フィルター ハンドラーを実装している ATL プロジェクトで定義でき、
// そのプロジェクトとのドキュメント コードの共有を可能にします。
#ifndef SHARED_HANDLERS
#include "lec0430.h"
#endif

#include "lec0430Doc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Clec0430Doc

IMPLEMENT_DYNCREATE(Clec0430Doc, CDocument)

BEGIN_MESSAGE_MAP(Clec0430Doc, CDocument)
END_MESSAGE_MAP()


// Clec0430Doc コンストラクション/デストラクション

Clec0430Doc::Clec0430Doc() noexcept
{
	// PointsNum = 0;
	LinesNum = 0;
}

Clec0430Doc::~Clec0430Doc()
{
}

BOOL Clec0430Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: この位置に再初期化処理を追加してください。
	// (SDI ドキュメントはこのドキュメントを再利用します。

	return TRUE;
}




// Clec0430Doc のシリアル化

void Clec0430Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 格納するコードをここに追加してください。
	}
	else
	{
		// TODO: 読み込むコードをここに追加してください。
	}
}

#ifdef SHARED_HANDLERS

//縮小版のサポート
void Clec0430Doc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// このコードを変更してドキュメントのデータを描画します
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 検索ハンドラーのサポート
void Clec0430Doc::InitializeSearchContent()
{
	CString strSearchContent;
	// ドキュメントのデータから検索コンテンツを設定します。
	// コンテンツの各部分は ";" で区切る必要があります

	// 例:      strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void Clec0430Doc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// Clec0430Doc の診断

#ifdef _DEBUG
void Clec0430Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void Clec0430Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Clec0430Doc コマンド
// インタフェイスの仕様が変わらないときはviewを変えなくてよい

void Clec0430Doc::NewLine(CPoint point, int width, COLORREF color)
{
	Points[LinesNum][0] = point;
	PointsNum[LinesNum] = 1;
	Width[LinesNum] = width;
	Color[LinesNum] = color;
	LinesNum++;

	// Points[0] = point;
	// PointsNum = 1;
	UpdateAllViews(0);
}

void Clec0430Doc::AddPoint(CPoint point)
{
	Points[LinesNum - 1][PointsNum[LinesNum - 1]] = point;
	PointsNum[LinesNum - 1]++;

	// Points[PointsNum] = point;
	// PointsNum++;
	UpdateAllViews(0);
}

void Clec0430Doc::DrawLines(CDC* pDC)
{
	// LinesNumの本数分インクリメント
	CPen* pen, * oldPen;
	for (int i = 0; i < LinesNum; i++) {
		pen = new CPen(PS_SOLID, Width[i], Color[i]); // 新しいカラーと色でペンを作る
		oldPen = pDC->SelectObject(pen);
		pDC->Polyline(Points[i], PointsNum[i]); // ペンで書く
		pDC->SelectObject(oldPen); //元のペンを渡す
		delete pen; // 使い終わったペンは消す
	}
	// pDC->Polyline(Points, PointsNum);
}
