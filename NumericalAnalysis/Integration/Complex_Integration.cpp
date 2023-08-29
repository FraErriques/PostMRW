#include "Complex_Integration.h"
#include "../RealAnalysis/Real.h"
#include "../RealMatrix/RealMatrix.h"

namespace Complex_Integration{

        //  #region exampleFunctions : such simple functions can be inline.
        /// <summary>
        /// the functions chosen for the example are f(z)=z which implies u(x,y)=x, v(x,y)=y;
        /// the choice for the contour is x(t)=t,y(t)=2*t+1,dx=dt which means dx=1
        /// dy=2*dt which means dy=2.
        /// </summary>
        // Jordan
        double x( double t)
        {// x(t)=t
            return t;
        }// x(t)
        double y( double t )
        {// y(t)=2*t+1
            return 2 * t + 1;
        }// y(t)
        double dx( double t )
        {// x(t)=t->dx(t)=x'(t)dt=1*dt
            return +1.0;
        }// x(t)
        double dy( double t )
        {// y(t)=2*t+1 ->dy(y)=y'(t)dt==2*dt
            return +2.0;
        }// y(t)
        // f(z)==u+I*v
        double genericIntegrand_u_part( double x, double y )
        {// f(z)==z -> Re(f(z))==Re(f(x+I*y))==Re(x+I*y)==x.
            return x;
        }// u(x,y)
        double genericIntegrand_v_part( double x, double y )
        {// f(z)==z -> Im(f(z))==Im(f(x+I*y))====Im(x+I*y)==y.
            return y;
        }// v(x,y)
        /// w==f(z)
        Numerics::Complex integrand_ComplexAsScalar( double x, double y)// the whole image, treated as an algebraic scalar.
        {
            Numerics::Complex res( x,y);
            return res;
        }// integrand_ComplexAsScalar
        // #endregion exampleFunctions



        /// <summary>
        /// Trapezium Integration. NB.: f(z)*(dx+I*dy)
        /// ComplexImageAsScalar
        /// </summary>
        Numerics::Complex Integrate_equi_trapezium_ComplexImageAsScalar(
            double t0, double tn, // extrema in the pull-back t in [t0,tn]
            fPtr_ComplexAsScalar_ complexAsScalar,
            fPtr_Jordan_parametriz_ x_coordinate,
            fPtr_Jordan_parametriz_ y_coordinate,
            fPtr_Jordan_parametriz_ dx_differential,
            fPtr_Jordan_parametriz_ dy_differential,
            unsigned long long n )// #trapezia in the partition
        {
            double DeltaT = (tn - t0) / (double)n;
            double t = t0 + DeltaT;// the boundaries {t0,tn} are computed separately, after the core-loop. So trapezium starts at 1*DeltaT.
            Numerics::Complex res(0,0);
            // kordell starts here.
            for (; t < tn; t += DeltaT)// stop at the second to last, i.e. <tn. The boudaries are computed separately: t=t0, t=tn.
            {// sum all the internal sides
                res += complexAsScalar(x_coordinate(t), y_coordinate(t)) * Numerics::Complex(dx_differential(t),dy_differential(t));
            }
            // post kordell adjustments
            res *= DeltaT; // multiply them for the common base
            res += (
                    complexAsScalar(x_coordinate(t0), y_coordinate(t0)) * Numerics::Complex(dx_differential(t0),dy_differential(t0))+
                    complexAsScalar(x_coordinate(tn), y_coordinate(tn)) * Numerics::Complex(dx_differential(tn),dy_differential(tn))
                    ) * 0.5 * DeltaT; // add extrema * base/2
            // ready
            return res;
        }//


        /// <summary>
        /// Trapezium Integration. NB.: (u(x,y)+I*v(x,y) )*(dx+I*dy)==u*dx-v*dy + I*( u*dy+v*dx)
        /// Real Part of the Integral means : RealPart[u*dx-v*dy + I*( u*dy+v*dx)]==u*dx-v*dy
        /// </summary>
        double Integrate_equi_trapezium_RealPart(
            double t0, double tn, // extrema in the pull-back t in [t0,tn]
            fPtr_U_or_V_ u_Part,
            fPtr_U_or_V_ v_Part,
            fPtr_Jordan_parametriz_ x_coordinate,
            fPtr_Jordan_parametriz_ y_coordinate,
            fPtr_Jordan_parametriz_ dx_differential,
            fPtr_Jordan_parametriz_ dy_differential,
            unsigned long long n )// #trapezia in the partition
        {// RealPart == ==u*dx-v*dy
            double DeltaT = (tn - t0) / (double)n;
            double res = 0.0;
            double t = t0 + DeltaT;// the boundaries {t0,tn} are computed separately, after the core-loop. So trapezium starts at 1*DeltaT.
            // kordell starts here.
            for (; t < tn; t += DeltaT)// stop at the second to last, i.e. <tn. The boudaries are computed separately: t=t0, t=tn.
            {// sum all the internal sides
                res += u_Part(x_coordinate(t), y_coordinate(t)) * dx_differential(t) - v_Part(x_coordinate(t), y_coordinate(t)) * dy_differential(t);
            }
            // post kordell adjustments
            res *= DeltaT; // multiply them for the common base
            res += (
                u_Part(x_coordinate(t0), y_coordinate(t0)) * dx_differential(t0) - v_Part(x_coordinate(t0), y_coordinate(t0)) * dy_differential(t0) +
                +u_Part(x_coordinate(tn), y_coordinate(tn)) * dx_differential(tn) - v_Part(x_coordinate(tn), y_coordinate(tn)) * dy_differential(tn)
                      ) * 0.5 * DeltaT; // add extrema * base/2
            // ready
            return res;
        }//


        /// <summary>
        /// Trapezium Integration. NB.: (u(x,y)+I*v(x,y) )*(dx+I*dy)==u*dx-v*dy + I*( u*dy+v*dx)
        /// Immaginary Part of the Integral means : ImmaginaryPart[u*dx-v*dy + I*( u*dy+v*dx)]==u*dy+v*dx
        /// </summary>
        double Integrate_equi_trapezium_ImmaginaryPart(
            double t0, double tn, // extrema in the pull-back t in [t0,tn]
            fPtr_U_or_V_ u_Part,
            fPtr_U_or_V_ v_Part,
            fPtr_Jordan_parametriz_ x_coordinate,
            fPtr_Jordan_parametriz_ y_coordinate,
            fPtr_Jordan_parametriz_ dx_differential,
            fPtr_Jordan_parametriz_ dy_differential,
            unsigned long long n )// #trapezia in the partition
        {// ImmaginaryPart== [I*]( u*dy+v*dx)
            double DeltaT = (tn - t0) / (double)n;
            double res = 0.0;
            double t = t0 + DeltaT;// the boundaries {t0,tn} are computed separately, after the core-loop. So trapezium starts at 1*DeltaT.
            // kordell starts here.
            for (; t < tn; t += DeltaT)// stop at the second to last, i.e. <tn. The boudaries are computed separately: t=t0, t=tn.
            {// sum all the internal sides
                res += u_Part(x_coordinate(t), y_coordinate(t)) * dy_differential(t) + v_Part(x_coordinate(t), y_coordinate(t)) * dx_differential(t);
            }
            // post kordell adjustments
            res *= DeltaT; // multiply them for the common base
            res += (
                u_Part(x_coordinate(t0), y_coordinate(t0)) * dy_differential(t0) + v_Part(x_coordinate(t0), y_coordinate(t0)) * dx_differential(t0) +
                +u_Part(x_coordinate(tn), y_coordinate(tn)) * dy_differential(tn) + v_Part(x_coordinate(tn), y_coordinate(tn)) * dx_differential(tn)
                      ) * 0.5 * DeltaT; // add extrema * base/2
            // ready
            return res;
        }//


        bool extremaCheck(//NB. le coordinate sono riferite al piano dell'argomento
            Numerics::Complex z0,
            Numerics::Complex z1,
            double t0, double tn, // extrema in the pull-back
            fPtr_Jordan_parametriz_ x_coordinate,
            fPtr_Jordan_parametriz_ y_coordinate
            )
        {
            double localEpsilon = +1.0E-05;
            double z0_x_byJordan = x_coordinate(t0);// extrema in the push forward
            double z0_y_byJordan = y_coordinate(t0);
            double z1_x_byJordan = x_coordinate(tn);
            double z1_y_byJordan = y_coordinate(tn);
            //
            double delta_Z0x = fabs(z0_x_byJordan - z0.Re());
            double delta_Z0y = fabs(z0_y_byJordan - z0.Im());
            double delta_Z1x = fabs(z1_x_byJordan - z1.Re());
            double delta_Z1y = fabs(z1_y_byJordan - z1.Im());
            //
            bool res = (
                   delta_Z0x   < localEpsilon
                && delta_Z0y   < localEpsilon
                && delta_Z1x   < localEpsilon
                && delta_Z1y   < localEpsilon
                );
            //ready.
            return res;
        }// extremaCheck



        /// <summary>
        /// This function evaluates a contour integral in the complex plane, via the numerical method of Riemann partition
        /// on the pullback of a parametrization of the Chain.
        /// The caller is required to provide suitable function-pointers to the real and the immaginary part of the CoChain.
        /// f[z]==f[x+I*y]==u(x,y)+I*v(x,y) the u() and the v() have to be implemented, before calling this function.
        /// The implementation is:
        /// (u(x,y)+I*v(x,y))*(dx+I*dy)==u*dx-v*dy + I*( u*dy+v*dx)
        /// </summary>
        /// <param name="z0">start point of the Chain, in the argument plane</param>
        /// <param name="z1">end point of the Chain, in the argument plane</param>
        /// <param name="t0">start point in the pullback of the chain</param>
        /// <param name="tn">end point in the pullback of the chain</param>
        /// <param name="u_Part">real part of the image</param>
        /// <param name="v_Part">immaginary part of the image</param>
        /// <param name="x_coordinate">the first(abscissa) coordinate function, in the parametrization</param>
        /// <param name="y_coordinate">the second(ordinate) coordinate function, in the parametrization</param>
        /// <param name="dx_differential">the differential(i.e. measure) of the first(abscissa) coordinate function, in the parametrization</param>
        /// <param name="dy_differential">the differential(i.e. measure) of the second(ordinate) coordinate function, in the parametrization</param>
        /// <param name="n">the cardinality of the Riemann style partition, in the pullback of the Chain</param>
        /// <returns> a Complex:: instance, containing the integral result</returns>
        Numerics::Complex * ContourIntegral_ManagementMethod(
            Numerics::Complex z0,
            Numerics::Complex z1,
            double t0, double tn, // extrema in the pull-back
            fPtr_U_or_V_ u_Part,
            fPtr_U_or_V_ v_Part,
            fPtr_Jordan_parametriz_ x_coordinate,
            fPtr_Jordan_parametriz_ y_coordinate,
            fPtr_Jordan_parametriz_ dx_differential,
            fPtr_Jordan_parametriz_ dy_differential,
            unsigned long long n )// #trapezia in the partition
        {
            Process::LogWrappers::SectionOpen("ContourIntegral_ManagementMethod",0);
            bool extremaAdequacy = extremaCheck(
                z0, z1,
                t0, tn,
                x_coordinate,
                y_coordinate
            );
            if (!extremaAdequacy)
            {// log & return null
                Process::LogWrappers::SectionContent_fromMultipleStrings(0,1,"Integration extrema do not match, between coChain and Jordan-path.");
                return nullptr;
            }//{ throw new System.Exception("Integration extrema do not match, between coChain and Jordan-path."); }
            //
            Numerics::Complex * res = new Numerics::Complex(
                Integrate_equi_trapezium_RealPart(
                    t0,tn
                    ,u_Part
                    ,v_Part
                    , x_coordinate
                    , y_coordinate
                    , dx_differential
                    , dy_differential
                    ,n )

                , Integrate_equi_trapezium_ImmaginaryPart(
                    t0, tn
                    , u_Part
                    , v_Part
                    , x_coordinate
                    , y_coordinate
                    , dx_differential
                    , dy_differential
                    , n)
            );
            Process::LogWrappers::SectionClose();
            // ready.
            return res;// Caller has to delete.
        }// ContourIntegral_ManagementMethod


        Numerics::Complex * ContourIntegral_AsScalar_ManagementMethod(
            Numerics::Complex z0,
            Numerics::Complex z1,
            double t0, double tn, // extrema in the pull-back
            fPtr_ComplexAsScalar_ complexAsScalar,
            fPtr_Jordan_parametriz_ x_coordinate,
            fPtr_Jordan_parametriz_ y_coordinate,
            fPtr_Jordan_parametriz_ dx_differential,
            fPtr_Jordan_parametriz_ dy_differential,
            unsigned long long n )// #trapezia in the partition
        {
            Process::LogWrappers::SectionOpen("ContourIntegral_AsScalar_ManagementMethod",0);
            bool extremaAdequacy = extremaCheck(
                z0, z1,
                t0, tn,
                x_coordinate,
                y_coordinate
            );
            if (!extremaAdequacy)//NB. le coordinate sono riferite al piano dell'argomento
            {// log & return null
                Process::LogWrappers::SectionContent_fromMultipleStrings(0,1,new std::string("Integration extrema do not match, between coChain and Jordan-path."));
                return nullptr;
            }//{ throw new System.Exception("Integration extrema do not match, between coChain and Jordan-path."); }
            //
            Numerics::Complex * res = new Numerics::Complex(
                Integrate_equi_trapezium_ComplexImageAsScalar(
                    t0,tn
                    ,complexAsScalar
                    , x_coordinate
                    , y_coordinate
                    , dx_differential
                    , dy_differential
                    ,n )
            );
            Process::LogWrappers::SectionClose();
            // ready.
            return res;// Caller has to delete.
        }// ContourIntegral_AsScalar_ManagementMethod


/// Contour Integral can be performed also as Sum[f(z)*(dx+I*dy)]
/// so that there is no need to separate Real and Imaginary parts in the image.
/// this implementation finds the layout of a parametric LINE between the points {z0,z1}
/// the line equation is always the same and takes parameters for:{angular coefficient,independent variable,translation}
/// the differential is the angular coefficient. The methods that find the line layout are in RealAnalysis::linear_parametric(
/// for vertical lines, the implementation is {x=t0,y=t}. For non-vertical ones {x=t,y=m*t+q}, t in [t1,t2]
/// so  [t1,t2] is the pullback domain; it is identified by means of RealAnalysis::Real::parametricLinear_ante_image,here inlined.
Numerics::Complex * ContourIntegral_AsScalar_JordanLinearAutoDetect(
    Numerics::Complex z0,
    Numerics::Complex z1,
    // extrema in the pull-back will be auto-detected
    fPtr_ComplexAsScalar_ complexAsScalar,
    //the JordanLinear equations are always the same, since they take all as parameter(m,x,q) in y=m*t+q
    //the differentials need no function pointer either, since d/dt{m*t+q}==m
    //the JordanLinear equations are a pair, since they allow for a representation of all lines in the plane
    unsigned long long n )// #trapezia in the partition
{
    Process::LogWrappers::SectionOpen("ComplexIntegration::ContourIntegral_AsScalar_JordanLinearAutoDetect", 0);
    Couple left;//it's assumed that integration goes from_z0_to_z1
    left.argument = z0.Re();// left is z0
    left.image = z0.Im();
    Couple right;
    right.argument = z1.Re();// right is z1; if the pullback will grow backwards, the sign will be changed.
    right.image = z1.Im();
    //
    Two_Points_Interpolation::Parametric_Linear_Manifold parametric_giacitura =
        Two_Points_Interpolation::linear_parametric( left,right);
    if( parametric_giacitura.isProblemWellPosed==false)
    {
        Process::LogWrappers::SectionContent("Interpolation: Parametric Problem not well posed.",0);
    }
    Numerics::RealMatrix Mat_ParametricLayout(2,2,true);
    Mat_ParametricLayout.insert( parametric_giacitura.alpha_x, 0,0);Mat_ParametricLayout.insert( parametric_giacitura.beta_x , 0,1);// row (I)
    Mat_ParametricLayout.insert( parametric_giacitura.alpha_y, 1,0);Mat_ParametricLayout.insert( parametric_giacitura.beta_y , 1,1);// row (II)
    double pullback_alpha;
    double pullback_beta;
    Numerics::Complex * res = new Numerics::Complex(0.0, 0.0);//results will be added on it.
    double * t0x_t0y = nullptr;// needed for deletion outside the try-block.
    double * tnx_tny = nullptr;
    try
    {
        Mat_ParametricLayout.show();// DBG
        if( fabs(Mat_ParametricLayout.get_at(0,0))<+1E-20// two zeros on the principal diagonal->vertical line.
            && fabs(Mat_ParametricLayout.get_at(1,1))<+1E-20 )
        {// caso retta verticale: the y==t and the x=x0
            pullback_alpha = left.image;
            pullback_beta = right.image;
        }
        else if( fabs(Mat_ParametricLayout.get_at(0,0)-1.0)<+1E-20)//+1 at[0][0] means x[t]==t ->function->NON_vertical line.
        {// caso retta y==y(x), x[t]=t
            pullback_alpha = left.argument;
            pullback_beta = right.argument;
        }
        else
        {
            Crash crash("DEBUG: unknown matrix layout.");
            throw crash;
        }
        // the algo requires the pullback to be stepped-in in growing sense:
        if( pullback_alpha<=pullback_beta)
        {}//nothing to change
        else
        {// change sign
            double tmp = pullback_alpha;
            pullback_alpha = pullback_beta;
            pullback_beta = tmp;// swap extrema
            *res *= Numerics::Complex( -1.0, 0.0);// and multiply the integral by minus one
        }
        // check integration extrema adequacy in the push forward
        double localEpsilon = +1.0E-05;
        t0x_t0y = parametricLinear_image( parametric_giacitura, pullback_alpha);
        tnx_tny = parametricLinear_image( parametric_giacitura, pullback_beta);
        //
        double z0_x_byJordan = t0x_t0y[0];// extrema in the push forward
        double z0_y_byJordan = t0x_t0y[1];
        double z1_x_byJordan = tnx_tny[0];
        double z1_y_byJordan = tnx_tny[1];
        //
        double delta_Z0x = fabs(z0_x_byJordan - z0.Re());
        double delta_Z0y = fabs(z0_y_byJordan - z0.Im());
        double delta_Z1x = fabs(z1_x_byJordan - z1.Re());
        double delta_Z1y = fabs(z1_y_byJordan - z1.Im());
        //
        bool extremaAdequacy = (
               delta_Z0x   < localEpsilon
            && delta_Z0y   < localEpsilon
            && delta_Z1x   < localEpsilon
            && delta_Z1y   < localEpsilon
            );
        if (!extremaAdequacy)//NB. le coordinate sono riferite al piano dell'argomento
        {// log & return null
            Process::LogWrappers::SectionContent_fromMultipleStrings(0,1,new std::string("Integration extrema do not match, between coChain and Jordan-path."));
            return nullptr;
        }//{ throw new System.Exception("Integration extrema do not match, between coChain and Jordan-path."); }
        //
        // now let's step into the integral:
        //NB. Integrate_equi_trapezium_ComplexImageAsScalar has been inlined here
        //-------------------- legenda:
        double t0 = pullback_alpha;
        double tn = pullback_beta;
        double DeltaT = (tn - t0) / (double)n;
        double t = t0 + DeltaT;// the boundaries {t0,tn} are computed separately, after the core-loop. So trapezium starts at 1*DeltaT.
        double * tix_tiy = nullptr;
        //-------------------- legenda:
        // tix_tiy[0] == (ti)->x
        // tix_tiy[1] == (ti)->y
        // parametric_giacitura.alpha_x == dx_differential(t)
        // parametric_giacitura.alpha_y == dy_differential(t)
        // kordell starts here.
        for (; t< tn; t+= DeltaT)// stop at the second to last, i.e. <tn. The boudaries are computed separately: t=t0, t=tn.
        {// sum all the internal sides
            tix_tiy = parametricLinear_image( parametric_giacitura, t);
            if(nullptr!= tix_tiy)
            {
                *res += complexAsScalar(tix_tiy[0],tix_tiy[1]) * Numerics::Complex(parametric_giacitura.alpha_x,parametric_giacitura.alpha_y);
                delete[] tix_tiy;
                tix_tiy = nullptr;
            }
            else
            {
                Crash crash("DBG: invalid result in pushForward calculation.");
                throw crash;
            }
        }// for step in Riemann integration.
        // post kordell adjustments
        *res *= DeltaT; // multiply them for the common base
        // re-use the following arrays, before deletion:
        // t0x_t0y point z0 in the argument plane through pushForward of pullback_alpha
        // tnx_tny point z1 in the argument plane through pushForward of pullback_beta
        *res += (
                complexAsScalar(t0x_t0y[0],t0x_t0y[1]) * Numerics::Complex(parametric_giacitura.alpha_x,parametric_giacitura.alpha_y)+
                complexAsScalar(tnx_tny[0],tnx_tny[1]) * Numerics::Complex(parametric_giacitura.alpha_x,parametric_giacitura.alpha_y)
                ) * 0.5 * DeltaT; // add extrema * base/2
        // ready.
    }// try
    catch( Crash curExcp)
    {//this same deletion block is needed three times in a few rows, due to different possible execution paths.
        Process::LogWrappers::SectionContent("from inside: catch( Crash curExcp) specific of JordanLinear interpolation",0);
        if(nullptr!= t0x_t0y)
        {
            delete[] t0x_t0y;
            t0x_t0y = nullptr;
        }
        if(nullptr!= tnx_tny)
        {
            delete[] tnx_tny;
            tnx_tny = nullptr;
        }
    }
    catch(...)
    {
        Process::LogWrappers::SectionContent("from inside: catch(...) generic in JordanLinear interpolation",0);
        if(nullptr!= t0x_t0y)
        {//this same deletion block is needed three times in a few rows, due to different possible execution paths.
            delete[] t0x_t0y;
            t0x_t0y = nullptr;
        }
        if(nullptr!= tnx_tny)
        {
            delete[] tnx_tny;
            tnx_tny = nullptr;
        }
    }
    if(nullptr!= t0x_t0y)
    {//this same deletion block is needed three times in a few rows, due to different possible execution paths.
        delete[] t0x_t0y;
        t0x_t0y = nullptr;
    }
    if(nullptr!= tnx_tny)
    {
        delete[] tnx_tny;
        tnx_tny = nullptr;
    }
    // ready.
    Process::LogWrappers::SectionClose();
    return res;//caller has to delete.
}// ContourIntegral_AsScalar_JordanLinearAutoDetect


/// Contour Integral can be performed also as Sum[f(z)*(dx+I*dy)]
/// so that there is no need to separate Real and Imaginary parts in the image.
/// this implementation finds the layout of a parametric LINE between the points {z0,z1}
/// the line equation is always the same and takes parameters for:{angular coefficient,independent variable,translation}
/// the differential is the angular coefficient. The methods that find the line layout are in RealAnalysis::linear_parametric(
/// for vertical lines, the implementation is {x=t0,y=t}. For non-vertical ones {x=t,y=m*t+q}, t in [t1,t2]
/// so  [t1,t2] is the pullback domain; it is identified by means of RealAnalysis::Real::parametricLinear_ante_image,here inlined.
/// the function pointer to CoChain : complexAsScalar=ExpIntegralEi_CoChain;//---this fixes the CoChain. Specific method.
Numerics::Complex * ContourIntegral_AsScalar_JordanLinearAutoDetect_ExpIntegralEiRiemann(
    // no more a parameter: Numerics::Complex z0, in ExpIntegralEi is fixed in -Infinity on Real_minus_axis.
    Numerics::Complex z1,
    // extrema in the pull-back will be auto-detected
    // no more a parameter: fixed on ExpIntegralEi from inside; fPtr_ComplexAsScalar_ complexAsScalar,
    //the JordanLinear equations are always the same, since they take all as parameter(m,x,q) in y=m*t+q
    //the differentials need no function pointer either, since d/dt{m*t+q}==m
    //the JordanLinear equations are a pair, since they allow for a representation of all lines in the plane
    unsigned long long n )// #trapezia in the partition
{// throws : call in a try-catch
    Process::LogWrappers::SectionOpen("ComplexIntegration::ContourIntegral_AsScalar_JordanLinearAutoDetect_ExpIntegralEiRiemann", 0);
    //it's assumed that integration goes from_z0_to_z1 but z0 is fixed in minus_Infinity and Integral[Exp[t]/t,{-Inf,Log[Sqrt[+2]]}] is cached.
    Couple left;
    left.argument = log(sqrt(+2.0));// left is z0
    left.image = 0.0;//Integral[Exp[t]/t,{-Inf,Log[Sqrt[+2]]}] is cached. So Log[Sqrt[+2]]+I*0 is the "left".
    Couple right;
    right.argument = z1.Re();// right is z1; if the pullback will grow backwards, the sign will be changed.
    right.image = z1.Im();
    //
    Two_Points_Interpolation::Parametric_Linear_Manifold parametric_giacitura =
        Two_Points_Interpolation::linear_parametric( left,right);
    if( parametric_giacitura.isProblemWellPosed==false)
    {
        Process::LogWrappers::SectionContent("Interpolation: Parametric Problem not well posed.",0);
        Crash crash("Interpolation: Parametric Problem not well posed.");
        throw crash;
    }// else can continue.
    Numerics::RealMatrix Mat_ParametricLayout(2,2,true);
    Mat_ParametricLayout.insert( parametric_giacitura.alpha_x, 0,0);Mat_ParametricLayout.insert( parametric_giacitura.beta_x , 0,1);// row (I)
    Mat_ParametricLayout.insert( parametric_giacitura.alpha_y, 1,0);Mat_ParametricLayout.insert( parametric_giacitura.beta_y , 1,1);// row (II)
    double pullback_alpha;
    double pullback_beta;
    Numerics::Complex * res = new Numerics::Complex(0.0, 0.0);//results will be added on it.
    double * t0x_t0y = nullptr;// needed for deletion outside the try-block.
    double * tnx_tny = nullptr;
    try
    {
        // DBG  Mat_ParametricLayout.show();
        if( fabs(Mat_ParametricLayout.get_at(0,0))<+1E-20// two zeros on the principal diagonal->vertical line.
            && fabs(Mat_ParametricLayout.get_at(1,1))<+1E-20 )
        {// caso retta verticale: the y==t and the x=x0
            pullback_alpha = left.image;
            pullback_beta = right.image;
        }
        else if( fabs(Mat_ParametricLayout.get_at(0,0)-1.0)<+1E-20)//+1 at[0][0] means x[t]==t ->function->NON_vertical line.
        {// caso retta y==y(x), x[t]=t
            pullback_alpha = left.argument;
            pullback_beta = right.argument;
        }
        else
        {
            Crash crash("DEBUG: unknown matrix layout.");
            throw crash;
        }
        // the algo requires the pullback to be stepped-in in growing sense:
        if( pullback_alpha<=pullback_beta)
        {}//nothing to change
        else
        {// change sign
            double tmp = pullback_alpha;
            pullback_alpha = pullback_beta;
            pullback_beta = tmp;// swap extrema
            *res *= Numerics::Complex( -1.0, 0.0);// and multiply the integral by minus one
        }
        // check integration extrema adequacy in the push forward
        double localEpsilon = +1.0E-05;
        t0x_t0y = parametricLinear_image( parametric_giacitura, pullback_alpha);
        tnx_tny = parametricLinear_image( parametric_giacitura, pullback_beta);
        //
        double z0_x_byJordan = t0x_t0y[0];// extrema in the push forward
        double z0_y_byJordan = t0x_t0y[1];
        double z1_x_byJordan = tnx_tny[0];
        double z1_y_byJordan = tnx_tny[1];
        //
        Numerics::Complex z0( log(sqrt(+2.0)), 0.0);//-----start from the cached point.
        double delta_Z0x = fabs(z0_x_byJordan - z0.Re());
        double delta_Z0y = fabs(z0_y_byJordan - z0.Im());
        double delta_Z1x = fabs(z1_x_byJordan - z1.Re());
        double delta_Z1y = fabs(z1_y_byJordan - z1.Im());
        //
        bool extremaAdequacy = (
               delta_Z0x   < localEpsilon
            && delta_Z0y   < localEpsilon
            && delta_Z1x   < localEpsilon
            && delta_Z1y   < localEpsilon
            );
        if (!extremaAdequacy)//NB. le coordinate sono riferite al piano dell'argomento
        {// log & return null
            Process::LogWrappers::SectionContent_fromMultipleStrings(0,1,new std::string("Integration extrema do not match, between coChain and Jordan-path."));
            return nullptr;
        }//{ throw new System.Exception("Integration extrema do not match, between coChain and Jordan-path."); }
        //
        // now let's step into the integral:
        //NB. Integrate_equi_trapezium_ComplexImageAsScalar has been inlined here
        //-------------------- legenda:
        double t0 = pullback_alpha;
        double tn = pullback_beta;
        double DeltaT = (tn - t0) / (double)n;
        double t = t0 + DeltaT;// the boundaries {t0,tn} are computed separately, after the core-loop. So trapezium starts at 1*DeltaT.
        double * tix_tiy = nullptr;
        //-------------------- legenda:
        // tix_tiy[0] == (ti)->x
        // tix_tiy[1] == (ti)->y
        // parametric_giacitura.alpha_x == dx_differential(t)
        // parametric_giacitura.alpha_y == dy_differential(t)
        // kordell starts here.
        fPtr_ComplexAsScalar_ complexAsScalar = ExpIntegralEi_CoChain;//---this fixes the CoChain. Specific method.---------------------------------
        const double ExpIntegralEi_LogSqrt2 = -0.103371;
        Numerics::Complex ExpIntegralEi_LogSqrt2_inC( ExpIntegralEi_LogSqrt2, 0.0);
        *res += ExpIntegralEi_LogSqrt2_inC;
        for (; t< tn; t+= DeltaT)// stop at the second to last, i.e. <tn. The boudaries are computed separately: t=t0, t=tn.
        {// sum all the internal sides
            tix_tiy = parametricLinear_image( parametric_giacitura, t);
            if(nullptr!= tix_tiy)
            {
                *res += complexAsScalar(tix_tiy[0],tix_tiy[1]) * Numerics::Complex(parametric_giacitura.alpha_x,parametric_giacitura.alpha_y);
                delete[] tix_tiy;
                tix_tiy = nullptr;
            }
            else
            {
                Crash crash("DBG: invalid result in pushForward calculation.");
                throw crash;
            }
        }// for step in Riemann integration.
        // post kordell adjustments
        *res *= DeltaT; // multiply them for the common base
        // re-use the following arrays, before deletion:
        // t0x_t0y point z0 in the argument plane through pushForward of pullback_alpha
        // tnx_tny point z1 in the argument plane through pushForward of pullback_beta
        *res += (
                complexAsScalar(t0x_t0y[0],t0x_t0y[1]) * Numerics::Complex(parametric_giacitura.alpha_x,parametric_giacitura.alpha_y)+
                complexAsScalar(tnx_tny[0],tnx_tny[1]) * Numerics::Complex(parametric_giacitura.alpha_x,parametric_giacitura.alpha_y)
                ) * 0.5 * DeltaT; // add extrema * base/2
        // ready.
    }// try
    catch( Crash curExcp)
    {//this same deletion block is needed three times in a few rows, due to different possible execution paths.
        Process::LogWrappers::SectionContent("from inside: catch( Crash curExcp) specific of JordanLinear interpolation",0);
        if(nullptr!= t0x_t0y)
        {
            delete[] t0x_t0y;
            t0x_t0y = nullptr;
        }
        if(nullptr!= tnx_tny)
        {
            delete[] tnx_tny;
            tnx_tny = nullptr;
        }
    }
    catch(...)
    {
        Process::LogWrappers::SectionContent("from inside: catch(...) generic in JordanLinear interpolation",0);
        if(nullptr!= t0x_t0y)
        {//this same deletion block is needed three times in a few rows, due to different possible execution paths.
            delete[] t0x_t0y;
            t0x_t0y = nullptr;
        }
        if(nullptr!= tnx_tny)
        {
            delete[] tnx_tny;
            tnx_tny = nullptr;
        }
    }
    if(nullptr!= t0x_t0y)
    {//this same deletion block is needed three times in a few rows, due to different possible execution paths.
        delete[] t0x_t0y;
        t0x_t0y = nullptr;
    }
    if(nullptr!= tnx_tny)
    {
        delete[] tnx_tny;
        tnx_tny = nullptr;
    }
    // ready.
    Process::LogWrappers::SectionClose();
    return res;//caller has to delete.
}// ContourIntegral_AsScalar_JordanLinearAutoDetect_ExpIntegralEiRiemann

Numerics::Complex ExpIntegralEi_CoChain( double x, double y)// ExpIntegralEi[Log[z]]==LogIntegral[z]
{// the whole image, treated as an algebraic scalar.
    Numerics::Complex z( x,y);
    Numerics::Complex exp_z( z.ExpC());
    if(z.length()<+1.0E-15)
    {
        Crash crash("too near to the Pole in z==0+i*0");
        throw crash;
    }// else continue.
    Numerics::Complex res( exp_z/z );
    return res;
}// integrand_ComplexAsScalar

Numerics::Complex LogIntegral_CoChain( double x, double y)// ExpIntegralEi[Log[z]]==LogIntegral[z]
{// the whole image, treated as an algebraic scalar.
    Numerics::Complex z( x,y);
    if( (z.Im()<+1.0E-15 && z.Re()<0.0)  // on branch-cut R_minus
// NOT singular there:  || z.length()<+1.0E-15            // on the essential-singularity z=0
       || (z-Numerics::Complex(+1.0,0.0)).length() <+1.0E-15 ) // on the essential-singularity z=1
    {
        Crash crash("too near to the branch-cut R_minus or to the Essential-singularity (1,0) ");
        throw crash;
    }// else continue.
    Numerics::Complex res( Numerics::Complex(+1.0,0.0) / z.LnC() );
    return res;
}// integrand_ComplexAsScalar

}// nmsp
