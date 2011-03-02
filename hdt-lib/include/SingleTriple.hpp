/*
 * SingleTriple.hpp
 *
 *  Created on: 02/03/2011
 *      Author: mck
 */

#ifndef SINGLETRIPLE_HPP_
#define SINGLETRIPLE_HPP_

#include <iostream>
#include <string>
#include <vector>

namespace hdt {
class TripleID
{
	private:
		unsigned int subject;
		unsigned int object;
		unsigned int predicate;

	public:

		TripleID() {
		}

		TripleID(unsigned int subject, unsigned int predicate, unsigned int object) {
			this->subject = subject;
			this->predicate = predicate;
			this->subject = subject;
		}

		~TripleID() {

		}
		unsigned int getSubject() {
			return subject;
		}
		void setSubject(unsigned int subject) {
			this->subject = subject;
		}

		unsigned  getPredicate() {
			return predicate;
		}

		void setPredicate(unsigned int predicate) {
			this->predicate = predicate;
		}

		unsigned int getObject() {
			return object;
		}

		void setObject(unsigned int object) {
			this->object = object;
		}

		void clear() {
			subject = predicate = object = 0;
		}
};

class TripleString
{
	private:
		std::string subject;
		std::string predicate;
		std::string object;

	public:
		TripleString() {

		}

		TripleString(std::string subject, std::string predicate, std::string object) {
			this->subject = subject;
			this->predicate = predicate;
			this->object = object;
		}

		~TripleString() {

		}

		void readN3Line(std::string &line) {

			using namespace std;

			string value = line;
			int pos = 0;
			int firstIndex = 0;
			int lastIndex = 0;
			bool errorParsing = false;

			vector<string> node(3);

			while (true) {
				value = value.substr(firstIndex);

				if (value == "." || value == "\n" || value == "" || value.at(0) == '#')
					break;

				//obvious space
				if (value.at(0) == ' ') {
					//do nothing
					lastIndex = 0;
				}
				//URI
				else if (value.at(0) == '<') {
					lastIndex = value.find(">");
					//check size of pos
					if (pos > 2) {
						errorParsing = true;
						break;
					}
					node[pos] = value.substr(0, lastIndex + 1);
					pos++;
				}
				//Literal
				else if (value.at(0) == '"') {
					lastIndex = value.find('"', 1);
					//check if literal is escaped
					while (true) {
						bool escaped = false;
						int temp = lastIndex - 1;

						while (temp > 0 && value.at(temp) == '\\') {
							if (escaped)
								escaped = false;
							else
								escaped = true;
							temp--;
						}

						if (!escaped)
							break;
						lastIndex++;
						if (lastIndex == value.length())
							//Cannot find the (unescaped) end
							errorParsing = true;
						lastIndex = value.find('"', lastIndex);
						if (lastIndex == string::npos)
							//Cannot find the (unescaped) end
							errorParsing = true;
					}

					// literal can extend to a bit more than just the ",
					// also take into account lang and datatype strings
					if (value.at(lastIndex + 1) == '@') {
						// find end of literal/lang tag
						lastIndex = value.find(' ', lastIndex + 1) - 1;
					} else if (value.at(lastIndex + 1) == '^') {
						lastIndex = value.find('>', lastIndex + 1);
					}
					//check size of pos
					if (pos > 2) {
						errorParsing = true;
						break;
					}
					node[pos] = value.substr(0, lastIndex + 1);
					pos++;
				}
				//blank, a variable, a relative predicate
				else if (value.at(0) == '_' || (value.find(":") != string::npos)) {
					lastIndex = value.find(" ");
					//check size of pos
					if (pos > 2) {
						errorParsing = true;
						break;
					}
					node[pos] = value.substr(0, lastIndex);
					pos++;
				}
				//parameter or variable ---> obviate for Triples. In future, add to a Hash
				else if (value.at(0) == '@' || value.at(0) == '?') {
					break;
				}
				// test if number
				else {
					// else it is a parsing error
					lastIndex = value.find(" ");
					for (int j = 0; j < lastIndex; j++) {
						if (!isdigit(value.at(j)) && value.at(j) != '.' && value.at(j)
								!= ',') {
							errorParsing = true;
						}
					}

					if (errorParsing == false) {
						node[pos] = value.substr(0, lastIndex);
						pos++;
					}

					break;
				}

				firstIndex = lastIndex + 1;
			}

			if (errorParsing == true) {
				char* except =
						(char*) " :*********** FORMAT ERROR (NOT N3?) ***********";
				throw except;
			} else if (pos != 0 && pos != 3) {
				char* except =
						(char*) " :*********** FORMAT ERROR (NOT N3?) ***********";
				throw except;
			}

			subject = node[0];
			predicate = node[1];
			object = node[2];
		}

		std::string &getSubject() {
			return subject;
		}
		void setSubject(std::string &subject) {
			this->subject = subject;
		}

		std::string &getPredicate() {
			return predicate;
		}
		void setPredicate(std::string &predicate) {
			this->predicate = predicate;
		}

		std::string &getObject() {
			return object;
		}
		void setObject(std::string &object) {
			this->object = object;
		}

		friend std::ostream &operator<<(std::ostream &stream, const TripleString &ts) {
			stream << "TS: "<< ts.subject << ", "<< ts.predicate <<", "<< ts.object;

			return stream;
		}

		void clear() {
			subject = predicate = object = "";
		}

}; // TripleString{}

 class IteratorTripleID {
 	public:
 		virtual bool hasNext() { }
 		virtual TripleID next() { }
 };

 class IteratorTripleString {
 	public:
 		virtual bool hasNext() { }
 		virtual TripleString next() { }
 };
}

#endif /* SINGLETRIPLE_HPP_ */