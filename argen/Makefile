objects =	src/bison/bison.cpp										\
			src/flex/lex.cpp										\
			src/argen.cpp 											\
																	\
			src/utils/globals.cpp									\
			src/utils/log.cpp										\
																	\
			src/generator/rule.cpp									\
																	\
			src/generator/collection/collection.cpp					\
			src/generator/collection/interpretationsCollection.cpp	\
			src/generator/collection/meshCollection.cpp				\
			src/generator/collection/rulesCollection.cpp			\
			src/generator/collection/symbolCollection.cpp			\
																	\
			src/generator/model/face.cpp							\
			src/generator/model/mesh.cpp							\
																	\
			src/functions/block.cpp									\
			src/functions/interpretation.cpp						\
																	\
			src/functions/manipulation/compare.cpp					\
			src/functions/manipulation/getter.cpp					\
			src/functions/manipulation/setter.cpp					\
			src/functions/manipulation/initializerList.cpp			\
			src/functions/manipulation/operators.cpp				\
																	\
			src/functions/basic/functionPrototype.cpp				\
			src/functions/basic/if.cpp								\
			src/functions/basic/for.cpp								\
			src/functions/basic/while.cpp							\
			src/functions/basic/return.cpp							\
			src/functions/basic/print.cpp							\
																	\
			src/functions/advanced/rotate.cpp						\
																	\
			src/functions/varSym/varSymPrototype.cpp				\
			src/functions/varSym/symbol.cpp							\
			src/functions/varSym/variable.cpp						\
			src/functions/varSym/nullary.cpp						\
			src/functions/varSym/symbolCreator.cpp					\
			src/functions/varSym/symCollection.cpp					\
																	\
			src/functions/output/outputAdd.cpp						\
			src/functions/output/outputEmpty.cpp					\
																	\
			src/functions/mesh/meshAddVertex.cpp					\
			src/functions/mesh/meshAddFace.cpp						\

progName = output

includes = 	-Ilibs/glm/		\
			-Isrc/

all:
ifeq ($(OS),Windows_NT)
	@$(MAKE) windows
else
	@$(MAKE) linux
endif

linux:
	@cd src/bison && bison -d bison.y
	@cd src/flex && flex lexical.l
	@g++ -std=c++11 -c $(objects) $(includes)
	@mkdir objects
	@mv *.o objects/
	@ar rcs libs/libargen.a objects/*.o
	@rm -rf objects

windows:
	@cd src/bison && bison -d bison.y
	@cd src/flex && flex lexical.l
	@g++ -std=c++11 -c $(objects) $(includes)
	@md objects
	@move *.o objects/
	@ar rcs libs/libargen.a objects/*.o
	@rmdir /Q /S objects

clean:
	@rm -f *.o
	@rm -f libs/libargen.a
	@rm -f src/bison/bison.cpp src/bison/bison.h
	@rm -f src/flex/lex.cpp src/flex/lex.h