#include <Camera/Camera.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

Mat4 Camera::GetViewMat()
{
	glm::quat q(rotation.w, rotation.x, rotation.y, rotation.z);
	glm::mat4 view = glm::mat4_cast(glm::conjugate(q)) * glm::translate(glm::mat4(1.f), -glm::vec3(position.x, position.y, position.z));
	return view;
}
