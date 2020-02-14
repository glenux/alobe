/* vim: set sw=4 ts=4 si et: */		

#include <stdlib.h>
#include "console.h"

#define DEBUG 	1


Console_t * console_create(){
	Console_t * result = (Console_t *) malloc (sizeof(Console_t));
	result->varlist = list_create();
	result->cmdlist = list_create();
	return result;
}

void console_destroy(Console_t * console){
	// FIXME: on d�pop toutes les listes et on d�truit tout...
	free (console);
}

void console_register_variable(Console_t * console,
		char * varname, 
		char * vardesc,
		short varnamelen,
		bool (* varcheckfun) (char *, short))
{
	// v�rifier si la variable est enregistr�e		
	if (!console_is_registered_variable(console, varname)){
		// on cr�e la structure;
		console_var_t * cell_var = 
			(console_var_t *) malloc (sizeof(console_var_t));

		// remplir les champs de cell_var;
		//cell_var->name = strndup(varname, varnamelen);
		cell_var->namelen = varnamelen;
		cell_var->checkfun = varcheckfun;
		cell_var->value = NULL;

		// sinon, on ajoute au d�but... 
		list_push_back(console->varlist, (void *)cell_var);
	}
}


bool console_is_registered_variable(Console_t * console, 
		char * varname){
	// FIXME console_is_registered_variable
	return false;
}

void console_register_command(Console_t * console, 
		char * cmdname, 
		char * cmdparamcount,
		void * cmdcheckfun,
		void * cmdrunfun){
}

bool console_is_registered_command(Console_t * console,
		char * cmdname){
	// FIXME: console_is_registered_command
}


#undef DEBUG
