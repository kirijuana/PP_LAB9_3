#include "pch.h"
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <Windows.h>
#include <string>
#include <ctime>
#include <fstream>

	using namespace std;
	string alph = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
	string ALPH = "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
	int count_alphs_1[33];

	std::ifstream in_2;
	std::ifstream in_1;
	string buffer1_1;
	string buffer2_2;
	int length_file_1 = 0;

	int main()
	{
		setlocale(LC_ALL, "Russian");
		setlocale(LC_ALL, "rus");

		MPI_Init(0, 0); //инициализация MPI
		int rank, size; //узнаем ранг каждого процесса и их общее число
		MPI_Comm_rank(MPI_COMM_WORLD, &rank);
		MPI_Comm_size(MPI_COMM_WORLD, &size);

		srand(time(NULL) * rank);

		string buffer1_1;
		string buffer2_1;

		string file_name;
		if (rank == 1)
		{
			file_name = "in_1.txt";
		}		
		if (rank == 2)
		{
			file_name = "in_2.txt";
		}

		if (rank != 0)
		{
			std::ifstream in_1;
			in_1.open(file_name);

			for (int i = 0; i < 33; i++)
			{
				count_alphs_1[i] = 0;
			}

			int length_file_1 = 0;
			while (!in_1.eof()) // ïîäñ÷åò êîë-âà ñëîâ â ôàéëå 
			{
				getline(in_1, buffer1_1, ' ');
				length_file_1++;
			}

			auto position_in_file_1 = 0;
			while (length_file_1 > 0)
			{
				in_1.seekg(position_in_file_1, ios::beg);
				getline(in_1, buffer1_1, ' ');
				position_in_file_1 = in_1.tellg();
			
				while (!in_1.eof())
				{
					getline(in_1, buffer2_1, ' ');
					if (buffer1_1[0] == buffer2_1[0])
					{
						for (int i = 0; i < 33; i++)
						{
							if (buffer1_1[0] == alph[i] || buffer1_1[0] == ALPH[i])
							{
								count_alphs_1[i]++;
								break;
							}
						}
					}
				}
				length_file_1--;
			}
			//cout << "File name: " << file_name << endl;
			//for (int i = 0; i < 33; i++)
			//{
			//	if (count_alphs_1[i] != 0)
			//	{
			//		printf("%c = %d\n", alph[i], count_alphs_1[i]);
			//		count_alphs_1[i] = 0;
			//	}
			//}
		}
		
		if (rank != 0)
		{
			MPI_Send(&count_alphs_1, 33, MPI_INT, 0, 0, MPI_COMM_WORLD);
			MPI_Send(&file_name, file_name.length(), MPI_INT, 0, 0, MPI_COMM_WORLD);
		}

		if (rank == 0)
				{
			int buf[33]; //для приема
			string buf_file_name;
					MPI_Status status;
		
					for (int i = 0; i < size - 1; i++)
					{
						MPI_Recv(&buf, 33, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
						MPI_Recv(&file_name, 8, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
						cout << "File name: " << file_name << endl;
						for (int j = 0; j < 33; j++)
						{
							if (buf[j] != 0)
							{
								cout << alph[j] << " = " << buf[j] << endl;
							}
						}
					}
				}
			
			MPI_Finalize(); 
			return 1;
		}

