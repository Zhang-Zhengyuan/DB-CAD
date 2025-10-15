/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// file     : ds4rk.hxx
// created  : Sept. 28, 1996
// author   : George Celniker
// synopsis : Fourth order Runge-Kutta for time integration of
//            differential equations.
// contains : DS_runge_kutta()
//            DS_runge_kutta_step()

#ifndef DS_RUNGE_KUTTA
#define DS_RUNGE_KUTTA

#include "dcl_adm.h"

DECL_ADM void 
DS_runge_kutta             // eff: integrate eqns over span of time
 (int dof_count,           // in : number of state variables
  double *w,               // in : state variables stored for time=t0
  double t0,               // in : start time
  double t1,               // in : stop time
  int store_count,         // in : number of saved states (min = 2)
                           //      store_dt = (t1-t0)/(store_count-1) ;
  int inc_count,           // in : number of steps between stored steps
                           //      dt = store_dt/inc_count ; (min = 1)
  void (* dwdt)            // in : funtion to compute (dw/dt)
    (int,                  //      in : dof_count
     double,               //      in : time t
     double *,             //      in : state values w
     double *),            //      out: state derivatives dw/dt at time t
  double *wt,              // out: all stored w values, 
                           //      [w(t0),w(t0+dt),...w(t1)]
                           //      sized:[store_count*dof_count]
  double *tt) ;            // out: the time of each stored step in wt
                           //      sized:[store_count]

DECL_ADM void
DS_runge_kutta_step        // eff: integrate eqns one time step
 (int dof_count,           // in : number of state_variables
  double  *w,              // in : values of state variables at time=t
  double *dw,              // in : valued of dw/dt at time=t
  double   t,              // in : current time t
  double  dt,              // in : time step size
  void (* dwdt)            // in : funtion to compute (dw/dt)
    (int,                  //      in : dof_count
     double,               //      in : time t
     double *,             //      in : state values w
     double *),            //      out: state derivatives dw/dt at time t
  double *wout,            // out: state variable values at time = t+dt
  double *scratch) ;       // tmp: sized:[3*dof_count]



#endif // DS_RUNGE_KUTTA

