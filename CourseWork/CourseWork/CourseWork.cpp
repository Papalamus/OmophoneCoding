// CourseWork.cpp : main project file.

#include "stdafx.h"

#include <iostream>
#include <fstream>
#include <iostream>
#include <vector>
#include <array>
#include <algorithm> 
#include <string>
#include <map>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


using namespace System;


std::map<char, int> mapTheString(std::string text)
{
	std::map<char, int> dict;

	#pragma omp parallel shared(text)
	{
		int theadRank = omp_get_thread_num();
		int threadNum = omp_get_num_threads();

		int chunk = text.length()/threadNum;

		int beginNum =theadRank*chunk;
		int endNum=(theadRank==(threadNum-1))?text.length():beginNum + chunk;
			std::map<char, int> localDict;

			for(int i = beginNum;i< endNum;i++){
				localDict[text[i]]++;
			}

			#pragma omp critical
			{
				for(auto it = localDict.begin(); it != localDict.end(); ++it)
				{
					dict[it->first]+=it->second;
				}
			}
		
	}
	return dict;
}

unsigned short findReplace(char ch , std::map<char,std::vector<unsigned short>> replaceMap){
	
	std::vector<unsigned short> options = replaceMap[ch];
	int size = options.size();
	int randIndex = rand()%size;
	return options[randIndex];
}


std::map<unsigned short,char> getKey(std::map<char,std::vector<unsigned short>> replaceMap){
	std::map<unsigned short,char> result; 
	for(auto it = replaceMap.begin(); it != replaceMap.end(); ++it){
		std::vector<unsigned short> tmp = it->second;
		for(int i = 0 ;i<tmp.size(); i++){
			result[tmp[i]] = it->first;
		}
	}
	return result;
}

std::map<char,std::vector<unsigned short>> getReplaceMap(short* randArr, int randSize,std::map<char, int> N)
{
	unsigned short* urandArr = (unsigned short*) randArr;
	std::map<char,std::vector<unsigned short>> result;
		
	for(auto it = N.begin(); it != N.end(); ++it){
		int size = it->second;
		std::vector<unsigned short> temp(size);
		for(int i = 0 ;i< size; i++){
			temp[i] = *(urandArr++);	
		}
		result[it->first] = temp;
	}
	return result;
}

 short* getRandomArray(int arr_size, int range_min, int range_max)
{
	int range_width= range_max-range_min+1;
	short* result = (short*)malloc( range_width*sizeof(short) );


	short* parr= (short*)malloc( range_width*sizeof(short) );
	
	for (int i= 0; i<range_width; i++ ) {
	    parr[i]= range_min+i;
	}
	 
	int arr2_size = range_width;
	int index;
	for (int i= 0; i<arr_size; i++ ) {
	    index= rand()%arr2_size;
	    result[i]= parr[index];
	    arr2_size--;
	    parr[index]= parr[arr2_size];
	}
	 
	if ( parr != NULL ) { free( parr ); parr= NULL; }
	return result; 
}

  int getRandomArrayLength( std::map<char, int> N){
		int sum = 0 ;
		for(auto it = N.begin(); it != N.end(); ++it){
			sum+=it->second;
		}
	return sum;
  }

  
int findMin(std::map<char, int> W){
	auto pr = std::min_element(W.begin(), W.end(),[](const std::pair<char, int>& p1, const std::pair<char, int>& p2) {
        return p1.second < p2.second; });
		return pr->second;
}


std::map<char, int> getN(std::map<char, int> W){
	int W_min =findMin(W);
	for(auto it = W.begin(); it != W.end(); ++it){
		it->second /= W_min;
	}
	return W;
}


#ifndef _OPENMP
#endif
int main(array<System::String ^> ^args)
{
	clock_t start, end;
 
    
	std::string t;
 std::FILE *fp = std::fopen("file.txt", "rb");
  if (fp)
  {
    
    std::fseek(fp, 0, SEEK_END);
    t.resize(std::ftell(fp));
    std::rewind(fp);
    std::fread(&t[0], 1, t.size(), fp);
    std::fclose(fp);
  }
	std::map<char, int> W;
	start = clock();
/*
	std::string t = "Hellooe";*/
	W = mapTheString(t);
	
	std::map<char, int> N = getN(W);
	int arrSize = getRandomArrayLength(N);

	short* rndArr = getRandomArray(arrSize,0,arrSize-1);
	std::map<char,std::vector<unsigned short>>  replaceMap = getReplaceMap(rndArr, arrSize,N);
	std::vector<unsigned short> cryptedText(t.length());
	
	for(int i = 0 ;i<t.length();i++){
		cryptedText[i] = findReplace(t[i],replaceMap);
	}

	std::map<unsigned short,char> key = getKey(replaceMap);

	/*#pragma omp for
	for(int i = 0 ;i<t.length();i++){
		std::cout<< key[cryptedText[i]];
	}*/
	end = clock();
 
    printf("The above code block was executed in %.4f second(s)\n", ((double) end - start) / ((double) CLOCKS_PER_SEC));
	std::cin.get();

}

