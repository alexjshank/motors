#pragma once
#include <string>
#include <vector>
 
struct var {
	std::string variable;
	std::string value;
};
 
class gamevars
{
	public:
		gamevars(void);
		~gamevars(void);
 
		void cleanup();
 
		var *getValue(const char *variable);
		int getIntValue(const char *variable);
		float getFloatValue(const char *variable);
 
		void setValue(const char *variable, const char *value, bool create);
		void setValue(const char *variable, int value);
		void setValue(const char *variable, float value);
		void setValue(var *variable);
 
		bool loadCfgFile(const char *filename);			// reads a file and passes the contents to processCfgScript
		bool processCfgScript(const char *script);		// processes variable assignment commands from a script
 
		std::vector<var*> vars;
};
