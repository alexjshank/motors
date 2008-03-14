/* * * * * * * * * * * * * * * *\
 * Title: A programming demo   *
 * Author: Alex Shank          *
 * Date: 2007                  *
\* * * * * * * * * * * * * * * */


#include "Variables.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
 
gamevars::gamevars(void)
{
}
 
gamevars::~gamevars(void)
{
 
}
 
void gamevars::cleanup()
{
	for (int i=0;i<(int)vars.size();i++)
		if (vars[i]) delete (vars[i]);
	vars.clear();
}
 
 
var *gamevars::getValue(const char *variable)
{
	for (int i=0;i<(int)vars.size();i++)
	{
		if (strcmp(variable,vars[i]->variable.c_str())==0)
		{
			return (vars[i]);
		}
	}
	std::cout << "request for gamevar that isn't defined: " << variable << std::endl;
//	log_addline("*** Request for game variable not defined \"%s\"",variable);
 
	return NULL;
}
 
int gamevars::getIntValue(const char *variable)
{
	int v;
	var *value = getValue(variable);
	if (!value) {	
		return 0;
	}
 
	sscanf(value->value.c_str(), "%d", &v);
	return v;
}
 
float gamevars::getFloatValue(const char *variable)
{
	float v;
	var *value = getValue(variable);
	if (!value) return 0;
 
	sscanf(value->value.c_str(), "%f", &v);
	return v;
}
 
void gamevars::setValue(const char *variable, const char *value, bool create = false)
{
	var *varvalue;
	if (create || !(varvalue = getValue(variable)))
	{
		// this variable name doesn't exist, create it.
		varvalue = new var;
		varvalue->variable = variable;
		varvalue->value = value;
		vars.push_back(varvalue);
	}
	else
	{
		varvalue->value = value;
	}
}
 
void gamevars::setValue(const char *variable, int value)
{
	var *varvalue = getValue(variable);
	if (!varvalue)
	{
		// this variable name doesn't exist, create it.
		varvalue = new var;
		varvalue->variable = variable;
		char buffer[1024];
		sprintf(buffer,"%d",value);
		varvalue->value = buffer;
		vars.push_back(varvalue);
	}
	else
	{
		varvalue->value = value;
	}
}
 
void gamevars::setValue(const char *variable, float value)
{
	var *varvalue = getValue(variable);
	if (!value)
	{
		// this variable name doesn't exist, create it.
		varvalue = new var;
		varvalue->variable = variable;
		char buffer[1024];
		sprintf(buffer,"%f",value);
		varvalue->value = buffer;
		vars.push_back(varvalue);
	}
	else
	{
		char buf[50];
		sprintf(buf,"%f",value);
		varvalue->value = buf;
	}
}
 
void gamevars::setValue(var *variable)
{
	vars.push_back(variable);
}
 
bool gamevars::loadCfgFile(const char *filename)
{
	FILE *fin = fopen(filename,"r");
	if (!fin)
	{
		std::cout << "failed to open configuration file " << filename << std::endl;
		return false;
	}	
 
	std::cout << "loading configuration file " << filename << std::endl;
 
	fseek(fin,0,SEEK_END);
	long size = ftell(fin);
	fseek(fin,0,SEEK_SET);
	char *buffer = new char[size];
	buffer[fread((void *)buffer,1,size,fin)] = 0;
 
	return processCfgScript(buffer);
}
 
bool gamevars::processCfgScript(const char *script)
{
	int i=0;	// script buffer index
	int o=0;	// current line index
 
	int varname_start=-1;
	int value_start=-1;
 
	var *new_var = new var;
 
	bool set_varname = false;
	bool set_value = false;
 
	while (script[i])
	{
		if (script[i] == 0 || script[i] == '\n' || script[i] == '\r' || (set_value && script[i] == '"'))
		{
			if ((set_varname && set_value))
			{
				setValue(new_var);
//				log_addline("--- new variable '%s' -> '%s'",new_var->variable.c_str(), new_var->value.c_str());
 
				new_var = new var;
			}
 
			set_varname = false;
			set_value = false;
		}
 
		if (script[i] == '#') // ignore comments... skip to next line
		{
			for (;(script[i]) && (script[i] != '\n' && script[i] != '\r'); i++);
			set_varname = false;
			set_value = false;
			new_var->variable.clear();
			new_var->value.clear();
			i++;
		}
 
		if (!set_varname && isalpha(script[i]))
		{
			set_varname = true;
		}
//		else
		{
			if (set_varname && !set_value)
			{
				if (script[i] == ' ' || script[i] == '=')
				{
					for (;(script[i]) && (script[i] == ' ' || script[i] == '=' || script[i] == '"' || script[i] == '\''); i++);
					set_value = true;
				}
				else
				{
					char b[2];
					b[0] = script[i];
					b[1] = 0;
					new_var->variable += b;
				}
			}
 
			if (set_value)
			{
				new_var->value += script[i];
			}
		}
 
		if (script[i] == 0) 
			break;
 
		i++;
	}
	return true;
}
