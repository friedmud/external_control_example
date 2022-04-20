#include "ExternalControlExampleApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"
#include "MooseSyntax.h"

#include "OtherApp.h"

InputParameters
ExternalControlExampleApp::validParams()
{
  InputParameters params = MooseApp::validParams();

  // Do not use legacy material output, i.e., output properties on INITIAL as well as TIMESTEP_END
  params.set<bool>("use_legacy_material_output") = false;

  return params;
}

ExternalControlExampleApp::ExternalControlExampleApp(InputParameters parameters) : MooseApp(parameters)
{
  ExternalControlExampleApp::registerAll(_factory, _action_factory, _syntax);
}

ExternalControlExampleApp::~ExternalControlExampleApp() {}

void
ExternalControlExampleApp::registerAll(Factory & f, ActionFactory & af, Syntax & syntax)
{
  ModulesApp::registerAll(f, af, syntax);
  Registry::registerObjectsTo(f, {"ExternalControlExampleApp"});
  Registry::registerActionsTo(af, {"ExternalControlExampleApp"});

  /* register custom execute flags, action syntax, etc. here */
}

void
ExternalControlExampleApp::registerApps()
{
  registerApp(ExternalControlExampleApp);

  OtherApp::registerApps();
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
extern "C" void
ExternalControlExampleApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  ExternalControlExampleApp::registerAll(f, af, s);
}
extern "C" void
ExternalControlExampleApp__registerApps()
{
  ExternalControlExampleApp::registerApps();
}
