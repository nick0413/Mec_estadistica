#include <iostream>
#include <cmath>
#include <fstream>
#include <exception>
#include <memory>
#include "Random64.h"

std::ofstream datos;
std::ofstream config;
const int N_cells=200;
int t_max;
int t=0;
int lines=0;

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
				if(start_val>3)
					{throw std::invalid_argument( "received invalid start_val" );}
				for(int ii=0;ii<N_cells;ii++)
				{for(int jj=0;jj<N_cells;jj++)
					{cells[ii][jj]=start_val;}	
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


		void add_grain(Crandom & random64)
			{
				last_x=random64.r()*N_cells;
				last_y=random64.r()*N_cells;
				cells[last_x][last_y]+=1;
			}
		void topple(Crandom & random64,int t,int count)
			{	
				float p=1/count;

				for(int ii=0;ii<N_cells;ii++)
					{for(int jj=0;jj<N_cells;jj++)
						{	
							if(cells[ii][jj]>3 )
								{	
									if(true)
										{
											cells_new[ii][jj]-=4;

											if(ii!=0)
												{cells_new[ii-1][jj]+=1;}
											if(ii!=N_cells-1)
												{cells_new[ii+1][jj]+=1;}

											if(jj!=0)
												{cells_new[ii][jj-1]+=1;}
											if(jj!=N_cells-1)
												{cells_new[ii][jj+1]+=1;}
											break;
											break;
											std::cout<<"topple\n";
											t++;
										}

								}
						}	
					}
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

		void safe_data(int t,bool print=false)
			{
				for(int ii=0;ii<N_cells;ii++)
					{for(int jj=0;jj<N_cells;jj++)
						{	
					
							datos<<cells[ii][jj]<<",";
						}
						datos<<std::endl;
					}
				// datos<<"--------"<<t<< "\n";;
			}


		void time_step(Crandom & random64)
			{	
				int count=count_piles();
				// std::cout<<count<<"\t"<<t <<"\n";
				if (count==0)
					{add_grain(random64);}


				if(t%10==0 && t>320000)
					{safe_data(t,true); lines++;}
				update_new();
				topple(random64,t,count);
				update_cells();
			}
};

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
int main(void)
	{	
		t_max=420000;
		datos.open("datos_"+std::to_string(t_max)+".csv");
		config.open("config.csv");
		

		BTW_model model;
		model.start(0);
		Crandom random64(2);
		

		write_config();
		while(t<t_max)
			{
				model.time_step(random64);
				displayProgressBar(float(t)/t_max);
				t++;
				// std::cout<<t<<"\n";

			}
		datos.close();
		config.close();
		std::cout<<"printed times: "<<lines<<"\n";
		std::cout<<"end\n";

		return 0;
	}

