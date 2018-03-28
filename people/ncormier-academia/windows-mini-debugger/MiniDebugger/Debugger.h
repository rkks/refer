// Debugger.h
//

#pragma once

#define TF_BIT			0x100 

namespace MiniDebugger
{

	class Debugger : public CWinThread
	{
		DECLARE_DYNCREATE(Debugger)
	protected:
		Debugger();
		virtual ~Debugger();

	public:
		Debugger(HWND, DWORD);
		Debugger(HWND, CString&);
		bool IsPause();

	private:
		bool CreateSonProcess();
		bool AttachProcess();
		bool ProcessDebugEvents(DEBUG_EVENT);
		bool SendDebugString(CString);
		bool Continue(DWORD, DWORD);
		bool ContinueEveryThread();
		bool StopEveryThread();
		bool ResumeEveryThread();
		bool SingleStepEveryThread();
		bool PostNullMsgEveryThread();

		// Overrides
		//
	public:
		virtual BOOL	InitInstance();
		virtual int		ExitInstance();
		virtual int		Run();

		// Mine
		//
	private:
		// Create Process Info
		//
		STARTUPINFO         _si;
		PROCESS_INFORMATION _pi;
		bool				_pause;
		// Prop.
		//
		CList<DWORD>		_threadList;
		DWORD				_processId;
		
		bool				_attach;
		CString				_path;
		
		HWND				_hWin;
		UINT				_msgID;

		// On Msg Funcs
		//
	private:
		void OnHelloMsg(WPARAM wParam, LPARAM lParam);
		void OnStopMsg(WPARAM wParam, LPARAM lParam);
		void OnStepMsg(WPARAM wParam, LPARAM lParam);
		void OnContinueMsg(WPARAM wParam, LPARAM lParam);
		void OnDetachMsg(WPARAM wParam, LPARAM lParam);
		void OnKillMsg(WPARAM wParam, LPARAM lParam);
		void OnBacktraceMsg(WPARAM wParam, LPARAM lParam);
		void OnBreakpointMsg(WPARAM wParam, LPARAM lParam);

	protected:
	public:
		//afx_msg void OnMsgUI(UINT pParam, long lParam);
		DECLARE_MESSAGE_MAP()
	};


}