#ifndef __XML_H__
#define __XML_H__

#include <stdexcept>
#include <string>
#include <vector>
#include "rapidxml/rapidxml.hpp"

#define READ_BUFFER 2048

class XML {
	public:
		XML ();
		virtual ~XML ();

		void Load (const std::string & file) throw(std::runtime_error);
		void Unload ();

		bool SelectSection (const std::string & name) throw(std::runtime_error);
		bool SelectNode (const std::string & name) throw(std::runtime_error);
		bool SelectNextNode (const std::string & name) throw(std::runtime_error);

		rapidxml::xml_node<>* GetCurrentNode () throw(std::runtime_error);

		std::string GetValue () throw(std::runtime_error);
		std::string GetValue (const std::string & subnode) throw(std::runtime_error);
		std::string GetAttribute (const std::string & attr) throw(std::runtime_error);

	protected:
	private:
		rapidxml::xml_node<> * CurrentNode;
		rapidxml::xml_document<> CurrentDocument;
		std::vector<char> Data;
		bool Loaded;
		bool NodeSelected;
};

#endif
