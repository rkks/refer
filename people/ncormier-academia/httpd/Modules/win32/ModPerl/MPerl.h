// Mozia - $Id$

#ifndef _MPERL_H_
# define _MPERL_H_

# include <iostream>

# include "TicketsHandler.h"

# include <perl.h>
# include <EXTERN.H>

namespace mozia
{
  namespace modules
  {
    namespace mperl
    {
      class MPerl
      {
      public:
        MPerl(TicketsHandler *th, XmlContainer *xmlconf);
        ~MPerl(void);
        void  MainLoop(void);
        void  Run(string path);
        int  RunThreaded(int argc, char **argv);

      private:
        TicketsHandler  *_th;
        XmlContainer  *_xml;
        PerlInterpreter *_perl;
      };
    }
  }
}

#endif /* _MPERL_H_ */
