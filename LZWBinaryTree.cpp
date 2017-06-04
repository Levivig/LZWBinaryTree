// LZWBinaryTree.cpp
//
// Contains the implementation of the LZWBinaryTree and Node classes
// To find out more about the compression method used read
//  man compress
// and the following site:
// 	http://www.progpater.blog.hu
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

#include <iosfwd>
#include <fstream>
#include <cmath>
#include <vector>
#include "LZWBinaryTree.hpp"

//-----------------------------------------------Binfa Public------------------------------------------------//

LZWBinaryTree::LZWBinaryTree()		//constructor
{
//	std::cout << "Binary tree constructor: " << this << std::endl;
	bTree = &root;
}
LZWBinaryTree::~LZWBinaryTree()	//destructor
{
//	std::cout << "Binary tree destructor: " << this << std::endl;
	free(root.oneChild());
	free(root.nullChild());
}
LZWBinaryTree::LZWBinaryTree (const LZWBinaryTree& tree)	//másoló konstruktor
{
	//copy függvény végzi a mélymásolást, 233.sorban található
	//átadjuk neki a bal és jobboldali gyermekeket (külön-külön), illetve a fa mutatót ahol éppen áll

//	std::cout <<"Binary tree copy constructor: " << this << std::endl;

	root.createOneChild ( copy(tree.root.oneChild(), tree.bTree) );
	root.createNullChild ( copy(tree.root.nullChild(), tree.bTree) );
}

LZWBinaryTree & LZWBinaryTree::operator= (const LZWBinaryTree &tree)
{
//	std::cout << "Binary tree = operator" << std::endl;

	if(this == &tree)
		return *this;

	root.createOneChild ( copy(tree.root.oneChild(), tree.bTree ) );  
	root.createNullChild ( copy(tree.root.nullChild(), tree.bTree ) );

	return (*this);
}


LZWBinaryTree & LZWBinaryTree::operator<< (std::vector<int> & binTreeVector)
{
	int b;

	for (unsigned int i = 0; i < binTreeVector.size(); ++i)
	{
		b = binTreeVector.at(i);

		if (b == 0)
		{
			if (bTree->nullChild() == nullptr)
			{
				Node *newChild = new Node('0');
				bTree->createNullChild(newChild);
				bTree = &root;
			}

			else
			{
				bTree = bTree->nullChild();
			}
		}

		else	//b == 1
		{
			if (bTree->oneChild() == nullptr)
			{
				Node *newChild = new Node('1');
				bTree->createOneChild(newChild);
				bTree = &root;
			}

			else
			{
				bTree = bTree->oneChild();
			}
		}
	}

	return (*this);
}

LZWBinaryTree& LZWBinaryTree::operator<< (char b)
{
	if (b == '0')
	{
		if (bTree->nullChild() == nullptr)
		{
			Node *newChild = new Node('0');
			bTree->createNullChild(newChild);
			bTree = &root;
		}

		else
		{
			bTree = bTree->nullChild();
		}
	}

	else	//b == 1
	{
		if (bTree->oneChild() == nullptr)
		{
			Node *newChild = new Node('1');
			bTree->createOneChild(newChild);
			bTree = &root;
		}

		else
		{
			bTree = bTree->oneChild();
		}
	}

	return (*this); 
}


LZWBinaryTree& LZWBinaryTree::operator<< (std::fstream& inFile)
{
	unsigned char b;
	bool comment = false;

	while (inFile.read ((char *) &b, sizeof (unsigned char)))
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
			if (b & 0x80)	//1
			{
				if (bTree->oneChild() == nullptr)
				{
					Node *newChild = new Node('1');
					bTree->createOneChild(newChild);
					bTree = &root;
				}

				else
				{
					bTree = bTree->oneChild();
				}
			}
			else	//0
			{
				if (bTree->nullChild() == nullptr)
				{
					Node *newChild = new Node('0');
					bTree->createNullChild(newChild);
					bTree = &root;
				}

				else
				{
					bTree = bTree->nullChild();
				}
			}

			b <<= 1;
		}
	}
	return (*this); 
}

void LZWBinaryTree::write(void)
{
	depth = 0;
	write(&root, std::cout);
}

int LZWBinaryTree::getDepth(void)
{
	depth = maxDepth = 0;
	rdepth(&root);
	return maxDepth - 1;
}

double LZWBinaryTree::getMean(void)
{
	depth = meanSum = meanPc = 0;
	rmean(&root);
	mean = ((double)meanSum) / meanPc;
	return mean;
}

double LZWBinaryTree::getVar(void)
{
	mean = getMean();
	varSum = 0.0;
	depth = meanPc = 0;

	rvar(&root);

	if (meanPc -1 > 0)
	{
		var = std::sqrt(varSum / (meanPc -1));
	}
	else
	{
		var = std::sqrt(varSum);
	}

	return var;
}

std::ostream & operator<< (std::ostream& outfile, LZWBinaryTree & bTree)
{
	bTree.write(outfile);
	return outfile;
}

//-----------------------------------------------Binfa Private------------------------------------------------//

void LZWBinaryTree::write(std::ostream& outfile)
{
	depth = 0;
	write(&root, outfile);
}

LZWBinaryTree::Node* LZWBinaryTree::copy(Node* element, Node* old_tree)
{
	Node* newptr = nullptr;	//segédváltozó, az új elemre fog mutatni, ez lesz a visszatési érték

	if (element != nullptr)	//rekurzió miatt, ha már nincs több elem, nem megyünk tovább
	{
		newptr = new Node(element->getLetter() );	//a paraméterként kapott elem betűjével létrehozunk egy új csomópontot

		//rekurzív hívás, az adott elem mindkét részfáját feldolgozzuk ugyanilyen módon
		newptr->createNullChild( copy( element->nullChild(), old_tree ) );
		newptr->createOneChild( copy( element->oneChild(), old_tree ) );

		//ha az elem ahol járunk az eredeti fának a "fa" mutatójával egyenló, akkor az újban is arra állítjuk
		//azért van rá szükség, mert ha további elemeket szúrunk be az új fánkba, akkor alapból nem tudjuk, hogy annak
		// a fa mutatója hova mutat
		if(old_tree == element)
			bTree = newptr;	//az új fa fa mutatója lesz az éppen feldolgozott elem
	}

	return newptr;
}

//-----------------------------------------------Node Public------------------------------------------------//

LZWBinaryTree::Node::Node(char b)
{
//	std::cout << "Node constructor: " << this << std::endl;
	letter = b;
	leftNull = nullptr;
	rightOne = nullptr;
}
LZWBinaryTree::Node::~Node()
{
//	std::cout << "Node destructor: " << this << std::endl;
}

char LZWBinaryTree::Node::getLetter() const
{
	return letter;
}

LZWBinaryTree::Node *LZWBinaryTree::Node::nullChild() const
{
	return leftNull;
}
LZWBinaryTree::Node *LZWBinaryTree::Node::oneChild() const
{
	return rightOne;
}

void LZWBinaryTree::Node::createNullChild(Node* child)
{
	leftNull = child;
}
void LZWBinaryTree::Node::createOneChild(Node* child)
{
	rightOne = child;
}

//Inforder
void LZWBinaryTree::write(LZWBinaryTree::Node* actual, std::ostream & outfile)
{
	if (actual != nullptr)
	{
		++depth;
		write(actual->oneChild(), outfile);

		for (int i = 0; i < depth; ++i)
		{
			outfile << "---";
		}

		outfile << actual->getLetter() << "(" << depth - 1 << ")" << std::endl;
		write(actual->nullChild(), outfile);
		--depth;
	}
}

/*
//Postorder
void LZWBinaryTree::write(LZWBinaryTree::Node* actual, std::ostream & outfile)
{
	if (actual != nullptr)	//recursion
	{
		++depth;
		write(actual->oneChild(), outfile);
		write(actual->nullChild(), outfile);

		for (int i = 0; i < depth; ++i)
		{
			outfile << "---";
		}

		outfile << actual->getLetter() << "(" << depth - 1 << ")" << std::endl;
		--depth;
	}
}

//Preorder
void LZWBinaryTree::write(LZWBinaryTree::Node* actual, std::ostream & outfile)
{
	if (actual != nullptr)	//recursion
	{
		++depth;
		for (int i = 0; i < depth; ++i)
		{
			outfile << "---";
		}

		outfile << actual->getLetter() << "(" << depth - 1 << ")" << std::endl;
		
		write(actual->oneChild(), outfile);
		write(actual->nullChild(), outfile);
		
		--depth;
	}
}
*/


void LZWBinaryTree::free(Node* actual)
{
	if (actual != nullptr)
	{
		free(actual->nullChild());
		free(actual->oneChild());
		
		delete actual;
	}
}

//-----------------------------------------------Binfa Proteced------------------------------------------------//

void LZWBinaryTree::rdepth(Node *actual)
{
	if (actual != nullptr)
	{
		++depth;
		if (depth > maxDepth)
		{
			maxDepth = depth;
		}
		rdepth(actual->oneChild());
		rdepth(actual->nullChild());
		--depth;
	}
}

void LZWBinaryTree::rmean(Node* actual)
{
	if (actual != nullptr)
	{
		++depth;
		rmean(actual->oneChild());
		rmean(actual->nullChild());
		--depth;

		if (actual->oneChild() == nullptr && actual->nullChild() == nullptr)
		{
			++meanPc;
			meanSum += depth;
		}
	}
}

void LZWBinaryTree::rvar(Node* actual)
{
	if (actual != nullptr)
	{
		++depth;
		rvar(actual->oneChild());
		rvar(actual->nullChild());
		--depth;

		if (actual->oneChild() == nullptr && actual->nullChild() == nullptr)
		{
			++meanPc;
			varSum += ((depth - mean) * (depth - mean));
		}
	}
}
