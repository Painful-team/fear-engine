#include "GuiEvent.hpp"

#include <components/CameraComponent.hpp>

namespace FearEngine::Events
{
FearEngine::Events::GuiUpdate::GuiUpdate() {}
ActiveViewport::ActiveViewport(Component::Camera* activeCam)
 : activeCamera(activeCam)
{}

FearEngine::Component::Camera*& FearEngine::Events::ActiveViewport::getActive() { return activeCamera; }
}  // namespace FearEngine::Events
