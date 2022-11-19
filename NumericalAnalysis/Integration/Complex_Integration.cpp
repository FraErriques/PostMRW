#include "Complex_Integration.h"

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
        {// RealPart == ==u*dx-v*dy
            double DeltaT = (tn - t0) / (double)n,
            t = DeltaT;// the boundaries {t0,tn} are computed separately, after the core-loop. So trapezium starts at 1*DeltaT.
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
            double DeltaT = (tn - t0) / (double)n,
            res = 0.0,
            t = DeltaT;// the boundaries {t0,tn} are computed separately, after the core-loop. So trapezium starts at 1*DeltaT.
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
            double DeltaT = (tn - t0) / (double)n,
            res = 0.0,
            t = DeltaT;// the boundaries {t0,tn} are computed separately, after the core-loop. So trapezium starts at 1*DeltaT.
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


        bool extremaCheck(
            Numerics::Complex z0,
            Numerics::Complex z1,
            double t0, double tn, // extrema in the pull-back
            fPtr_Jordan_parametriz_ x_coordinate,
            fPtr_Jordan_parametriz_ y_coordinate
            )
        {
            double localEpsilon = +1.0E-13;
            double z0_x_byJordan = x_coordinate(t0);
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
            if (!extremaAdequacy)
            {// log & return null
                Process::LogWrappers::SectionContent_fromMultipleStrings(0,1,"Integration extrema do not match, between coChain and Jordan-path.");
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

}// nmsp
