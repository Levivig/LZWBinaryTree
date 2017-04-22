// main.cpp
//
// Example usage of the LZWBinaryTree class
//
// -----------------------------------
// Copyright (C) Levente Vig 2017.
// -----------------------------------
//
// The following terms and conditions apply:
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <iostream>
#include <fstream>
#include <string>	//stol maitt
#include <limits>	//bytes_limit miatt
#include "LZWBinaryTree.cpp"

void usage();

int main (int argc, char *argv[])
{
	bool print_tree = false;
	
	if (argc < 5)
	{
	//ha nincs megfelelő számú argumentum
	//szólunk a usernek erről
		usage ();
		return -1;
	}
	else if (argc == 6)
		print_tree = true;	

	std::fstream inFile (argv[1], std::ios_base::in);	

	if (!inFile)	
	{
	//infile nem létezik
		std::cout << argv[1] << " does not exist.." << std::endl;
		usage ();
		return -3;
	}


	//a user megadhatja,hogy mennyit dolgozzunk fel
	//a bemeneti fileból
	long long int bytes_limit = std::stol(argv[2]);	//string to long int
	bytes_limit *= 1024; // kilobytes -> bytes

	if (bytes_limit < 0)
		bytes_limit = std::numeric_limits<long long int>::max();	//#include <limits>

	if (argv[3][1] != 'o')
	{
		usage();
  		return -1;
	}

	std::fstream outFile (argv[4], std::ios_base::out);

	unsigned char b;
	long long int bytes_read = 0;
	LZWBinaryTree binTree;

	bool comment = false;
	
	while (inFile.read ((char *) &b, sizeof (unsigned char)) && (bytes_read++ < bytes_limit))
	{
		//'>' karakterrel kezdődő sor komment, ígyazt figyelmen kívül hagyjuk
		if (b == '>')
		{
			comment = true;
			continue;
		}

		//ha új sor kezdődik, az már nem komment
		if (b == 0x0a) //new line
		{
			comment = false;
			continue;	//valamint, az újsor karaktereket sem dolgozzuk fel
		}

		if (comment)
			continue;

		//az N betűket sem dolgozzuk fel
		if (b == 'N')
			continue;


		for (int i = 0; i < 8; ++i)
		{
			//minden bit 1 vagy 0 (char) lesz
			//a konvertálást bit maszkolás és shiftelés segítségével végezzük
			//a maszkot így shifteljük 1000_0000 -> 0100_0000
			if (b & 0x80)
				binTree << '1';
			else
				binTree << '0';
			b <<= 1;
		}
	}

	if(print_tree)
		outFile << binTree;

	outFile << "depth = " << binTree.getDepth () << std::endl;
	outFile << "mean = " << binTree.getMean () << std::endl;
	outFile << "var = " << binTree.getVar () << std::endl;

	outFile.close ();
	inFile.close ();

	std::cout << "kimeneti file 4. karaktere: " << argv[4][3] << std::endl;

	return 0;
}

void usage (void)
{
	std::cout << "Usage: lzw input kilobytes -o output [-tree]" << std::endl
	<< "kilobytes: -1 means the whole file" << std::endl
	<< "-tree: print tree into output" << std::endl;
}
