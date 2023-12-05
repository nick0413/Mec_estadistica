#include <iostream>
#include <cmath>
#include <fstream>
#include <exception>
#include <memory>
#include "Random64.h"
#include <string>
#include <omp.h>
#include <chrono>
#include <cstdlib> // for atoi


std::ofstream datos;
std::ofstream config;
std::ofstream consecutive_fires;
std::string folder;

const int N_cells=500;
int t_max=20000;
/*
El valor de una celda lo vamos a definir asi:
c = 0: vacio
c = N: Edad del arbol
c = -1: Fuego
*/


class Forest_model
{
	public:
		float p;
		float f;
		int current_burn=0;
		int forest[N_cells][N_cells];
		int forest_new[N_cells][N_cells];
		Forest_model(Crandom & rand64,float p0,float f0)
			{	
				p=p0;// La probabilidad de que aparecezca un arbol en una celda
				f=f0;// La probabilidad de caiga un rayo en alguna celda
				double current_p=0;
		

				for(int ii=0;ii<N_cells;ii++)
					{for(int jj=0;jj<N_cells;jj++)
						{
							current_p=rand64.r();
							if(current_p<0.1)
								{forest[ii][jj]=1;}
							else
								{forest[ii][jj]=0;}
						}

					}
				update_new();

			}

		void update_new(void)
			{	
				#pragma omp parallel for
				for(int ii=0;ii<N_cells;ii++)
					{for(int jj=0;jj<N_cells;jj++)
						{forest_new[ii][jj]=forest[ii][jj];}
					}
			}
		void update_forest(void)
			{
				#pragma omp parallel for
				for(int ii=0;ii<N_cells;ii++)
					{for(int jj=0;jj<N_cells;jj++)
						{forest[ii][jj]=forest_new[ii][jj];}
					}
			}

		void save_data(void)
			{	
				int ii=0;
				int jj=0;
				for(ii=0;ii<N_cells;ii++)
					{
						for(jj=0;jj<N_cells;jj++)
							{datos<<forest[ii][jj]<<",";}
						datos<<std::endl;
					}
			}
		void write_bin(std::string filename)
			{
				std::ofstream out(filename, std::ios::binary);

				for(int i = 0; i < N_cells; i++)
					for(int j = 0; j < N_cells; j++)
						out.write((char*)&forest[i][j], sizeof(forest[i][j]));
				out.close();
			}

		int count_fires(void)
			{	
				int count=0;
				for(int ii=0;ii<N_cells;ii++)
					{for(int jj=0;jj<N_cells;jj++)
						{
							if(forest[ii][jj]==-1)
								{count++;}
						}
					}
				return count;
			}
		void propagate_fire(Crandom & rand64)
			{	
				// std::cout<<"propagate_fire-------------------------------------------->"<<std::endl;
				#pragma omp parallel for
				for(int ii=0;ii<N_cells;ii++)
					{for(int jj=0;jj<N_cells;jj++)
						{	

							if(forest[ii][jj]>0)
								{
									forest_new[ii][jj]++;
									if(ii!=0)	// Si no estoy en el borde izquierdo
										{
											if(forest[ii-1][jj]==-1)	// Si la celda de la izquierda esta en llamas
												{forest_new[ii][jj]=-1;} // Propago el fuego
										}
									if(ii!=N_cells-1)	
										{
											if(forest[ii+1][jj]==-1)	
												{forest_new[ii][jj]=-1;} 
										}
									if(jj!=0)	
										{
											if(forest[ii][jj-1]==-1)
												{forest_new[ii][jj]=-1;}
										}
									if(jj!=N_cells-1)
										{
											if(forest[ii][jj+1]==-1)
												{forest_new[ii][jj]=-1;}
										}
								}
							if(forest[ii][jj]==-1)
								{forest_new[ii][jj]=0;}

							

							if(forest[ii][jj]==0)
								{
									if(rand64.r()<f)	// Si se cumple la probabilidad de que crezca un arbol
										{forest_new[ii][jj]=1;}	// Creo un arbol
								}
						}
						
					}
			}

		void ignite_fire(Crandom & rand64)
			{
				int ii=rand64.r()*N_cells;
				int jj=rand64.r()*N_cells;
				forest_new[ii][jj]=-1;

				// ii=rand64.r()*N_cells;
				// jj=rand64.r()*N_cells;
				// if(forest_new[ii][jj]==0)
				// 	{forest_new[ii][jj]=1;}

			}
		
		void save_fires(int time)
			{
				consecutive_fires<<time <<","<<current_burn<<std::endl;
				current_burn=0;
			}
		void time_step(Crandom & rand64,int time)
			{	

				int count=count_fires();

				if(count==0)
					{	
						
						ignite_fire(rand64);
						
						current_burn=1;
				
					}
				else
					{propagate_fire(rand64);current_burn+=count;}


				if (false)
					{write_bin(folder+"/datos"+std::to_string(time) +".bin");}
				update_forest();
				save_fires(time);
			}

};

void write_config(void)
	{
		config<<N_cells<<","<<t_max<< std::endl;

	}
void displayProgressBar(float progress, int barWidth = 70, std::string line="=",int int_progress=0)
	{
		std::cout << "[";
		int pos = static_cast<int>(progress * barWidth);
		for (int i = 0; i < barWidth; ++i) 
			{
				if (i < pos)
					{std::cout << line;}
				else if (i == pos)
					{std::cout << ">";}
				else
					{std::cout << " ";}
			}
		std::cout << "] " << static_cast<int>(progress * 100.0)<< "%\r";
		if(int_progress!=0){ std::cout<<int_progress;}
		std::cout.flush();
	}

int main(int argc, char *argv[])
{	
	folder="datos";
	std::chrono::time_point<std::chrono::system_clock> start, end;
	start = std::chrono::system_clock::now();
	Crandom rand64(1);
	float p=0.01;
	float f=0.00001;
	if (argc >= 3) 
		{
			p = std::atof(argv[1]);
			printf("p__: %f\n",argv[1]);
			f = std::atof(argv[2]);
		}
	else
		{
			p=0.01;
			f=0.00001;
		}

	
	Forest_model forest(rand64,p,f);
	printf("N_cells: %d\n",N_cells);
	printf("t_max: %d\n",t_max);
	printf("p: %f\n",forest.p);
	printf("f: %f\n",forest.f);
	printf("f/p: %f\n",forest.f/forest.p);

	config.open(folder+"/config.csv");
	write_config();
	config.close();

	consecutive_fires.open(folder+"/consecutive_fires_#"+std::to_string(p) + "-"+std::to_string(f) + ".csv");


	
	
	
	for(int time=0;time<t_max;time++)
		{	

			forest.time_step(rand64,time);
			displayProgressBar((float)time/t_max,70,"=",time);

		}
	consecutive_fires.close();
	
	end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end-start;
	std::cout << "\nTiempo de ejecucion: " << elapsed_seconds.count() << "s\n";
	return 0;
}