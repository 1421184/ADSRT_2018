all: compile 

clean:
	rm captura
github:
	git clone https://github.com/1421184/ADSRT_2018
executar:
	./captura -t 30 -r 1421184@campus.euss.org -c basedades.db -d 1306924@campus.euss.org
compile:
	gcc captura.c -lsqlite3 -o captura
help:
	@echo Opcions compilacio
	@echo   make clean
	@echo   make compile
	@echo	make github
	@echo   make executar, "paràmetre per defecte 30º"


