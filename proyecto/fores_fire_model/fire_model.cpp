#include <iostream>
#include <cmath>
#include <fstream>
#include "Random64.h"

const int L=256;

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


int main()
{
	Crandom rand(1);
	fores_fire_model forest(rand);

	forest.writeMatrixToFile("forest.bin");


	return 0;

}