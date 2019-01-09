all: compile 

clean:
	rm -rf *o main
github:
	git clone https://github.com/1421184/ADSRT_2018
executar:
	LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/pi/Desktop/ADSRT_2018 ./captura -c basedades.db -r 1422047@campus.euss.org -d 1421184@campus.euss.org -t 30
compile:
	gcc  -fPIC -c -o mail.o mail.c
	gcc -c -o main_mail.o -lsqlite3 captura.c -I/home/pi/Desktop/ADSRT_2018
	gcc captura.c -lsqlite3 -lpthread -lrt -L/home/pi/Desktop/ADSRT_2018 -lmail -o captura 
	gcc -o captura main_mail.o -lrt -lsqlite3 -L/home/pi/Desktop/ADSRT_2018 -lmail
	
help:
	@echo Opcions compilacio
	@echo   make clean
	@echo   make compile
	@echo	make github
	@echo   make executar, "paràmetre per defecte 30º"

compilar: mail2 mail3
	gcc -o informe main_mail.o -lsqlite3 -L/home/pi/Desktop/ADSRT_2018 -lmail 

mail1: mail.c
	gcc  -fPIC -c -o mail.o mail.c

mail2: mail1
	gcc -shared -fPIC -o libmail.so mail.o

mail3: informe.c
	gcc -c -o main_mail.o -lsqlite3 informe.c -I/home/pi/Desktop/ADSRT_2018
funcionar: 
	LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/pi/Desktop/ADSRT_2018 ./informe -b basedades.db -r 1422047@campus.euss.org -d 1421184@campus.euss.org
