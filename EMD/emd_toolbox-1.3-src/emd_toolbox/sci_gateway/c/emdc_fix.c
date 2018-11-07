/*
* H. Nahrstaedt - Aug 2010
* G. Rilling, last modification: 3.2007
* gabriel.rilling@ens-lyon.fr
*
* code based on a student project by T. Boustane and G. Quellec, 11.03.2004
* supervised by P. Chainais (ISIMA - LIMOS - Universite Blaise Pascal - Clermont II
* email : pchainai@isima.fr).
*/

/* IMPORTANT: uncomment the following line if you experience MATLAB chrashes */
/* instead of a normal error message when calling the function with bad syntax */
/* This bug is apparently restricted to GNU/Linux systems and to MATLAB versions prior to R2007a */
/*#define _ALT_MEXERRMSGTXT_*/

#include <stdlib.h>
#include <stdio.h>
// #include <stack-c.h>
#include "emd.h"


/************************************************************************/
/*                                                                      */
/* MAIN FUNCTION                                                   */
/*                                                                      */
/************************************************************************/

//void mexFunction(int nlhs,mxArray *plhs[],int nrhs,const mxArray *prhs[]) {
int int_emdc_fix(char *fname)
{
    /* declarations */
  int i,n,nb_imfs,max_imfs,iteration_counter,nb_iterations,stop_status,allocated_x,stop_EMD;
  extrema_t ex;
  input_t input;
  envelope_t env;
  double *x,*y,*z,*m,*a;
  imf_list_t list;
  FILE *fid;
  
  SciErr sciErr;
  CheckRhs (2,4);
  CheckLhs (1,2);
  
    /* get input data */
  input=get_input_fix(pvApiCtx,Rhs,&sciErr);
  if(sciErr.iErr)
  {
    printError(&sciErr, 0);
    return 0;
  }
  n=input.n;
  max_imfs=input.max_imfs;
  nb_iterations=input.nb_iterations;
  allocated_x=input.allocated_x;
  x=input.x;
  y=input.y;
  
    /* initialisations */
  ex=init_extr(n+2*NBSYM);
  list=init_imf_list(n);
  z=(double *)malloc(n*sizeof(double));
  m=(double *)malloc(n*sizeof(double));
  a=(double *)malloc(n*sizeof(double));
  env=init_local_mean(n+2*NBSYM);
  
    /* MAIN LOOP */
  
  nb_imfs=0;
  stop_EMD=0;
  
  while ((!max_imfs || (nb_imfs < max_imfs)) && !stop_EMD) {
    
        /* initialisation */
    for (i=0;i<n;i++) z[i]=y[i];
    for (i=0;i<n;i++) m[i]=y[i];
    iteration_counter=0;
    
    stop_status = mean_and_amplitude(x,z,m,a,n,&ex,&env);
    
        /* SIFTING LOOP */
    
    while (!stop_status && iteration_counter<nb_iterations) {
      
            /* subtract the local mean */
      for (i=0;i<n;i++) z[i]=z[i]-m[i];
      iteration_counter++;
      
      stop_status = mean_and_amplitude(x,z,m,a,n,&ex,&env);
      
      
    }
    
        /* save current IMF into list if at least     */
        /* one sifting iteration has been performed */
    if (iteration_counter) {
      add_imf(&list,z,iteration_counter);
      nb_imfs++;
      for (i=0;i<n;i++) y[i]=y[i]-z[i];
      
    }
    else
      stop_EMD = 1;
    
  }
  
    /* save the residual into list */
  add_imf(&list,y,0);
  
    /* output into a MATLAB array */
 write_output(list,pvApiCtx,Rhs,Lhs);
    LhsVar(1) = Rhs + 1; 
   if (Lhs>1)
      LhsVar(2) = Rhs + 2; 
   PutLhsVar();

    /* free allocated memory */
  //if (allocated_x)
    free(x);
  free(y);
  free(m);
  free(a);
  free_local_mean(env);
  free(z);
  free_imf_list(list);
  free_extr(ex);
      return 0;
}
