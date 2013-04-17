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
#include "Sphere.hpp"
#include "Fractal.hpp"
#include "HeightField.hpp"
#include "StaticModel.hpp"
#include "FrameAnimation.hpp"

/* Diffuse BxDFs */
#include "Materials/OrenNayarReflection.hpp"
#include "Materials/LambertianReflection.hpp"

/* Specular BxDFs */
#include "Materials/SpecularReflection.hpp"
#include "Materials/SpecularTransmission.hpp"

/* Glossy BxDFs */
#include "Materials/WardReflection.hpp"
#include "Materials/BlinnReflection.hpp"
#include "Materials/BlinnTransmission.hpp"

/* BSDF */
#include "Materials/Material.hpp"

/* Light sources */
#include "Lights/Skylight.hpp"
#include "Lights/PointLight.hpp"
#include "Lights/DistantLight.hpp"
#include "Lights/EnvironmentMap.hpp"
#include "Lights/DiffuseRectangleLight.hpp"

#include "Camera.hpp"
#include "ObjectFactory.hpp"
#include "TransformKernel.hpp"

using namespace rtrt::scenegraph;

KernelPtr ObjectFactory::createKernel( const std::string& name )
{
  if( name == "Transform" )
    return KernelPtr( new TransformKernel() );

  if( name == "Camera" )
    return KernelPtr( new Camera() );


  /* Geometry */

  if( name == "Sphere" )
    return KernelPtr( new Sphere() );
  if( name == "Fractal" )
    return KernelPtr( new Fractal() );
  if( name == "HeightField" )
    return KernelPtr( new HeightField() );
  if( name == "StaticModel" )
    return KernelPtr( new StaticModel() );
  if( name == "FrameAnimation" )
    return KernelPtr( new FrameAnimation() );


  /* Diffuse BxDFs */

  if( name == "OrenNayarReflection" )
    return KernelPtr( new OrenNayarReflection() );
  if( name == "LambertianReflection" )
    return KernelPtr( new LambertianReflection() );


  /* Glossy BxDFs */

  if( name == "WardReflection" )
    return KernelPtr( new WardReflection() );
  if( name == "BlinnReflection" )
    return KernelPtr( new BlinnReflection() );
  if( name == "BlinnTransmission" )
    return KernelPtr( new BlinnTransmission() );


  /* Specular BxDFs */

  if( name == "SpecularReflection" )
    return KernelPtr( new SpecularReflection() );
  if( name == "SpecularTransmission" )
    return KernelPtr( new SpecularTransmission() );


  /* BSDF */

  if( name == "Material" )
    return KernelPtr( new Material() );


  /* Light sources */

  if( name == "Skylight" )
    return KernelPtr( new Skylight() );
  if( name == "PointLight" )
    return KernelPtr( new PointLight() );
  if( name == "DistantLight" )
    return KernelPtr( new DistantLight() );
  if( name == "EnvironmentMap" )
    return KernelPtr( new EnvironmentMap() );
  if( name == "DiffuseRectangleLight" )
    return KernelPtr( new DiffuseRectangleLight() );


  return KernelPtr();
}

