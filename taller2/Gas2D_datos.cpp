//Simulacion de un gas ideal 2D de N=25 partículas
#include <iostream>
#include <cmath>
#include <fstream>
#include "Random64.h"
using namespace std;

ofstream posiciones;
ofstream velocidades;

const double Deltat=0.001;
const double chi=0.193183325037836;

const double VEL0=2;
const double K=50;

const double Lx=300,Ly=300;
const int Nx=15,Ny=15;

const int N=Nx*Ny;

class Cuerpo;
class Colisionador;
//--------------------class Cuerpo -----------------------
class Cuerpo
	{
		private:
		double m,R,x,y,Vx,Vy,Fx,Fy;
		public:
		void Inicie(double x0,double y0,double Vx0,double Vy0,double m0,double R0)
			{
				x=x0; 
				y=y0; 
				Vx=Vx0; 
				Vy=Vy0; 
				m=m0; 
				R=R0;
			}
		void Mueva_r1(double dt);
		void Mueva_V(double dt);
		void Mueva_r2(double dt);
		void Dibujese(void);

		double Getx(void){return x;};
		double Gety(void){return y;};
		double GetEc(void){return m*(Vx*Vx+Vy*Vy);};
		double GetV(void){return sqrt(Vx*Vx+Vy*Vy);};
		double GetVx(void){return Vx;};

		void write_pos(void){posiciones<<x<<","<<y<<",";};
		void write_vel(void){velocidades<<Vx<<","<<Vy<<",";};
		
		friend class Colisionador;
	};

void Cuerpo::Mueva_r1(double dt){
  x+=Vx*chi*dt;  y+=Vy*chi*dt;
}
void Cuerpo::Mueva_V(double dt){
  Vx+=Fx*(dt/(2*m));  Vy+=Fy*(dt/(2*m));
}
void Cuerpo::Mueva_r2(double dt){
  x+=Vx*((1-2*chi)*dt);  y+=Vy*((1-2*chi)*dt);
}
void Cuerpo::Dibujese(void){
  posiciones<<" , "<<x<<"+"<<R<<"*cos(t),"<<y<<"+"<<R<<"*sin(t)"; 
}
//--------------------class Colisionador -----------------------
class Colisionador
	{
		private:
		double Ep;
		public:
		void Inicie(void);
		void CalculeFuerzas(Cuerpo *Grano);
		void Choque(Cuerpo &Cuerpo1,Cuerpo &Cuerpo2,bool print);
		double GetEp(void){return Ep;};
	};
void Colisionador::Inicie(void){
  Ep=0;
}
void Colisionador::CalculeFuerzas(Cuerpo *Grano){
  int i,j;
  Ep=0;
  for(i=0;i<N+4;i++)  
    {Grano[i].Fx=0; Grano[i].Fy=0;}
	for(i=0;i<N;i++)  
		for(j=i+1;j<N+4;j++)
			{	
				if (j<N)
					Choque(Grano[i],Grano[j],true);
				else
				Choque(Grano[i],Grano[j],false);
			}
		
}
void Colisionador::Choque(Cuerpo &Cuerpo1,Cuerpo &Cuerpo2,bool print){
	double h,R1,R2,dx,dy,d,m1,m2,m12,Fn,Fx,Fy;
	dx=Cuerpo1.x-Cuerpo2.x;  
	dy=Cuerpo1.y-Cuerpo2.y;

	d=sqrt(dx*dx+dy*dy);  
	
	h=Cuerpo1.R+Cuerpo2.R-d;
	// if(print && h>0)
	// 	{
	// 	std::cout<<"----------------------------------\n";
	// 	std::cout<<"h: "<<h<<"\n";
	// 	std::cout<<Cuerpo1.x<<"\t"<<Cuerpo1.y<<"\n";
	// 	std::cout<<Cuerpo2.x<<"\t"<<Cuerpo2.y<<"\n";
	// 	std::cout<<"+++++++++++++++++++++++++++++++++++\n";
	// 	}

	if(h>0){
		Fn=K*pow(h,1.5);
		Fx=Fn*dx/d;    Fy=Fn*dy/d;
		Cuerpo1.Fx+=Fx;     Cuerpo1.Fy+=Fy;
		Cuerpo2.Fx-=Fx;     Cuerpo2.Fy-=Fy;
		Ep+=K/2.5*pow(h,2.5);
	}
	}

//--------------------Funciones de Animacion -----------------------

void new_line(ofstream & write)
	{write<<"\n";}

void InicieAnimacion(void){
  //    posiciones<<"set terminal gif animate"<<endl; 
  //    posiciones<<"set output 'Relajacion.gif'"<<endl;
  posiciones<<"unset key"<<endl;
  posiciones<<"set xrange [-10:110]"<<endl;
  posiciones<<"set yrange [-10:110]"<<endl;
  posiciones<<"set size ratio -1"<<endl;
  posiciones<<"set parametric"<<endl;
  posiciones<<"set trange [0:7]"<<endl;
  posiciones<<"set isosamples 12"<<endl; 
}
void InicieCuadro(void){
    posiciones<<"plot 0,0 ";
    //    posiciones<<" , "<<100/7<<"*t,0";
    //    posiciones<<" , "<<100/7<<"*t,100";
    //    posiciones<<" , 0,"<<100/7<<"*t";
    //    posiciones<<" , 100,"<<100/7<<"*t";
}
void TermineCuadro(void){
    posiciones<<endl;
}



int main()
	{	
		posiciones.open("Gas2D_pos.txt");
		velocidades.open("Gas2D_vel.txt");
		Cuerpo Grano[N+4];
		Colisionador Cundall;
		Crandom Ran2(0);
		double t,tdibujo,dx,dy,R,Alpha,Ec,Ep;  
		int i;

		int fps=30;

		float q =1.0/fps;
		float t_max=5;
  
		Cundall.Inicie();
		//-------------(x0 ,y0,Vx0,Vy0,m0,R0);

		//PAREDES
		
		Grano[N].Inicie(-10000,Ly/2,0,0,0.1,10000); //Pared izquierda		
		Grano[N+1].Inicie(Lx+10000,Ly/2,0,0,0.1,10000); //Pared derecha		
		Grano[N+2].Inicie(Lx/2,-10000,0,0,0.1,10000); //Pared abajo
		Grano[N+3].Inicie(Lx/2,Ly+10000,0,0,0.1,10000); //Pared arriba


		dx=Lx/(Nx+1); 
		dy=Ly/(Ny+1); 
		R=dx/3; 
		
		if(dx/3 > dy/3) 
			R=dy/3;
		// R=5;
		std::cout<<"R: " <<R<<"\n";
		std::cout<<"q: "<<q<<"\n";
		std::cout<<"t_max: "<<t_max<<"\n";
		for(i=0;i<N;i++){
			Alpha=2*M_PI*Ran2.r();

			Grano[i].Inicie(Ran2.r()*Lx,Ran2.r()*Ly,VEL0*cos(Alpha),VEL0*sin(Alpha),1,R);

		}

		// Grano[0].Inicie(30,50,VEL0,0,1,R);
		// Grano[1].Inicie(70,50,-VEL0,0,1,R);

		for(t=tdibujo=0;t<t_max;t+=Deltat,tdibujo+=Deltat){

			if(tdibujo>=q)
				{

					for(i=0;i<N;i++) 
						{Grano[i].write_pos();}
					new_line(posiciones);
					for(i=0;i<N;i++) 
						{Grano[i].write_vel();}
					new_line(velocidades);

					tdibujo=0;
					
				}
			// std::cout<<t<<"\n";

			for(Ec=0,i=0;i<N;i++)
			Ec+=Grano[i].GetEc();
			Ep=Cundall.GetEp();
			
			// if (t>2)
			// for(i=0;i<N;i++) cout<<Grano[i].GetVx()<<endl;      //ACTIVAR PARA EL HISTOGRAMA DE Vx
			// for(i=0;i<N;i++) cout<<Grano[i].GetV()<<endl;      //ACTIVAR PARA EL HISTOGRAMA DE V


			//Velocidad Verlet Optimizado
			for(i=0;i<N;i++) Grano[i].Mueva_r1(Deltat);
			Cundall.CalculeFuerzas(Grano);
			for(i=0;i<N;i++) {Grano[i].Mueva_V(Deltat);Grano[i].Mueva_r2(Deltat);}
			Cundall.CalculeFuerzas(Grano);
			for(i=0;i<N;i++) {Grano[i].Mueva_V(Deltat);Grano[i].Mueva_r1(Deltat);}
		}
		posiciones.close();
		return 0;
	}
