#ifndef _CLUSTER_H_
   #define _CLUSTER_H_

   #include "Computador.h"
   
   #include <climits>
   
   class Descritor {
      public:
         Computador *primeiro, *ultimo;
         short int qt_comp;

         Descritor();
   };

   class Cluster {
      public:
         short int indice[SHRT_MAX][2];
         short int qt_proc_tot;
         Descritor *desc;

         Cluster();

         void incluir_comp(short int cid, short int qt_cpu);

         void incluir_proc(short int pid);

         void excluir_comp(short int cid);
         
         void excluir_proc(short int pid);

         void libera_comp();

         void libera_proc(short int cid);
         
         void verifica_proc();
         
         void imprime(bool g_mode);

         void imprime_comp(short int cid);

         void imprime_proc();

         bool vazio();

         Computador* busca(short int cid);

         short int PonderedBinarySearch(short int pid, short int lo, short int hi);
   };
   
#endif