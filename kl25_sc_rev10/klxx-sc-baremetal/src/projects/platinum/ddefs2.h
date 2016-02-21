 /*
 *  ddefs2.h
 *
 *  Created on: Oct 28, 2015
 *      Author: tsd316
 */

#ifndef DDEFS2_H_
#define DDEFS2_H_

#define NMODES 8
#define NTYPES 5
#define NUM (sizeof(prstbl) / sizeof(struct pars_list))
#define NUMOUT (sizeof(outtbl) / sizeof(struct pars_list))
#define MAXLEN 256
#define MAXLINES 100
#define MAXNODES 30

struct pars_list
  {
  char *name;
  int type,nstrs,nvals,imit;
  };

struct element {
  char *name,*node;
  int type,dim,size;
  struct pars_list *par;
  struct dat *eldat;
  struct t_line *tlp;
  };
typedef struct element elem_t;



#endif /* DDEFS2_H_ */
