#include "Cluster.h"

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <climits>

using namespace std;

static const string LINHA = "________________________________________________________________________________\n";

static const int SAIR = 0;
static const int VOLTAR = 9;

static const int INCLUIR_MAQUINA = 1;
static const int EXCLUIR_MAQUINA = 2;
static const int INCLUIR_PROCESSO = 3;
static const int EXCLUIR_PROCESSO = 4;
static const int RELATORIO = 5;
static const int TESTE = 7;

static const int RELAT_GERAL_G = 1;
static const int RELAT_GERAL_T = 2;
static const int RELAT_PROC = 3;
static const int RELAT_MAQUINA = 4;

void cabecalho(string);
void cabecalho();
void imprimir_menu();
void imprimir_menu_relatorio();
void incluir_maquina();
void excluir_maquina();
void incluir_processo();
void excluir_processo();
void teste_desempenho();
void relatorio_geral_g();
void relatorio_geral_t();
void relatorio_proc();
void relatorio_maquina();


Cluster *cluster = new Cluster();

int main(int argc, char *argv[]) {  
   int opcao;

   do {
      imprimir_menu();
      cin >> opcao;

      switch(opcao) {
         case INCLUIR_MAQUINA:
            incluir_maquina();
            break;
         case EXCLUIR_MAQUINA:
            excluir_maquina();
            break;
         case INCLUIR_PROCESSO:
            incluir_processo();
            break;
         case EXCLUIR_PROCESSO:
            excluir_processo();
            break;
         case RELATORIO:
            do {
               imprimir_menu_relatorio();
               cin >> opcao;

               switch(opcao) {
                  case RELAT_GERAL_G:
                     relatorio_geral_g();
                     break;
                  case RELAT_GERAL_T:
                     relatorio_geral_t();
                     break;
                  case RELAT_PROC:
                     relatorio_proc();
                     break;
                  case RELAT_MAQUINA:
                     relatorio_maquina();
                     break;
               }
            } while (opcao != SAIR && opcao != VOLTAR);
            break;
         case TESTE:
            teste_desempenho();
            break;
      }
   } while (opcao != SAIR);
   
   return 0;
}

void cabecalho(string titulo) {
   system("clear");
   cout << LINHA;
   cout << "Algoritmos e Estrutura de Dados" << endl;
   cout << "Universidade Federal de Juiz de Fora" << endl;
   cout << "Estrutura de Dados Elementares" << endl;
   cout << LINHA;
   if (titulo != "")
      cout << titulo << endl;
   endl(cout);
}

void cabecalho() {
    cabecalho("");
}

void imprimir_menu() {
    cabecalho();
    cout << "[1] Incluir Maquina" << endl;
    cout << "[2] Excluir Maquina" << endl;
    cout << "[3] Incluir Processo" << endl;
    cout << "[4] Excluir Processo" << endl;
    cout << "[5] Relatorios" << endl;
    endl(cout);
    cout << "[7] Teste de Desempenho" << endl;
    endl(cout);
    cout << "[0] Sair"<<endl;
    endl(cout);
    cout << LINHA;
}

void imprimir_menu_relatorio() {
    cabecalho();
    cout << "[1] Relatorio Geral (Grafico)" << endl;
    cout << "[2] Relatorio Geral (Texto)" << endl;
    cout << "[3] Relatorio de Processos" << endl;
    cout << "[4] Relatorio de Maquinas" << endl;
    endl(cout);
    cout << "[9] Voltar" << endl;
    cout << "[0] Sair"<<endl;
    endl(cout);
    cout << LINHA;
}

void incluir_maquina() {
    cabecalho("Nova Maquina");
    int cid, num_cpu;
    cout << "CID: ";
    cin >> cid;
    cout<<"Numero de Processadores: ";
    cin >> num_cpu;
    
    cluster->verifica_proc();
    cluster->incluir_comp(cid, num_cpu);
}

void excluir_maquina() {
    cabecalho("Infome o CID da Maquina que deseja excluir");
    int cid;
    cout << "CID: ";
    cin >> cid;
    
    cluster->verifica_proc();
    cluster->excluir_comp(cid);
}

void incluir_processo() {
    cabecalho("Novo Processo");
    int pid;
    cout << "PID: ";
    cin >> pid;
    
    cluster->verifica_proc();
    cluster->incluir_proc(pid);
}

void excluir_processo() {
    cabecalho("Informe o PID do Processo que deseja excluir");
    int pid;
    cout << "PID: ";
    cin >> pid;
    
    cluster->verifica_proc();
    cluster->excluir_proc(pid);
}

void teste_desempenho() {
   cabecalho("Informe a quantidade de testes a serem realizados");
   int max;
   cout << "Quantidade: ";
   cin >> max;
   
   cabecalho("Aguarde...");
   float tf;
   char str[80];
   clock_t t = clock();
   Cluster *cluster_teste = new Cluster();
   
   for(int a = 0; a < max; a++) {
      for(int b = 1; b < 100; b++) {
         cluster->incluir_comp(b, 331);
      }
      for(int b = 1; b < SHRT_MAX; b++) {
         cluster->incluir_proc(b);
      }

      cluster->libera_comp();
   }
   tf = (float)(clock() - t)/(float)CLOCKS_PER_SEC;
   sprintf(str, "Tempo de Execucao: %fs ('q' para sair)", tf);
   delete cluster_teste;
   
   char c;
   while((c = getchar()) != 'q') {
      cabecalho(str);
   }
}

void relatorio_geral_g() {
   cabecalho("Relatorio Geral - Grafico ('q' para sair, 'r' para recarregar)");
   
   cluster->verifica_proc();
   cluster->imprime(true);
   
   char c;
   while((c = getchar()) != 'q') {
      if(c == 'r') {
         cabecalho("Relatorio Geral - Grafico ('q' para sair, 'r' para recarregar)");
      
         cluster->verifica_proc();
         cluster->imprime(true);
      }
   }
}

void relatorio_geral_t() {
   cabecalho("Relatorio Geral - Texto ('q' para sair, 'r' para recarregar)");
   
   cluster->verifica_proc();
   cluster->imprime(false);
    
   char c;
   while((c = getchar()) != 'q') {
      if(c == 'r') {
         cabecalho("Relatorio Geral - Texto ('q' para sair, 'r' para recarregar)");
         
         cluster->verifica_proc();
         cluster->imprime(false);
      }
   }
}

void relatorio_proc() {
   cabecalho("Relatorio de Processos ('q' para sair, 'r' para recarregar)");
   
   cluster->verifica_proc();
   cluster->imprime_proc();
    
   char c;
   while((c = getchar()) != 'q') {
      if(c == 'r') {
         cabecalho("Relatorio de Processos ('q' para sair, 'r' para recarregar)");
         
         cluster->verifica_proc();
         cluster->imprime_proc();
      }
   }
}

void relatorio_maquina() {
   cabecalho("Informe o CID da Maquina que deseja monitorar");
   int cid;
   cout << "CID: ";
   cin >> cid;
   
   cabecalho("Relatorio de Maquina ('q' para sair, 'r' para recarregar)");
   
   cluster->verifica_proc();
   cluster->imprime_comp(cid);
   
   char c;
   while((c = getchar()) != 'q') {
      if(c == 'r') {
         cabecalho("Relatorio de Maquina ('q' para sair, 'r' para recarregar)");
         
         cluster->verifica_proc();
         cluster->imprime_comp(cid);
      }
   }
}
