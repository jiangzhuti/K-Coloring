#include <random>
#include <string>
#include <cstdio>
std::random_device rd;
std::mt19937 gen(rd());

const static char* usage = "%s <#nodes> <#probability> [#out-file]\n";

int main(int argc, char** argv)
{
    if (argc < 3) {
        printf(usage, argv[0]);
        return 1;
    }
    int nodes;
    double prob;
    FILE* out = stdout;

    try {
        nodes = std::stoi(argv[1]);
        if (nodes < 2) {
            puts("nodes must >= 2");
            return 1;
        }
        prob = std::stod(argv[2]);
        if (prob < 0.001) {
            puts("prob must >= 0.001");
            return 1;
        }
    } catch (std::exception& e) {
        puts(e.what());
        return 1;
    }
    if (argc >= 4) {
        out = fopen(argv[3], "w");
        if (out == NULL) {
            printf("cannot open file %s\n", argv[3]);
            return 1;
        }
    }
    std::vector<std::string> g_text;
    std::bernoulli_distribution d(prob);
    for (int i = 0;i < nodes; i++) {
        for (int j = i + 1;j < nodes; j++) {
            if (d(gen)) {
                std::string str;
                str.append(std::to_string(i+1)).append(" ").append(std::to_string(j+1)).append("\n");
                g_text.emplace_back(std::move(str));
            }
        }
    }
    fprintf(out, "%d %d %f\n", nodes, (int)g_text.size(), prob);
    for (const auto& s : g_text) {
        fprintf(out, "%s", s.c_str());
    }
    if (out != stdout) {
        fclose(out);
    }
    return 0;
}
