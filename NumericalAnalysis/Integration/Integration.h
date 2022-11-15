#ifndef INTEGRATION_H_INCLUDED
#define INTEGRATION_H_INCLUDED


namespace RealIntegration{


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


#endif // INTEGRATION_H_INCLUDED
