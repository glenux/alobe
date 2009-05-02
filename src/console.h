/* vim: set sw=4 ts=4 si et: */		

#ifndef _CONSOLE_H
#define _CONSOLE_H

#include "units.h"
#include "list.h"

typedef struct {
	char * name;
	short namelen;
	char * value;
	void * checkfun;
} console_var_t;

typedef struct {
	char * cmdname;
	char * cmdparams; 
	void * cmdcheckfun;   
	void * cmdrunfun;
} console_cmd_t;


typedef struct {
	List_t * varlist;
	List_t * cmdlist;
} Console_t;


Console_t * console_create();

void console_destroy(Console_t * console);

void console_register_variable(Console_t * console, 
							char * varname, 
							char * vardesc,
							short varnamelen,
							bool (* varcheckfun) (char *, short));

bool console_is_registered_variable(Console_t * console, 
							char * varname);

void console_register_command(Console_t * console, 
							char * cmdname, 
							char * cmdparamcount,
							void * cmdcheckfun,
							void (*  cmdrunfun)
							);

bool console_is_registered_command(Console_t * console,
							char * cmdname);

#endif /* _CONSOLE_H */

