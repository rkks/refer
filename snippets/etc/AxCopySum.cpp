/*! \file
    \file AxCopySum.cpp - AxCopySum/AxSumCopy Fast file copy for RAID-systems etc.

    @(#) $Id$
*/
/*! \page License AxCopySum/AxSumCopy/AxSum Fast simple file copy and checksummer.

    Copyright (C) 2005 Svante Seleborg/Axantum Software AB, All rights reserved.
*/
/*! \mainpage AxCopySum/AxSumCopy/AxSum Fast simple file copy and checksummer, for huge files in SAN and RAID-environments

    \author
    Svante Seleborg/Axantum Software AB

    \par License:
    \ref License "Copyright"

    Use asynchronous I/O to start many concurrent read and writes, thereby increasing speed of copy.

    For added security, the result may be compared using an MD5 checksum, and since we then have all
    the logic necessary, we also support a simple MD5-summer mode. The modes are controlled by switches
    as usual in combination with the program name. Depending on how it is named, the mode and the defaults
    change.

    The code uses Win32 I/O completion ports to accomplish I/O concurrency, there is no multi-threading and
    none is needed.

    AxCopySum - Defaults suitable for the main task, fast file copying with optional checksum verification
    AxSumCopy - Defaults to an MD5-summer, but with full options available
    AxSum - A plain MD5-summer, no options
*/

// Require Windows 2000 or higher
#define WINVER 0x0500
#define _WIN32_WINNT 0x0500

#include <windows.h>

// The rest of the includes...
#include <io.h>
#include <shlwapi.h>
#include <stdio.h>
#include <tchar.h>
#include <wincrypt.h>

// Should be included last
#include <strsafe.h>

const int iVersionMajor = 0;
const int iVersionMinor = 92;
const _TCHAR * gszSimpleSummerName =_T("AxSum");
const _TCHAR * gszSummerName = _T("AxSumCopy");

// Forward
namespace my {
    tcprintf(FILE *fp, const _TCHAR *szFmt, ...);
}

/// Get the 'raw' message definition from the system, based on LastError.
/// \return a pointer to a static buffer (and shut off warning about same).
#pragma warning(disable:4172)
const _TCHAR *
LastErrorMsg() {
    // Since this is only intended to be called for fatal failed asserts, including
    // memory allocation, we preallocate a fixed size buffer here.
    static _TCHAR szMsg[1024];

    (void)StringCbCopy(szMsg, sizeof szMsg, _T("Invalid GetLastError()"));
    DWORD dwLastError = GetLastError();
    if (FormatMessage(FORMAT_MESSAGE_IGNORE_INSERTS|FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_MAX_WIDTH_MASK,
        NULL, dwLastError, 0, szMsg, sizeof szMsg / sizeof szMsg[0], NULL) == 0) {
            (void)StringCbPrintf(szMsg,sizeof szMsg, _T("Error Code %d."), dwLastError);
            return szMsg;
    }

    // Overambitious perhaps, but remove trailing space that appears sometimes.
    _TCHAR *s = &szMsg[_tcslen(szMsg)];
    while (s-- != szMsg) {
        if (s[0] == ' ') {
            s[0] = _T('\0');
        } else {
            break;
        }
    }

    return szMsg;
}
#pragma warning(default:4172)

/// If the an assertion fails, format and display a message instead before exiting, by throwing an int exception
/// \param fOk The assertion, if true nothing happens.
/// \param sz The message to display.
/// \param szFile The module name where the assertion is done.
/// \param iLine The line number in the module where the assertion is done.
void
AssFunc(bool fOk, const _TCHAR *sz, const _TCHAR *szFile, int iLine) {
    if (!fOk) {
        my::tcprintf(stderr, _T("%s (Line %d in %s)\n"), sz, iLine, szFile);
        throw int(1);
    }
}

/// If the a check fails, format and display a message instead before exiting, by throwing an int exception.
/// \param fOk The check, if true nothing happens.
/// \param sz The message to display.
void
ChkFunc(bool fOk, const _TCHAR *sz) {
    if (!fOk) {
        my::tcprintf(stderr, _T("%s\n"), sz);
        throw int(2);
    }
}

void
ChkApiFunc(bool fOk, const _TCHAR *szLastError, const _TCHAR *szMsg = _T("")) {
    if (!fOk) {
        if (szMsg && *szMsg) {
            my::tcprintf(stderr, _T("%s (%s)\n"), szMsg, szLastError);
        } else {
            my::tcprintf(stderr, _T("%s\n"), szLastError);
        }
        throw int(3);
    }
}

/// \brief Assert any custom condition
/// \param fOk An expression that must validate to 'true'
/// \param sz A string with a message about the assertion.
#define ASSCHK(fOk, sz) AssFunc(fOk, sz, _T(__FILE__), __LINE__)

/// \brief Assert the result from a Win32 API operation.
///
/// It should be called immediately after using a Win32 API function that may
/// fail, and that sets the GetLastError() error code.
/// The message shown will use the message defintion from Windows.
/// \param fOk An expression that must validate to TRUE
// Do the if to ensure that the condition is evaluted before the call to LastErrorMsg()
#define ASSAPI(fOk) if (!(fOk)) AssFunc(false, LastErrorMsg(), _T(__FILE__), __LINE__)

/// \brief Assert that a pointer is non-NULL
/// \param p A pointer expression that must not be NULL
#define ASSPTR(p) AssFunc((p) != NULL, _T("NULL pointer"), _T(__FILE__), __LINE__)

/// \brief Assert the result from a Win32 API operation.
///
/// It should be called immediately after using a Win32 API function that may
/// fail, and that sets the GetLastError() error code.
/// The message shown will use the message defintion from Windows.
/// \param fOk An expression that must validate to TRUE
// Do the if to ensure that the condition is evaluted before the call to LastErrorMsg()
#define CHKAPI(fOk, sz) if (!(fOk)) ChkApiFunc(false, LastErrorMsg(), sz)

/// \brief Use a private namespace for names that might collide, or confuse the reader
namespace my {
/// \brief TCHAR enabled, Console aware, printf
/// \param szFmt a printf() format string
int
tcprintf(FILE *fp, const _TCHAR *szFmt, ...) {
    va_list vaArgs;
    va_start(vaArgs, szFmt);

    _TCHAR szBuf[4096];
    ASSCHK(SUCCEEDED(StringCbVPrintf(szBuf, sizeof szBuf, szFmt, vaArgs)), _T("Buffer overrun in printf"));
    va_end(vaArgs);

    HANDLE hDest = (HANDLE)_get_osfhandle(fileno(fp));
    if (GetFileType(hDest) == FILE_TYPE_CHAR) {
        DWORD cc;
        ASSAPI(WriteConsole(hDest, szBuf, (DWORD)_tcslen(szBuf), &cc, NULL));
    } else {
        _fputts(szBuf, fp);
    }
    return (int)_tcslen(szBuf);
}
}

/// \brief Small helper template to be able to use auto_ptr-like acces for arrays.
template <class T> class auto_arr {
    T *m_p;
public:
    auto_arr(T *p) { m_p = p; }
    ~auto_arr() { delete[] m_p; }
    T *get() { return m_p; }
};

/// \brief Small helper to format a byte-array as hex
/// \param bytes The byte-array
/// \param cb The number of bytes to format
/// \return A new[]'d c-string. Please delete[]
_TCHAR *
GetHexString(const unsigned char* bytes, size_t cb) {
    _TCHAR *sHex = new _TCHAR[cb + cb + 1];
    const _TCHAR sHexDigits[] = _T("0123456789abcdef");
    for (size_t i = 0; i < cb; i++) {
        sHex[i + i + 0] = sHexDigits[bytes[i] >> 4];
        sHex[i + i + 1] = sHexDigits[bytes[i] & 0xf];
    }
    sHex[cb + cb] = _T('\0');
    return sHex;
}

// Forward
VOID CALLBACK
ReadNextBuffer(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED pOverlapped);
VOID CALLBACK
WriteThisBuffer(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED pOverlapped);

// Disable warning about not being able to generate assignment operator. We don't want one.
#pragma warning(disable:4512)
/// \brief Do copy and/or checksum using async I/O and many buffers
class CAxCopySum {
private:
    /// \brief The frequency for progress display
    const unsigned long m_timeProgressMilliseconds;

    bool m_fVerify;                         ///< True if we're checkumming the input file
    int m_iVerbosityLevel;                  ///< 0 means fully quiet, 1 normal, 2 or higher for more trace

    int m_countOutstanding;                 ///< The number of currently active buffers, waiting for read or write completion
    int m_nBuffers;                         ///< The number of read/write's to use simultaneously
    size_t m_cbBufferSize;                  ///< The buffer size in bytes

    OVERLAPPED *m_pOverlapped;              ///< m_nBuffers of overlapped structures for the ReadEx/WriteEx calls
    typedef struct {
        BYTE *buffer;                       ///< m_cbBufferSize buffer
        bool fRestartRead;                  ///< flag indicating if a read-restart is necessary
        bool fChecksumPending;              ///< Waiting for checksum to be done
    } BufferStateT;
    BufferStateT *m_pBufferState;           ///< m_nBuffers buffer state structures

    HCRYPTPROV m_hCryptProv;                ///< The provider used for hashing
    HCRYPTHASH m_hHash;                     ///< The active hash
    unsigned char m_hash[16];               ///< The resulting hash-buffer

    LARGE_INTEGER m_offsetNextRead;         ///< The file offset for the next read
    LARGE_INTEGER m_offsetFileSize;         ///< The input file length
    LARGE_INTEGER m_cbBytesWritten;         ///< The number of bytes written so far
    LARGE_INTEGER m_offsetNextChecksum;     ///< The next offset to checksum (we must do this in order)

    DWORD m_timeLastProgress;               ///< The time-stamp for the last progress display
    bool m_fNeedNewLine;                    ///< Pending new line neded (progress display stays on the same line)

    const _TCHAR *m_szFileIn;               ///< The expanded full input file name
    const _TCHAR *m_szFileOut;              ///< The expanded full input file name
    HANDLE m_hFileIn;                       ///< The input file
    HANDLE m_hFileOut;                      ///< The output file (INVALID_HANDLE_VALUE if no output)
    DWORD m_cbSectorSizeIn;                 ///< What the name says
    DWORD m_cbSectorSizeOut;                ///< What the name says
    bool fInIsNetwork;                      ///< true if the input is from a network share
    bool fOutIsNetwork;                     ///< true if the output is on a network share

public:
    /// \brief Initialize the copy/summer
    /// \param nBuffers The number of buffers to use
    /// \param cbBufferSize The size of the buffers to use
    /// \param iVerbosityLevel 0 for none, 1 for normal
    /// \param fVerify true if we are to hash as well
    CAxCopySum(int nBuffers, size_t cbBufferSize, int iVerbosityLevel, bool fVerify) : m_timeProgressMilliseconds(1000) {
        m_nBuffers = nBuffers;
        m_cbBufferSize = cbBufferSize;
        m_iVerbosityLevel = iVerbosityLevel;
        m_fVerify = fVerify;

        m_fNeedNewLine = false;
        m_hCryptProv = NULL;
        m_hHash = NULL;
        m_offsetNextRead.QuadPart = 0;
        m_offsetFileSize.QuadPart = 0;
        m_cbBytesWritten.QuadPart = 0;
        m_timeLastProgress = 0;
        m_offsetNextChecksum.QuadPart = 0;
        m_hFileIn = m_hFileOut = INVALID_HANDLE_VALUE;

        m_pOverlapped = NULL;
        m_pBufferState = NULL;

        memset(m_hash, 0, sizeof m_hash);

        // Allocate all memory structures
        m_pOverlapped = new OVERLAPPED[m_nBuffers];
        ASSPTR(m_pOverlapped);
        memset(m_pOverlapped, 0, sizeof *m_pOverlapped * m_nBuffers);

        m_pBufferState = new BufferStateT[m_nBuffers];
        ASSPTR(m_pBufferState);
        memset(m_pBufferState, 0, sizeof *m_pBufferState * m_nBuffers);

        if (m_fVerify) {
            ASSAPI(CryptAcquireContext(&m_hCryptProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT | CRYPT_MACHINE_KEYSET) != 0);
        }
    }


    ~CAxCopySum() {
        Free();
    }

    /// \brief An exception catching wrapper for the real work, checksum and/or copy
    /// \param szFileIn The input file to read from
    /// \param szFileOut The output file, or NULL if no copy just summing
    /// \return The status code to return. Zero for ok.
    int CopyOrVerify(const _TCHAR *szFileIn, const _TCHAR *szFileOut) {
        int iReturn;
        try {
            iReturn = Process(szFileIn, szFileOut);
        } catch (int iStatus) {
            // No assertion checks here, just best effort to clean up
            if (m_hFileIn != INVALID_HANDLE_VALUE) {
                CancelIo(m_hFileIn);
            }
            // Only delete the destination if we actually managed to open it
            if (m_hFileOut != INVALID_HANDLE_VALUE) {
                CancelIo(m_hFileOut);
                CloseHandle(m_hFileOut);
                m_hFileOut = INVALID_HANDLE_VALUE;
                DeleteFile(m_szFileOut);
            }
            iReturn = iStatus;
        }
        return iReturn;
    }

    /// \brief Get the input file size
    /// \return The input file size
    const LARGE_INTEGER &GetFileSize() {
        return m_offsetFileSize;
    }

    /// \brief Get the binary bytes of the hash
    /// \return The hash
    const unsigned char *GetHash() {
        return m_hash;
    }

    /// \brief Get the length of the hash
    /// \return The length of the hash buffer in bytes
    size_t GetHashLen() {
        return sizeof m_hash;
    }

private:

    /// \brief Calculate length of vali data in buffer - either a full buffer, or a partial last
    /// \return The length in bytes of valid data in the buffer
    size_t BufferLen(int index) {
        LARGE_INTEGER offsetThis;
        offsetThis.LowPart = m_pOverlapped[index].Offset;
        offsetThis.HighPart = m_pOverlapped[index].OffsetHigh;
        if (offsetThis.QuadPart + m_cbBufferSize > m_offsetFileSize.QuadPart) {
            return (size_t)(m_offsetFileSize.QuadPart - offsetThis.QuadPart);
        } else {
            return m_cbBufferSize;
        }
    }

    friend VOID CALLBACK
    ::WriteThisBuffer(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED pOverlapped);

    /// \brief Called upon completion of Read, to write the buffer
    /// \param dwErrorCode Set by Windows to indicate success or not
    /// \param dwNumberOfBytesTransfered The actual number of bytes transfered
    /// \param pOverlapped The overlapped structure controlling this transfer
    void
    WriteThisBuffer(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED pOverlapped) {
        ASSAPI((SetLastError(dwErrorCode), dwErrorCode == ERROR_SUCCESS));
        ASSCHK(dwNumberOfBytesTransfered > 0, _T("Nothing transferred!"));

        ProgressDisplay();

        int index = (int)(pOverlapped - m_pOverlapped);

        // Verify that the right number of bytes were transfered
        ASSCHK(BufferLen(index) == dwNumberOfBytesTransfered, _T("Read error"));

        if (m_fVerify) {
            m_pBufferState[index].fChecksumPending = true;
        }

        // According to the docs, we always need to write an even number of sectors to write the way we do things,
        // so we round upwards here.
        dwNumberOfBytesTransfered += (m_cbSectorSizeOut - dwNumberOfBytesTransfered % m_cbSectorSizeOut) % m_cbSectorSizeOut;

        if (m_hFileOut != INVALID_HANDLE_VALUE) {
            // The documentation lies. If all is very ok, it doesn't change last error to ERROR_SUCCESS it just returns TRUE
            SetLastError(ERROR_SUCCESS);
            ASSAPI(WriteFileEx(m_hFileOut, m_pBufferState[index].buffer, dwNumberOfBytesTransfered, pOverlapped, ::ReadNextBuffer) != 0);
            ASSAPI(GetLastError() == ERROR_SUCCESS);
            
            if (m_fVerify) {
                // We can start checksumming (if required) now that we've initiated the write
                m_pBufferState[index].fChecksumPending = !Checksum(index);
            }
        } else {
            ReadNextBuffer(ERROR_SUCCESS, ~DWORD(0), pOverlapped);
        }
    }

    friend VOID CALLBACK
    ::ReadNextBuffer(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED pOverlapped);

    /// \brief Called upon completion of Write (or at startup time)
    /// \param dwErrorCode Reslut from Windows
    /// \param dwNumberOfBytesTransfered from the WriteEx, or ~0 to indicate a separately initiated read
    /// \param pOverlapped The OVERLAPPED structure used for this transfer
    void
    ReadNextBuffer(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED pOverlapped) {
        ASSAPI((SetLastError(dwErrorCode), dwErrorCode == ERROR_SUCCESS));
        ASSCHK(dwNumberOfBytesTransfered > 0, _T("Nothing transferred"));

        ProgressDisplay();

        int index = (int)(pOverlapped - m_pOverlapped);

        // Add to the number of bytes actually written etc
        if (dwNumberOfBytesTransfered != ~0) {
            // The actual bytes written may not correspond exactly to the valid data length due to sector alignment
            m_cbBytesWritten.QuadPart += BufferLen(index);
        }

        // We cannot start a new read, unless this buffer has been checksummed. If it has not, we do not start a new read
        // here, instead we return, and let the wait loop check if there is more to do.
        if (m_pBufferState[index].fChecksumPending) {
            m_pBufferState[index].fRestartRead = true;
            return;
        }

        // We're all done with a read(-checksum(-write)) cycle
        m_countOutstanding--;

        // Check if end of file
        if (m_offsetNextRead.QuadPart >= m_offsetFileSize.QuadPart) {
            return;
        }

        if (m_iVerbosityLevel >= 6) {
            CheckNewLineNeed();
            my::tcprintf(stderr, _T("Initiating read at %I64d\n"), m_offsetNextRead.QuadPart);
        }

        pOverlapped->Offset = m_offsetNextRead.LowPart;
        pOverlapped->OffsetHigh = m_offsetNextRead.HighPart;  

        // Read not more than what is required to read a sector past the end of the file
        size_t cbBytesToRead = m_cbBufferSize;
        if (m_offsetNextRead.QuadPart + cbBytesToRead > m_offsetFileSize.QuadPart) {
            cbBytesToRead = (size_t)(m_offsetFileSize.QuadPart - m_offsetNextRead.QuadPart);
            size_t cbPartialBytesInSector = cbBytesToRead % m_cbSectorSizeIn;
            if (cbPartialBytesInSector) {
                cbBytesToRead += m_cbSectorSizeIn - cbPartialBytesInSector;
            }
        }

        m_offsetNextRead.QuadPart += cbBytesToRead;
        // Ensure the statistics reflect actual bytes, not extra bytes in the final sector
        if (m_offsetNextRead.QuadPart > m_offsetFileSize.QuadPart) {
            m_offsetNextRead.QuadPart = m_offsetFileSize.QuadPart;
        }

        // The documentation lies. If all is very ok, it doesn't change last error to ERROR_SUCCESS it just returns TRUE
        SetLastError(ERROR_SUCCESS);
        m_countOutstanding++;
        ASSAPI(ReadFileEx(m_hFileIn, m_pBufferState[index].buffer, (DWORD)cbBytesToRead, pOverlapped, ::WriteThisBuffer) != 0);
        ASSAPI(GetLastError() == ERROR_SUCCESS);
    }

    /// \brief The the real work, checksum and/or copy
    /// \param szFileIn The input file to read from
    /// \param szFileOut The output file, or NULL if no copy just summing
    /// \return The status code to return. Zero for ok.
    int Process(const _TCHAR *szFileIn, const _TCHAR *szFileOut) {
        SetFiles(szFileIn, szFileOut);

        m_hFileIn = CreateFile(m_szFileIn, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, (fInIsNetwork ? 0 : FILE_FLAG_NO_BUFFERING)|FILE_FLAG_OVERLAPPED, NULL);
        CHKAPI(m_hFileIn != INVALID_HANDLE_VALUE, _T("Could not open input file"));

        FILETIME ftCT = {0}, ftLAT = {0}, ftLWT = {0};
        ASSAPI(GetFileTime(m_hFileIn, &ftCT, &ftLAT, &ftLWT) == TRUE);

        m_offsetNextChecksum.QuadPart = m_offsetNextRead.QuadPart = 0;
        ASSAPI(GetFileSizeEx(m_hFileIn, &m_offsetFileSize) == TRUE);
        if (m_iVerbosityLevel >= 2) {
            CheckNewLineNeed();
            my::tcprintf(stderr, _T("The input file size is %I64d bytes\n"), m_offsetFileSize.QuadPart);
        }

        if (m_szFileOut) {
            m_hFileOut = CreateFile(m_szFileOut, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,  (fOutIsNetwork ? 0 : FILE_FLAG_NO_BUFFERING)|FILE_FLAG_OVERLAPPED, NULL);
            CHKAPI(m_hFileOut != INVALID_HANDLE_VALUE, _T("Could not open output file"));

            // Check for enough space
            LARGE_INTEGER offsetRoundedSize = m_offsetFileSize;
            offsetRoundedSize.QuadPart += (m_cbSectorSizeOut - offsetRoundedSize.QuadPart % m_cbSectorSizeOut) % m_cbSectorSizeOut;
            CHKAPI(SetFilePointerEx(m_hFileOut, offsetRoundedSize, NULL, FILE_BEGIN) != 0, _T("Could not reserve disk space"));
            CHKAPI(SetEndOfFile(m_hFileOut) != 0, _T("Could not reserve disk space"));
        }

        if (m_fVerify) {
            ASSAPI(CryptCreateHash(m_hCryptProv, CALG_MD5, 0, 0, &m_hHash) != 0);
        }

        // Now we're ready to do the actual work!
        m_fNeedNewLine = false;

        m_countOutstanding = 0;
        m_cbBytesWritten.QuadPart = 0;

        m_timeLastProgress = GetTickCount() - m_timeProgressMilliseconds;

        for (int i = 0; i < m_nBuffers; i++) {
            m_pOverlapped[i].hEvent = (HANDLE)this;
            m_pOverlapped[i].Offset = ~DWORD(0);
            m_pOverlapped[i].OffsetHigh = ~DWORD(0);
            ASSPTR((m_pBufferState[i].buffer = (BYTE *)VirtualAlloc(NULL, m_cbBufferSize, MEM_COMMIT, PAGE_READWRITE)));

            m_pBufferState[i].fRestartRead = false;
            m_pBufferState[i].fChecksumPending = false;

            // Compensate for the fact that ReadNextBuffer implies that a request is finished.
            m_countOutstanding++;
            ReadNextBuffer(ERROR_SUCCESS, (DWORD)~0, &m_pOverlapped[i]);
        }

        DWORD dwEvent;
        do {
            dwEvent = SleepEx(m_timeProgressMilliseconds/2, TRUE);
            ASSCHK(dwEvent == WAIT_IO_COMPLETION || dwEvent == 0, _T("Unexpected return value from SleepEx()"));
            switch (dwEvent) {
                case 0:
                    ProgressDisplay();
                    if (m_iVerbosityLevel >= 5) {
                        CheckNewLineNeed();
                        my::tcprintf(stderr, _T("SleepEx returned TIMEOUT\n"));
                    }
                    break;
                case WAIT_IO_COMPLETION:
                    if (m_iVerbosityLevel >= 5) {
                        CheckNewLineNeed();
                        my::tcprintf(stderr, _T("SleepEx returned WAIT_IO_COMPLETION\n"));
                    }

                    if (m_fVerify) {
                        // We may need several passes through the buffer list to do as much as we can.
                        int nPendingChecksums = 0;
                        int i = 0;
                        while (i < m_nBuffers) {
                            if (m_pBufferState[i].fChecksumPending) {
                                m_pBufferState[i].fChecksumPending = !Checksum(i);
                                if (!m_pBufferState[i].fChecksumPending) {
                                    if (m_pBufferState[i].fRestartRead) {
                                        m_pBufferState[i].fRestartRead = false;
                                        ReadNextBuffer(ERROR_SUCCESS, ~DWORD(0), &m_pOverlapped[i]);
                                    }
                                    // Since we did fix a checksum, restart the scan
                                    i = nPendingChecksums = 0;
                                    continue;
                                }
                            }
                            // Count the number of buffers where we've waited past the write completion to do the checksum
                            if (m_pBufferState[i].fRestartRead) {
                                nPendingChecksums++;
                            }
                            i++;
                        }
                        // If all buffers are waiting for checksum calculation, something is badly wrong
                        ASSCHK(nPendingChecksums < m_nBuffers, _T("Internal error in checksum sequencing logic"));
                    }
                    break;
            }
        } while (m_countOutstanding > 0 || m_offsetNextRead.QuadPart < m_offsetFileSize.QuadPart);
        ProgressDisplay(true);

        CheckNewLineNeed();

        ASSAPI(CloseHandle(m_hFileIn));
        m_hFileIn = INVALID_HANDLE_VALUE;

        if (m_hFileOut != INVALID_HANDLE_VALUE) {
            ASSAPI(FlushFileBuffers(m_hFileOut) != 0);
            ASSAPI(CloseHandle(m_hFileOut) == TRUE);

            // Adjust file size and times to the correct values
            m_hFileOut = CreateFile(m_szFileOut, GENERIC_WRITE, 0, NULL, OPEN_EXISTING,  FILE_ATTRIBUTE_NORMAL, NULL);
            ASSAPI(m_hFileOut != INVALID_HANDLE_VALUE);
            ASSAPI(SetFilePointerEx(m_hFileOut, m_offsetFileSize, NULL, FILE_BEGIN) != 0);
            ASSAPI(SetEndOfFile(m_hFileOut) != 0);
            ASSAPI(SetFileTime(m_hFileOut, &ftCT, &ftLAT, &ftLWT) != 0);
            ASSAPI(FlushFileBuffers(m_hFileOut) != 0);
            ASSAPI(CloseHandle(m_hFileOut));
            m_hFileOut = INVALID_HANDLE_VALUE;

            // check that we've really written all
            ASSCHK(m_cbBytesWritten.QuadPart == m_offsetFileSize.QuadPart, _T("Incorrect number of bytes written"));
        }

        if (m_fVerify) {
            DWORD cbHashLen = sizeof m_hash;
            ASSAPI(CryptGetHashParam(m_hHash, HP_HASHVAL, (unsigned char *)&m_hash, &cbHashLen, 0) != 0);
            ASSCHK(cbHashLen == sizeof m_hash, _T("Unexpected hash length"));
            if (m_iVerbosityLevel >= 3) {
                CheckNewLineNeed();
                my::tcprintf(stderr, _T("MD5 of file read is %s\n"), auto_arr<_TCHAR>(GetHexString(m_hash, sizeof m_hash)));
            }
        }

        if (m_hHash) {
            ASSAPI(CryptDestroyHash(m_hHash) != 0);
            m_hHash = NULL;
        }

        return 0;
    }

    /// \brief Get disk parameters
    /// \param szFilePath A complete path to the file
    /// \param pfIsNetWork Pointer to bool to return an indicator if the file is a network share
    /// \return The size in bytes of the sector of the disk where the file resides
    DWORD GetDiskInfo(const _TCHAR *szFilePath, bool *pfIsNetwork) {
        _TCHAR szWork[MAX_PATH];
        StringCbCopy(szWork, sizeof szWork, szFilePath);

        // Assume here that this is converted to a full path!!!
        ASSCHK(PathStripToRoot(szWork) == TRUE, _T("Can't extract root path spec"));

        DWORD ciSectorsPerCluster = 0, cbBytesPerSector = 0, ciNumberOfFreeClusters = 0, ciTotalNumberOfClusters = 0;
        ASSAPI(GetDiskFreeSpace(szWork, &ciSectorsPerCluster, &cbBytesPerSector, &ciNumberOfFreeClusters, &ciTotalNumberOfClusters) != 0);
        ASSCHK(cbBytesPerSector != 0, _T("Bytes per sector reported as zero"));

        *pfIsNetwork = (GetDriveType(szWork) == DRIVE_REMOTE);
        // The API sucks at times... UNC paths, i.e. \\server\share\etc does not work with GetDriveType.
        *pfIsNetwork = *pfIsNetwork || (_tcsnicmp(szWork, _T("\\\\"), 2) == 0) || (_tcsnicmp(szWork, _T("\\?"), 2) == 0);

        if (m_iVerbosityLevel >= 4) {
            CheckNewLineNeed();
            my::tcprintf(stderr, _T("RootPath of %s is %s,\nthe sector size is %ld and %s a network drive\n"), szFilePath, szWork, cbBytesPerSector, *pfIsNetwork ? _T("is") : _T("is not"));
        }

        return cbBytesPerSector;
    }

    /// \brief Conditionally add this buffer to the running checksum
    ///
    /// Although the read and write may be done out of order, the checksumming must be done in correct order,
    /// so we check here if this is correct next buffer to checksum.
    /// \return true if we did do checksum (or we're not in checksum mode)
    /// \param index The buffer index in question
    bool Checksum(int index) {
        LARGE_INTEGER offsetThis;
        offsetThis.LowPart = m_pOverlapped[index].Offset;
        offsetThis.HighPart = m_pOverlapped[index].OffsetHigh;
        if (offsetThis.QuadPart == m_offsetNextChecksum.QuadPart) {
            size_t cbBytesToChecksum = BufferLen(index);

            if (m_iVerbosityLevel >= 6) {
                unsigned int sum = 0;
                for (size_t i=0; i < cbBytesToChecksum; i++) {
                    sum += m_pBufferState[index].buffer[i];
                }
                CheckNewLineNeed();
                my::tcprintf(stderr, _T("Checksumming at offset %I64d, quicksum %ld, datalen %ld\n"), offsetThis.QuadPart, sum, cbBytesToChecksum);
            }

            // Do the checksum here
            ASSAPI(CryptHashData(m_hHash, m_pBufferState[index].buffer, (DWORD)cbBytesToChecksum, 0));

            // Update the running offset for next checksum
            m_offsetNextChecksum.QuadPart += cbBytesToChecksum;
            // Mark this as done
            m_pBufferState[index].fChecksumPending = false;
            return true;
        } else {
            if (m_iVerbosityLevel >= 6) {
                CheckNewLineNeed();
                my::tcprintf(stderr, _T("Checksum miss, this is at %I64d, next checksum is at %I64d\n"), offsetThis.QuadPart, m_offsetNextChecksum.QuadPart);
            }
            return false;
        }
    }

    /// \brief Display progress. Called from the Async callbacks. Timers etc do not work well in this context.
    /// \param fForceDisplay Set to true if a progress display is to made regardless of other conditions
    void ProgressDisplay(bool fForceDisplay = false) {
        if (((GetTickCount() - m_timeLastProgress) > m_timeProgressMilliseconds && m_offsetNextRead.QuadPart > 0) || fForceDisplay) {
            // Only display if the right verbosity and we're actually writing to a console. This is only for interactive consumption.
            if (m_iVerbosityLevel >= 1 && GetFileType(GetStdHandle(STD_ERROR_HANDLE)) == FILE_TYPE_CHAR) {
                unsigned int percent = 100;
                if (m_cbBytesWritten.QuadPart) {
                    if ((m_offsetFileSize.QuadPart / 100)!= 0) {
                        percent = (unsigned int)(m_cbBytesWritten.QuadPart / (m_offsetFileSize.QuadPart / 100));
                    }
                    my::tcprintf(stderr, _T("%I64d bytes read, %I64d bytes written (%d%%)\r"), m_offsetNextRead.QuadPart, m_cbBytesWritten.QuadPart, percent);
                } else {
                    if ((m_offsetFileSize.QuadPart / 100) != 0) {
                        percent = (unsigned int)(m_offsetNextRead.QuadPart / (m_offsetFileSize.QuadPart / 100));
                    }
                    my::tcprintf(stderr, _T("%I64d bytes read (%d%%)\r"), m_offsetNextRead.QuadPart, percent);
                }
                SetNewLineNeed();
            }
            m_timeLastProgress = GetTickCount();
        }
    }

    /// \brief Set the names of the files to used, and calculate basic parameters
    /// \param szFileIn The input file to read from
    /// \param szFileOut The output file, or NULL if no copy just summing
    void SetFiles(const _TCHAR *szFileIn, const _TCHAR *szFileOut) {
        m_szFileIn = szFileIn;
        m_szFileOut = szFileOut;

        m_cbSectorSizeIn = GetDiskInfo(szFileIn, &fInIsNetwork);
        ASSCHK(m_cbSectorSizeIn > 0, _T("Sector size in reported as zero"));

        if (szFileOut) {
            m_cbSectorSizeOut = GetDiskInfo(szFileOut, &fOutIsNetwork);
        } else {
            m_cbSectorSizeOut = m_cbSectorSizeIn;
        }
        ASSCHK(m_cbSectorSizeOut > 0, _T("Sector size out reported as zero"));

        if (m_iVerbosityLevel >= 4) {
            CheckNewLineNeed();
            my::tcprintf(stderr, _T("Input sector size is %d, Output sector size is %d\n"), m_cbSectorSizeIn, m_cbSectorSizeOut);
        }

        DWORD cbSmallestCommonMultiple = m_cbSectorSizeIn > m_cbSectorSizeOut ? m_cbSectorSizeIn : m_cbSectorSizeOut;
        DWORD cbSectorSizeSmall = m_cbSectorSizeIn > m_cbSectorSizeOut ? m_cbSectorSizeOut : m_cbSectorSizeIn;

        // Primitive algorithm to find the smallest common multiple. Normally this is a no-brainer...
        // The result is the smallest possible buffer, technically.
        while (cbSmallestCommonMultiple % cbSectorSizeSmall) {
            cbSmallestCommonMultiple += cbSectorSizeSmall;
        }

        if (m_iVerbosityLevel >= 4) {
            CheckNewLineNeed();
            my::tcprintf(stderr, _T("Smallest common sector size multiple is %d\n"), cbSmallestCommonMultiple);
        }
        ASSCHK(cbSmallestCommonMultiple > 0, _T("Could not calculate smallest common multiple sector size"));

        // Calculate the actdual buffer size to use
        if (m_iVerbosityLevel >= 5) {
            CheckNewLineNeed();
            my::tcprintf(stderr, _T("Requested buffer size is %d\n"), m_cbBufferSize);
        }

        if (m_cbBufferSize <= cbSmallestCommonMultiple) {
            m_cbBufferSize = cbSmallestCommonMultiple;
        } else {
            // Attempted buffer size is larger the smallest technically possible, now we round to an even
            // multiple of the smallest common multiple.
            m_cbBufferSize -= m_cbBufferSize % cbSmallestCommonMultiple;
        }
        if (m_iVerbosityLevel >= 4) {
            CheckNewLineNeed();
            my::tcprintf(stderr, _T("Finally calculated buffer size %d\n"), m_cbBufferSize);
        }

        ASSCHK(m_cbBufferSize > 0, _T("Buffer size is zero"));
    }

    void CheckNewLineNeed() {
        if (m_fNeedNewLine) {
            my::tcprintf(stderr, _T("\n"));
            m_fNeedNewLine = false;
        }
    }

    void SetNewLineNeed() {
        m_fNeedNewLine = true;
    }

    /// \brief Free all remaining resources
    void Free() {
        if (m_hHash) {
            ASSAPI(CryptDestroyHash(m_hHash) != 0);
            m_hHash = NULL;
        }

        if (m_hCryptProv) {
            ASSAPI(CryptReleaseContext(m_hCryptProv, 0) != 0);
            m_hCryptProv = NULL;
        }

        // We're finished!
        if (m_pBufferState != NULL) {
            for (int i = 0; i < m_nBuffers; i++) {
                if (m_pBufferState[i].buffer != NULL) {
                    ASSAPI(VirtualFree(m_pBufferState[i].buffer, 0, MEM_RELEASE));
                    m_pBufferState[i].buffer = NULL;
                }
            }
        }

        delete[] m_pBufferState;
        m_pBufferState = NULL;

        delete[] m_pOverlapped;
        m_pOverlapped = NULL;

        if (m_hFileIn != INVALID_HANDLE_VALUE) {
            ASSAPI(CloseHandle(m_hFileIn));
            m_hFileIn = INVALID_HANDLE_VALUE;
        }

        if (m_hFileOut != INVALID_HANDLE_VALUE) {
            ASSAPI(CloseHandle(m_hFileOut));
            m_hFileOut = INVALID_HANDLE_VALUE;
        }
    }
}; // CAxCopySum
#pragma warning(default:4512)

// Called when Write completes, to read next buffer. This is only to get back into the class.
VOID CALLBACK
ReadNextBuffer(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED pOverlapped) {
    CAxCopySum *pThis = (CAxCopySum *)pOverlapped->hEvent;
    pThis->ReadNextBuffer(dwErrorCode, dwNumberOfBytesTransfered, pOverlapped);
}

// Called upon completion of Read, to write the buffer.  This is only to get back into the class.
VOID CALLBACK
WriteThisBuffer(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED pOverlapped) {
    CAxCopySum *pThis = (CAxCopySum *)pOverlapped->hEvent;
    pThis->WriteThisBuffer(dwErrorCode, dwNumberOfBytesTransfered, pOverlapped);
}

/// \brief in this mode, we're just a simple md5-summer
class CSimpleSummerMode {
private:
    int nBuffers;
    size_t cbBufferSize;
    int iVerbosityLevel;
    bool fVerify;
    bool fChecksumOnly;

public:
    CSimpleSummerMode() {
        nBuffers = 16;                      ///< Set default buffers
        cbBufferSize = 2*1024*1024;         ///< Set default buffer size
        iVerbosityLevel = 0;                ///< Set default verbosity level
        fVerify = true;                     ///< Set default verify option
    }

    int Main(int argc, _TCHAR* argv[], _TCHAR *szProgramName) {
        if (argc == 1) {
            Usage(szProgramName, nBuffers, cbBufferSize, fVerify, fChecksumOnly);
            return 1;
        }

        bool fSingleFile = argc == 2;

        if (!fSingleFile && iVerbosityLevel >= 0) {
            Copyright(szProgramName);
        }

        _TCHAR szFileIn[MAX_PATH];
        CAxCopySum theSummer(nBuffers, cbBufferSize, iVerbosityLevel, true);

        for (int i = 1; i < argc; i++) {
            TCHAR *szIgnoreFileName;
            DWORD ccResult = GetFullPathName(argv[i], sizeof szFileIn / sizeof *szFileIn, szFileIn, &szIgnoreFileName);
            ASSCHK(ccResult > 0 && ccResult <= sizeof szFileIn / sizeof *szFileIn, _T("Could not get full in pathname"));
    
            if (iVerbosityLevel >= 4) {
                my::tcprintf(stderr, _T("GetFullPathName for in gives: %s\n"), szFileIn);
            }
            int iReturn = theSummer.CopyOrVerify(szFileIn, NULL);
            if (iReturn) {
                return iReturn;
            }
            if (fSingleFile) {
                my::tcprintf(stderr, _T("%s\n"), auto_arr<_TCHAR>(GetHexString(theSummer.GetHash(), theSummer.GetHashLen())));
            } else {
                my::tcprintf(stderr, _T("%s %s\n"), auto_arr<_TCHAR>(GetHexString(theSummer.GetHash(), theSummer.GetHashLen())), argv[i]);
            }
        }

        return 0;
    }

    void Copyright(_TCHAR *sName) {
        my::tcprintf(stderr,
            _T("%s %d.%d - Pretty fast file MD5 checksummer\n")
            _T("Copyright 2005 Axantum Software AB, Svante Seleborg\n\n")
            ,sName, iVersionMajor, iVersionMinor
            );
    }

    void Usage(_TCHAR *sName, int nBuffers, size_t cbBufferSize, bool fVerify, bool fChecksumOnly) {
        Copyright(sName);
        my::tcprintf(stderr,
            _T("Usage: %s InFile[s]\n\n")
            _T("If only one file is specified, only the MD5 is displayed.\n")
            _T("If two or more files are specified, the MD5 and names are shown.\n")
            ,
            sName, nBuffers, cbBufferSize/1024, fVerify ? _T("ON") : _T("OFF"), fChecksumOnly ? _T(" (default)") : _T(""));
    }
};

// forward
namespace my {
extern TCHAR *optarg;
extern int optind;
int getopt(int argc, TCHAR *argv[], TCHAR *optstring);
}

class CFullMode {
    int m_nBuffers;
    size_t m_cbBufferSize;
    int m_iVerbosityLevel;
    bool m_fVerify;
    bool m_fChecksumOnly;
    bool m_fDisplayChecksum;

public:
    CFullMode() {
        m_nBuffers = 32;                      ///< Set default buffers
        m_cbBufferSize = 2*1024*1024;         ///< Set default buffer size
        m_iVerbosityLevel = 1;                ///< Set default verbosity level
        m_fVerify = true;                     ///< Set default verify option
        m_fDisplayChecksum = false;           ///< Set default checksum display option
    }

    void ParseOptions(int argc, _TCHAR *argv[], _TCHAR *szProgramName) {
        m_fChecksumOnly = _tcsicmp(szProgramName, gszSummerName) == 0;
        if (m_fChecksumOnly) {
            m_fVerify = false;
        }

        int c;
        while ((c = my::getopt(argc, argv, _T("vCcVqn:s:S:"))) != EOF) {
            switch (c) {
                // Increase verbosity by one
                case _T('v'):
                    m_iVerbosityLevel++;
                    break;

                case _T('q'):
                    m_iVerbosityLevel = 0;
                    break;

                case _T('c'):
                    m_fDisplayChecksum = true;
                    break;

                case _T('V'):
                    m_fVerify = !m_fVerify;
                    break;

                case _T('C'):
                    m_fChecksumOnly = true;
                    m_fVerify = false;
                    if (m_iVerbosityLevel == 1) {
                        m_iVerbosityLevel = 0;
                    }
                    break;

                case _T('n'):
                    if ((m_nBuffers = _ttol(my::optarg)) == 0 || m_nBuffers > 1024) {
                        Usage(szProgramName);
                        exit(3);
                    }
                    break;

                case _T('s'):
                case _T('S'):
                    m_cbBufferSize = _ttol(my::optarg) * 1024 * (c == _T('s') ? 1 : 1024);
                    if (m_cbBufferSize == 0 || m_cbBufferSize > 1024*1024*1024) {
                        Usage(szProgramName);
                        exit(4);
                    }
                    break;

                default:
                    Usage(szProgramName);
                    exit(1);
                    break;
            }
        }

        if (m_fChecksumOnly) {
            if (my::optind + 1 != argc || m_fVerify) {
                Usage(szProgramName);
                exit(5);
            }
        } else {   
            if (my::optind + 2 != argc) {
                Usage(szProgramName);
                exit(2);
            }
        }

        if (m_iVerbosityLevel >= 1) {
            Copyright(szProgramName);
        }

    }

    int Main(int argc, _TCHAR *argv[], _TCHAR *szProgramName) {
        ParseOptions(argc, argv, szProgramName);

        _TCHAR szFileIn[MAX_PATH];
        _TCHAR szFileOut[MAX_PATH];

        DWORD ccResult;
        TCHAR *szIgnoreFileName;

        ccResult = GetFullPathName(argv[my::optind], sizeof szFileIn / sizeof *szFileIn, szFileIn, &szIgnoreFileName);
        ASSCHK(ccResult > 0 && ccResult <= sizeof szFileIn / sizeof *szFileIn, _T("Could not get full in pathname"));
        ASSCHK(PathFindFileName(szFileIn) != szFileIn, _T("No filename found in input path"));
        if (m_iVerbosityLevel >= 4) {
            my::tcprintf(stderr, _T("GetFullPathName for in gives: %s\n"), szFileIn);
        }

        if (!m_fChecksumOnly) {
            ccResult = GetFullPathName(argv[my::optind+1], sizeof szFileOut / sizeof *szFileOut, szFileOut, &szIgnoreFileName);
            ASSCHK(ccResult > 0 && ccResult <= sizeof szFileOut / sizeof *szFileOut, _T("Could not get full out pathname"));
            if (PathIsDirectory(szFileOut)) {
                ASSPTR(PathAddBackslash(szFileOut));
                ASSCHK(PathAppend(szFileOut, PathFindFileName(szFileIn)) == TRUE, _T("Path format error"));
            }
            if (m_iVerbosityLevel >= 4) {
                my::tcprintf(stderr, _T("GetFullPathName for out gives: %s\n"), szFileOut);
            }
        }

        DWORD timeStart, timeEnd, timeTotalEnd;
        timeStart = GetTickCount();

        CAxCopySum theCopier(m_nBuffers, m_cbBufferSize, m_iVerbosityLevel, m_fVerify || m_fDisplayChecksum || m_fChecksumOnly);

        int iReturn = theCopier.CopyOrVerify(szFileIn, m_fChecksumOnly ? NULL : szFileOut);
        if (iReturn) {
            return iReturn;
        }
        timeEnd = GetTickCount();

        // If we specifcally asked for a checksum display, let's show it!
        if ((m_fDisplayChecksum && m_iVerbosityLevel >= 1) || m_fChecksumOnly) {
            my::tcprintf(stderr, _T("%s%s\n"), m_iVerbosityLevel > 0 ? _T("MD5 is ") : _T(""), auto_arr<_TCHAR>(GetHexString(theCopier.GetHash(), theCopier.GetHashLen())));
        }

        if (m_fVerify && !m_fChecksumOnly) {
            if (m_iVerbosityLevel >= 1) {
                my::tcprintf(stderr, _T("Calculating MD5 for %s\n"), szFileOut);
            }
            CAxCopySum theVerifier(m_nBuffers, m_cbBufferSize, m_iVerbosityLevel, true);
            int iReturn = theVerifier.CopyOrVerify(szFileOut, NULL);
            if (iReturn) {
                return iReturn;
            }
            ASSCHK(memcmp(theCopier.GetHash(), theVerifier.GetHash(), theCopier.GetHashLen()) == 0, _T("Verification FAILED!"));
            if (m_iVerbosityLevel >= 1) {
                // If we already displayed the checksum, there's no need to do it again, but we will explicity state the success
                if (m_fDisplayChecksum) {
                    my::tcprintf(stderr, _T("Verify succeeded\n"));
                } else {
                    my::tcprintf(stderr, _T("MD5 is %s\nVerify succeeded\n"), auto_arr<_TCHAR>(GetHexString(theCopier.GetHash(), theCopier.GetHashLen())));
                }
            }
        }

        if (m_iVerbosityLevel >= 1) {
            DWORD timeMilliSeconds = timeEnd - timeStart;
            if (timeMilliSeconds / 1000) {
                LARGE_INTEGER speed; 
                speed.QuadPart = theCopier.GetFileSize().QuadPart/(timeMilliSeconds / 1000);
                my::tcprintf(stderr, _T("%s %I64d byte/s"), m_fChecksumOnly ? _T("Checksummed") : _T("Copied"), speed.QuadPart);

                const _TCHAR * szSizeNames[] = {_T("KB"), _T("MB"), _T("GB"), _T("TB") ,_T("PB") };

                for (int i = 0; i < sizeof szSizeNames / sizeof *szSizeNames; i++) {
                    if (speed.QuadPart > 1024) {
                        speed.QuadPart = speed.QuadPart / 1024;
                        my::tcprintf(stderr, _T(", or %I64d %s/s"), speed.QuadPart, szSizeNames[i]);
                    }
                }
                my::tcprintf(stderr, _T("\n"));
            } else if (timeMilliSeconds) {
                my::tcprintf(stderr, _T("%s %I64d byte/s\n"), m_fChecksumOnly ? _T("Checksummed") : _T("Copied"), (theCopier.GetFileSize().QuadPart/timeMilliSeconds) * 1000);
            }

            timeTotalEnd = GetTickCount();
            timeMilliSeconds = timeEnd - timeStart;
            if (timeMilliSeconds) {
                DWORD timeHours, timeMinutes, timeSeconds;
                timeHours = timeMilliSeconds / (60*60*1000);
                timeMinutes = (timeMilliSeconds - timeHours * 60*60*1000) / (60*1000);
                timeSeconds = (timeMilliSeconds - timeHours * 60*60*1000 - timeMinutes * 60*1000) / 1000;
                my::tcprintf(stderr, _T("Total time %02d:%02d:%02d.%03d\n"), (int)timeHours, (int)timeMinutes, (int)timeSeconds, (int)(timeMilliSeconds % 1000));
            }
        }
        return 0;
    }

private:
    /// \brief Display a copyright messate
    /// \param sName The program name to display
    void Copyright(_TCHAR *sName) {
        my::tcprintf(stderr,
            _T("%s %d.%d - Pretty fast file copy and/or MD5 checksum\n")
            _T("Copyright 2005 Axantum Software AB, Svante Seleborg\n\n")
            ,sName, iVersionMajor, iVersionMinor
            );
    }

    /// \param Display a usage message
    /// \param sName The program name to display
    void Usage(_TCHAR *sName) {
        Copyright(sName);
        my::tcprintf(stderr,
            _T("Usage: %s [-v[v[...]]] [-q] [-n buffers] [-s|-S size] [-c|C|V] InFile [OutFile]\n\n")
            _T("-v Verbose, may be repeated for increased verbosity\n")
            _T("-q Quiet, very quiet mode with no progress only fatal errors\n")
            _T("-n Buffers, number of m_buffers, current is %d\n")
            _T("-s|S size, buffer size in kilo-|megabytes, current is %dK\n")
            _T("-c Checksum, Calculate MD5 during copy, and display\n")
            _T("-V verify, Toggle MD5 verify option, current is %s\n")
            _T("-C Checksum, Calculate MD5 of a single file without copying%s\n")
            _T("\n")
            _T("The optimal combination of buffer size and numbers depend on the situation.\n")
            _T("In a SAN or RAID environment, the number of buffers should probably be at\n")
            _T("least as large as the number of drives. The buffer size should probably be\n")
            _T("between 1 and 8 MB. When working over gigabit LAN or slower, a smaller\n")
            _T("buffer size may improve performance. Benchmark using a large file,\n")
            _T("try several times to even out temporary differences.\n")
            _T("All output is flushed at the end for added security, but may skew results\n")
            _T("compared to other programs when working with smaller files.\n")
            _T("If an error occurs, a non-zero status code is returned.")
            ,
            sName, m_nBuffers, m_cbBufferSize/1024, m_fVerify ? _T("ON") : _T("OFF"), m_fChecksumOnly ? _T(" (enabled)") : _T(""));
    }
};

/// \brief parse command line options
///
///  NAME
///       getopt -- parse command line options
///
///  SYNOPSIS
///       int getopt(int argc, TCHAR *argv[], TCHAR *optstring)
///
///       extern TCHAR *optarg;
///       extern int optind;
///
///  DESCRIPTION
///       The getopt() function parses the command line arguments. Its
///       arguments argc and argv are the argument count and array as
///       passed into the application on program invocation.  In the case
///       of Visual C++ programs, argc and argv are available via the
///       variables __argc and __argv (double underscores), respectively.
///       getopt returns the next option letter in argv that matches a
///       letter in optstring.  (Note:  Unicode programs should use
///       __targv instead of __argv.  Also, all character and string
///       literals should be enclosed in _T( ) ).
///
///       optstring is a string of recognized option letters;  if a letter
///       is followed by a colon, the option is expected to have an argument
///       that may or may not be separated from it by white space.  optarg
///       is set to point to the start of the option argument on return from
///       getopt.
///
///       Option letters may be combined, e.g., "-ab" is equivalent to
///       "-a -b".  Option letters are case sensitive.
///
///       getopt places in the external variable optind the argv index
///       of the next argument to be processed.  optind is initialized
///       to 0 before the first call to getopt.
///
///       When all options have been processed (i.e., up to the first
///       non-option argument), getopt returns EOF, optarg will point
///       to the argument, and optind will be set to the argv index of
///       the argument.  If there are no non-option arguments, optarg
///       will be set to NULL.
///
///       The special option "--" may be used to delimit the end of the
///       options;  EOF will be returned, and "--" (and everything after it)
///       will be skipped.
///
///  RETURN VALUE
///       For option letters contained in the string optstring, getopt
///       will return the option letter.  getopt returns a question mark (?)
///       when it encounters an option letter not included in optstring.
///       EOF is returned when processing is finished.
///
///  BUGS
///       1)  Long options are not supported.
///       2)  The GNU double-colon extension is not supported.
///       3)  The environment variable POSIXLY_CORRECT is not supported.
///       4)  The + syntax is not supported.
///       5)  The automatic permutation of arguments is not supported.
///       6)  This implementation of getopt() returns EOF if an error is
///           encountered, instead of -1 as the latest standard requires.
///
///  EXAMPLE
///       BOOL CMyApp::ProcessCommandLine(int argc, TCHAR *argv[])
///       {
///           int c;
///
///           while ((c = getopt(argc, argv, _T("aBn:"))) != EOF)
///           {
///               switch (c)
///               {
///                   case _T('a'):
///                       TRACE(_T("option a\n"));
///                       ///
///                       /// set some flag here
///                       ///
///                       break;
///
///                   case _T('B'):
///                       TRACE( _T("option B\n"));
///                       ///
///                       /// set some other flag here
///                       ///
///                       break;
///
///                   case _T('n'):
///                       TRACE(_T("option n: value=%d\n"), atoi(optarg));
///                       ///
///                       /// do something with value here
///                       ///
///                       break;
///
///                   case _T('?'):
///                       TRACE(_T("ERROR: illegal option %s\n"), argv[optind-1]);
///                       return FALSE;
///                       break;
///
///                   default:
///                       TRACE(_T("WARNING: no handler for option %c\n"), c);
///                       return FALSE;
///                       break;
///               }
///           }
///           ///
///           /// check for non-option args here
///           ///
///           return TRUE;
///       }
///
/// \param argc The argument counter from the run time
/// \param argv The argument array from the run time
/// \param optstring The list of switches
/// \return The option letter, or '?' if unrecognized or EOF if end of option-letters

namespace my {

_TCHAR *optarg;                              ///< global argument pointer
int optind = 0;                             ///< global argv index

int getopt(int argc, _TCHAR *argv[], _TCHAR *optstring) {
    static _TCHAR *next = NULL;
    if (optind == 0) {
        next = NULL;
    }

    optarg = NULL;

    if (next == NULL || *next == _T('\0'))  {
        if (optind == 0) {
            optind++;
        }

        if (optind >= argc || argv[optind][0] != _T('-') || argv[optind][1] == _T('\0')) {
            optarg = NULL;
            if (optind < argc) {
                optarg = argv[optind];
            }
            return EOF;
        }

        if (_tcscmp(argv[optind], _T("--")) == 0) {
            optind++;
            optarg = NULL;
            if (optind < argc) {
                optarg = argv[optind];
            }
            return EOF;
        }

        next = argv[optind];
        next++;                             // skip past -
        optind++;
    }

    _TCHAR c = *next++;
    _TCHAR *cp = _tcschr(optstring, c);

    if (cp == NULL || c == _T(':')) {
        return _T('?');
    }

    cp++;
    if (*cp == _T(':')) {
        if (*next != _T('\0')) {
            optarg = next;
            next = NULL;
        } else if (optind < argc) {
            optarg = argv[optind];
            optind++;
        }
    }

    return c;
}
}

/// \brief The main entry - just calls the various real-work classes
int _tmain(int argc, _TCHAR* argv[])
{
    // Set mode depending on the name. We're expecting to be named AxSumCopy, AxCopySum or AxSum
    _TCHAR szModuleName[MAX_PATH];
    ASSAPI(GetModuleFileName(NULL, szModuleName, sizeof szModuleName) > 0);
    
    _TCHAR *szProgramName = PathFindFileName(szModuleName);
    PathRemoveExtension(szProgramName);

    if (_tcsicmp(szProgramName, gszSimpleSummerName) == 0) {
        return CSimpleSummerMode().Main(argc, argv, szProgramName);
    } else {
        return CFullMode().Main(argc, argv, szProgramName);
    }
}
