#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include "Random64.h"

double gamma_v;//viscosity
double T;//temperature

double Kb=1;//Boltzmann constant
double KbT;//temperature*Kb


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
		if (progress>=1.0)
			{std::cout<<"\n";}
	}

class particle
{
	public:
		double x,vx;
		double v_dt;
		int m=1;
		float F_m=0.0;
		double dt=0.1;
		double alpha=0.0;
		// declare an empty std::vector
		std::vector<double> xpos;
		std::vector<double> vel;

		// constructor
		particle(double v0,double dt)
		{
			x = 0.0;
			vx = v0;
			v_dt=v0;
			alpha=1-exp(-dt*gamma_v);
		}

		double Vi(double dt)
			{
				double Vi=vx;
				return Vi;
			}

		double dV(Crandom & rand64,double dt)
			{
				double dV=-alpha*Vi(dt)+sqrt(alpha*(2-alpha)*KbT/m)*rand64.gauss(0,1);
				return dV;
			}

		void time_step(Crandom & rand64,double dt)
			{
				double new_x=x+(Vi(dt)+0.5*dV(rand64,dt))*dt;
				// std::cout<<Vi(dt)<<"\t"<<dV(rand64,dt)<<"\t"<<new_x<<"\t"<<rand64.gauss(0,1) <<std::endl;
				double new_v=Vi(dt)+dV(rand64,dt);
				x=new_x;
				vx=new_v;
				xpos.push_back(x);
				vel.push_back(vx);
			}
};		

int main(int argc, char *argv[])
	{	
		int c;

		double dt=0.01;
		int t_max=10000;
		std::string a="-a";
		std::string b="-b";


		if(argc>1)
			{
				std::string arg1 = argv[1];

				if(arg1==a)
					{c=1;}

				else if(arg1==b)
					{c=2;}		
				else{c=3;}
			}
		else{c=3;}

		int N_particles=5000;
		int V0=10;
		dt=0.01;
		V0=10;
		switch (c)
			{
				case 1:
					gamma_v=1;
					N_particles=1000;
					T=4;
					t_max=10000;
					std::cout<<"Simulando punto A"<<std::endl;

					break;
				case 2:
					gamma_v=0.05;
					N_particles=1000;
					T=4;
					t_max=250;
					std::cout<<"Simulando punto B"<<std::endl;
					break;
				default:
					std::cout<<"default config"<<std::endl;
					gamma_v=1;
					T=4;
					t_max=1000;
					break;
			}

		KbT=Kb*T;

		Crandom rand64(2);
		
		std::ofstream config("config"+std::to_string(c)+".txt");
		std::ofstream times("times"+std::to_string(c)+".txt");
		config<<t_max<<"\n";
		config<<dt<<"\n";
		config<<V0<<"\n";
		config<<T<<"\n";
		config<<gamma_v<<"\n";
		config.close();

		int iterations=(float)t_max/dt;
		
		for(int ii=0;ii<N_particles;ii++)
			{
				particle p1(V0,dt);
				std::ofstream file("positions"+std::to_string(c)+"/data_"+std::to_string(ii)+".bin",std::ios::binary);
				std::ofstream file2("velocities"+std::to_string(c)+"/data_"+std::to_string(ii)+".bin",std::ios::binary);
				for(int jj=0;jj<iterations;jj++)
					{
						
						p1.time_step(rand64,dt);

						file.write(reinterpret_cast<char*>(&p1.x),sizeof(double));
						file2.write(reinterpret_cast<char*>(&p1.vx),sizeof(double));

						
					}
				file.close();
				file2.close();
				displayProgressBar((float)(ii+1)/N_particles);
			}

		std::cout<<"Simulacion terminada"<<std::endl;
		return 0;
	}