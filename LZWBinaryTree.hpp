// LZWBinaryTree.hpp
//
// This file contains the definition of the LZWBinaryTree and the Node classes.
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

#ifndef LZW_BINARY_TREE_H
#define LZW_BINARY_TREE_H

class LZWBinaryTree
{
public:
	LZWBinaryTree();	//konstruktor
	~LZWBinaryTree();	//destruktor
	LZWBinaryTree (const LZWBinaryTree &tree);	//másoló konstruktor

	LZWBinaryTree & operator<< (std::vector<int> & binTreeVector);
	LZWBinaryTree & operator<< (char b);

	void write();
	void write(std::ostream & outfile);

	int getDepth();
	double getMean();
	double getVar();

	friend std::ostream & operator<< (std::ostream &outfile, LZWBinaryTree &bTree);

private:
	class Node
	{
	public:
		Node(char const b = '/');	//konstruktor
		~Node();	//destruktor

		char getLetter() const;
		Node *nullChild() const;
		Node *oneChild() const;

		void createNullChild(Node * child);
		void createOneChild(Node * child);

	private:
		Node(Node const &);	//másoló konstruktor
		Node & operator=(Node const &);	//másoló értékadás operátor (a.k.a egyenlőségjel)

		char letter;
		Node *leftNull;
		Node *rightOne;

	};

	Node* copy(Node* element);

	
	//LZWBinaryTree & operator= (const LZWBinaryTree &);	//másoló értékadás operátor (a.k.a egyenlőségjel)

	Node *bTree;
	int depth, meanSum, meanPc;
	double varSum;
	void write(Node *actual, std::ostream & outfile);
	void free(Node *actual);

protected:
	Node root;
	int maxDepth;
	double mean, var;
	void rdepth(Node *actual);
	void rmean(Node *actual);
	void rvar(Node *actual);

};

#endif //LZW_BINARY_TREE_H