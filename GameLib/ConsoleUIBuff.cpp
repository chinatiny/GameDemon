#include "ConsoleUIBuff.h"
#include <stdio.h>
#include <windows.h>
#pragma comment(lib, "user32.lib")


ConsoleUIBuff * ConsoleUIBuff::s_pConsoleUIBuff = nullptr;


ConsoleUIBuff* ConsoleUIBuff::GetInstance()
{
	if (nullptr == s_pConsoleUIBuff)
	{
		s_pConsoleUIBuff = new ConsoleUIBuff();
	}
	return s_pConsoleUIBuff;
}

void ConsoleUIBuff::Destory()
{
	
	if (nullptr != s_pConsoleUIBuff ->m_pUIBuff)
	{
		delete[] s_pConsoleUIBuff -> m_pUIBuff;
		s_pConsoleUIBuff ->m_pUIBuff = nullptr;
	}
	if (nullptr != s_pConsoleUIBuff->m_pOldUIBuff)
	{
		delete[] s_pConsoleUIBuff->m_pOldUIBuff;
		s_pConsoleUIBuff->m_pOldUIBuff = nullptr;
	}
	if (nullptr != s_pConsoleUIBuff ->pszUIBuffBegin)
	{
		delete[] s_pConsoleUIBuff ->pszUIBuffBegin;
		s_pConsoleUIBuff->pszUIBuffBegin = nullptr;
	}
	if (nullptr != s_pConsoleUIBuff ->pszOldUIBuffBegin)
	{
		delete[] s_pConsoleUIBuff->pszOldUIBuffBegin;
		s_pConsoleUIBuff->pszOldUIBuffBegin = nullptr;
	}

	if (nullptr != s_pConsoleUIBuff)
	{
		delete s_pConsoleUIBuff;
		s_pConsoleUIBuff = nullptr;
	}
}

bool ConsoleUIBuff::SetWindowBuff(const char* pszTitle, int nWidth, int nHeight, int nDrawIntervaltickets)
{
	SetWindowsBaseInfo(pszTitle, nWidth, nHeight);
	//
	m_u64Tickets = 0;
	m_nDrawIntervalTickets = nDrawIntervaltickets;
	m_u64NextDrawTicket = m_u64Tickets + m_nDrawIntervalTickets;
	//
	m_pUIBuff = new UIBuffData[nWidth * nHeight * sizeof(UIBuffData)];
	m_pOldUIBuff = new UIBuffData[nWidth * nHeight * sizeof(UIBuffData)];
	pszUIBuffBegin = new char[(nWidth * 2 + 1) * nHeight * nWidth];
	pszOldUIBuffBegin = new char[(nWidth * 2 + 1) * nHeight * nWidth];
	if (nullptr == m_pUIBuff ||
		nullptr == m_pOldUIBuff ||
		nullptr == pszUIBuffBegin ||
		nullptr == pszOldUIBuffBegin)
		goto __ERROR_EXIT;
	//
	memset(m_pUIBuff, 0, nWidth * nHeight * sizeof(UIBuffData));
	memset(m_pOldUIBuff, 0, nWidth * nHeight * sizeof(UIBuffData));
	memset(pszUIBuffBegin, 0, (nWidth * 2 + 1) * nHeight * nWidth);
	memset(pszOldUIBuffBegin, 0, (nWidth * 2 + 1) * nHeight  * nWidth);
	//
	m_nWidth = nWidth;
	m_nHeight = nHeight;
	m_nPerBuffLen = (nWidth * 2 + 1); //通过动态计算出来，在设置的时候避免重复计算，提高效率
	//设置数组的缓冲区，提前做好内存分配可以避免频繁的内存分配导致的性能损失
	int nCount = 0;
	for (int i = 0; i < nHeight; i++)
	{
		for (int j = 0; j < nWidth; j++)
		{
			int nIndex = i * nWidth + j;
			m_pUIBuff[nIndex].pszBuff = pszUIBuffBegin + nCount * (nWidth * 2 + 1);
			m_pOldUIBuff[nIndex].pszBuff = pszOldUIBuffBegin + nCount * (nWidth * 2 + 1);
			nCount++;
		}
	}
	goto __NORMAL_EXIT;

__ERROR_EXIT:
	if (nullptr != m_pUIBuff)
	{
		delete [] m_pUIBuff;
		m_pUIBuff = nullptr;
	}
	if (nullptr != m_pOldUIBuff)
	{
		delete [] m_pOldUIBuff;
		m_pOldUIBuff = nullptr;
	}
	if (nullptr != pszUIBuffBegin)
	{
		delete [] pszUIBuffBegin;
		pszUIBuffBegin = nullptr;
	}
	if (nullptr != pszOldUIBuffBegin)
	{
		delete[] pszOldUIBuffBegin;
		pszOldUIBuffBegin = nullptr;
	}

	return false;
__NORMAL_EXIT:
	return true;
}

void ConsoleUIBuff::SetDrawIntervalTickets(int nDrawIntervaltickets)
{
	m_u64NextDrawTicket += (nDrawIntervaltickets - m_nDrawIntervalTickets);
	m_nDrawIntervalTickets = nDrawIntervaltickets;

}

void ConsoleUIBuff::UIDrawCallBack()
{
	++GetInstance()->m_u64Tickets;
	if (GetInstance()->m_u64Tickets >= GetInstance()->m_u64NextDrawTicket)
	{
		GetInstance()->DoDrawDelay();
		GetInstance()->m_u64NextDrawTicket = GetInstance()->m_u64Tickets + GetInstance()->m_nDrawIntervalTickets;
	}
}

void ConsoleUIBuff::DoDrawDelay()
{
	for (int i = 0; i < m_nHeight; i++)
	{
		for (int j = 0; j < m_nWidth; j++)
		{
			int nIndex = i  * m_nWidth + j;
			bool bIsColorSame = (m_pUIBuff[nIndex].usColor == m_pOldUIBuff[nIndex].usColor);
			bool bIsStrSame = !strcmp(m_pUIBuff[nIndex].pszBuff, m_pOldUIBuff[nIndex].pszBuff);
			if (bIsStrSame && bIsColorSame) continue;
			//设置之前的状态，方便下次比对，仅仅有改动才执行写
			m_pOldUIBuff[nIndex].usColor = m_pUIBuff[nIndex].usColor;
			strcpy_s(m_pOldUIBuff[nIndex].pszBuff, m_nPerBuffLen, m_pUIBuff[nIndex].pszBuff);
			//画图
			WriteChar(j, i, m_pUIBuff[nIndex].pszBuff, m_pUIBuff[nIndex].usColor);
		}
	}
}

void ConsoleUIBuff::DrawImme(int nX, int nY, char* pszText, unsigned short usColor)
{
	int nIndex = nY  * m_nWidth + nX;
	bool bIsColorSame = (usColor == m_pOldUIBuff[nIndex].usColor);
	bool bIsStrSame = !strcmp(pszText, m_pOldUIBuff[nIndex].pszBuff);
	if (bIsStrSame && bIsColorSame) return;

	m_pUIBuff[nIndex].usColor = usColor;
	strcpy_s(m_pUIBuff[nIndex].pszBuff, m_nPerBuffLen, pszText);
	m_pOldUIBuff[nIndex].usColor = usColor;
	strcpy_s(m_pOldUIBuff[nIndex].pszBuff, m_nPerBuffLen, pszText);

	WriteChar(nX, nY, pszText, usColor);
}

void ConsoleUIBuff::WriteChar(int nX, int nY, const char* pszBuff, unsigned short usColor)
{
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD cdCurSor = { nX * 2, nY };
	SetConsoleCursorPosition(hStdOut, cdCurSor);
	// 设置光标不可见
	CONSOLE_CURSOR_INFO ci = {};
	ci.bVisible = FALSE;
	ci.dwSize = 1;
	SetConsoleCursorInfo(hStdOut, &ci);
	//
	SetConsoleTextAttribute(hStdOut, usColor);
	printf("%s", pszBuff);
}

void ConsoleUIBuff::DrawDelay(int nX, int nY, char *pszText, unsigned short usColor)
{
	int nIndex = nY  * m_nWidth + nX;
	bool bIsColorSame = (usColor == m_pOldUIBuff[nIndex].usColor);
	bool bIsStrSame = !strcmp(pszText, m_pOldUIBuff[nIndex].pszBuff);
	//
	if (bIsStrSame && bIsColorSame) return;
	m_pUIBuff[nIndex].usColor = usColor;
	strcpy_s(m_pUIBuff[nIndex].pszBuff, m_nPerBuffLen, pszText);
}

void ConsoleUIBuff::SetWindowsBaseInfo(const char* pszTitle, int nWidth, int nHeight)
{
		SetConsoleTitleA(pszTitle);
		HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
		COORD cd = GetLargestConsoleWindowSize(hStdOut);
		SetConsoleScreenBufferSize(hStdOut, cd);
		SMALL_RECT rt = { 0, 0, 2 * (nWidth - 1), nHeight-1};
		if (!SetConsoleWindowInfo(hStdOut, true, &rt)) {
			printf("Error:最大窗口宽高为:(%d,%d\n)", cd.X, cd.Y);
		}
		COORD cdNow = { 2 * nWidth, nHeight};// 宽度值
		SetConsoleScreenBufferSize(hStdOut, cdNow);
		//
 		HWND hwStdOut = GetConsoleWindow();
 		SendMessage(hwStdOut, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
}

void ConsoleUIBuff::CleanBuff()
{
	memset(pszUIBuffBegin, 0, (m_nWidth * 2 + 1) * m_nHeight * m_nWidth);
	memset(pszOldUIBuffBegin, 0, (m_nWidth * 2 + 1) * m_nHeight * m_nWidth);

	for (int i = 0; i < m_nHeight; i++)
	{
		for (int j = 0; j < m_nWidth; j++)
		{
			int nIndex = i  * m_nWidth + j;
			m_pUIBuff[nIndex].usColor = 0;
			m_pOldUIBuff[nIndex].usColor = 0;
		}
	}

	system("cls");
}

void ConsoleUIBuff::CleanBuff(int nX, int nY, int nLen)
{
	char *pBlank = new char[nLen + 1];
	memset(pBlank, 0, nLen);
	for (int i = 0; i < nLen; i++)
	{
		pBlank[i] = ' ';
	}
	DrawDelay(nX, nY, pBlank, 0);
	delete[]pBlank;
}


void ConsoleUIBuff::SetCursorUnVisable()
{
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	// 设置光标不可见
	CONSOLE_CURSOR_INFO ci = {};
	ci.bVisible = FALSE;
	ci.dwSize = 1;
	SetConsoleCursorInfo(hStdOut, &ci);
}
