
CFLAGS=-g -Wall -O2
MPICC = mpicc
OBJS =  main.o julia.o savebmp.o color.o getparams.o parallel-julia.o savestat.o

#julia: $(OBJS)

all: julia

julia: $(OBJS)
	$(MPICC) -o julia $(OBJS)

main.o: main.c
	$(MPICC) $(CFLAGS) -c main.c

julia.o: julia.c julia.h
	$(MPICC) $(CFLAGS) -c julia.c

parallel-julia.o: parallel-julia.c parallel-julia.h
	$(MPICC) $(CFLAGS) -c parallel-julia.c

#keeper
run11: 
	mpirun -np 8 ./julia 1 -0.4 0.621  -0.3 0 0  0.3  4000 4000 100  image1.bmp stats1.txt

#keeper
run12: 
	mpirun -np 8 ./julia 1 -0.8  0.156  -.2 .2 -.2 .2 1000 1000 10000  image2.bmp stats2.txt 

#keeper
run13: 
	mpirun -np 8 ./julia 1 -0.8  0.156  -.625 -.425 -.1 .3 10000 10000 40  image3.bmp stats3.txt

#keeper
run14: 
	mpirun -np 8 ./julia 1 -0.8  0.156  -2 2 -2 2 4000 4000 200  image4.bmp stats4.txt

run15: 
	mpirun -np 16 ./julia 0 -0.4 0.6  -0.181862 -0.181772 1.019085 1.019175 1000 1000 2000  image.bmp stats.txt




clean:
	@rm -rf $(OBJS) julia *~ *.bak
