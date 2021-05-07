
// lec0430Doc.h : Clec0430Doc クラスのインターフェイス
//


#pragma once


class Clec0430Doc : public CDocument
{
protected: // シリアル化からのみ作成します。
	Clec0430Doc() noexcept;
	DECLARE_DYNCREATE(Clec0430Doc)

// 属性
public:

// 操作
public:

// オーバーライド
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 実装
public:
	virtual ~Clec0430Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成された、メッセージ割り当て関数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 検索ハンドラーの検索コンテンツを設定するヘルパー関数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	// CPoint Points[1000]; // 1000個の点を保存できる

	CPoint Points[1000][1000]; 
	// int PointsNum;
	int PointsNum[1000];
	int LinesNum;
	void NewLine(CPoint point);
	// void newLine(CPoint point);
	void AddPoint(CPoint point);
	void DrawLines(CDC* pDC);
};
