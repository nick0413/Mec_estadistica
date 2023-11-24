#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <fstream>
#include <exception>
#include <memory>
#include "Random64.h"

ofstream salida;


using namespace std;

const int Lx=1024;
const double p=0.5; //probabilidad de voltear la direccion de una celda 
const int Q=2; // q es el numero de flechas


class LatticeGas
    {
        private:
            double ff[Lx][Q], ff_new[Lx][Q]; // ff[pos][dir]
            int V[Q]; // Q=0 es derecha y Q=1 es izquierda
        public:
            LatticeGas(void)
                {
                    V[0]=1;
                    V[1]=-1;
                }
            void clear(void)
                {
                    for(int ix=0; ix<Lx; ix++)
                        {
                            for(int i=0; i<Q; i++)
                                {
                                    ff[ix][i]=ff_new[ix][i]=0;
                                }
                        }
                }
			void start(int N, double mu, double sigma)
				{   double ro;
					for(int ix=0; ix<Lx; ix++)
					{
						double ro=(N/(sigma*sqrt(2*M_PI)))*exp(-0.5*pow((ix-mu)/sigma,2.0));
						for(int i=0; i<Q; i++)
							{ff[ix][i]=ro/Q;}
					}
				}
            void Collision()
                {
                    
                    int ix,i,j;
                    for (ix=0;ix<Lx;ix++)
                        {
                            for(i=0;i<Q;i++)
                                {
                                    j=(i+1)%Q;
                                    ff_new[ix][i]=ff[ix][i]+(1-p)*(ff[ix][j]-ff[ix][i]);
                                }
                        }

                }
            void Advection(void)
                {
                    for(int ix=0; ix<Lx; ix++)
                        {
                            for(int i=0; i<Q; i++)
                                {
                                    ff[(ix+V[i]+Lx)%Lx][i]=ff_new[ix][i];   //esta estructura del indices genera 
                                                                            // condiciones de frontera periodicas:
                                                                            // Haga la prueba para difrentes valores 
                                                                            // de ix V[i] para darse cuenta.


                                }
                        }
                }
            double rho(int ix)
                {
                    double suma;
                    int i;
                    for(suma=0,i=0;i<Q;i++)
                        {
                            suma+=ff[ix][i];
                        }
                    return suma;
                }
            double variance(void)
                {
                    double k, Xprom, sigma2;
                    int ix;
                    for(k=0, ix=0; ix<Lx ; ix++)
                        {
                            k+=rho(ix);
                        }
                    for(Xprom=0, ix=0; ix<Lx; ix++)
                        {
                            Xprom+=ix*rho(ix);
                        }
                    Xprom=Xprom/k;
                    for(sigma2=0,ix=0; ix<Lx; ix++)
                        {
                            sigma2+=pow(ix-Xprom,2)*rho(ix);
                        }
                    sigma2=sigma2/(k);
                    return sigma2;
                }
            void show(void)
                {
                    for(int i=0; i<Q; i++)
                        {
                            for(int ix=0; ix<Lx; ix++)
                                {
                                    salida<<ff[ix][i];
                                }
                            salida<<"\n";

                        }
                }
            void showrho(void)
                {
                    for(int ix=0; ix<Lx; ix++)
                        {
                            salida<<ix<<" "<< rho(ix)<<"\n";
                        }
                }



    
    };



//----------------Principal----------------//
int main(void)
    {   
        int N=400;
        double mu=Lx/2;
        double sigma=Lx/8;
        int t, tmax=400;
        
        salida.open("datos.txt");

        LatticeGas Difusion;

        Difusion.clear();
        Difusion.start(N,mu,sigma);
        for(t=0; t<tmax; t++)
            {
                salida<<t<<" "<<Difusion.variance()<<"\n";
                Difusion.Collision();
                Difusion.Advection();
            }
        //Difusion.show();
        //Difusion.showrho();


        salida.close();
        return 0;


    }