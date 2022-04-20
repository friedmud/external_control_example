#include "OtherApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"
#include "MooseSyntax.h"

InputParameters
OtherApp::validParams()
{
  InputParameters params = MooseApp::validParams();

  // Do not use legacy material output, i.e., output properties on INITIAL as well as TIMESTEP_END
  params.set<bool>("use_legacy_material_output") = false;

  return params;
}

OtherApp::OtherApp(InputParameters parameters) : MooseApp(parameters)
{
  OtherApp::registerAll(_factory, _action_factory, _syntax);
}

OtherApp::~OtherApp() {}

void
OtherApp::registerAll(Factory & f, ActionFactory & af, Syntax & syntax)
{
  ModulesApp::registerAll(f, af, syntax);
  Registry::registerObjectsTo(f, {"OtherApp"});
  Registry::registerActionsTo(af, {"OtherApp"});

  /* register custom execute flags, action syntax, etc. here */
}

void
OtherApp::registerApps()
{
  registerApp(OtherApp);
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
extern "C" void
OtherApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  OtherApp::registerAll(f, af, s);
}
extern "C" void
OtherApp__registerApps()
{
  OtherApp::registerApps();
}
