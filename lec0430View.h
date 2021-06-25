
// lec0430View.h : Clec0430View クラスのインターフェイス
//

#pragma once

class Clec0430View : public CView
{
protected: // シリアル化からのみ作成します。
	Clec0430View() noexcept;
	DECLARE_DYNCREATE(Clec0430View)

// 属性
public:
	Clec0430Doc* GetDocument() const;

// 操作
public:

// オーバーライド
public:
	virtual void OnDraw(CDC* pDC);  // このビューを描画するためにオーバーライドされます。
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 実装
public:
	virtual ~Clec0430View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成された、メッセージ割り当て関数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	// CPoint Points[1000]; // 1000個の頂点を保存できる
	// int PointsNum;
	afx_msg void OnPenWidth1();
	afx_msg void OnPenWidth3();
	afx_msg void OnPenWidth5();
private:
	bool mIsLButtonDown; // bool -> false or true しかもたない
	int mPenWidth;
	COLORREF mPenColor;
public:
	afx_msg void OnUpdatePenWidth1(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePenWidth3(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePenWidth5(CCmdUI* pCmdUI);
	afx_msg void OnPenBlack();
	afx_msg void OnPenBlue();
	afx_msg void OnPenColor();
	afx_msg void OnPenRed();
	afx_msg void OnPenGreen();
	afx_msg void OnImageLoad();
	afx_msg void OnImageGray();
	afx_msg void OnImageOriginal();
	afx_msg void OnUpdateImageOriginal(CCmdUI* pCmdUI);
	afx_msg void OnUpdateImageGray(CCmdUI* pCmdUI);
	afx_msg void OnUpdateImage(CCmdUI* pCmdUI);

private:
	int mFilterType;
	CBitmap mFilter;
	CDC mFilterDC;
	CBitmap mBackBuffer;
	CDC mBackBufferDC;
	void ApplyFilter();
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnImageAverage();
	afx_msg void OnUpdateImageAverage(CCmdUI* pCmdUI);
	afx_msg void OnImageAveraged();
	afx_msg void OnUpdateImageAveraged(CCmdUI* pCmdUI);
};

#ifndef _DEBUG  // lec0430View.cpp のデバッグ バージョン
inline Clec0430Doc* Clec0430View::GetDocument() const
   { return reinterpret_cast<Clec0430Doc*>(m_pDocument); }
#endif

