/*****************************************************************************************
* Copyright (c) 2008 Jonathan 'Bladezor' Bastnagel.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the GNU Lesser Public License v2.1
* which accompanies this distribution, and is available at
* http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
* 
* Contributors:
*     Jonathan 'Bladezor' Bastnagel - initial implementation and documentation
*****************************************************************************************/
#ifndef FPSCAMERA_H_
#define FPSCAMERA_H_

#include <GL/glfw.h>
#include "Engine/Tools/Math/Common.h"
#include "Engine/Input/Input.h"
#include <iostream>

#define CAMERA_SPEED 250
#define MOUSE_SENSITIVITY 0.25

namespace Odorless
{
	namespace Engine
	{
		namespace Cameras
		{
			class FPSCamera : public Odorless::Engine::Input::InputListener
			{
			public:
				FPSCamera(double eyeX, double eyeY, double eyeZ, double centerX, double centerY, double centerZ,
					double upX, double upY, double upZ)
				{
					_dEyeX = eyeX;
					_dEyeY = eyeY;
					_dEyeZ = eyeZ;
					_dCenterX = centerX;
					_dCenterY = centerY;
					_dCenterZ = centerZ;
					_dUpX = upX;
					_dUpY = upY;
					_dUpZ = upZ;
					xChange = 0;
					yChange = 0;
					zAngle = 0;
					if(Odorless::Engine::Input::InputManager::IsInitialized()==false)
						std::cerr << "Warning: Camera - The Input Manager has not yet been initialized!" << std::endl;
					else
						Odorless::Engine::Input::InputManager::AddInputListener(this);
				}
				~FPSCamera()
				{
				}

				void OnKeyEvent(const int key, const int action)
				{
					if(key==GLFW_KEY_TAB && action == GLFW_PRESS)
					{
						Odorless::Engine::Input::InputManager::SetForceToMouseReference(!Odorless::Engine::Input::InputManager::IsMouseForcedToReference());
					}
				};

				void MoveForward(const float &dt);
				void MoveBackward(const float &dt);
				void StrafeLeft(const float &dt);
				void StrafeRight(const float &dt);

				void Update(const float &dt);
				void Render();

			private:
				double xChange, yChange, zAngle;
				double _dEyeX, _dEyeY, _dEyeZ;
				double _dCenterX, _dCenterY, _dCenterZ;
				double _dUpX, _dUpY, _dUpZ;
			};
		}
	}
}
#endif