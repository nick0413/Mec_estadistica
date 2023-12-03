#include <iostream>
#include <cmath>
#include <fstream>
#include "Random64.h"

std::ofstream datos;
std::ofstream config;

const int N_cells=256;
int t_max;

void write_config(void)
	{
		config<<N_cells<<","<<t_max<<std::endl;
		// config<<t_max<<std::endl;
	}

void displayProgressBar(float progress, int barWidth = 70, std::string line="=")
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



class fores_fire_model
{
	public:

		int cells[L][L];

		fores_fire_model(Crandom &rand)
			{
				for (int i = 0; i < L; ++i)
				{
					for (int j = 0; j < L; ++j)
					{
						cells[i][j] = int(rand.r()*3);
					}
				}
			}



		void writeMatrixToFile(const std::string& filename) 
		{
			std::ofstream outFile(filename, std::ios::binary);
			if (outFile.is_open()) 
				{
					for (int i = 0; i < L; ++i) 
					{	for (int j = 0; j < L; ++j) 
							{outFile.write(reinterpret_cast<const char*>(&cells[i][j]), sizeof(cells[i][j]));}
					}
					outFile.close();
				} 
			
			else {
				std::cerr << "Unable to open file";
			}
		}

};

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

		void write_bin(std::string filename)
			{
				std::ofstream out(filename, std::ios::binary);
				for(int i = 0; i < N_cells; i++)
					for(int j = 0; j < N_cells; j++)
						out.write((char*)&cells[i][j], sizeof(cells[i][j]));
				out.close();
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
				update_new();


				if (true)
					{write_bin("datos/datos"+std::to_string(time) +".bin");}

				int piles=count_piles();

				if(piles==0)
					{add_grain(rand64);
					grains_added=true;}
				else
					{topple(false);}


		
				update_cells();


			}

};


int main()
{
	Crandom rand(1);
	fores_fire_model forest(rand);

	forest.writeMatrixToFile("forest.bin");


	return 0;

}