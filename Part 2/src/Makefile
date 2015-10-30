CFLAGS= -Wall 
OPTFLAGS = -O2
MPICC = mpicc -lgmp -fopenmp
OBJS =  main.o julia.o savebmp.o color.o getparams.o parallel-julia.o

#julia: $(OBJS)


julia: $(OBJS)
	$(MPICC) -o julia $(OBJS)

main.o: main.c
	$(MPICC) ${OPTFLAGS} $(CFLAGS) -c main.c

julia.o: julia.c julia.h
	$(MPICC) ${OPTFLAGS} $(CFLAGS) -c julia.c

parallel-julia.o: parallel-julia.c parallel-julia.h
	$(MPICC) ${OPTFLAGS} $(CFLAGS) -c parallel-julia.c

#getparams.c: getparams.c
#	$(MPICC) ${OPTFLAGS} $(CFLAGS) -c getparams.c

run1: 
	mpirun -np 4 ./julia params.dat


clean:
	@rm -rf $(OBJS) julia *~ *.bak
