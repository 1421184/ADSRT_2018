all: compile 

clean:
	rm Fita3
github:
	git clone https://github.com/1421184/ADSRT_2018
executar:
	./Fita3 -t 30
compile:
	gcc Fita3.c -lsqlite3 -o Fita3

help:
	@echo Opcions compilacio
	@echo   make clean
	@echo   make compile
	@echo	make github
	@echo   make executar, "paràmetre per defecte 30º"

