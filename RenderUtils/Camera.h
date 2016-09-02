#pragma once
#define GLM_SWIZZLE
#include <GLM\glm.hpp>
#include <GLM\ext.hpp>

class FlyCamera {
public:
	FlyCamera( float fov = 45.f, float a = 16.f / 9.f, float n = 1.f, float f = 100.f )
		: fov( fov ), aspect( a ), n( n ), f( f ), direction( 0, 0, 1 ), azimuth(0), altitude(0) {}

	glm::vec3 position;
	glm::vec3 direction;

	float azimuth, altitude;

	float speed = 10;
	float sensitivity = .1f;

	float aspect, fov, n, f;

	void Update( const class Input & i, const class Timer & t );

	void lookAt( const glm::vec3 &target ) { direction = glm::normalize( target - position ); }
	void jumpTo( const glm::vec3 &loc ) { position = loc; }

	glm::mat4 getView() const { return glm::lookAt( position, position + direction, glm::vec3( 0, 1, 0 ) ); }
	glm::mat4 getProj() const { return glm::perspective( fov, aspect, n, f ); }
};