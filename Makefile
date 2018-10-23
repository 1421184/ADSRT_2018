all: compile 

clean:
	rm Fita3
executar:
	./Fita3 -t 30
compile:
	gcc Fita3.c -lsqlite3 -o Fita3

help:
	@echo Opcions compilacio
	@echo   make clean
	@echo   make compile
	@echo   make executar, "paràmetre per defecte 30º"