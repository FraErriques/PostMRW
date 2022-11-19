#ifndef COMPLEX_INTEGRATION_H
#define COMPLEX_INTEGRATION_H

#include "../Complex/Complex.h"
#include "../../Common/LogFs_wrap/LogFs_wrap.h"

//the present namespace is intended to provide Contour-Integrals in the Complex plane, as in:
//Integrate[f(x+I*y)dz]==Integrate[ (u(x+I*y)+I*v(x+I*y))*(dx+I*dy)]==
//Integrate[(u,v)(dx,dy)]== Integrate[u_dx-v_dy +I*(u_dy+v_dx)]==
//Integrate[u(x,y)_dx]-Integrate[v(x,y)_dy] +I*{Integrate[u(x,y)_dy]+Integrate[v(x,y)_dx]}
//Oss. In the last two lines the notation Integrate[f()_dm] has been assumed, to indicate that the CoChain f()
//is integrated in the Measure dm.
//for the purposes of Numerical Analysis, a single Integrate[f(z)_dz] leads to four real integrals, as shown
//three lines above.
//The Chain has to be parametrized, in a single real variable (it's a one dimensional variety) and the CoChain has
//to be restricted to the Chain(on each smooth portion of it).
//The differentials of the coordinate-functions of the chain, will introduce measure terms.
//Eg. u(z)==u(x+I*y)==u(x[t]+I*y[t]) -> u(z)_dx==u(x[t]+I*y[t])_x'[t]dt
//where the factor x'[t] is a coonsequence of the Diffeomorphism of integrating in dx==d(x[t])==x'[t]dt
//The subject matter of the Diffeomorphism of u(z)_dx has to applied to all four Real integrals:
// { u_dx,u_dy, v_dx,v_dy}
//
//notes from Wiki:
//Calcolare gli integrali con metodi diretti significa calcolarli con metodi simili a quelli usati per gli
//integrali curvilinei di funzioni a più variabili. Ciò significa che usiamo il seguente metodo:
//    - parametrizzazione del contorno
//        Il contorno è parametrizzato da una funzione derivabile che manda numeri reali in numeri complessi, oppure il
//        contorno viene diviso in pezzi che vengono parametrizzati separatamente.
//    - sostituzione della parametrizzazione nell'integranda
//        Sostituire la parametrizzazione nell'integranda trasforma l'integrale in uno di una variabile reale.
// la nota riporta inoltre il metodo di cercare la primitiva ed applicare il Thm di Stokes; cosa non praticabile in un contesto
// di AnalisiNumerica, nel quale non sia disponibile un parser e l'algoritmo di Risch; cose queste che appartengono al calcolo simbolico,
// implementato da prodotti di Computer-Algebra, quali Wolfram-Mathematica. Nel contesto dell'Analisi Numerica si utilizzano invece discretizzazioni
// quali l'Integrale di Riemann.
//
//notes from Calculus2015 :
// Trapezium Integration. NB.: (u(x,y)+I*v(x,y) )*(dx+I*dy)==u*dx-v*dy + I*( u*dy+v*dx)
// Purpose of this routine -Trapezia- is to provide a numerical solution, suitable for the cases where no primitive can be found.
// The technique of Trapezia is to subdivide the domain in "n" equal Deltas and evaluate the integrand at the extrema of each trapezium
// instead of evaluating a primitive at the boundary, as in Stokes' theorems. Therefore the first step is to choose a contour, over which
// the integration will be performed. Due to Cauchy theorem, for olomorphic functions there's no involvment of the contour when integrating
// over omotopic paths. Nonetheless the utility of numerical methods -as Trapezia- is to allow the evaluation even of functions
// not endowed of an elementary primitive. For that purpose, we cannot rely on the behaviour (of the primitive) at the extrema. We have to
// explicitly follow a path that connects (in a Jordan way) the extrema. First step will be the choice of a contour and of its parametrization.
// In the C-plane the contour will be as L:{x(t)=f(t), y(t)=g(t)} and so dx=x'(t)dt , dy=y'(t)dt. Due to this, the two coordinate functions:
// u=u(x,y) and v(x,y) will have to be restricted to the path as in u=u(x(t),y(t)) , v=v(x(t),y(t))
// Therefore the classical:  (u(x,y)+I*v(x,y) )*(dx+I*dy)==u*dx-v*dy + I*( u*dy+v*dx)
// will become: u(x(t),y(t))*dx-v(x(t),y(t))*dy + I*( u(x(t),y(t))*dy+v(x(t),y(t))*dx) which will be
// u(x(t),y(t))*x'(t)dt-v(x(t),y(t))*y'(t)dt + I*( u(x(t),y(t))*y'(t)dt+v(x(t),y(t))*x'(t)dt)
// the two not-homogeneous quantities that must be calculated separately are:
//      u(x(t),y(t))*x'(t)dt-v(x(t),y(t))*y'(t)dt   for the Real-part
//      u(x(t),y(t))*y'(t)dt+v(x(t),y(t))*x'(t)dt   for the Immaginary-part
//
// The routine in this class is intended to be for elementary functions, so they will accept just the complex argument and no other parameter whatsoever.
// The user will have to provide function-pointers (i.e. delegates) to the following methods:
// double x(double t) it's the x-coordinate function -abscissa- of the parametrization for the Jordan curve, in the C-plane
// double y(double t) it's the y-coordinate function -ordinate- of the parametrization for the Jordan curve, in the C-plane
// double dx(double t) it's the x-differential; it has to contain everything that is a factor of dt es. dx=2*t*dt -> dx returns(2*t), accepting t.
// double dy(double t) it's the y-differential; it has to contain everything that is a factor of dt es. dy=3*Sin(t)*dt -> dy returns(3*Sin(t)), accepting t.
// The user will have to provide also function-pointers to:
// double u(double x, double y) which will be fed as u(x(t),y(t)) and the same for v(x(t),y(t)).
// This class instead will provide:
// a method double RealPartIntegral which performs: u*dx-v*dy in terms of u(x(t),y(t))*dx(t)-v(x(t),y(t))*dy(t)
// the RealPartIntegral method will accept: t_inf, t_sup, n_card_trapezia, f_pointer_x(t), f_pointer_y(t), f_pointer_dx(t), f_pointer_dy(t), f_pointer_u(x,y), f_pointer_v(x,y)
// a method double ImmaginaryPartIntegral which performs: I*( u*dy+v*dx) in terms of u(x(t),y(t))*dy(t)+v(x(t),y(t))*dx(t)
// the ImmaginaryPartIntegral method will accept: t_inf, t_sup, n_card_trapezia, f_pointer_x(t), f_pointer_y(t), f_pointer_dx(t), f_pointer_dy(t), f_pointer_u(x,y), f_pointer_v(x,y)
// NB. in future implementations the management method, which calls both {RealPartIntegral,ImmaginaryPartIntegral} could spawn separate threads for them and then join the results.
// So this class instead will provide a management method -the only public one- wich will have signature:
// ComplexField.Complex ContourIntegral(  t_inf, t_sup, n_card_trapezia, f_pointer_x(t), f_pointer_y(t), f_pointer_dx(t), f_pointer_dy(t), f_pointer_u(x,y), f_pointer_v(x,y) )
// It will call  both {Integrate_equi_trapezium_RealPart,Integrate_equi_trapezium_ImmaginaryPart}.
// Have fun.
//
// As an aid to successive implementation, I'm providing a set of the function that are needed by this algorithm. They are u,v,x,y,dx,dy
// the functions choosen for the example are f(z)=z which implies u(x,y)=x, v(x,y)=y; the choice for the contour is x(t)=t,y(t)=2*t+1,dx=dt which means dx=1
// dy=2*dt which means dy=2.

namespace Complex_Integration{

// fPtr Types
//general setting for contour-integrals of elementary functions.
typedef double (*fPtr_Jordan_parametriz_)(double);// the x(t) or y(t) or dx(t) or dy(t) of the Jordan parametric contour.
typedef double (*fPtr_U_or_V_)(double,double);// the real or immaginary part of the image.// this will work fot u=u(x,y) and v=v(x,y)
typedef Numerics::Complex (*fPtr_ComplexAsScalar_)(double,double);// the whole image, treated as an algebraic scalar.

        //  #region exampleFunctions :
        /// <summary>
        /// the functions chosen for the example are f(z)=z which implies u(x,y)=x, v(x,y)=y;
        /// the choice for the contour is x(t)=t,y(t)=2*t+1,dx=dt which means dx=1
        /// dy=2*dt which means dy=2.
        /// </summary>
        // Jordan
        double x( double t);// x(t)=t
        double y( double t );// y(t)=2*t+1
        double dx( double t );// x(t)=t->dx(t)=x'(t)dt=1*dt
        double dy( double t );// y(t)=2*t+1 ->dy(t)=y'(t)dt==2*dt
        // f(z)==u(x,y)+I*(v(x,y)
        double genericIntegrand_u_part( double x, double y );// f(z)==z -> Re(f(z))==Re(f(x+I*y))==Re(x+I*y)==x.
        double genericIntegrand_v_part( double x, double y );// f(z)==z -> Im(f(z))==Im(f(x+I*y))==Im(x+I*y)==y.
        /// w==f(z) // the whole image, treated as an algebraic scalar.
        Numerics::Complex integrand_ComplexAsScalar( double x, double y);// the whole image, treated as an algebraic scalar.
        //  #endregion exampleFunctions


        Numerics::Complex Integrate_equi_trapezium_ComplexImageAsScalar(
            double t0, double tn, // extrema in the pull-back t in [t0,tn]
            fPtr_ComplexAsScalar_ complexAsScalar,
            fPtr_Jordan_parametriz_ x_coordinate,
            fPtr_Jordan_parametriz_ y_coordinate,
            fPtr_Jordan_parametriz_ dx_differential,
            fPtr_Jordan_parametriz_ dy_differential,
            unsigned long long n );// #trapezia in the partition


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
            unsigned long long n );// #trapezia in the partition



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
            unsigned long long n );// #trapezia in the partition


        /// a method to check the coherence between the coordinate function values at boundary points and the CoChain values ibidem.
        bool extremaCheck(
            Numerics::Complex z0,
            Numerics::Complex z1,
            double t0, double tn, // extrema in the pull-back
            fPtr_Jordan_parametriz_ x_coordinate,
            fPtr_Jordan_parametriz_ y_coordinate
            );


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
            unsigned long long n );// #trapezia in the partition

        Numerics::Complex * ContourIntegral_AsScalar_ManagementMethod(
            Numerics::Complex z0,
            Numerics::Complex z1,
            double t0, double tn, // extrema in the pull-back
            fPtr_ComplexAsScalar_ complexAsScalar,
            fPtr_Jordan_parametriz_ x_coordinate,
            fPtr_Jordan_parametriz_ y_coordinate,
            fPtr_Jordan_parametriz_ dx_differential,
            fPtr_Jordan_parametriz_ dy_differential,
            unsigned long long n );// #trapezia in the partition

}// nmsp

#endif // COMPLEX_INTEGRATION_H
