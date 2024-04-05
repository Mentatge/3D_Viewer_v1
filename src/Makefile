CC = gcc -Wall -Werror -Wextra -g -std=c11
SRC = $(wildcard *.c)
OBJ = $(patsubst %.c, %.o, ${SRC})
LFLAGS = -lcheck -lpthread
GCF = -ftest-coverage -fprofile-arcs --coverage

ifeq ($(shell uname -s), Linux)
	LFLAGS += -lrt -lsubunit -lm
endif

all: clean 3DViewer_v1.0.a gcov_report

rebuild: clean 3DViewer_v1.0.a

clean:
	rm -rf *.o && rm -rf *.gcno
	rm -rf *.a
	rm -rf *.gcda
	rm -rf *.dSYM
	rm -rf *.info && rm -rf *.gcov
	rm -rf ./test && rm -rf ./gcov_report
	rm -rf ./report/
	rm -rf test_output/
	rm -rf *.out
	rm -rf ./doxygen
	rm -rf html
	rm -rf docs
	rm -rf Doxyfile.bak
	rm -rf build*
	rm -rf Archive_calc_v1.0.tgz

3DViewer_v1.0.o:
	$(CC) -c 3DViewer_v1.0.c -o 3DViewer_v1.0.o

3DViewer_v1.0.a: 3DViewer_v1.0.o
	ar rcs 3DViewer_v1.0.a 3DViewer_v1.0.o
	ranlib 3DViewer_v1.0.a
	rm *.o

test: unit_test.c 3DViewer_v1.0.a
	$(CC) -o test $^ ${LFLAGS}
	./test

gcov_report: test
	$(CC) $(GCF) unit_test.c 3DViewer_v1.0.c $(LFLAGS) -o gcov_report
	./gcov_report
	lcov -t "test" -o gcov_report.info -c -d .
	genhtml -o test_output gcov_report.info
	open ./test_output/index.html\

install:
	make clean
	mkdir build
	cd OPENGL_v5 && qmake && make && make clean && rm -rf Makefile && cd ../
ifeq ($(shell uname -s), Linux)
	mv OPENGL_v5/5 build
else
ifeq ($(shell uname -s), Darwin)
	mv OPENGL_v5/5.app build
endif
endif

uninstall:
	rm -rf build*

dvi:
	doxygen Doxyfile
	google-chrome docs/html/index.html

style:
	cp ../materials/linters/.clang-format ./
	clang-format -n *.c *.h
	rm -f .clang-format

style_correct:
	cp ../materials/linters/.clang-format ./
	clang-format -i *.c *.h
	rm -f .clang-format

dist: install
	rm -rf Archive_Viewer_v1.0/
	mkdir Archive_Viewer_v1.0/
	mkdir Archive_Viewer_v1.0/src
ifeq ($(OS),Linux)
	cp -r ./build/5 Archive_Viewer_v1.0/src/
else
ifeq ($(OS),Darwin)
	cp -r ./build/5.app Archive_Viewer_v1.0/src/
endif
endif
	tar cvzf Archive_Viewer_v1.0.tgz Archive_Viewer_v1.0/
	rm -rf Archive_Viewer_v1.0/
