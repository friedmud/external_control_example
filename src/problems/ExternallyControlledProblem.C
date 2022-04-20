//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ExternallyControlledProblem.h"

#include "MooseMesh.h"
#include "AuxiliarySystem.h"

#include "libmesh/mesh_tools.h"

#include <thread>
#include <random>
#include <chrono>

registerMooseObject("ExternalControlExampleApp", ExternallyControlledProblem);

InputParameters
ExternallyControlledProblem::validParams()
{
  InputParameters params = ExternalProblem::validParams();
  return params;
}

ExternallyControlledProblem::ExternallyControlledProblem(const InputParameters & params)
  : ExternalProblem(params), _mesh_bounding_box(MeshTools::create_bounding_box(mesh().getMesh()))
{
  // Check that the mesh is symmetric
  if (!_mesh_bounding_box.max().relative_fuzzy_equals(-_mesh_bounding_box.min()))
    mooseError("Mesh must be symmetric around the x/y/z axes");
}

bool
ExternallyControlledProblem::converged()
{
  // This specifies whether the "solve" (which is just
  // fetching/receiving data from the external process/file)
  // was successful.
  //
  // Lots of options here - maybe even return false
  // if we were unable to get data from the external process?
  // But - returning true is fine for now
  return true;
}

void
ExternallyControlledProblem::externalSolve()
{
  // This is where you would fetch/receive data from the external process
  // You may be getting the data from MPI - or maybe reading it from a
  // file.  Or getting it from TCP/UDP... whatever.

  // The important part is that this would _block_ (stall, pause)
  // until the data has been received.
  //
  // To mimic that here we'll just sleep for a random amount of time

  // Shamelessly adapted from: https://stackoverflow.com/a/7577532
  std::mt19937_64 eng{std::random_device{}()};
  std::uniform_int_distribution<> dist{1, 4};
  std::this_thread::sleep_for(std::chrono::seconds{dist(eng)});
}

void
ExternallyControlledProblem::syncSolutions(Direction direction)
{
  if (direction == Direction::FROM_EXTERNAL_APP)
  {
    // This is where you would take data you got from the external app/process/file
    // and put it into the field variable we added in addExternalVariables

    // Now, to mimic getting the data I'm going to use a convolution
    // of two cos waves

    // One is going azimuthally down the z axis, it will be zero at the
    // top and bottom of the mesh and 1 in the middle - and then will
    // be scaled by the current time (so it always grows)

    // The other will be a sin wave along the x-axis that is
    // revolved around the z axis (therefore sampled radially in the
    // xy plane.  It will be zero at the x/y extremum of the mesh
    // and will be 1 in the middle.

    auto bounding_box_max = _mesh_bounding_box.max();
    auto t = time();

    // Azimuthal:
    auto z_extent = bounding_box_max(2);
    auto az_func = [z_extent, t](Real z)
    { return t * std::cos((1. / (2. * libMesh::pi * z_extent)) * z); };

    // Radial
    auto xy_extent = Point(bounding_box_max(0), bounding_box_max(1)).norm();
    auto radial_func = [xy_extent](Real r)
    { return std::cos((1. / (2. * libMesh::pi * xy_extent)) * r); };

    auto external_value = [az_func, radial_func](Point p)
    { return az_func(p(2)) * radial_func(Point(p(0), p(1)).norm()); };

    auto & aux_solution = _aux->solution();
    auto aux_sys_num = _aux->number();

    for (auto elem : _mesh.getMesh().active_local_element_ptr_range())
    {
      // Get the dof index for the variable we added in
      // addExternalVariables
      auto dof = elem->dof_number(aux_sys_num, /* var_num = */ 0, /* component = */ 0);

      // Set the value in the solution vector
      aux_solution.set(dof, external_value(elem->vertex_average()));
    }

    // Do parallel synchronization of the solution vector
    _aux->update();
  }
  else
  {
    // If you needed to transfer data back out you would do that here
  }
}

void
ExternallyControlledProblem::addExternalVariables()
{
  // Add a variable to hold the external temperature
  // since this is the only variable we're adding it will be variable `0`

  InputParameters params = _factory.getValidParams("MooseVariable");

  params.set<MooseEnum>("order") = "CONSTANT";
  params.set<MooseEnum>("family") = "MONOMIAL";
  params.set<Moose::VarKindType>("_var_kind") = Moose::VarKindType::VAR_AUXILIARY;

  _aux->addVariable("MooseVariable", "external_variable", params);
}
