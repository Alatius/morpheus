#include	<gkstring.h>

#include "sprntGkflags.h"
#include "../greeklib/xstrings.proto.h"
#include "morphflags.proto.h"
#include "morphkeys.proto.h"

/*
  Created:	04.24.92
  Author:		jjake
  This is essentially the SprintGKFlags routine, excpet I have modified it to accept
  a second delimiter for the lists of Dialects,regions,domains and morph names.
	
*/
void JakeSprintGkFlags(gk_string *gstr, char *buf, size_t bufsize, char *dels, char *more_dels, int pretty)
{
  char dialbuf[LONGSTRING*2];
  char * s;
  word_form wf;
  (void)more_dels;
  
  wf = forminfo_of(gstr);
  s=NameOfStemtype(stemtype_of(gstr));
  if( *s || *dels == '\t' )
    Xstrncat(buf,dels,bufsize);
  if( * s ) {
    Xstrncat(buf, NameOfStemtype(stemtype_of(gstr) ),bufsize);
  }

  s=NameOfDerivtype(derivtype_of(gstr));
  if( *s || *dels == '\t' )
    Xstrncat(buf,dels,bufsize);
  if( * s ) {
    Xstrncat(buf, NameOfDerivtype(derivtype_of(gstr) ),bufsize);
  }

  s=NameOfTense(wf);
  if( *s || *dels == '\t' )
    Xstrncat(buf,dels,bufsize);
  if( * s ) {
    Xstrncat(buf,s,bufsize);
  }

  s=NameOfMood(wf);
  if( *s || *dels == '\t' )
    Xstrncat(buf,dels,bufsize);
  if( * s ) {
    Xstrncat(buf, s,bufsize);
  }

  s=NameOfVoice(wf);
  if( *s || *dels == '\t' )
    Xstrncat(buf,dels,bufsize);
  if( * s ) {
    Xstrncat(buf, s,bufsize);
  }

  s=NameOfGender(wf);
  if( *s || *dels == '\t' )
    Xstrncat(buf,dels,bufsize);
  if( * s ) {
    Xstrncat(buf, s,bufsize);
  }

  s=NameOfCase(wf);
  if( *s || *dels == '\t' )
    Xstrncat(buf,dels,bufsize);
  if( * s ) {
    Xstrncat(buf, s,bufsize);
  }

  s=NameOfDegree(wf);
  if( *s || *dels == '\t' )
    Xstrncat(buf,dels,bufsize);
  if( * s ) {
    Xstrncat(buf, s,bufsize);
  }

  s=NameOfPerson(wf);
  if( *s || *dels == '\t' )
    Xstrncat(buf,dels,bufsize);
  if( * s ) {
    Xstrncat(buf, s,bufsize);
  }
  s=NameOfNumber(wf);
  if( *s || *dels == '\t' )
    Xstrncat(buf,dels,bufsize);
  if( * s ) {
    Xstrncat(buf, NameOfNumber(wf ),bufsize);
  }

  dialbuf[0] = 0;
  DialectNames(dialect_of(gstr),dialbuf,dels);
  if( dialbuf[0] || *dels == '\t' )
    Xstrncat(buf,dels,bufsize);
  if( dialbuf[0] ) {
    Xstrncat(buf,dialbuf,bufsize);
  }

  dialbuf[0] = 0;
  GeogRegionNames(geogregion_of(gstr),dialbuf,dels);
  if( dialbuf[0] || *dels == '\t' )
    Xstrncat(buf,dels,bufsize);
  if( dialbuf[0] ) {
    Xstrncat(buf,dialbuf,bufsize);
  }


  dialbuf[0] = 0;
  DomainNames(domains_of(gstr),dialbuf,dels);
  if( dialbuf[0] || *dels == '\t' )
    Xstrncat(buf,dels,bufsize);
  if( dialbuf[0] ) {
    Xstrncat(buf,dialbuf,bufsize);
  }


  dialbuf[0] = 0;
  MorphNames(morphflags_of(gstr),dialbuf,dels,pretty);
  if( dialbuf[0] || *dels == '\t' )
    Xstrncat(buf,dels,bufsize);
  if( dialbuf[0] ) {
    Xstrncat(buf,dialbuf,bufsize);
  }
}

void GregSprintGkFlags(gk_string *gstr, char *buf, size_t bufsize, char *dels, char *more_dels, int pretty)
{
  char dialbuf[LONGSTRING*2];
  char * s;
  word_form wf;
  (void)more_dels;
		
  wf = forminfo_of(gstr);

  s=NameOfTense(wf);
  if( *s || *dels == '\t' )
    Xstrncat(buf,dels,bufsize);
  if( * s ) {
    Xstrncat(buf,s,bufsize);
  }

  s=NameOfMood(wf);
  if( *s || *dels == '\t' )
    Xstrncat(buf,dels,bufsize);
  if( * s ) {
    Xstrncat(buf, s,bufsize);
  }

  s=NameOfVoice(wf);
  if( *s || *dels == '\t' )
    Xstrncat(buf,dels,bufsize);
  if( * s ) {
    Xstrncat(buf, s,bufsize);
  }

  s=NameOfGender(wf);
  if( *s || *dels == '\t' )
    Xstrncat(buf,dels,bufsize);
  if( * s ) {
    Xstrncat(buf, s,bufsize);
  }

  s=NameOfCase(wf);
  if( *s || *dels == '\t' )
    Xstrncat(buf,dels,bufsize);
  if( * s ) {
    Xstrncat(buf, s,bufsize);
  }

  s=NameOfDegree(wf);
  if( *s || *dels == '\t' )
    Xstrncat(buf,dels,bufsize);
  if( * s ) {
    Xstrncat(buf, s,bufsize);
  }

  s=NameOfPerson(wf);
  if( *s || *dels == '\t' )
    Xstrncat(buf,dels,bufsize);
  if( * s ) {
    Xstrncat(buf, s,bufsize);
  }
  s=NameOfNumber(wf);
  if( *s || *dels == '\t' )
    Xstrncat(buf,dels,bufsize);
  if( * s ) {
    Xstrncat(buf, NameOfNumber(wf ),bufsize);
  }

  dialbuf[0] = 0;
  DialectNames(dialect_of(gstr),dialbuf,dels);
  /*
    if( dialbuf[0] || *dels == '\t' )
    strcat(buf,dels);
    */
  Xstrncat(buf,"\t",bufsize);

  if( dialbuf[0] ) {
    Xstrncat(buf,dialbuf,bufsize);
  }

  dialbuf[0] = 0;
  GeogRegionNames(geogregion_of(gstr),dialbuf,dels);
  if( dialbuf[0] || *dels == '\t' )
    Xstrncat(buf,dels,bufsize);
  if( dialbuf[0] ) {
    Xstrncat(buf,dialbuf,bufsize);
  }


  dialbuf[0] = 0;
  DomainNames(domains_of(gstr),dialbuf,dels);
  if( dialbuf[0] || *dels == '\t' )
    Xstrncat(buf,dels,bufsize);
  if( dialbuf[0] ) {
    Xstrncat(buf,dialbuf,bufsize);
  }


  dialbuf[0] = 0;
  MorphNames(morphflags_of(gstr),dialbuf,dels,pretty);
  /*
    if( dialbuf[0] || *dels == '\t' )
    strcat(buf,dels);
    */
  Xstrncat(buf,"\t",bufsize);
  if( dialbuf[0] ) {
    Xstrncat(buf,dialbuf,bufsize);
  }

  s=NameOfStemtype(stemtype_of(gstr));
  Xstrncat(buf,"\t",bufsize);
  if( * s ) {
    Xstrncat(buf, NameOfStemtype(stemtype_of(gstr) ),bufsize);
  }

  s=NameOfDerivtype(derivtype_of(gstr));
  if( * s ) {
    Xstrncat(buf,",",bufsize);
    Xstrncat(buf, NameOfDerivtype(derivtype_of(gstr) ),bufsize);
  }
		


}

