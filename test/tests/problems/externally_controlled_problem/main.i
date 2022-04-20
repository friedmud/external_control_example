[Mesh]
  type = GeneratedMesh
  dim = 3
  nx = 10
  ny = 10
  nz = 10
  xmin = -1
  ymin = -1
  zmin = -2
  xmax = 1
  ymax = 1
  zmax = 2
[]

[Problem]
  type = ExternallyControlledProblem
[]

[Executioner]
  type = Transient
  dt = 1
  num_steps = 5
[]

[MultiApps]
  [sub]
    type = TransientMultiApp
    app_type = OtherApp
    input_files = sub.i
    positions = '0 0 0'
    execute_on = timestep_end
  []
[]

[Transfers]
  [to_sub]
    type = MultiAppNearestNodeTransfer
    to_multi_app = sub
    variable = from_external
    source_variable = external_variable
    fixed_meshes = true  #This is a HUGE optimiztion if your meshes aren't moving
  []
 []

[Outputs]
  exodus = true
[]
