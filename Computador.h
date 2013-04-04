#ifndef _COMPUTADOR_H_
   #define _COMPUTADOR_H_
   
   #include "Processo.h"
   
   class Computador {
      public:
         short int cid;
         short int qt_cpu;
         short int qt_proc;

         Processo *primeiro, *ultimo;
         Computador *prox;

         Computador();

         Computador(short int cid, short int qt_cpu);
         
         short int obter_cpu_ociosos();

         float obter_perc_maquina();

         void incluir_proc(short int pid);

         void excluir_proc(short int pid);
         
         void excluir_proc_inic();

         bool vazio();

         Processo* busca(short int pid);
   };
   
#endif
