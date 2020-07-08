#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

const static char* usage = "%s <#cnf-file> [#sat-file]\n";
std::vector<std::string> split(const std::string& str, const std::string& delim)
{
    std::vector<std::string> tokens;
    size_t prev = 0, pos = 0;
    do {
        pos = str.find(delim, prev);
        if (pos == std::string::npos) pos = str.length();
        std::string token = str.substr(prev, pos-prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    }
    while (pos < str.length() && prev < str.length());
    return tokens;
}

int main(int argc, char** argv)
{
    if (argc != 3) {
        printf(usage, argv[0]);
        return 1;
	}
	// initilaize node& color
    std::ifstream ifs_c,ifs_s;
    int nodes, colors = 0;

    ifs_c.open(argv[1]);
    if (!ifs_c.is_open()){
        std::cout << "can't open file " << argv[1] << std::endl;
        return 1;
	}
    std::string line;
    std::getline(ifs_c, line);
    auto tokens = split(line, " ");
    nodes = std::stoi(tokens[2]);
    std::getline(ifs_c, line);
    tokens = split(line, " ");
    colors = tokens.size() - 1;
    nodes /= colors;
    std::vector<int> vars;
    ifs_s.open(argv[2]);
    if(!ifs_s.is_open()){
        std::cout << "can't open file " << argv[2] << std::endl;
        return 1;
	}
    std::getline(ifs_s, line);
    if (line == "UNSAT")	{
        std::cout << "result is unsatisfiable\n";
        return 1;
	}
    std::getline(ifs_s, line);
    tokens = split(line, " ");
    for (const auto& i : tokens) {
        vars.push_back(std::stoi(i));
    }
    for (int i = 0; i < nodes; i++){
        std::cout << "node " << i + 1;
        for (int j = 0; j < colors; j++){
            if (vars[i * colors + j] > 0){
                std::cout << ": color " << j + 1 << std::endl;
				break;
			}
		}
	}
    return 0;
}
