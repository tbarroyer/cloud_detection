
#---------------------------------------------------------
#makefile permettant la compilation du programme principal
#---------------------------------------------------------
.KEEP_STATE:

#definition des variables
CC=gcc
OBJ=../OBJ
EXE=../EXE
CCFLAGS=-Wall -g `pkg-config --cflags gtk+-2.0`
LDFLAGS=`pkg-config --libs gtk+-2.0`

#compilation
MAI_FILE=main.o
MAIL_FILE=main_learn.o
OBJ_FILES=compute.o

all: ImaProjet.exe ImaProjetLearn.exe
	mv *.o ${OBJ}

.c.o:
	${CC} ${CCFLAGS} -o $@ -c $<

ImaProjet.exe:${OBJ_FILES} ${MAI_FILE}
	${CC} -o ${EXE}/$@ ${OBJ_FILES} ${MAI_FILE} ${LDFLAGS}

ImaProjetLearn.exe:${OBJ_FILES} ${MAIL_FILE}
	${CC} -o ${EXE}/$@ ${OBJ_FILES} ${MAIL_FILE} ${LDFLAGS}


clean:
	\rm -f ${OBJ}/*.o ../*/core ${EXE}/*.exe

install:
	cp *.h ${INC}

.SUFFIXES: .exe .o .c .h
