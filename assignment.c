#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>


double randomnum() 
{
    double range = 1; 
    double div = RAND_MAX / range;
    return 0 + (rand() / div);
}
 

int inout(double x,double y)
{
    double sum=(x*x)+(y*y);
    if(sum>1){
	return 0;
     }
    else{
	return 1;
    }	
}

void main(int argc,char* argv[])
{

	
	
	MPI_Init(&argc,&argv);
	double start=MPI_Wtime();

   	int rank, size;

   	MPI_Comm_size(MPI_COMM_WORLD,&size);
    	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    
    	MPI_Status status;

	int num=100000;
	int count=num/(size-1);
	int offset=num%(size-1);
	int countoff=count+offset;
	
	int square=0;
	int squarepoint;
	int circle=0;
	int circlepoint;
		
	if(rank==0){
		
		 for(int i=1;i<size;i++){
                	MPI_Recv(&circlepoint,1,MPI_INT,i,0,MPI_COMM_WORLD,&status);
			MPI_Recv(&squarepoint,1,MPI_INT,i,1,MPI_COMM_WORLD,&status);
                	circle=circlepoint+circle;
			square=squarepoint+square;
                    }
		
		double pi=(circle/(double)square)*4;
		
	}
	else{
		int circles=0;
		int squares=0;
		int n=0;
		double x,y;
		int insideout;
		if(rank==size-1){
		   while(n<count+offset){
			x=randomnum();
			y=randomnum();
			insideout=inout(x,y);
			if(insideout==1){
			circles=circles+1;
			}
			
			squares=squares+1;
			
			n=n+1;
		}
		   MPI_Send(&circles,1,MPI_INT,0,0,MPI_COMM_WORLD);				
	 	   MPI_Send(&squares,1,MPI_INT,0,1,MPI_COMM_WORLD);	
		}
		else{
			
		     while(n<count){
			x=randomnum();
			y=randomnum();
			insideout=inout(x,y);
			if(insideout==1){
			circles=circles+1;
			}
			
			squares=squares+1;
			
			n=n+1;
		}
		   MPI_Send(&circles,1,MPI_INT,0,0,MPI_COMM_WORLD);				
	 	   MPI_Send(&squares,1,MPI_INT,0,1,MPI_COMM_WORLD);		
		}
	 
	
	}	
			
	double end=MPI_Wtime();
double timed=end-start;
	printf("Time taken using %d processors is %f \n",size,timed);
	MPI_Finalize();
	
}
