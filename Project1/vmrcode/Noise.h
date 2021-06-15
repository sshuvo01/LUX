

//
//
// noise.h
//
// General purpose noise code
//
// Integrates multiple noise sources into a single framework
//
//
//


#ifndef __NOISE_H__
#define __NOISE_H__

#include "Vector.h"
#include <vector>
#include <iostream>
namespace lux
{

struct Noise_t 
{

  Noise_t() :
  frequency    (1),
  translate    (Vector(0,0,0)),
  octaves      (1.0),
  amplitude    (1),
  offset       (0),
  fjump        (2),
  roughness    (0.5),
  radius       (1.0),
  pscale       (1.0),
  gamma        (1.0),
  time         (0.0),
  fftLowCutoff (0.01),
  fftHighCutoff (1.0),
  fftPower      (3.5),
  fftNbGridPoints (128),
  fftLength     (10.0),
  lognormalmean (1.0),
  gaussianstandarddeviation (1.0),
  seed         (12345),
  tangent      (Vector(0,0,1)),
  normal       (Vector(0,1,0)),
  binormal     (Vector(1,0,0)),
  axis         (Vector(0,1,0)),
  angle        (0.0),
  P            (Vector(0,0,0)),
  v            (Vector(0,0,0)),
  A            (Vector(0,0,0)),
  age          (0.0),
  lifeTime     (1.0),
  shutter      (0.5),
  frameRate    (1.0/24.0),
  falloff      (1.0)

  {}

	float frequency;
	Vector translate;
	float octaves;
	float amplitude;
	float offset;
	float fjump;
        float roughness;
	float radius;
	float pscale;
	float gamma;
	float time;
	float fftLowCutoff;
	float fftHighCutoff;
	float fftPower;
	int   fftNbGridPoints;
	float fftLength;
	float lognormalmean;
	float gaussianstandarddeviation;
	int   seed;
	Vector tangent;
	Vector normal;
	Vector binormal;
	Vector axis;
	float angle;
	Vector P;
	Vector v;
	Vector A;
	float  age;
	float lifeTime;
	float shutter;
	float frameRate;
	float falloff;
};


class Noise
{
  public:

    Noise(){}
    virtual ~Noise(){}

    virtual const float eval( const float x ) const { return 0; }
    virtual const float eval( const Vector& x ) const { return 0; }

    virtual void setParameters( const Noise_t& parameters ){}
    virtual void getParameters( Noise_t& parameters ) const {}
};



template< typename BaseNoise>
class FractalSum : public Noise
{
  public:

    FractalSum() : 
	   S_amplitude  (1.0),

       octaves    (3.0), 
       fjump      (2.0), 
       roughness  (0.5),
       frequency  (0.666666),
       translate  (Vector(0,0,0)),
       offset     (0.0),
       axis       (Vector(0,0,1)),
       angle      (0.0)
    {}

   virtual ~FractalSum(){}

    const float eval( const float x ) const
    {
       float exponent = 1;
       //float amplitude = 1;
	   float amplitude = S_amplitude;

       float accum = 0;
       int ioct = (int)octaves;
       for( int oc=0;oc<ioct;oc++ )
       {
          const float X = (x - translate[0]) * frequency * exponent;
          accum += amplitude * _noise.eval( X );
          exponent *= fjump;
	  amplitude *= roughness;
       }
       const float X = (x - translate[0]) * frequency * exponent;
       float val = amplitude * _noise.eval( X );
       accum += (octaves - (int)octaves) * val;
       return (accum + offset);
    }

    const float eval( const Vector& x ) const
    {
		//std::cout << "Fractal noise\n";
       float exponent = 1;
       //float amplitude = 1;
	   float amplitude = S_amplitude;

       float accum = 0;
       int ioct = (int)octaves;
       Vector X = (x - translate);
       if( angle != 0.0 )
       {
          float ca = cos(angle);
	  float sa = sin(angle);
	  X = X*ca + axis*(axis*X)*(1.0-ca) + (axis^X)*sa;
       }
       X *= frequency*exponent;
       for( int oc=0;oc<ioct;oc++ )
       {
          accum += amplitude * _noise.eval( X );
          X *= fjump;
	  amplitude *= roughness;
       }
       float val = amplitude * _noise.eval( X );
       accum += (octaves - (int)octaves) * val;
       return (accum+offset);
    }


    void setParameters( const Noise_t& parameters )
    {
		S_amplitude = parameters.amplitude;

       octaves = parameters.octaves;
       fjump = parameters.fjump;
       roughness = parameters.roughness;
       frequency = parameters.frequency; 
       translate = parameters.translate; 
       offset = parameters.offset; 
       axis = parameters.axis;
       angle = parameters.angle;
       _noise.setTime( parameters.time );
    }

   
    void getParameters( Noise_t& parameters ) const
    {
		parameters.amplitude = S_amplitude;

       parameters.octaves = octaves;
       parameters.fjump = fjump;
       parameters.roughness = roughness;
       parameters.frequency = frequency; 
       parameters.translate = translate; 
       parameters.offset = offset; 
       parameters.axis = axis;
       parameters.angle = angle;
    }
 


  private:

    BaseNoise _noise;

	float S_amplitude;

    float octaves;
    float fjump;
    float roughness;
    float frequency;
    Vector translate;
    float offset;
    Vector axis;
    float angle;
};







}

#endif



