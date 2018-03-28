; Script Function:
;       Disables trackpad for 500ms any time a key is pressed (prevents accidental mouse clicks)
;

#NoEnv  ; Recommended for performance and compatibility with future AutoHotkey releases.
SendMode Input  ; Recommended for new scripts due to its superior speed and reliability.
SetWorkingDir %A_ScriptDir%  ; Ensures a consistent starting directory.

;keyboard hook code credit: http://www.autohotkey.com/forum/post-127490.html#127490
#Persistent
OnExit, Unhook

hHookKeybd := SetWindowsHookEx(WH_KEYBOARD_LL   := 13, RegisterCallback("Keyboard", "Fast"))
Return

ReenableTrackpad:
BlockInput, MouseMoveOff
Return

Unhook:
UnhookWindowsHookEx(hHookKeybd)
ExitApp

Keyboard(nCode, wParam, lParam)
{
   Critical
   If !nCode
   {
      BlockInput, MouseMove
      SetTimer, ReenableTrackpad, 500
   }
   Return CallNextHookEx(nCode, wParam, lParam)
}

SetWindowsHookEx(idHook, pfn)
{
   Return DllCall("SetWindowsHookEx", "int", idHook, "Uint", pfn, "Uint", DllCall("GetModuleHandle", "Uint", 0), "Uint", 0)
}

UnhookWindowsHookEx(hHook)
{
   Return DllCall("UnhookWindowsHookEx", "Uint", hHook)
}

CallNextHookEx(nCode, wParam, lParam, hHook = 0)
{
   Return DllCall("CallNextHookEx", "Uint", hHook, "int", nCode, "Uint", wParam, "Uint", lParam)
}