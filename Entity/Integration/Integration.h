#ifndef INTEGRATION_H_INCLUDED
#define INTEGRATION_H_INCLUDED

#include "../../Common/LogFs_wrap/LogFs_wrap.h"

namespace Entity
{

namespace Integration
{


typedef double (*FunctionalForm)(double);

double rettangoli( double inf
                   ,double sup
                   ,double cardPartiz
                   ,FunctionalForm functionalForm
                   ,bool calledFromTrapezi
                  );
double correttivoAlContorno( double inf
                   ,double sup
                   ,double cardPartiz
                   ,FunctionalForm functionalForm
                  );
double trapezi( double inf
                   ,double sup
                   ,double cardPartiz
                   ,FunctionalForm functionalForm
                  );


}// nmsp
}// nmsp


#endif // INTEGRATION_H_INCLUDED
