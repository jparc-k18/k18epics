#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/resource.h>

#include "alarm.h"
#include "cvtTable.h"
#include "dbDefs.h"
#include "dbAccess.h"
#include "recGbl.h"
#include "recSup.h"
#include "devSup.h"
#include "link.h"
#include "waveformRecord.h"
#include "epicsExport.h"

#include <libxml/HTMLparser.h>

namespace
{
  std::vector<double> values;
  std::vector<std::string> units;

  void Replace( std::string& str, const std::string& from,
		const std::string& to )
  {
    std::string::size_type pos = 0;
    while( pos = str.find(from, pos), pos != std::string::npos ){
      str.replace( pos, from.length(), to );
      pos += to.length();
    }
  }

  void FindInfo( xmlNode*& element )
  {
    for( htmlNodePtr node=element; node!=NULL; node=node->next ){
      if( node->type == XML_ELEMENT_NODE ){
	if( node->children ){
	  FindInfo( node->children );
	  if( node->children->content ){
	    std::string s( (char*)node->children->content );
	    Replace( s, " ", "" );
	    if( s.find("digital") != std::string::npos )
	      continue;
	    if( s.find("CH") != std::string::npos ){
	      values.push_back( -9999. );
	      units.push_back( "n/a" );
	    } else {
	      s.replace( 0, 2, "" );
	      if( values[values.size()-1] == -9999. ){
		if( s.empty() ){
		  values[values.size()-1] = 0.;
		} else {
		  char* end;
		  double v = std::strtod( s.c_str(), &end );
		  if( v != 0. ){
		    values[values.size()-1] = v*10.;
		  }
		}
	      } else if( units[units.size()-1] == "n/a" ){
		units[units.size()-1] = s;
	      }
	    }
	  }
	}
      }
    }
  }
}

static long read_wf(waveformRecord *rec)
{
  values.clear();
  values.resize( 0 );
  units.clear();
  units.resize( 0 );

  char file[] = "http://gl01/digital.cgi?chg=0";
  char enc[] = "utf-8";
  htmlDocPtr docptr = htmlReadFile( file, enc, HTML_PARSE_RECOVER );
  if( docptr ){
    htmlNodePtr root = xmlDocGetRootElement( docptr );
    if( root ){
      FindInfo( root );
    }
    xmlFreeDoc( docptr );
    docptr = NULL;
  }

  xmlCleanupParser();
  xmlCleanupCharEncodingHandlers();

  float* ptr = (float*)rec->bptr;
  rec->nord = values.size();
  for( int i=0, n=rec->nord; i<n; ++i ){
    ptr[i] = values[i];
  }
  return 0;
}

static long init_record(waveformRecord *rec, int pass)
{
  return 0;
}

struct IOC{
  long      number;
  DEVSUPFUN report;
  DEVSUPFUN init;
  DEVSUPFUN init_record;
  DEVSUPFUN get_ioint_info;
  DEVSUPFUN read_wf;
  DEVSUPFUN special_linconv;
};

struct IOC devGL840_TPC={
  6,
  NULL,
  NULL,
  (DEVSUPFUN) init_record,
  NULL,
  (DEVSUPFUN) read_wf,
  NULL
};
epicsExportAddress(dset,devGL840_TPC);
