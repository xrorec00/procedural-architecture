#ifndef VAR_SYM_PROTOTYPE_H
#define VAR_SYM_PROTOTYPE_H

#include <string>

class VarSymPrototype {
	public:
		enum ClassName {
			VARIABLE,
			SYMBOL
		};

		enum OperatorType{
			PLUS,
			MINUS,
			MULTIPLY,
			DIVIDE,

			EQUAL,
			NOTEQUAL,
			LESS,
			LESSEQUAL,
			GREATER,
			GREATEREQUAL
		};
	private:
		ClassName _className;
	protected:
		std::string _varSymType;
		std::string _varSymName;
	public:
		VarSymPrototype(ClassName);
		ClassName getClassName();

		std::string getVarSymName() const;
		void setVarSymName(std::string);

		std::string getVarSymType() const;
		void setVarSymType(std::string);

		virtual void printInfo()=0;
};

#endif