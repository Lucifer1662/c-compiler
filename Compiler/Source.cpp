#include <string>
#include <iostream>
#include "Parser.h"

//https://www.cs.virginia.edu/~evans/cs216/guides/x86.html

/*
	int add(int z, int y){
		return x + y;
	}
	
	void main(){
	int k = add(5,10);
	printf(k, "%10d");
	}

	for(int i = 0; i < 10; i = i + 1){
			printf(i, "%d\n");
		}

		Foo foo;
		printf("%d,%d\n", foo.y, foo.x);

		struct Foo {
		int x;
		float w;
	};

	struct Bar {
		int x;
		Foo foo;
	};

	Bar bar;
		bar.foo.w = 5.0f;

		printf("%f\n", bar.foo.w);
	
*/

void main() {
	std::string s(R"V0G0N(

	
	void main(){
		int* z = malloc(4);
		*z = malloc(4);
		**z = 5;

		printf("%d\n", **z);

		free(z);
	}

	
)V0G0N");

	int** d = (int**)malloc(4);
	*d = (int*)malloc(4);
	**d = 10;
	int f = 5 * **d;


	auto parse = Parser(s);
	auto globalScope = parse.parse();

	

	Scope global;
	auto [instructions, _] = globalScope->operator()(global);

	instructions += "section .data\n";
	for (auto& [name, instruction] : *global.globalNames.get()) {
		instructions += instruction;
	}
	

	auto finalInstructions = std::string("");
	finalInstructions += "global  _main\n";
	finalInstructions += "extern  _printf\n";
	finalInstructions += "extern  _malloc\n";
	finalInstructions += "extern  _free\n";
	finalInstructions += "section .text\n";
	finalInstructions += instructions;

	std::cout << finalInstructions << std::endl;

}