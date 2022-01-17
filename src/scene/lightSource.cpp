#include "lightSource.hpp"

LightSource::LightSource()
	: position(0.f)
	, Ia(1.f)
	, Id(1.f)
	, Is(1.f)
{
}

LightSource::LightSource(glm::vec3 position, glm::vec4 Ia, glm::vec4 Id, glm::vec4 Is)
	: position(position)
	, Ia(Ia)
	, Id(Id)
	, Is(Is)
{
}
