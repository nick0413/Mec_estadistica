#include <fstream>
#include <string>

// Assume N is the size of your 2D array
const int N = 10;
int array[N][N];


void write_bin(std::string filename, int array[N][N])
{
	std::ofstream out(filename, std::ios::binary);
	for(int i = 0; i < N; i++)
		for(int j = 0; j < N; j++)
			out.write((char*)&array[i][j], sizeof(array[i][j]));
	out.close();
}

int main()
{
for(int i = 0; i < N; i++)
	for(int j = 0; j < N; j++)
		array[i][j] = i * N + j;

// Open the binary file in write mode
std::ofstream out("data.bin", std::ios::binary);

// Write the 2D array to the binary file
for(int i = 0; i < N; i++)
	for(int j = 0; j < N; j++)
		out.write((char*)&array[i][j], sizeof(array[i][j]));

// Close the binary file
out.close();	
}



