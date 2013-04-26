#include "xml.h"

#include <fstream>
#include <iterator>

XML::XML () {
	Loaded = false;
}

XML::~XML () {
}

void XML::Load (const std::string & file) throw(std::runtime_error) {
	std::fstream f;
//	char buf[READ_BUFFER];
//	size_t readen;
	f.open(file.c_str(), std::ios_base::in);
	if (!f.is_open()) {
		throw std::runtime_error("Can't open XML-file " + file);
	}
	/*Data.clear()
	while (f.good() && !f.eof() && readen != 0) {
		readen = f.readsome(buf, READ_BUFFER);
		Data.insert(Data.end(), buf, buf+readen);
	}*/
	Data.assign((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>( ));
	Data.push_back('\0');

	try {
		CurrentDocument.parse<0>(Data.data());
	} catch (rapidxml::parse_error & er) {
		throw std::runtime_error("Can't parse XML");
	}

	CurrentNode = NULL;
	Loaded = true;
}

void XML::Unload () {
	NodeSelected = false;
	Loaded = false;
}


bool XML::SelectSection (const std::string & name) throw(std::runtime_error) {
	if (!Loaded) {
		throw std::runtime_error("Load XML before navigating");
	}
	rapidxml::xml_node<>* tmp = CurrentDocument.first_node(name.c_str());
	if (!tmp) {
		return false;
	}
	CurrentNode = tmp;
	NodeSelected = true;
	return true;
}

bool XML::SelectNode (const std::string & name) throw(std::runtime_error) {
	if (!Loaded) {
		throw std::runtime_error("Load XML before navigating");
	}
	if (!NodeSelected) {
		throw std::runtime_error("Can't get subnode of undefined node");
	}
	rapidxml::xml_node<>* tmp = CurrentNode->first_node(name.c_str());
	if (!tmp) {
		return false;
	}
	CurrentNode = tmp;
	return true;
	
}

bool XML::SelectNextNode (const std::string & name) throw(std::runtime_error) {
	if (!Loaded) {
		throw std::runtime_error("XML is not loaded");
	}
	if (!NodeSelected) {
		throw std::runtime_error("No current node");
	}
	rapidxml::xml_node<>* tmp = CurrentNode->next_sibling(name.c_str());
	if (!tmp) {
		return false;
	}
	CurrentNode  = tmp;
	return true;
}

std::string XML::GetValue () throw(std::runtime_error) {
	if (!Loaded) {
		throw std::runtime_error("XML is not loaded");
	}
	if (!NodeSelected) {
		throw std::runtime_error("No current node");
	}
	std::string tmp = CurrentNode->value();
	return tmp;
}

std::string XML::GetValue (const std::string & subnode) throw(std::runtime_error) {
	if (!Loaded) {
		throw std::runtime_error("Load XML before navigating");
	}
	if (!NodeSelected) {
		throw std::runtime_error("Can't get subnode of undefined node");
	}
	rapidxml::xml_node<>* tmp = CurrentNode->first_node(subnode.c_str());
	if (!tmp) {
		throw std::runtime_error("No such subnode: " + subnode);
	}
	std::string ret = tmp->value();
	return ret;
}

std::string XML::GetAttribute (const std::string & attr) throw(std::runtime_error) {
	if (!Loaded) {
		throw std::runtime_error("Load XML before navigating");
	}
	if (!NodeSelected) {
		throw std::runtime_error("Can't get subnode of undefined node");
	}
	rapidxml::xml_attribute<>* tmp = CurrentNode->first_attribute(attr.c_str());
	if (!tmp) {
		throw std::runtime_error("No such attribute: " + attr);
	}
	std::string ret = tmp->value();
	return ret;
}

rapidxml::xml_node<>* XML::GetCurrentNode () throw(std::runtime_error) {
	if (!NodeSelected) {
		throw std::runtime_error("No node selected");
	}
	return CurrentNode;
}
