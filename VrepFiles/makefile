# Specify compiler
CC=cl.exe
# Specify Linker
LINK=link.exe
# Flags

all: 
	gcc -Wall -DNON_MATLAB_PARSING -DMAX_EXT_API_CONNECTIONS=255 -fPIC -std=gnu99 -c vrep_main.c -o vrep_main.o 
	LINK $(CFLAGS) -c extApi.c -o extApi.o
	LINK $(CFLAGS) -c extApiPlatform.c -o extApiPlatform.o
	CC extApi.o extApiPlatform.o vrep_main.o -o vrepClient -lpthread
