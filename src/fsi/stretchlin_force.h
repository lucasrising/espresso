/*
  Copyright (C) 2010,2011 The ESPResSo project
  Copyright (C) 2002,2003,2004,2005,2006,2007,2008,2009,2010 Max-Planck-Institute for Polymer Research, Theory Group, PO Box 3148, 55021 Mainz, Germany
  
  This file is part of ESPResSo.
  
  ESPResSo is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  ESPResSo is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>. 
*/
#ifndef STRETCHLIN_FORCE_H
#define STRETCHLIN_FORCE_H

#include "utils.h"
#include "interaction_data.h"
#include "particle_data.h"

/** \file stretchlin_force.h
 *  Routines to calculate the STRETCHLIN_FORCE Energy or/and STRETCHLIN_FORCE force 
 *  for a particle pair. (Dupin2007)
 *  \ref forces.c
*/

/************************************************************/

/// set the parameters for the stretchlin_force potential
int stretchlin_force_set_params(int bond_type, double r0, double kslin);


/** Computes the STRETCHLIN_FORCE pair force and adds this
    force to the particle forces (see \ref #inter). 
    @param p1        Pointer to first particle.
    @param p2        Pointer to second particle.
    @param iaparams  spring stiffness ks, initial distance between particles (see \ref #inter).
    @param dx        particle distance vector
    @param force     returns force of particle 1
    @return true if the bond is broken
*/

MDINLINE int calc_stretchlin_force_pair_force(Particle *p1, Particle *p2, Bonded_ia_parameters *iaparams, double dx[3], double force[3])
{
  int i;
  double fac, dr, len2, len;

  len2 = sqrlen(dx);

  len = sqrt(len2);
  dr = len - iaparams->p.stretchlin_force.r0;

//  lambda = 1.0*len/iaparams->p.stretchlin_force.r0;
  fac = -iaparams->p.stretchlin_force.kslin * dr / iaparams->p.stretchlin_force.r0;
  
  for(i=0;i<3;i++)
    force[i] = fac*dx[i]/len;

  ONEPART_TRACE(if(p1->p.identity==check_id) fprintf(stderr,"%d: OPT: FENE f = (%.3e,%.3e,%.3e) with part id=%d at dist %f fac %.3e\n",this_node,p1->f.f[0],p1->f.f[1],p1->f.f[2],p2->p.identity,sqrt(len2),fac));
  ONEPART_TRACE(if(p2->p.identity==check_id) fprintf(stderr,"%d: OPT: FENE f = (%.3e,%.3e,%.3e) with part id=%d at dist %f fac %.3e\n",this_node,p2->f.f[0],p2->f.f[1],p2->f.f[2],p1->p.identity,sqrt(len2),fac));
  
  return 0;
}

#endif
