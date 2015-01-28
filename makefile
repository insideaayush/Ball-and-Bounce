num_thread=15
compile:
	g++ ./src/main.cpp ./src/ball.cpp -lpthread -lGL -lGLU -lglut;
	touch compile
execute:compile
	./a.out 2 $(num_thread)
	touch execute
DEBUG:
	g++ ./src/main.cpp ./src/ball.cpp -D DEBUG -lpthread -lGL -lGLU -lglut;
	./a.out 2 $(num_thread)
doc:./doc/COP290-Ass1-Doc.tex
	cp ./doc/diagram.jpg ./diagram.jpg
	pdflatex ./doc/COP290-Ass1-Doc.tex
	rm diagram.jpg
compile121:
	g++ ./src/main_2.cpp ./src/ball.cpp -ggdb -lpthread -lGL -lGLU -lglut;
clean:
	\rm -rf *.out *~ *.pdf *.aux *.log compile execute *.log


