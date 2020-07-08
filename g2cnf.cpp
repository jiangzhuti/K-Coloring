#include <fstream>
#include <string>
#include <vector>
#include <iostream>

const static char* usage = "%s <#gfile> <#colors> [#out-file]\n";
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
    if (argc < 3) {
        printf(usage, argv[0]);
        return 1;
    }
    std::ifstream ifs(argv[1]);
    if (!ifs.is_open()) {
        std::cout << "cannot open " << argv[1] << std::endl;
        return 1;
    }
    int colors;
    try {
        colors = std::stod(argv[2]);
        if (colors < 1) {
            std::cout << "colors must > 1" << std::endl;
            return 1;
        }
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    std::ostream* ostream = &std::cout;
    std::ofstream ofs;
    if (argc == 4) {
        ofs.open(argv[3]);
        if (!ofs.is_open()) {
            std::cout << "cannot open " << argv[3] << std::endl;
            return 1;
        }
        ostream = &ofs;
    }
    int nodes = -1, edges = -1;
    double prob = 0.0;
    std::string line;
    std::getline(ifs, line);
    auto token = split(line, " ");
    if (token.size() != 3) {
        std::cout << "wrong format" << std::endl;
        return 1;
    }
    nodes = std::stoi(token[0]);
    edges = std::stoi(token[1]);
    prob = std::stod(token[2]);
    *ostream << "p cnf " << colors * nodes << " " << nodes + nodes * colors * (colors - 1) / 2 + colors * edges << std::endl;

    std::vector<std::vector<int>> vec;
    for (int i = 0; i < nodes+1; i++) {
        std::vector<int> v(colors+1);
        for (auto& x : v) {
            x = 0;
        }
        vec.emplace_back(v);
    }
    int cnt = 1, x = 1, y = 1;
    for (x = 1; x <= nodes; x++) {
        for (y = 1; y <= colors; y++) {
            vec[x][y] = cnt;
            cnt++;
        }
    }
    for (x = 1; x <= nodes; x++) {
        for (y = 1; y <= colors; y++) {
            *ostream << vec[x][y] << " ";
            if (y == colors) {
                *ostream << "0" << std::endl;
            }
        }
    }
    *ostream << "\n";
    for (x = 1; x <= nodes; x++) {
        for (y = 1; y <= colors-1; y++) {
            for (int d = y + 1; d <= colors; d++) {
                *ostream << "-" << vec[x][y] << " -" << vec[x][d] << " 0" << std::endl;
            }
        }
    }
    for (int i = 0; i < edges; i++) {
        if (!std::getline(ifs, line)) {
            std::cout << "some edges are missing" << std::endl;
            return 1;
        }
        token = split(line, " ");
        x = std::stoi(token[0].c_str());
        y = std::stoi(token[1].c_str());

        for (int d = 1; d <= colors; d++) {
            *ostream << "-" << vec[x][d] << " -" << vec[y][d] << " 0" << std::endl;
        }
    }

    return 0;
}
