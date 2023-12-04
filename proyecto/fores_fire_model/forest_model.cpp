#include <iostream>
#include <cmath>
#include <fstream>
#include <exception>
#include <memory>
#include "Random64.h"
#include <string>

std::ofstream datos;
std::ofstream config;

std::string folder="datos";

const int N_cells=200;
int t_max=500;
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
		int forest[N_cells][N_cells];
		int forest_new[N_cells][N_cells];
		Forest_model(Crandom & rand64,float p0,float f0)
			{	
				p=p0;
				f=f0;
				double current_p=0;
		

				for(int ii=0;ii<N_cells;ii++)
					{for(int jj=0;jj<N_cells;jj++)
						{
							current_p=rand64.r();
							if(current_p<p)
								{forest[ii][jj]=1;}
							else
								{forest[ii][jj]=0;}
						}

					}
				update_new();

			}

		void update_new(void)
			{
				for(int ii=0;ii<N_cells;ii++)
					{for(int jj=0;jj<N_cells;jj++)
						{forest_new[ii][jj]=forest[ii][jj];}
					}
			}
		void update_forest(void)
			{
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
				for(int ii=0;ii<N_cells;ii++)
					{for(int jj=0;jj<N_cells;jj++)
						{	
							// std::cout<<"-----------------"<<std::endl;
							// std::cout<<ii<<","<<jj<<std::endl;
							// std::cout<<"-----------------"<<std::endl;
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
			}
		void time_step(Crandom & rand64,int time)
			{	

				int count=count_fires();

				if(count==0)
					{ignite_fire(rand64);}
				else
					{propagate_fire(rand64);}


				if (true)
					{write_bin(folder+"/datos"+std::to_string(time) +".bin");}
				update_forest();
			}

};

void write_config(void)
	{
		config<<N_cells<<","<<t_max<< std::endl;

	}


int main(void)
{	

	Crandom rand64(1);
	Forest_model forest(rand64,0.5,0.5);

	config.open(folder+"/config.csv");
	write_config();
	config.close();



	
	
	
	for(int time=0;time<t_max;time++)
		{	

			forest.time_step(rand64,time);

		}
	

	return 0;
}