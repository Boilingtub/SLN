TEST_ARGS = "hello testing string"

TEST_FLAGS = -Wall -Wextra -Werror -Wshadow -g -static-libasan -fanalyzer -fsanitize=address
FIN_FLAGS = -Wall -Wextra -Werror -Wshadow -O3 
SRC = src/
CFILES = $(SRC)sln.c 
LIBS = 
OUTPUT = output/sln

test: $(CFILES)
	gcc $(TEST_FLAGS) -g $(CFILES) -o $(OUTPUT) $(LIBS)
	#gdb -ex=r --batch --args ./$(OUTPUT) $(TEST_ARGS);
	./$(OUTPUT) $(TEST_ARGS)

final: $(CFILES)
	gcc $(FIN_FLAGS) -Os $(CFILES) -o $(OUTPUT) $(LIBS)
