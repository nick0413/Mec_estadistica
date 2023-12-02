#include <iostream>
#include <cmath>
#include <fstream>
#include <exception>
#include <memory>
#include "Random64.h"

std::ofstream datos;
std::ofstream datos_new;
std::ofstream config;
const int N_cells=30;
int t_max;

int lines=0;

void write_config(void)
	{
		config<<N_cells<<","<<t_max<<std::endl;
		// config<<t_max<<std::endl;
	}

void displayProgressBar(float progress, int barWidth = 50, std::string line="=")
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
		std::cout.flush();
	}

class BTW_model
{
	public:
		int cells[N_cells][N_cells];
		int cells_new[N_cells][N_cells];
		int last_x=0;
		int last_y=0;
		bool toppling=false;
		int topple_count=0;	


	
		void start(int start_val=1)
			{	
				// throw an error if start_val is >3
				if(start_val>3 || start_val<0)
					{throw std::invalid_argument( "received invalid start_val" );}

				for(int ii=0;ii<N_cells;ii++)
				{for(int jj=0;jj<N_cells;jj++)
					{cells[ii][jj]=start_val;}	
				}
			}
		void start_random(Crandom &rand64)
			{	
				for(int ii=0;ii<N_cells;ii++)
				{for(int jj=0;jj<N_cells;jj++)
					{cells[ii][jj]=rand64.r()*4;}	
				}
			}
		int count_piles(void)
			{	int count=0;
				for(int ii=0;ii<N_cells;ii++)
					{for(int jj=0;jj<N_cells;jj++)
						{if(cells[ii][jj]>3)
							{count++;}}	
					}

				return count;
			}

		void update_cells(void)
			{
				for(int ii=0;ii<N_cells;ii++)
					{for(int jj=0;jj<N_cells;jj++)
						{
							cells[ii][jj]=cells_new[ii][jj];
						}	
					}
			}
		void update_new(void)
			{
				for(int ii=0;ii<N_cells;ii++)
					{for(int jj=0;jj<N_cells;jj++)
						{
							cells_new[ii][jj]=cells[ii][jj];
						}	
					}
			}

		void save_data(int t,bool print=false)
			{	
				int ii=0;
				int jj=0;
				for(ii=0;ii<N_cells;ii++)
					{
						
						for(jj=0;jj<N_cells;jj++)
							{
								datos<<cells[ii][jj]<<",";
								// std::cout<<ii<<" "<<jj<<std::endl;
							}
						datos<<std::endl;
					}
				// std::cout<<t<<" "<<ii<<" "<<jj<<std::endl;
			}
		void save_new(int t,bool print=false)
			{	
				int ii=0;
				int jj=0;
				for(ii=0;ii<N_cells;ii++)
					{
						
						for(jj=0;jj<N_cells;jj++)
							{
								datos_new<<cells_new[ii][jj]<<",";
								// std::cout<<ii<<" "<<jj<<std::endl;
							}
						datos_new<<std::endl;
					}
				// std::cout<<t<<" "<<ii<<" "<<jj<<std::endl;
			}

		void add_grain(Crandom &rand64)
			{	
				for (int ii=0;ii<N_cells;ii++)
					{for (int jj=0;jj<N_cells;jj++)
						{cells_new[ii][jj]=cells[ii][jj];}}

				int x=rand64.r()*N_cells;
				int y=rand64.r()*N_cells;
				cells_new[x][y]=cells[x][y]+1;
			}

		void topple(bool print=false)
			{
				for(int ii=0;ii<N_cells;ii++)
					{for(int jj=0;jj<N_cells;jj++)
						{	
							if(print){std::cout<<ii<<"-,"<<jj<<std::endl;}
							if (cells[ii][jj]>3) // propio
								{	cells_new[ii][jj]-=4;
									if(print)
										{std::cout<<"propio"<<std::endl;}}

							if(ii!=0)
								{
									if (cells[ii-1][jj]>3) //izquierda
										{cells_new[ii][jj]+=1;
										if(print)
											{std::cout<<"izquierda"<<std::endl;}
										}

								}
							if (ii!=N_cells-1)
								{
									if (cells[ii+1][jj]>3) //derecha
										{cells_new[ii][jj]+=1;
										if(print)
										{std::cout<<"derecha"<<std::endl;}
										}
									
								}

							if(jj!=0)
								{
									if (cells[ii][jj-1]>3) //arriba
										{cells_new[ii][jj]+=1;
										if(print)
											{std::cout<<"arriba"<<std::endl;}
										}
									
								}

							if (jj!=N_cells-1)
								{
									if (cells[ii][jj+1]>3) //abajo
										{cells_new[ii][jj]+=1;
										if(print)
										{std::cout<<"abajo"<<std::endl;}}
									
								}

						}
					}
			}
		
		void time_step(Crandom &rand64, int time)
			{
				if (true)
					{save_data(time);}

				int piles=count_piles();
				std::cout<<time<<"->"<<piles<<std::endl;
				if(piles==0)
					{add_grain(rand64);
					std::cout<<"adding grain"<<std::endl;
					}
				else
					{topple(false);}


				

				update_cells();


			}

};


int main(void)
	{	
		t_max=300;
		Crandom rand64(1);
		BTW_model model;
		model.start(0);

		config.open("datos/config.csv");
		write_config();
		config.close();

		for(int time=0;time<t_max;time++)
			{	
				datos.open("datos/datos"+std::to_string(time) +".csv",std::ios::app);
				// datos_new.open("datos/datos"+std::to_string(time) +"_new.csv",std::ios::app);
				model.time_step(rand64,time);
				datos.close();
				// datos_new.close();
				std::cout<<"---------"<<std::endl;

			}

	}

