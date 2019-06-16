
// Auto_TDXBuy.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "Auto_TDXBuy.h"
#include "Auto_TDXBuyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAuto_TDXBuyApp

BEGIN_MESSAGE_MAP(CAuto_TDXBuyApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CAuto_TDXBuyApp ����

CAuto_TDXBuyApp::CAuto_TDXBuyApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴����ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

static char sMainClass[256] = "TdxW_MainFrame_Class";

void DoTrade(char* sRate, char * sDlgCaption, int iActionID, float fPriceOffset, char * buyCount_Default, char * sBuyBtnTxt)
{
	char sRefreshBtnTxt[256] = "ˢ";
	char sRateBtnTxt[256] = { 0 };

	if ((_tcsicmp(sRate, _T("1")) == 0) ||
		(_tcsicmp(sRate, _T("1/1")) == 0))
	{
		strcpy(sRateBtnTxt, "ȫ��");
	}
	else
		strcpy(sRateBtnTxt, sRate);

	HWND hTDX_MainWnd = ::FindWindowA(sMainClass, NULL);
	if (hTDX_MainWnd)
	{
		HWND hTDX_QuickTradeWnd = ::FindWindowA("#32770", sDlgCaption);
		while (NULL != hTDX_QuickTradeWnd)
		{
			Sleep(10);
			SendMessage(hTDX_QuickTradeWnd, WM_CLOSE, 0, 0L);
			hTDX_QuickTradeWnd = ::FindWindowA("#32770", sDlgCaption);
		}

		/*iActionID=
		��һ������5081���������5082����������5083���ļ�����5084���������5085
		����������5086���߼�����5087��˼�����5088��ż�����5089��ʮ������5090
		��һ������5091����������5092����������5093���ļ�����5094���������5095
		����������5096���߼�����5097���˼�����5098���ż�����5099��ʮ������5100
		��ͣ������5101��ͣ������5102

		��һ������5111���������5112����������5113���ļ�����5114���������5115
		����������5116���߼�����5117��˼�����5118��ż�����5119��ʮ������5120
		��һ������5121����������5122����������5123���ļ�����5124���������5125
		����������5126���߼�����5127���˼�����5128���ż�����5129��ʮ������5130
		��ͣ������5131��ͣ������5132
		*/
		//�������細��
		//::SendMessage((HWND)hTDX_MainWnd, WM_COMMAND, MAKEWPARAM(5085, 0), NULL);
		::SendMessage((HWND)hTDX_MainWnd, WM_COMMAND, MAKEWPARAM(iActionID, 0), NULL);

		hTDX_QuickTradeWnd = NULL;
		while (!hTDX_QuickTradeWnd)
		{
			Sleep(100);
			hTDX_QuickTradeWnd = ::FindWindowA("#32770", sDlgCaption);
		}

		CWnd *pWnd = CWnd::FromHandle(hTDX_QuickTradeWnd)->GetWindow(GW_CHILD);

		CWnd *pCurRateBtnWnd = NULL;
		CWnd *pRefreshBtnWnd = NULL;
		TCHAR szBuf[256];
		while (pWnd != NULL)
		{
			GetClassName(pWnd->m_hWnd, szBuf, 256);
			if (_tcsicmp(szBuf, _T("Button")) == 0)
			{
				char temp1[256] = { 0 };
				pWnd->GetWindowText(temp1, 255);

				if (_tcsicmp(temp1, sRateBtnTxt) == 0)
				{
					pCurRateBtnWnd = pWnd;
					//SendMessage(pCurRateWnd->m_hWnd, BM_CLICK, 0, 0L);
				}
				else if (_tcsicmp(temp1, sRefreshBtnTxt) == 0)
				{
					pRefreshBtnWnd = pWnd;
					//SendMessage(pCurRateWnd->m_hWnd, BM_CLICK, 0, 0L);
				}
			}

			pWnd = pWnd->GetNextWindow();
		}

		pWnd = CWnd::FromHandle(hTDX_QuickTradeWnd)->GetWindow(GW_CHILD);
		int idxEdit = 0;
		int idxButton = 0;
		while (pWnd != NULL)
		{
			GetClassName(pWnd->m_hWnd, szBuf, 256);
			if (_tcsicmp(szBuf, _T("Edit")) == 0)
			{
				idxEdit++;
				if (idxEdit == 1) // 1 �۸�
				{
					char temp2[256] = { 0 };
					char tempNew[256] = { 0 };
					while (strlen(temp2) == 0)
					{
						Sleep(10);
						::SendMessageA(pWnd->m_hWnd, WM_GETTEXT, 256, (LPARAM)temp2);//EDIT�ľ������Ϣ�����ջ�������С�����ջ�����ָ��
					}
					char str[255] = { 0 };
					// �۸� + - 0.001
					sprintf(str, "%.3f", atof(temp2) + fPriceOffset);
					::SendMessageA(pWnd->m_hWnd, WM_SETTEXT, 0, (LPARAM)str);

					while (_tcsicmp(tempNew, str) != 0)
					{
						Sleep(10);
						::SendMessageA(pWnd->m_hWnd, WM_GETTEXT, 256, (LPARAM)tempNew);//EDIT�ľ������Ϣ�����ջ�������С�����ջ�����ָ��
					}

					if (NULL != pRefreshBtnWnd)
						SendMessage(pRefreshBtnWnd->m_hWnd, BM_CLICK, 0, 0L);

					//CWnd::FromHandle(hTDX_QuickTradeWnd)->UpdateWindow();
					Sleep(200);//���⣺��+ 0.001������֪����ʱ�µ���������������ֵҲ���ܲ��䣩
				}
				if (idxEdit == 3) //  3 ����
				{
					if (NULL == pCurRateBtnWnd)
					{
						::SendMessageA(pWnd->m_hWnd, WM_SETTEXT, 0, (LPARAM)buyCount_Default);
						char temp2[256] = { 0 };
						while (strcmp(temp2, buyCount_Default) != 0)
						{
							Sleep(10);
							::SendMessageA(pWnd->m_hWnd, WM_GETTEXT, 256, (LPARAM)temp2);//EDIT�ľ������Ϣ�����ջ�������С�����ջ�����ָ��
						}
					}
					else
					{
						char temp2[256] = { 0 };
						while ((strcmp(temp2, "0") == 0) || (strlen(temp2) == 0))
						{
							SendMessage(pCurRateBtnWnd->m_hWnd, BM_CLICK, 0, 0L);
							Sleep(10);
							SendMessage(pCurRateBtnWnd->m_hWnd, BM_CLICK, 0, 0L);
							Sleep(10);
							::SendMessageA(pWnd->m_hWnd, WM_GETTEXT, 256, (LPARAM)temp2);//EDIT�ľ������Ϣ�����ջ�������С�����ջ�����ָ��
						}

					}
				}
			}
			//else if (_tcsicmp(szBuf, _T("Button")) == 0)
			//{
			//	char temp1[256] = { 0 };
			//	pWnd->GetWindowText(temp1, 255);

			//	if (_tcsicmp(temp1, sRateBtnTxt) == 0)
			//	{
			//		pCurRateBtnWnd = pWnd;
			//		//SendMessage(pCurRateWnd->m_hWnd, BM_CLICK, 0, 0L);
			//	}
			//	else if (_tcsicmp(temp1, sRefreshBtnTxt) == 0)
			//	{
			//		pRefreshBtnWnd = pWnd;
			//		//SendMessage(pCurRateWnd->m_hWnd, BM_CLICK, 0, 0L);
			//	}
			//}

			pWnd = pWnd->GetNextWindow();
		}

		{
			HWND hWnd0 = NULL;
			while (NULL == hWnd0)
			{
				Sleep(10);
				hWnd0 = ::FindWindowExA(hTDX_QuickTradeWnd, NULL, "Button", sBuyBtnTxt);
			}
			//Sleep(100);
			while (NULL != hWnd0)
			{
				Sleep(10);
				SendMessage(hWnd0, BM_CLICK, 0, 0L);
				hWnd0 = ::FindWindowExA(hTDX_QuickTradeWnd, NULL, "Button", sBuyBtnTxt);
			}

			//ί�г�����
			//hTDX_MainWnd = ::FindWindowA(sMainClass, NULL);
			if (hTDX_MainWnd)
			{
				::SendMessage((HWND)hTDX_MainWnd, WM_COMMAND, MAKEWPARAM(5333, 0), NULL);
			}
		}
	}
}

//�޷�������Ʊ��ֻ�ܽ��׵�ǰ��ͼ��ʾ�����Ʊ
void OnBuy(char* sCode, char* sRate, float fTotalRate)
{
	char buyCount_Default[256] = "19000";
	char sDlgCaption[256] = "��������";
	char sBuyBtnTxt[256] = "�� ��";
	int iActionID = 5081;
	float fPriceOffset = 0.001;

	DoTrade(sRate, sDlgCaption, iActionID, fPriceOffset, buyCount_Default, sBuyBtnTxt);
}

//�޷�������Ʊ��ֻ�ܽ��׵�ǰ��ͼ��ʾ�����Ʊ
void OnSell(char* sCode, char* sRate, float fTotalRate)
{
	char buyCount_Default[256] = "19000";
	char sDlgCaption[256] = "��������";
	char sBuyBtnTxt[256] = "��  ��";
	int iActionID = 5121;
	float fPriceOffset = -0.001;

	DoTrade(sRate, sDlgCaption, iActionID, fPriceOffset, buyCount_Default, sBuyBtnTxt);
}

// Ψһ��һ�� CAuto_TDXBuyApp ����

CAuto_TDXBuyApp theApp;


// CAuto_TDXBuyApp ��ʼ��

BOOL CAuto_TDXBuyApp::InitInstance()
{
	if (3 < __argc)
	{
		if (strstr(__argv[0], "Auto_TDXBuy"))
		{
			OnBuy(__argv[1], __argv[2], atof(__argv[3]));
		}
		else if (strstr(__argv[0], "Auto_TDXSell"))
		{
			OnSell(__argv[1], __argv[2], atof(__argv[3]));
		}
	}
	//if ((m_lpCmdLine[0] == _T('/0')) || (lstrcmp(m_lpCmdLine, _T("b1")) != 0))
	//{
	//	m_bCmdRet = TRUE;
	//}
	//else
	//	m_bCmdRet = FALSE;
	//OnSell();
	return FALSE;
	//============================================

	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()��  ���򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ���Windows Native���Ӿ����������Ա��� MFC �ؼ�����������
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CAuto_TDXBuyDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô�����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô�����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "����: �Ի��򴴽�ʧ�ܣ�Ӧ�ó���������ֹ��\n");
		TRACE(traceAppMsg, 0, "����: ������ڶԻ�����ʹ�� MFC �ؼ������޷� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS��\n");
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

#ifndef _AFXDLL
	ControlBarCleanUp();
#endif

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}
