#ifndef _JSP_PARSER_H
#define _JSP_PARSER_H

#include "jsp_defs.h"
#include "parser.h"
#include "mpstrstream.h"

class CoreParser : public ParserComponent
{
protected:
	int assignID(Directory & container, const char * category);

    virtual void parseOption(Directory &);    
public:
	virtual bool parse(Parser & p, Directory & container);
};

class ConfigurationFileGenerator : public Component
{
protected:
	bool restrict_zero;

	enum tagDefinition { HEADER=1, TNUM=2, BUFFER=4, DEFINITION=8, CONTROLBLOCK=16, SHORT_ID=32, TNUMNO=64, INIT=128 };
	void createObjectDefinition(mpstrstream *, Directory &, int, ...);

	virtual void parseOption(Directory &);
	virtual void body(Directory &);
};

class OrtiDescriptionFileGenerator : public Component
{
protected:
	virtual void parseOption(Directory &);
	virtual void body(Directory &);
};

class CheckScriptGenerator : public Component
{
protected:
	virtual void parseOption(Directory &);
	virtual void body(Directory &);
};


#endif