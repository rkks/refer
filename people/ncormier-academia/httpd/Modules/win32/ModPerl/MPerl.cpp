// Mozia - $Id$

#include "MPerl.h"

using namespace mozia::modules::mperl;

namespace mozia
{
  namespace modules
  {
    namespace mperl
    {
      MPerl::MPerl(TicketsHandler *th, XmlContainer *xmlconf)
      {
        this->_th = th;
        this->_xml = xmlconf;
        //this->_perl = perl_alloc();
        //perl_construct(this->_perl);
      }

      MPerl::~MPerl(void)
      {
        perl_destruct(this->_perl);
        perl_free(this->_perl);
      }

      void  MPerl::MainLoop(void)
      {
        this->Run("C:\\www\\cgi-bin\\lala2.pl");
        while (this->_th->Get())
        {
        }
      }

      void  Thread(void *ptr)
      {
        PerlInterpreter *perl = (PerlInterpreter *) ptr;

        cout << "autiste\n";
        perl_run(perl);
        cout << "passed!\n";
        ExitThread(0);
      }

      int  MPerl::RunThreaded(int argc, char **argv)
      {
        SECURITY_ATTRIBUTES saAttr; 
        HANDLE  hChildStdoutRd, hChildStdoutWr, hChildStdinRd, hChildStdinWr, hChildStdoutRdDup, hChildStdinWrDup;

        saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
        saAttr.bInheritHandle = TRUE; 
        saAttr.lpSecurityDescriptor = NULL; 
        if (CreatePipe(&hChildStdoutRd, &hChildStdoutWr, &saAttr, 0) == 0)
          return (0);
        if (DuplicateHandle(GetCurrentProcess(), hChildStdoutRd, GetCurrentProcess(), &hChildStdoutRdDup , 0, FALSE, DUPLICATE_SAME_ACCESS) == 0)
          return (0);
        if (CloseHandle(hChildStdoutRd) == 0)
          return (0);
        if (CreatePipe(&hChildStdinRd, &hChildStdinWr, &saAttr, 0) == 0)
          return (0);
        if (DuplicateHandle(GetCurrentProcess(), hChildStdinWr, GetCurrentProcess(), &hChildStdinWrDup, 0, FALSE, DUPLICATE_SAME_ACCESS) == 0)
          return (0);
        if (CloseHandle(hChildStdinWr) == 0)
          return (0);

        HANDLE  h = CreateThread(&saAttr, 0, (LPTHREAD_START_ROUTINE) Thread, this->_perl,
          0, NULL);
        if (h == NULL)
          throw new ThreadCreationException("TODO");
        WaitForSingleObject(h, INFINITE);
        CloseHandle(h);
        cout << "fin\n";
        return (1);
      }

      void  MPerl::Run(string path)
      {
        char *argv[2] = { "", (char *) path.c_str() };
        int argc = 2;
        PerlInterpreter *my_perl;

        my_perl = perl_alloc();
        perl_construct(my_perl);

        perl_parse(my_perl, NULL, argc, argv, (char **) NULL);
        PerlIO *lala = PerlIO_stdout();
        int lalafd = PerlIO_fileno(lala);
      
        /*int fds[2];
        int a = _pipe(fds, 256, O_BINARY);
        int b = _dup2(fds[1], lalafd);*/

        char buf[4096];
        FILE *f = PerlIO_exportFILE(lala, "r");
         int d = errno;

        perl_run(my_perl);
        
        //this->RunThreaded(argc, argv);

        /*PerlIO *PerlStdout = Perl_PerlIO_stdout(this->_perl);
        char buf[4096];
        int a = Perl_PerlIO_read(this->_perl, PerlStdout, buf, 4096);*/

        

        int c = PerlIO_read(lala, buf, sizeof(buf));
       
        char *e = win32_strerror(d);

        this->~MPerl();
      }
    }
  }
}
