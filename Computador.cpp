#include "Computador.h"
#include "Processo.h"

Computador::Computador() { }

Computador::Computador(short int cid, short int qt_cpu) {
   this->cid = cid;
   this->qt_cpu = qt_cpu;
   qt_proc = 0;
   prox = NULL;
   primeiro = NULL;
   ultimo = NULL;
}

short int Computador::obter_cpu_ociosos() {
   return qt_cpu - qt_proc;
}
   
float Computador::obter_perc_maquina() {
   return ((float)qt_proc/(float)qt_cpu)*100; 
}

void Computador::incluir_proc(short int pid) {
   Processo *p = new Processo(pid);

   // Se fila estiver vazia
   if(vazio()) {
      ultimo = primeiro = p;
   } else {
      p->prox = ultimo;
      ultimo = p;
   }

   // Incrementa o número de processos
   qt_proc++;
}

void Computador::excluir_proc(short int pid) {
   Processo *p, *ant = NULL;

   // Se fila estiver vazia
   if(vazio())
      return; // Erro: Fila vazia

   // Se fila contém apenas um elemento
   if(ultimo == primeiro) {
      // Se PID for encontrado nesse elemento
      if(primeiro->pid == pid) {
         delete primeiro;
         ultimo = NULL;
         primeiro = NULL;
      }
   // Se fila contém vários elementos e PID é o ultimo
   } else if(ultimo->pid == pid) {
      p = ultimo;
      ultimo = ultimo->prox;
      delete p;
   // Se não, executar o loop a procura do PID
   } else {
      for(p = ultimo; p->prox != NULL; p = p->prox)
         if((p->prox)->pid == pid) {
            ant = p;
            p = p->prox;
            break;
         }
      // Se ant for NULL, PID não foi encontrado
      if(ant == NULL) {
         return; // Erro: PID não encontrado
      // Se PID for o primeiro da fila, define o ant como primeiro elemento
      } else if(p == primeiro) {
         ant->prox = NULL;
         primeiro = ant;
         delete p;
      // Se não, ant passa a apontar para o próximo e apaga o atual
      } else {
         ant->prox = p->prox;
         delete p;
      }
   }

   // Decrementa o número de processos
   qt_proc--;
}

void Computador::excluir_proc_inic() {
   Processo *ant;
   
   // Se fila estiver vazia
   if(vazio())
      return; // Erro: Fila vazia
      
   // Se fila contém apenas um elemento
   if(ultimo == primeiro) {
      delete primeiro;
      ultimo = NULL;
      primeiro = NULL;
   } else {
      for(ant = ultimo; ant->prox != primeiro; ant = ant->prox);
      
      ant->prox = NULL;
      delete primeiro;
      primeiro = ant;
   }

   // Decrementa o número de processos
   qt_proc--;
}

bool Computador::vazio() {
   return (primeiro == NULL);
}

Processo* Computador::busca(short int pid) {
   Processo *p;

   for(p = ultimo; p != NULL; p = p->prox)
      if(p->pid == pid)
         return p;

   return NULL;
}
