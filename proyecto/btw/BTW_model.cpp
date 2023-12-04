#include <iostream>
#include <cmath>
#include <fstream>
#include <exception>
#include <memory>
#include "Random64.h"

std::string folder="datos4";
std::ofstream datos;
std::ofstream datos_new;
std::ofstream config;
const int N_cells=250;
int t_max;
int start_time=25;

int lines=0;

void write_config(void)
	{
		config<<N_cells<<","<<t_max<<","<<start_time<< std::endl;
		// config<<t_max<<std::endl;
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

class BTW_model
{
	public:
		int cells[N_cells][N_cells];
		int cells_new[N_cells][N_cells];
		int last_x=0;
		int last_y=0;
		bool toppling=false;
		int topple_count=0;	
		bool grains_added=false;


	
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

		void start_fractal(int grains)
			{
				int x_center=N_cells/2;
				int y_center=N_cells/2;

				for(int ii=0;ii<N_cells;ii++)
				{for(int jj=0;jj<N_cells;jj++)
					{cells[ii][jj]=0;}	
				}
				cells[x_center][y_center]=grains;

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
							{datos<<cells[ii][jj]<<",";}
						datos<<std::endl;
					}
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

		void write_bin(std::string filename)
			{
				std::ofstream out(filename, std::ios::binary);
				for(int i = 0; i < N_cells; i++)
					for(int j = 0; j < N_cells; j++)
						out.write((char*)&cells[i][j], sizeof(cells[i][j]));
				out.close();
			}

		void add_grain(Crandom &rand64, bool in_center)
			{	
				for (int ii=0;ii<N_cells;ii++)
					{for (int jj=0;jj<N_cells;jj++)
						{cells_new[ii][jj]=cells[ii][jj];}}


				if(in_center)
					{cells_new[N_cells/2][N_cells/2]+=1;}
				else
					{cells_new[int(rand64.r()*N_cells)][int(rand64.r()*N_cells)]+=1;}

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
				update_new();


				if (time>=start_time)
					{write_bin(folder+"/datos"+std::to_string(time) +".bin");}

				int piles=count_piles();

				if(piles==0)
					{add_grain(rand64,true);
					grains_added=true;}
				else
					{topple(false);}


		
				update_cells();


			}

};


int main(void)
	{	
		t_max=30;
		Crandom rand64(1);
		BTW_model model;
		model.start(0);

		config.open(folder+"/config.csv");
		write_config();
		config.close();

		for(int time=0;time<t_max;time++)
			{	
				model.time_step(rand64,time);
				datos.close();

				displayProgressBar((float)time/t_max, 70,"=",time);

			}
		if(model.grains_added)
			{std::cout<<"grains added"<<std::endl;}
		else
			{std::cout<<"no grains added"<<std::endl;}

		return 0;

	}

