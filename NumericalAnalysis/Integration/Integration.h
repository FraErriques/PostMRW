#ifndef INTEGRATION_H_INCLUDED
#define INTEGRATION_H_INCLUDED

#include "../../Common/LogFs_wrap/LogFs_wrap.h"

namespace Entity
{

namespace Integration
{


typedef long double (*FunctionalForm)(long double);

long double rettangoli(
                    long double inf
                    ,long double sup
                    ,long double cardPartiz
                    ,FunctionalForm functionalForm
                    ,bool calledFromTrapezi
                    );

long double correttivoAlContorno(
                    long double inf
                    ,long double sup
                    ,long double cardPartiz
                    ,FunctionalForm functionalForm
                    );

long double trapezi(
                long double inf
                ,long double sup
                ,long double cardPartiz
                ,FunctionalForm functionalForm
                );


}// nmsp
}// nmsp


#endif // INTEGRATION_H_INCLUDED
