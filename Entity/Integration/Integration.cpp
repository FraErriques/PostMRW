#include <cmath>
#include "Integration.h"
#include "../../Common/StringBuilder/StringBuilder.h"
#include "../../Common/DbConnectionService/dbCall.h"

namespace Entity
{

namespace Integration
{

// const double M_PI = atan(+1.0) * +4.0;  needed only on Windows ,while on Linux it's a symbolic constant.




double rettangoli( double inf
                   ,double sup
                   ,double cardPartiz
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
    double dx = (sup-inf) / cardPartiz;
    double res = 0.0;// step adding on it
    size_t i = 0;// step ordinal
    for( double x=inf; x<sup; x+=dx)
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


double correttivoAlContorno( double inf
                   ,double sup
                   ,double cardPartiz
                   ,FunctionalForm functionalForm
                  )
{
    Common::LogWrappers::SectionOpen("Entity::Integration::correttivoAlContorno()", 0);
    //
    double dx = (sup-inf) / cardPartiz;
    double res = (+1.0/+2.0)*dx*(functionalForm(sup)-functionalForm(inf));
    std::string * tmpCorrettivo = Common::StrManipul::doubleToString( res);
    Common::LogWrappers::SectionContent( (*tmpCorrettivo).c_str() , 0);
    delete tmpCorrettivo;
    Common::LogWrappers::SectionClose();
    return res;
}

double trapezi( double inf
                   ,double sup
                   ,double cardPartiz
                   ,FunctionalForm functionalForm
                  )
{
    Common::LogWrappers::SectionOpen("Entity::Integration::trapezi()", 0);
    //
    double res = rettangoli(  inf
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


}// nmsp
}// nmsp
