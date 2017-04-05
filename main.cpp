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
#include <string>
#include <limits>
#include "LZWBinaryTree.cpp"

void usage();

int main (int argc, char *argv[])
{
	bool print_tree = false;
	if (argc < 5)
	{
		usage ();
		return -1;
	}
	else if (argc == 6)
		print_tree = true;

	std::fstream inFile (argv[1], std::ios_base::in);

	if (!inFile)
	{
		std::cout << argv[1] << " does not exist.." << std::endl;
		usage ();
		return -3;
	}

	long long int bytes_limit = std::stol(argv[2]);
	bytes_limit *= 1024 ; // kilobytes -> bytes

	if (bytes_limit < 0)
		bytes_limit = std::numeric_limits<long long int>::max();

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
		if (b == '>')
		{
			comment = true;
			continue;
		}

		if (b == 0x0a) //new line
		{
			comment = false;
			continue;
		}

		if (comment)
			continue;

		if (b == 'N')
			continue;

		for (int i = 0; i < 8; ++i)
		{
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

	return 0;
}

void usage (void)
{
	std::cout << "Usage: lzw input kilobytes -o output [-tree]" << std::endl
	<< "kilobytes: -1 means the whole file" << std::endl
	<< "-tree: print tree into output" << std::endl;
}
