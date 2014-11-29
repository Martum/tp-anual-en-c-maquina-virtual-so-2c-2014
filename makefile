kernel-instalar:
	cd kernel/src && make instalar
kernel-desinstalar:
	cd kernel/src && make desinstalar


memoria-instalar:
	cd msp/src && make instalar
memoria-desinstalar:
	cd msp/src && make desinstalar


consola-instalar:
	cd consola/src && make instalar
consola-desinstalar:
	cd consola/src && make desinstalar


cpu-instalar:
	cd cpu/src && make instalar
cpu-desinstalar:
	cd cpu/src && make desinstalar


hu4sockets-instalar:
	cd libs/hu4sockets && make instalar
hu4sockets-desinstalar:
	cd libs/hu4sockets && make desinstalar


ansisop-instalar:
	cd libs/ansisop-panel/Release && make all
ansisop-desinstalar:
	cd libs/ansisop-panel/Release && make clean
