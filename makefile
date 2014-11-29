# Instala todo (incluido el ansisop)
instalar: ansisop-instalar instalar-sin-ansisop

# Instala solo nuestros programas
instalar-sin-ansisop: hu4sockets-instalar kernel-instalar msp-instalar consola-instalar cpu-instalar

desinstalar:  ansisop-desinstalar desinstalar-sin-ansisop

desinstalar-sin-ansisop: kernel-desinstalar msp-desinstalar consola-desinstalar cpu-desinstalar hu4sockets-desinstalar




kernel-instalar:
	cd kernel/src && make instalar
kernel-desinstalar:
	cd kernel/src && make desinstalar


msp-instalar:
	cd msp/src && make instalar
msp-desinstalar:
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
