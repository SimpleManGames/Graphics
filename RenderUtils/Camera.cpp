#include "Camera.h"

#include "Input.h"
#include "Timer.h"

void FlyCamera::Update( const Input & i, const Timer & t ) {

	azimuth -= i.getMouseHorz() * t.getDeltaTime() * sensitivity;
	altitude += i.getMouseVer() * t.getDeltaTime() * sensitivity;

	//glm::vec3 moveDir = glm::vec3( 0, 0, 0 );
	//glm::vec3 right = glm::cross( direction, glm::vec3( 0, 1, 0 ) );
	//glm::vec3 up = glm::cross( right, direction );

	//direction = glm::vec3(glm::rotate( azimuth, glm::vec3(0,1,0))[2]);
	//right = glm::cross( direction, glm::vec3( 0, 1, 0 ) );
	//direction = glm::mat3(glm::rotate( altitude, right )) * direction;

	altitude = glm::clamp( altitude, -glm::pi<float>() / 2 + 0.0001f, glm::pi<float>() / 2 - 0.0001f);

	glm::mat4 tbn = glm::eulerAngleYXZ( azimuth, altitude, 0.0f );

	glm::vec3 right = glm::vec3( tbn[ 0 ] );
	glm::vec3 up = glm::vec3( tbn[ 1 ] );
	direction = glm::vec3( tbn[ 2 ] );

	glm::vec3 moveDir = glm::vec3( 0, 0, 0 );

	if( i.getKeyState( 'W' ) == Input::DOWN ) { moveDir += direction; }
	if( i.getKeyState( 'S' ) == Input::DOWN ) { moveDir -= direction; }

	if( i.getKeyState( 'A' ) == Input::DOWN ) { moveDir += right; }
	if( i.getKeyState( 'D' ) == Input::DOWN ) { moveDir -= right; }

	if( i.getKeyState( 340 ) == Input::DOWN ) { moveDir -= up; }
	if( i.getKeyState( ' ' ) == Input::DOWN ) { moveDir += up; }


	/*direction.x = cos( azimuth ) * sin( altitude );
	direction.y = cos( azimuth );
	direction.z = sin( azimuth ) * sin( altitude );*/

	if( glm::length( moveDir ) >= .7f ) {
		moveDir = glm::normalize( moveDir );
		position += moveDir * t.getDeltaTime() * speed;
	}
}
