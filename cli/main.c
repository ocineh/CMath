#include "interpreter.h"

static int print_help(const char *error);

int main(int argc, char *argv[]) {
	for(int i = 0; i < argc; ++i)
		if(strcmp(argv[i], "-h") == 0)
			return print_help(NULL);
	FILE *input = stdin, *output = stdout, *error = stderr;
	for(int i = 1; i < argc; ++i) {
		if(strcmp(argv[i], "-i") == 0) {
			if(++i == argc) return print_help("-i requires an argument");
			input = fopen(argv[i], "r");
			if(input == NULL) {
				fprintf(stderr, "Error: Cannot open input file\n");
				return 1;
			}
		} else if(strcmp(argv[i], "-o") == 0) {
			if(++i == argc) return print_help("-o requires an argument");
			output = fopen(argv[i], "w");
			if(output == NULL) {
				fprintf(stderr, "Error: Cannot open output file\n");
				return 1;
			}
		} else if(strcmp(argv[i], "-e") == 0) {
			if(++i == argc) return print_help("-e requires an argument");
			error = fopen(argv[i], "w");
			if(error == NULL) {
				fprintf(stderr, "Error: Cannot open error file\n");
				return 1;
			}
		}
	}

	interpreter *inter = create_interpreter(input, output, error);
	interpret(inter);
	destroy_interpreter(inter);
	return 0;
}

int print_help(const char *error) {
	if(error != NULL) fprintf(stderr, "Error: %s\n", error);
	fprintf(stderr, "Usage: cmd [-i <input>] [-o <output>] [-e <error>]\n");
	fprintf(stderr, "  -i <input>\t\tInput file by default standard input\n");
	fprintf(stderr, "  -o <output>\t\tOutput file by default standard output\n");
	fprintf(stderr, "  -e <error>\t\tError file by default standard error\n");
	fprintf(stderr, "  -h\t\t\tPrint this help\n");
	return -1;
}
