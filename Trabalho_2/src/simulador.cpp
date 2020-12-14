#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <iterator>
#include "../include/wrapper.hpp"

using namespace std;

int main(int argc, char const *argv[]){
	wrapper* w = new wrapper();
	w->AplicacaoTransmissora();
	 
	delete w;
	return 0;
}
