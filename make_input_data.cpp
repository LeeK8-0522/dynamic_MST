#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

string generateRandomCommand(int N) {
    int commandType = rand() % 4;
    int u = rand() % N + 1;
    int v = rand() % N + 1;
    int weight = rand() % 1000 + 1; // 가중치는 1과 1000 사이로 설정

    while (u == v) {
        v = rand() % N + 1; // u와 v가 같지 않도록 조정
    }

    switch (commandType) {
        case 0: // insertEdge
            return "insertEdge " + to_string(u) + " " + to_string(v) + " " + to_string(weight);
        case 1: // deleteEdge
            return "deleteEdge " + to_string(u) + " " + to_string(v);
        case 2: // changeWeight
            return "changeWeight " + to_string(u) + " " + to_string(v) + " " + to_string(weight);
        case 3: // findMST
            return "findMST";
    }
    return "";
}

int main() {
    ofstream outFile("mst.in");
    srand(time(0)); // 난수 생성을 위한 시드 초기화

    int N = 500; // 노드의 개수
    int commandsCount = 10000; // 명령어의 개수

    outFile << N << endl;

    for (int i = 0; i < commandsCount; ++i) {
        outFile << generateRandomCommand(N) << endl;
    }

    outFile.close();
    return 0;
}
