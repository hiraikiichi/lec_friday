
// lec0430.h : lec0430 アプリケーションのメイン ヘッダー ファイル
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'pch.h' をインクルードしてください"
#endif

#include "resource.h"       // メイン シンボル


// Clec0430App:
// このクラスの実装については、lec0430.cpp を参照してください
//

class Clec0430App : public CWinApp
{
public:
	Clec0430App() noexcept;


// オーバーライド
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 実装
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern Clec0430App theApp;
