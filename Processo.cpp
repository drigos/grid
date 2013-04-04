#include "Processo.h"

#include <ctime>

using namespace std;

Processo::Processo() { }

Processo::Processo(short int pid) {
   this->pid = pid;
   tempo_inic = time(NULL);
   prox = NULL;
}