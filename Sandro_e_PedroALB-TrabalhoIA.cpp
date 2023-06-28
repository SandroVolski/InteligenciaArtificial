#include <iostream> // Biblioteca para entrada/saída padrão
#include <vector> // Biblioteca para trabalhar com vetores
#include <queue> // Biblioteca para trabalhar com filas
#include <unordered_set> // Biblioteca para trabalhar com conjuntos não ordenados
using namespace std; // Namespace padrão para evitar repetição de "std::"


//struct estado




// Estrutura para representar o estado do problema
struct Estado {
    int missionarios_esquerda; // Número de missionários na margem esquerda
    int canibais_esquerda; // Número de canibais na margem esquerda
    bool barco_na_esquerda; // Indica se o barco está na margem esquerda

    Estado(int missionarios_esq, int canibais_esq, bool barco_esq)
        : missionarios_esquerda(missionarios_esq),
          canibais_esquerda(canibais_esq),
          barco_na_esquerda(barco_esq) {}
};

// Função para verificar se um estado é válido
bool is_estado_valido(const Estado& estado) {
    // Verifica se o estado não viola as regras do jogo
    if (estado.missionarios_esquerda < 0 || estado.missionarios_esquerda > 3) {
        return false;
    }
    if (estado.canibais_esquerda < 0 || estado.canibais_esquerda > 3) {
        return false;
    }
    if (estado.missionarios_esquerda > 0 && estado.canibais_esquerda > estado.missionarios_esquerda) {
        return false;
    }
    int missionarios_direita = 3 - estado.missionarios_esquerda;
    int canibais_direita = 3 - estado.canibais_esquerda;
    if (missionarios_direita < 0 || missionarios_direita > 3) {
        return false;
    }
    if (canibais_direita < 0 || canibais_direita > 3) {
        return false;
    }
    if (missionarios_direita > 0 && canibais_direita > missionarios_direita) {
        return false;
    }
    return true;
}

// Função para gerar os próximos estados possíveis
vector<Estado> gerar_proximos_estados(const Estado& estado_atual) {
    vector<Estado> proximos_estados;
    // Gera todas as possíveis combinações de missionários e canibais que podem atravessar o rio
    for (int i = 0; i <= 3; i++) {
        for (int j = 0; j <= 3; j++) {
            if (i + j >= 1 && i + j <= 2) {
                if (estado_atual.barco_na_esquerda) {
                    Estado novo_estado(estado_atual.missionarios_esquerda - i,
                                       estado_atual.canibais_esquerda - j,
                                       !estado_atual.barco_na_esquerda);
                    if (is_estado_valido(novo_estado)) {
                        proximos_estados.push_back(novo_estado);
                    }
                } else {
                    Estado novo_estado(estado_atual.missionarios_esquerda + i,
                                       estado_atual.canibais_esquerda + j,
                                       !estado_atual.barco_na_esquerda);
                    if (is_estado_valido(novo_estado)) {
                        proximos_estados.push_back(novo_estado);
                    }
                }
            }
        }
    }
    return proximos_estados;
}

// Função para buscar a solução do problema
vector<Estado> buscar_solucao() {
    Estado estado_inicial(3, 3, true); // Estado inicial com 3 missionários, 3 canibais e barco na margem esquerda
    Estado estado_final(0, 0, false); // Estado final com 0 missionários, 0 canibais e barco na margem direita
    unordered_set<long long> visitados; // Conjunto para armazenar os estados visitados
    queue<vector<Estado>> fila; // Fila de vetores de estados (caminhos)
    vector<Estado> caminho; // Vetor para armazenar o caminho atual

    fila.push({estado_inicial}); // Adiciona o estado inicial na fila como o primeiro caminho
    visitados.insert(estado_inicial.missionarios_esquerda * 10000 +
                     estado_inicial.canibais_esquerda * 100 +
                     estado_inicial.barco_na_esquerda); // Insere a chave única do estado inicial no conjunto de visitados

    while (!fila.empty()) {
        caminho = fila.front(); // Armazena o caminho atual removido da frente da fila
        fila.pop(); // Remove o caminho atual da fila
        Estado estado_atual = caminho.back(); // Obtém o estado atual do caminho

        if (estado_atual.missionarios_esquerda == estado_final.missionarios_esquerda &&
            estado_atual.canibais_esquerda == estado_final.canibais_esquerda &&
            estado_atual.barco_na_esquerda == estado_final.barco_na_esquerda) {
            return caminho; // Se o estado atual for igual ao estado final, retorna o caminho
        }

        vector<Estado> proximos_estados = gerar_proximos_estados(estado_atual); // Gera os próximos estados a partir do estado atual

        for (const auto& proximo_estado : proximos_estados) {
            vector<Estado> novo_caminho = caminho; // Cria uma cópia do caminho atual
            novo_caminho.push_back(proximo_estado); // Adiciona o próximo estado ao novo caminho

            long long hash_estado = proximo_estado.missionarios_esquerda * 10000 +
                                     proximo_estado.canibais_esquerda * 100 +
                                     proximo_estado.barco_na_esquerda; // Calcula o hash único do próximo estado

            if (visitados.find(hash_estado) == visitados.end()) { // Verifica se o próximo estado não foi visitado
                fila.push(novo_caminho); // Adiciona o novo caminho à fila
                visitados.insert(hash_estado); // Insere o hash do próximo estado no conjunto de visitados
            }
        }
    }

    return vector<Estado>(); // Retorna um vetor vazio caso não encontre uma solução
}

// Função para imprimir a solução
void imprimir_solucao(const vector<Estado>& caminho) {
    if (caminho.empty()) {
        cout << "Nenhuma solução encontrada." << endl;
        return;
    }

    for (const auto& estado : caminho) {
        int missionarios_esquerda = estado.missionarios_esquerda;
        int canibais_esquerda = estado.canibais_esquerda;
        bool barco_na_esquerda = estado.barco_na_esquerda;

        int missionarios_direita = 3 - missionarios_esquerda;
        int canibais_direita = 3 - canibais_esquerda;
        bool barco_na_direita = !barco_na_esquerda;

        cout << "-----------------------" << endl;
        cout << "Margem Esquerda: ";
        for (int i = 0; i < missionarios_esquerda; i++) {
            cout << "\x1b[34mM\x1b[0m";  // Cor azul para missionários
        }
        for (int i = 0; i < canibais_esquerda; i++) {
            cout << "\x1b[31mC\x1b[0m";  // Cor vermelha para canibais
        }
        cout << (barco_na_esquerda ? "\x1b[33mB\x1b[0m" : " ") << endl;  // Cor amarela para o barco

        cout << "Margem Direita: ";
        cout << (barco_na_direita ? "\x1b[33mB\x1b[0m" : " ");
        for (int i = 0; i < missionarios_direita; i++) {
            cout << "\x1b[34mM\x1b[0m";
        }
        for (int i = 0; i < canibais_direita; i++) {
            cout << "\x1b[31mC\x1b[0m";
        }
        cout << endl;
        cout << "-----------------------" << endl;
    }
}

int main() {
    cout << "Solucao:" << endl; // Imprime o título da solução
    vector<Estado> caminho_solucao = buscar_solucao(); // Encontra o caminho da solução
    imprimir_solucao(caminho_solucao); // Imprime a solução encontrada

    return 0;
}
