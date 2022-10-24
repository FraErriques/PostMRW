#include <cmath>
#include "Integration.h"
#include "../../Common/StringBuilder/StringBuilder.h"
#include "../../Common/DbConnectionService/dbCall.h"

namespace Entity
{

namespace Integration
{

// TODO pass to long double

long double rettangoli_withLog(
                   long double inf
                   ,long double sup
                   ,long double cardPartiz
                   ,FunctionalForm functionalForm
                   ,bool calledFromTrapezi
                  )
{
    if ( ! calledFromTrapezi)
    {
        Common::LogWrappers::SectionOpen("Entity::Integration::rettangoli()", 0);
    }
    else
    {
        Common::LogWrappers::SectionOpen("Entity::Integration::trapezi:inner core==rettangoli", 0);
    }
    //
    long double dx = (sup-inf) / cardPartiz;
    long double res = 0.0;// step adding on it
    size_t i = 0;// step ordinal
    for( long double x=inf; x<sup; x+=dx)
    {
        ++i;
        if( i > cardPartiz)
        {
            break;
        }// else continue.
        res += functionalForm(x) * dx;
        Common::StringBuilder sb( 200);
        std::string * tmpStep = Common::StrManipul::intToString(i);
        sb.append("cumulated integral value at step # ");
        sb.append( *tmpStep);
        sb.append( " = ");
        delete tmpStep;
        std::string * numStr = Common::StrManipul::doubleToString(res);
        sb.append( *numStr);
        Common::LogWrappers::SectionContent( sb.str().c_str(), 0);
        delete numStr;
        //Common::dbCall::cantiere_intg_Sin_0_Pi_INSERT_SINGLE( res);
        //bool isThereInsertionError = Common::dbCall::cantiere_numericTable_INSERT_SINGLE( res);
    }
    //
    Common::LogWrappers::SectionClose();
    return res;
}



/*
Trapezium is generally more accurate than rectangle.
Only in case the interval is bounded from gaussian-zeroes, the calculation gets equivalent.
So, it's possible to compute Trapezium in the general case as Rectangle+correttivoAlContorno, where
correttivoAlContorno==0 in gaussian conditions.
Demonstration:
for a Riemann-partition of cardinality #n Trapezium uses f(xi)[i=0->i=n]
while Rectangle uses f(xi)[i=0->i=n-1]
explicitly:
Trapezium== Dx*Sum[f(xi)[i=1->i=n-1]]+Dx/2*(f(x0)+f(xn))
Rectangle== Dx*f(x0)+Dx*Sum[f(xi)[i=1->i=n-1]]
to verify that, just consider the Geometry here involved.
Between the two formulas there is a common core, which consists in: Dx*Sum[f(xi)[i=1->i=n-1]]
What else is contained in the two formulas is zero, whenever f(x0)==f(xn)==0 (i.e. null boundary condition, C.F.Gauss).
When there's no null-boundary, the term CorrettivoAlContorno consists of Dx/2*(f(x0)+f(xn))-Dx*f(x0)==Dx/2*(f(xn)-f(x0)).
Q.D.E.
*/
long double correttivoAlContorno_withLog( long double inf
                   ,long double sup
                   ,long double cardPartiz
                   ,FunctionalForm functionalForm
                  )
{
    Common::LogWrappers::SectionOpen("Entity::Integration::correttivoAlContorno()", 0);
    //
    long double dx = (sup-inf) / cardPartiz;
    long double res = (+1.0/+2.0)*dx*(functionalForm(sup)-functionalForm(inf));// see demostration above, to get that trapezium==rettangolo+correttivoAlContorno.
    std::string * tmpCorrettivo = Common::StrManipul::doubleToString( res);
    Common::LogWrappers::SectionContent( (*tmpCorrettivo).c_str() , 0);
    delete tmpCorrettivo;
    Common::LogWrappers::SectionClose();
    return res;
}

long double trapezi_withLog(
                   long double inf
                   ,long double sup
                   ,long double cardPartiz
                   ,FunctionalForm functionalForm
                  )
{
    Common::LogWrappers::SectionOpen("Entity::Integration::trapezi()", 0);
    //
    long double res = rettangoli(
                    inf
                   ,  sup
                   ,  cardPartiz
                   ,  functionalForm
                   ,  true // in this call, "rettangoli()" function is the inner core of "trapezi()" function.
                  );
    res += correttivoAlContorno(  inf // trapezi() consists of an inner core "rettangoli" + corretivoAlContorno.
                   ,  sup
                   ,  cardPartiz
                   ,  functionalForm
                  );
    std::string * tmp = Common::StrManipul::doubleToString( res);
    Common::LogWrappers::SectionContent( "\n" , 0 );
    Common::LogWrappers::SectionContent( (*tmp).c_str() , 0 );
    delete tmp;
    Common::LogWrappers::SectionClose();
    return res;
}

//###### below this mark, no_log versions: ###########



long double rettangoli(
                   long double inf
                   ,long double sup
                   ,long double cardPartiz
                   ,FunctionalForm functionalForm
                   ,bool calledFromTrapezi
                  )
{
    double dx = (sup-inf) / cardPartiz;
    long double res = 0.0;// step adding on it
    unsigned long long stepOrdinal = 0;// step ordinal
    for( long double x=inf; x<sup; x+=dx)
    {
        ++stepOrdinal;
        if( stepOrdinal > cardPartiz)
        {
            break;
        }// else continue.
        res += functionalForm(x) * dx;
    }
    return res;
}



/*
Trapezium is generally more accurate than rectangle.
Only in case the interval is bounded from gaussian-zeroes, the calculation gets equivalent.
So, it's possible to compute Trapezium in the general case as Rectangle+correttivoAlContorno, where
correttivoAlContorno==0 in gaussian conditions.
Demonstration:
for a Riemann-partition of cardinality #n Trapezium uses f(xi)[i=0->i=n]
while Rectangle uses f(xi)[i=0->i=n-1]
explicitly:
Trapezium== Dx*Sum[f(xi)[i=1->i=n-1]]+Dx/2*(f(x0)+f(xn))
Rectangle== Dx*f(x0)+Dx*Sum[f(xi)[i=1->i=n-1]]
to verify that, just consider the Geometry here involved.
Between the two formulas there is a common core, which consists in: Dx*Sum[f(xi)[i=1->i=n-1]]
What else is contained in the two formulas is zero, whenever f(x0)==f(xn)==0 (i.e. null boundary condition, C.F.Gauss).
When there's no null-boundary, the term CorrettivoAlContorno consists of Dx/2*(f(x0)+f(xn))-Dx*f(x0)==Dx/2*(f(xn)-f(x0)).
Q.D.E.
*/
long double correttivoAlContorno(
                    long double inf
                   ,long double sup
                   ,long double cardPartiz
                   ,FunctionalForm functionalForm
                  )
{
    long double dx = (sup-inf) / cardPartiz;
    long double res = (+1.0/+2.0)*dx*(functionalForm(sup)-functionalForm(inf));// see demostration above, to get that trapezium==rettangolo+correttivoAlContorno.
    return res;
}


long double trapezi(
                long double inf
                   ,long double sup
                   ,long double cardPartiz
                   ,FunctionalForm functionalForm
                  )
{
    long double res = rettangoli(
                   inf
                   ,  sup
                   ,  cardPartiz
                   ,  functionalForm
                   ,  true // in this call, "rettangoli()" function is the inner core of "trapezi()" function.
                  );
    res += correttivoAlContorno(
                   inf // trapezi() consists of an inner core "rettangoli" + corretivoAlContorno.
                   ,  sup
                   ,  cardPartiz
                   ,  functionalForm
                  );
    return res;
}//trapezi


}// nmsp
}// nmsp
