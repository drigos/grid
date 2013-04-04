#ifndef _PROCESSO_H_
   #define _PROCESSO_H_
   
   #include <ctime>
   
   using namespace std;

   class Processo {
      public:
         short int pid;
         time_t tempo_inic;
         Processo *prox;

         Processo();
         Processo(short int pid);
   };

#endif