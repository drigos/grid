#include "Cluster.h"
#include "Computador.h"

#include <iostream>
#include <ctime>
#include <climits>
#include <cstdio>
#include <string>

#define TTL 60

using namespace std;

void error(string mensagem) {
   char err;
   cin.ignore();
   endl(cout);
   cout<<mensagem<<endl;
   cin.get(err);
}

Descritor::Descritor() {
   primeiro = NULL;
   ultimo = NULL;
   qt_comp = 0;
}

Cluster::Cluster() {
   desc = new Descritor();
}

void Cluster::incluir_comp(short int cid, short int qt_cpu) {
   if(qt_cpu <= 0) {
     error("Erro: Quantidade de processadores estah fora dos valores permitidos");
     return; 
   }
   if(cid <= 0 || cid > 99) {
      error("Erro: Quantidade de computadores deve estar entre 1 e 99");
      return;
   }

   // Verifica se o CID j� existe
   if(busca(cid) != NULL) {
     error("Erro: CID repetido"); 
     return;
   }

   Computador *c = new Computador(cid, qt_cpu);

   // Se lista estiver vazia
   if(vazio()) {
      desc->primeiro = desc->ultimo = c;
   // Se n�o, verifica se computador c tem mais CPUs que o primeiro elemento
   } else if(c->qt_cpu >= (desc->primeiro)->qt_cpu) {
      c->prox = desc->primeiro;
      desc->primeiro = c;
   // Se n�o, executar o loop a procura do primeiro computador com menos CPUs que c
   } else {
      Computador *ant;
      for(ant = desc->primeiro; ant->prox != NULL; ant = ant->prox)
         if(c->qt_cpu >= (ant->prox)->qt_cpu) break;

      // Se ant n�o for null
      if(ant->prox != NULL)
         c->prox = ant->prox;
      // Se ant for null, nenhum computador com menos CPUs que c foi encontrado
      else
         desc->ultimo = c;

      // Executado mesmo se nenhum computador com menos CPUs que c for encontrado
      ant->prox = c;
   }

   desc->qt_comp++;
}

void Cluster::incluir_proc(short int pid) {
   //Se lista estiver vazia
   if (vazio()) {
     error("Erro: Cluster estah vazio");
     return;
   }

   if(pid >= SHRT_MAX || pid <= 0) {
     error("Erro: PID invalido"); 
     return;
   }

   Computador *c = desc->primeiro, *t = desc->primeiro;
   while(t->prox != NULL) {
      t = t->prox;
      if(t->obter_perc_maquina() < c->obter_perc_maquina())
         c = t;
   }
   t = NULL;
   
   if(c->obter_cpu_ociosos() <= 0){
     error("Erro: Cluster estah cheio");
     return; 
   }

   int i = 0;
   if(qt_proc_tot > 0) {
      i = PonderedBinarySearch(pid, 0, qt_proc_tot-1);
      if(pid == indice[i][0]) {
        error("Erro: PID jah existe");
        return; 
      }
   }

   // Verificar erros na fun��o
   c->incluir_proc(pid);

   for(int a = qt_proc_tot; a > i; a--) {
      indice[a][0] = indice[a-1][0];
      indice[a][1] = indice[a-1][1];
   }
   indice[i][0] = pid;
   indice[i][1] = c->cid;

   qt_proc_tot++;
}

void Cluster::excluir_comp(short int cid) {
   if(cid <= 0 || cid > 99)
      return; // Erro: Quantidade de computadores deve estar entre 1 e 99

   Computador *c, *ant = NULL;
   
   // Se lista estiver vazia
   if(vazio()) {
     error("Erro: Lista vazia");
     return; 
   }

   c = busca(cid);
   if(c == NULL) {
     error("Erro: CID nao foi encontrado");
     return; 
   } else if(c->qt_proc > 0) {
     error("Erro: O computador nao estah ocioso");
     return; 
   }

   // Se lista cont�m apenas um elemento
   if(desc->ultimo == desc->primeiro) {
      // Se CID for encontrado nesse elemento
      if((desc->primeiro)->cid == cid) {
         delete desc->primeiro;
         desc->ultimo = NULL;
         desc->primeiro = NULL;
      }
   // Se lista cont�m v�rios elementos e CID � o primeiro
   } else if((desc->primeiro)->cid == cid) {
      desc->primeiro = (desc->primeiro)->prox;
      delete c;
   // Se n�o, executar o loop a procura do CID
   } else {
      for(ant = desc->primeiro; ant->prox != c; ant = ant->prox);

      // Se CID for o �ltimo da lista, define o ant como o primeiro elemento
      if(c == desc->ultimo) {
         ant->prox = NULL;
         desc->ultimo = ant;
         delete c;
      // Se n�o, ant passa a apontar para o pr�ximo e apaga o atual
      } else {
         ant->prox = c->prox;
         delete c;
      }
   }

   //Decrementa o n�mero de computadores
   desc->qt_comp--;
}

void Cluster::excluir_proc(short int pid) {
   if(pid >= SHRT_MAX || pid <= 0)
      return; // Erro: PID invalido

   int i;
   if(qt_proc_tot > 0) {
      i = PonderedBinarySearch(pid, 0, qt_proc_tot-1);
      if(pid != indice[i][0]) {
        error("Erro: PID nao existe");
        return; 
      }
   } else {
     error("Erro: Cluster nao possui processos");
      return; 
   }

   Computador *c = busca(indice[i][1]);
   // Verificar erros na fun��o
   c->excluir_proc(pid);

   qt_proc_tot--;

   for(int a = i; a < qt_proc_tot; a++) {
      indice[a][0] = indice[a+1][0];
      indice[a][1] = indice[a+1][1];
   }
   indice[qt_proc_tot][0] = 0;
   indice[qt_proc_tot][1] = 0;
}

void Cluster::libera_comp() {
   Computador *c = desc->primeiro, *t;

   while(c != NULL) {
      libera_proc(c->cid);
      t = c;
      c = c->prox;
      delete t;
   }
   desc->primeiro = NULL;
   desc->ultimo = NULL;
   desc->qt_comp = 0;

   for(int a = 0; a < qt_proc_tot; a++) {
      indice[a][0] = 0;
      indice[a][1] = 0;
   }
   qt_proc_tot = 0;
}

void Cluster::libera_proc(short int cid) {
   Computador *c = busca(cid);
   Processo *p = c->primeiro, *t;

   while(p != NULL) {
      t = p;
      p = p->prox;
      delete t;
   }
   c->primeiro = NULL;
   c->ultimo = NULL;
   c->qt_proc = 0;
}

void Cluster::verifica_proc() {
   Computador *c;

   int i;
   for(c = desc->primeiro; c != NULL; c = c->prox) {
      while(!c->vazio() && (time(NULL) - (c->primeiro)->tempo_inic) >= TTL) {

         i = PonderedBinarySearch((c->primeiro)->pid, 0, qt_proc_tot-1);

         c->excluir_proc_inic();

         qt_proc_tot--;

         for(int a = i; a < qt_proc_tot; a++) {
            indice[a][0] = indice[a+1][0];
            indice[a][1] = indice[a+1][1];
         }

         indice[qt_proc_tot][0] = 0;
         indice[qt_proc_tot][1] = 0;
      }
   }
}


/*void Cluster::verifica_proc() {
  Computador *c;
  Processo *p;

  for (c = desc->primeiro ; c != NULL ; c = c->prox) {
    for (p = c->ultimo ; p != NULL ; p = p->prox) {
      if (time(NULL) - p->tempo_inic >= TTL) {
        
        
        int i = PonderedBinarySearch(p->pid, 0, qt_proc_tot-1);
      
        c->excluir_proc_inic();
        
        qt_proc_tot--;

         for(int a = i; a < qt_proc_tot; a++) {
            indice[a][0] = indice[a + 1][0];
            indice[a][1] = indice[a + 1][1];
         }

         indice[qt_proc_tot][0] = 0;
         indice[qt_proc_tot][1] = 0;
        }
    }
  }
}*/

void Cluster::imprime(bool g_mode) {
   Computador *c;

   if(desc->primeiro == NULL) {
     //error("Erro: Cluster estah vazio");
     return; 
   }

   if(!g_mode) {
      for(c = desc->primeiro; c != NULL; c = c->prox) {
         cout << "Maquina " << c->cid << endl;
         cout << "# CPUs: " << c->qt_cpu << endl;
         cout << "% Uso:  ";
         cout.precision ( 4 );
         cout << c->obter_perc_maquina();
         cout << "%" << endl;
         cout << endl;
         //Grafico representando o percentual de uso
      }
   } else {
      for(c = desc->primeiro; c != NULL; c = c->prox) {
         cout << "/----\\" << endl;

         cout << "|    | -";
         for(int a = 0; a < c->qt_cpu; a++)
            cout << "--";
         cout << endl;

         cout << "| ";
         cout.width ( 2 );
         cout << c->cid;
         cout << " | |";
         for(int a = 0; a < c->qt_proc; a++)
            cout << "X|";
         for(int a = 0; a < c->obter_cpu_ociosos(); a++)
            cout << " |";
         cout << "  " << c->qt_proc << "/" << c->qt_cpu << endl;
         
         cout << "|    | -";
         for(int a = 0; a < c->qt_cpu; a++)
            cout << "--";
         cout << endl;

         cout << "\\----/" << endl;
         cout << endl;
      }
   }
}
         
void Cluster::imprime_comp(short int cid) {
   Computador *c = busca(cid);
   int a;

   if(c != NULL) {
      Processo *p = c->ultimo;

      cout << "Maquina " << c->cid << endl;
      cout << "# CPUs: " << c->qt_cpu << endl;
      for(a = 1; a <= c->qt_proc; a++, p = p->prox) {
         cout << "\t#";
         cout.width ( 3 );
         cout << a;
         cout << " - ";
         cout.width ( 5 );
         cout << p->pid;
         cout << " - ";
         cout.width ( 2 );
         cout.fill ( '0' );
         cout << (time(NULL) - p->tempo_inic) / 60;
         cout << ":";
         cout.width ( 2 );
         cout << (time(NULL) - p->tempo_inic) % 60;
         cout.fill ( ' ' );
         cout << endl;
      }
      cout << endl;
      cout << "% Uso:  " << c->obter_perc_maquina() << "%" << endl;
      cout << endl;
   } else {
     error("Erro: CID nao existe");
     return; 
   }
}

void Cluster::imprime_proc() {
   if(qt_proc_tot <= 0) {
     //error("Erro: Cluster nao possui nenhum processo em execucao");
     return; 
   }

   FILE *comando = fopen("procs.txt", "r");
   char str[80];

   cout << "  PID  | CID | TEMPO | COMANDO" << endl;
/*
   for(int a = 0; a < qt_proc_tot; a++) {
      fscanf(comando, "%s", str);

      cout << " ";
      cout.width ( 5 );
      cout << indice[a][0];
      cout << " | ";
      cout.width ( 3 );
      cout << indice[a][1];
      cout << " | ";
      cout << str << endl;
   }
*/
   Computador *c;
   Processo *p;

   for (c = desc->primeiro ; c != NULL ; c = c->prox) {
      for (p = c->ultimo ; p != NULL ; p = p->prox) {
         fscanf(comando, "%s", str);
         cout<<"| ";
         cout.width(5);
         cout<<p->pid;
         cout<< " | ";
         cout.width(3);
         cout<<c->cid;
         cout<<" | ";
         cout.width ( 2 );
         cout.fill ( '0' );
         cout << (time(NULL) - p->tempo_inic) / 60;
         cout << ":";
         cout.width ( 2 );
         cout << (time(NULL) - p->tempo_inic) % 60;
         cout.fill ( ' ' );
         cout<<" | ";
         cout<<str;
         endl(cout);
      }
   }

   fclose(comando);
}

bool Cluster::vazio() {
   return (desc->primeiro == NULL);
}

Computador* Cluster::busca(short int cid) {
   Computador *c;

   for(c = desc->primeiro; c != NULL; c = c->prox)
      if(c->cid == cid)
         return c;

   return NULL;
}

short int Cluster::PonderedBinarySearch(short int pid, short int lo, short int hi) {
   if(pid <= indice[lo][0]) return lo;
   else if(pid > indice[hi][0]) return hi+1;
   else {
      short int mid = ((hi-lo) / 2) + lo;

      if(pid > indice[mid][0])
         return PonderedBinarySearch(pid, mid+1, hi);
      if(pid < indice[mid][0])
         return PonderedBinarySearch(pid, lo, mid-1);

      return mid;
   }
}
