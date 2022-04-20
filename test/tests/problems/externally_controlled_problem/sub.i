[Mesh]
  type = GeneratedMesh
  dim = 3
  nx = 20
  ny = 20
  nz = 20
  xmin = -1
  ymin = -1
  zmin = -2
  xmax = 1
  ymax = 1
  zmax = 2
[]

[Variables]
  [u]
  []
[]

[AuxVariables]
  [from_external]
    order = CONSTANT
    family = MONOMIAL
  []
[]

[Kernels]
  [diff]
    type = Diffusion
    variable = u
  []
  [force]
    type = CoupledForce
    variable = u
    v = from_external
  []
[]

[BCs]
  [all]
    type = DirichletBC
    variable = u
    boundary = 'left right top bottom front back'
    value = 0
  []
[]

[Executioner]
  type = Transient
  dt = 1
  num_steps = 1
  solve_type = 'PJFNK'
  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
[]

[Outputs]
  exodus = true
[]
