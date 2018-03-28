/*
**
**	$HeadURL: svn://svn.mozia.info/repo/zia/src/CmdParser.cpp $
**	$Author: nicor $
**	$Revision: 624 $
**	$Date: 2006-04-18 14:40:42 +0200 (Tue, 18 Apr 2006) $
**
*/

#include <iostream>
#include <string>

#include "CmdParser.h"

using namespace std;

namespace mozia
{
  namespace cmdparser
  {
    CmdParser::CmdParser(int argc, char **argv)
    {
      int c;
      int coption = 0;

      this->_optind = 0;
      while ((c = this->_getopt(argc, argv, "c:")) != EOF)
      {
        switch (c)
        {
        case ('c'):
          this->_addOpt(c, this->_optarg);
          break;
        case ('?'):
          this->_usage();
          break;
        default:
          cout << "ERROR: illegal option or syntax " << argv[this->_optind - 1] << endl;
          break;
        }
      }
      if (coption == 0)
        this->_addOpt('c', "config/config.xml");
    }

    CmdParser::~CmdParser(void)
    {
      map<const char, vector<string *> *, mozia::utils::eqchr>::iterator iter;
      vector<string *> *tmp;
      for (iter = this->_optHash.begin(); iter != this->_optHash.end(); )
      {
        tmp = iter->second;
        for (int i = 0; i < (int) tmp->size(); i++)
          delete tmp->at(i);
        delete tmp;
        //TOFIX
        #ifdef _WIN32
        iter = this->_optHash.erase(iter);
        #endif
      }
    }

    void  CmdParser::_usage(void)
    {
      cerr << "usage: mozia [-c config_file]" << endl;
      exit(0);
    }

    int CmdParser::_getopt(int argc, char **argv, char *optstring)
    {
      static char *next = NULL;
      if (this->_optind == 0)
        next = NULL;

      this->_optarg = NULL;

      if (next == NULL || *next == '\0')
      {
        if (this->_optind == 0)
          this->_optind++;

        if (this->_optind >= argc || argv[this->_optind][0] != '-' || argv[this->_optind][1] == '\0')
        {
          this->_optarg = NULL;
          if (this->_optind < argc)
            this->_optarg = argv[this->_optind];
          return (EOF);
        }

        if (strcmp(argv[this->_optind], "--") == 0)
        {
          this->_optind++;
          this->_optarg = NULL;
          if (this->_optind < argc)
            this->_optarg = argv[this->_optind];
          return (EOF);
        }

        next = argv[this->_optind];
        next++;
        this->_optind++;
      }

      char c = *next++;
      char *cp = strchr(optstring, c);

      if (cp == NULL || c == ':')
        return ('?');

      cp++;
      if (*cp == ':')
      {
        if (*next != '\0')
        {
          this->_optarg = next;
          next = NULL;
        }
        else if (this->_optind < argc)
        {
          this->_optarg = argv[this->_optind];
          this->_optind++;
        }
        else
        {
          return ('?');
        }
      }
      return (c);
    }

    void  CmdParser::_addOpt(char opt, char *value)
    {
      string  *val = new string(value);
      vector<string *>  *v;

      v = this->GetOpt(opt);
      if (v == NULL)
      {
        v = new vector<string *>;
        this->_optHash[opt] = v;
      }
      v->insert(v->end(), val);
    }

    vector<string *>  *CmdParser::GetOpt(char opt)
    {
      return (this->_optHash[opt]);
    }
  }
}
