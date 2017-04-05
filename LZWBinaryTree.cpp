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

#include <iostream>
#include <cmath>
#include "LZWBinaryTree.hpp"

//-----------------------------------------------Binfa Public------------------------------------------------//

LZWBinaryTree::LZWBinaryTree()		//constructor
{
//	std::cout << "Binary tree constructor" << std::endl;
	bTree = &root;
}
LZWBinaryTree::~LZWBinaryTree()	//deconstructor
{
	free(root.oneChild());
	free(root.nullChild());
}

LZWBinaryTree & LZWBinaryTree::operator<< (char b)
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

//-----------------------------------------------Node Public------------------------------------------------//

LZWBinaryTree::Node::Node(char b)
{
//	std::cout << "Node constructor" << std::endl;
	letter = b;
	leftNull = nullptr;
	rightOne = nullptr;
}
LZWBinaryTree::Node::~Node()
{
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
