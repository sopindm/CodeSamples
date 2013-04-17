/*
   -----------------------------------------------------------------------------
   |                 R T R T    S C E N E G R A P H   L I B R A R Y                 |
   -----------------------------------------------------------------------------
                              
                             
   Copyright (c) 2010 - 2012  Dmitry Sopin ( sopindm @ gmail.com )

   This library is free software: you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free Software
   Foundation, either version 3 of the License, or (at your option) any later
   version.

   This library is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
   FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along with
   this library. If not, see <http://www.gnu.org/licenses/>.
 */

/* Geometry */
#include "ModelStash.hpp"
#include "SphereStash.hpp"
#include "FractalStash.hpp"

/* Light sources */
#include "Lights/Skylight.hpp"
#include "Lights/PointLight.hpp"
#include "Lights/DistantLight.hpp"
#include "Lights/DiffuseRectangleLight.hpp"
#include "Lights/EnvironmentMap.hpp"

/* Diffuse materials */
#include "Materials/OrenNayarReflection.hpp"
#include "Materials/LambertianReflection.hpp"

/* Glossy materials */
#include "Materials/WardReflection.hpp"
#include "Materials/BlinnReflection.hpp"
#include "Materials/BlinnTransmission.hpp"

/* Specular materials */
#include "Materials/SpecularReflection.hpp"
#include "Materials/SpecularTransmission.hpp"

/* Composite material */
#include "Materials/MaterialStash.hpp"

#include "CameraStash.hpp"
#include "StashFactory.hpp"

using namespace rtrt::scenegraph;

BaseStash* StashFactory::create( const std::string& name )
{
  if( name == CameraStash::stashName() )
    return new CameraStash();

  /* Geometry */

  if( name == ModelStash::stashName() )
    return new ModelStash();
  if( name == SphereStash::stashName() )
    return new SphereStash();
  if( name == FractalStash::stashName() )
    return new FractalStash();

  /* Light sources */

  if( name == SkylightStash::stashName() )
    return new SkylightStash();
  if( name == PointLightStash::stashName() )
    return new PointLightStash();
  if( name == DistantLightStash::stashName() )
    return new DistantLightStash();
  if( name == EnvironmentMapStash::stashName() )
    return new EnvironmentMapStash();
  if( name == DiffuseRectangleLightStash::stashName() )
    return new DiffuseRectangleLightStash();

  /* BxDF stashes */
  
  if( name == OrenNayarReflectionStash::stashName() )
    return new OrenNayarReflectionStash();
  if( name == LambertianReflectionStash::stashName() )
    return new LambertianReflectionStash();
  if( name == WardReflectionStash::stashName() )
    return new WardReflectionStash();
  if( name == BlinnReflectionStash::stashName() )
    return new BlinnReflectionStash();
  if( name == BlinnTransmissionStash::stashName() )
    return new BlinnTransmissionStash();
  if( name == SpecularReflectionStash::stashName() )
    return new SpecularReflectionStash();
  if( name == SpecularTransmissionStash::stashName() )
    return new SpecularTransmissionStash();
  
  /* BSDF stash */
  
  if( name == MaterialStash::stashName() )
    return new MaterialStash();

  return NULL;
}
