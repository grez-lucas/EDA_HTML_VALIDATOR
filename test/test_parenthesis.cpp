#include "adts/node.hpp"
#include "adts/queue.hpp"
#include "adts/stack.hpp"
#include <iostream>
#include <cstring>
#include <fstream>

using namespace eda;
using namespace std;

/// @brief Validate HTML tags
/// @param inFile Input file
/// @param outFile Output file
/// @return true if there is an error, false otherwise

bool validateHTMLTags(ifstream &inFile, ofstream &outFile)
{
	int pos = 0;
	bool error = false;
	int i = 0, j;
	string buffer, tag;
	Stack stack;
	while (getline(inFile, buffer))
	{
		i++;
		j = 0;

		outFile << "LINE "<< i <<":" << buffer << std::endl;
		while (!error && j < buffer.length())
		{
			if (buffer[j] == '<')
			{
				j++;
				tag = "";
				while (buffer[j] != '>') // fetch tag
				{
					tag += buffer[j];
					j++;
				}

				outFile << "\tTag Recognized: " << tag << std::endl;
				if (tag[0] != '/')
				{
					stack.push(new Node(tag));
				}
				else
				{
					if (stack.isEmpty())
					{
						error = true;
						outFile << "Error at line " << i << ": ";
						outFile << "No opening tag found for <" << tag << ">" << std::endl;
					}
					else
					{
						Node *node = stack.top();
						if (node->getData() != tag.substr(1))
						{
							error = true;
							outFile << "Error at line " << i << ": ";
							outFile << "Expected </" << node->getData() << "> but found <" << tag << ">" << std::endl;
						}
						else
						{
							outFile << "\ttag <" << tag.substr(1) << "> OK" << std::endl;
							stack.pop();
						}
					}
				}
			}
			j++;
		}
	}


	if (error)
	{
		std::cout << "Error at line: " << i << std::endl;
	}
	else{
		std::cout << "No errors found" << std::endl;
	}

	return error;
}

int main(int nargs, char **vargs)
{
	if (nargs == 1)
	{
		std::cout << "Porfavor ingrese el nombre del archivo a validar como argumento" << std::endl;
		return 1;
	}

	string file_name = vargs[1];

	file_name = "../html/" + file_name; // TODO: read from command line vargs[1]

	ifstream inFile;
	ofstream outFile;

	inFile.open(file_name);
	outFile.open("../html/" + file_name.substr(0, file_name.length() - 5) + ".log");

	bool error = validateHTMLTags(inFile, outFile);

	std::cout << "output-> " << file_name.substr(0, file_name.length() - 5) + ".log" << std::endl;

	inFile.close();
	outFile.close();

	return 0;
}
