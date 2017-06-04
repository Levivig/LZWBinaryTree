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
#include <vector>
#include "LZWBinaryTree.cpp"

void usage();

int main (int argc, char *argv[])
{
	bool print_tree = false;
	
	if (argc < 4)
	{
	//ha nincs megfelelő számú argumentum
	//szólunk a usernek erről
		usage ();
		return -1;
	}
	else if (argc == 5)
		print_tree = true;	

	std::fstream inFile (argv[1], std::ios_base::in);	

	if (!inFile)	
	{
	//infile nem létezik
		std::cout << argv[1] << " does not exist.." << std::endl;
		usage ();
		return -3;
	}

	if (argv[2][1] != 'o')
	{
		usage();
		return -1;
	}

	std::fstream outFile (argv[3], std::ios_base::out);

	LZWBinaryTree binTree;

	binTree << inFile;

/*
	binTree << '1' << '0'<< '1';

	LZWBinaryTree binTree2(binTree);

	binTree2 << '1';
	binTree << '1';
*/

	if(print_tree)
		outFile << binTree;

	outFile << "depth = " << binTree.getDepth () << std::endl;
	outFile << "mean = " << binTree.getMean () << std::endl;
	outFile << "var = " << binTree.getVar () << std::endl;

/*	std::cout << std::endl;

	if(print_tree)
		std::cout << binTree2;

	std::cout << "depth = " << binTree2.getDepth () << std::endl;
	std::cout << "mean = " << binTree2.getMean () << std::endl;
	std::cout << "var = " << binTree2.getVar () << std::endl;
*/

	outFile.close ();
	inFile.close ();


	return 0;
}

void usage (void)
{
	std::cout << "Usage: lzw input -o output [-tree]" << std::endl
	<< "-tree: print tree into output" << std::endl;
}
