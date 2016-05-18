#ifndef FUNCTION_PROTOTYPE_H
#define FUNCTION_PROTOTYPE_H

class Function {
	public:
		enum Name {
			IF,
			FOR,
			WHILE,
			RETURN,
			PRINT,

			SYMBOL,
			VARIABLE,
			NULLARY,
			INITIALIZER,
			TRUE,
			FALSE,

			OUTPUTEMPTY,
			OUTPUTADD,
			OUTPUTGET,

			MESHADD,

			GETTER,
			GETVARIABLE,
			GETSYMBOL,
			VARIABLESETTER,
			SYMBOLCREATOR,
			OPERATOR,
			COMPARISON,

			ROTATE
		};
	protected:
		Name name; 
		bool shouldReturn;
		int output;
	public:
		Function(Name name);
		~Function() = default;

		bool getShouldReturn();

		virtual void operator()() = 0;
		virtual int getOutput() = 0;	

		Name getName();
};

#endif