//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html
#include "ExternalControlExampleTestApp.h"
#include "ExternalControlExampleApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "MooseSyntax.h"
#include "ModulesApp.h"

InputParameters
ExternalControlExampleTestApp::validParams()
{
  InputParameters params = ExternalControlExampleApp::validParams();
  return params;
}

ExternalControlExampleTestApp::ExternalControlExampleTestApp(InputParameters parameters) : MooseApp(parameters)
{
  ExternalControlExampleTestApp::registerAll(
      _factory, _action_factory, _syntax, getParam<bool>("allow_test_objects"));
}

ExternalControlExampleTestApp::~ExternalControlExampleTestApp() {}

void
ExternalControlExampleTestApp::registerAll(Factory & f, ActionFactory & af, Syntax & s, bool use_test_objs)
{
  ExternalControlExampleApp::registerAll(f, af, s);
  if (use_test_objs)
  {
    Registry::registerObjectsTo(f, {"ExternalControlExampleTestApp"});
    Registry::registerActionsTo(af, {"ExternalControlExampleTestApp"});
  }
}

void
ExternalControlExampleTestApp::registerApps()
{
  registerApp(ExternalControlExampleApp);
  registerApp(ExternalControlExampleTestApp);
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
// External entry point for dynamic application loading
extern "C" void
ExternalControlExampleTestApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  ExternalControlExampleTestApp::registerAll(f, af, s);
}
extern "C" void
ExternalControlExampleTestApp__registerApps()
{
  ExternalControlExampleTestApp::registerApps();
}
