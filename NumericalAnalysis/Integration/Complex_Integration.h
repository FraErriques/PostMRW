#ifndef COMPLEX_INTEGRATION_H
#define COMPLEX_INTEGRATION_H


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


namespace Complex_Integration{



}// nmsp

#endif // COMPLEX_INTEGRATION_H
